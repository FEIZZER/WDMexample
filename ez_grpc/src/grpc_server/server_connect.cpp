#include "server_connect.h"

#include <iostream>

#include "flog.h"


using namespace ez_grpc;


ServerConnectImpl::ServerConnectImpl(Base::AsyncService* async_service, grpc::ServerCompletionQueue* cq)
	: stream_(&context_), connect_status_(ServerConnectImpl::Status::Created)
{
	cq_ = cq;
	async_service_ = async_service;

	context_.AsyncNotifyWhenDone(this);
	async_service_->RequestStreamTransmit(&context_, &stream_, cq_, cq_, this);
}



EZCode ServerConnectImpl::Proceed(BaseRequest& request)
{
	EZCode eRet = EZCode::Connect_Work_Success;

	if (connect_status_ == Status::DisConnected)
	{
		return EZCode::Connect_Disconnected;
	}

	// get connected
	if (connect_status_ == Status::Created)
	{
		INFO_LOG("obj_ptr:{} connect_name:{} get connected", (uintptr_t)this, connect_name_);
		connect_status_ = Status::Connected;
		eRet = EZCode::Connect_Get_Connected;
	}

	if (context_.IsCancelled())
	{
		INFO_LOG("obj_ptr:{} connect_name:{} have be disconnected from client", (uintptr_t)this, connect_name_);
		connect_status_ = Status::DisConnected;
		return EZCode::Connect_Work_Success;
	}

	
	stream_.Read(&request_, this);

	request = std::move(request_);

	INFO_LOG("after read:{}", (char*)request_.buffer().c_str());

	return eRet;
}



bool ServerConnectImpl::Write(void* buffer, int length)
{
	if (buffer == nullptr || length == 0)
	{
		return false;
	}

	if (connect_status_ != Status::Connected)
	{
		return false;
	}

	BaseReply reply;
	reply.set_length(length);
	reply.set_buffer(buffer, length);
	stream_.Write(reply, 0);

	return true;
}



void ServerConnectImpl::DisConnect()
{
	if (connect_status_ == Status::Connected)
	{
		std::cout << (ULONG_PTR)this << ": start server disconnect" << std::endl;
		stream_.Finish(grpc::Status::OK, this);
		connect_status_ = Status::DisConnected;
	}
}