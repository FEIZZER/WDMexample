#include "client_connect_internal.h"

#include <grpcpp/grpcpp.h>

using namespace ez_grpc;

#define Max_Connect_MiliSec 2000

ClientConnectInternal::ClientConnectInternal(const std::string& ip, int target_port)
{
	connected_ = false;
	auto channel = grpc::CreateChannel(ip + std::to_string(target_port), grpc::InsecureChannelCredentials());
	if (channel == nullptr)
	{
		return;
	}

	auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(Max_Connect_MiliSec);
	if (!channel->WaitForConnected(deadline))
	{
		return;
	}

	stub_ = Base::NewStub(channel);
	if (stub_ == nullptr)
	{
		return;
	}

	ip_ = ip;
	server_port_ = target_port;
	connected_ = true;
}

ClientConnectInternal::~ClientConnectInternal()
{

}


bool ClientConnectInternal::IsConnected()
{
	return connected_;
}

bool ClientConnectInternal::Request(void* buffer, unsigned int length,
									void** out_buffer /*= nullptr*/, unsigned int* out_length /*= nullptr*/)
{
	bool bRet = false;

	do{

		if (buffer == nullptr || length == 0)
		{
			break;
		}
		// todo: buffer最长校验 

		if (!connected_)
		{
			break;
		}

		BaseReply reply;
		BaseRequest request;
		request.set_length(length);
		request.set_buffer(buffer, length);
		grpc::ClientContext client_ctx;
		client_ctx.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(Max_Connect_MiliSec));

		auto status = stub_->BaseTransmit(&client_ctx, request, &reply);
		if (!status.ok())
		{
			// todo: error log
			break;
		}

		bRet = true;

		if (out_buffer == nullptr || out_length == nullptr)
		{
			break;
		}

		auto reply_length = reply.length();
		void* reply_buffer = (void*)reply.buffer().c_str();
		//todo: reply_length 长度验证

		char* tmp = new char[reply_length];
		if (tmp == nullptr)
		{
			break;
		}

		memcpy(tmp, reply_buffer, reply_length);
		*out_buffer = tmp;
		*out_length = reply_length;

	} while (0);

	return bRet;
}

void ClientConnectInternal::DisConnect()
{
}


ClientConnectStream::ClientConnectStream(const std::string& ip, int target_port)
{
	connected_ = false;
	auto channel = grpc::CreateChannel(ip + std::to_string(target_port), grpc::InsecureChannelCredentials());
	if (channel == nullptr)
	{
		return;
	}

	auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(Max_Connect_MiliSec);
	if (!channel->WaitForConnected(deadline))
	{
		return;
	}

	stub_ = Base::NewStub(channel);
	if (stub_ == nullptr)
	{
		return;
	}

	stream_ = stub_->StreamTransmit(&context_);
	if (stream_ == nullptr)
	{
		return;
	}

	ip_ = ip;
	server_port_ = target_port;
	connected_ = true;
}