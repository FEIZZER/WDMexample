#include "stream_server.h"

#include <memory>

#include <grpcpp/grpcpp.h>

#include "ez_grpc/common/status.h"
#include "server_connect.h"
#include "flog.h"


using namespace ez_grpc;

StreamServer::~StreamServer()
{
	Shutdown();
}


void StreamServer::Shutdown()
{
	INFO_LOG("");
	exit_ = true;
	
	INFO_LOG("thread_num_:{}", thread_num_);
	for (int i = 0; i < threads_.size(); i++)
	{
		threads_[i].join();
		INFO_LOG("thread: {} has exited", i);
	}

	for (auto it : connect_pool_)
	{
		it.second->DisConnect();
	}

	if (server_ != nullptr)
	{
		server_->Shutdown(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
		std::cout << "after shutdown" << std::endl;
		server_.release();
	}
	
	INFO_LOG("server_->Shutdown");

	void* ignored_tag;
	bool ignored_ok;
	for (int i = 0; i < cqs_.size(); i++)
	{
		cqs_[i]->Shutdown();
		while (cqs_[i]->Next(&ignored_tag, &ignored_ok)) {}
		INFO_LOG("cq: {} has empty", i);
	}

	thread_num_ = 0;
	threads_.clear();
	cqs_.clear();
	connect_pool_.clear();

	if (async_service_ != nullptr)
	delete async_service_;
}


bool StreamServer::Init()
{
	async_service_ = new Base::AsyncService();
	if (async_service_ == nullptr)
	{
		return false;
	}

	grpc::ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:8800", grpc::InsecureServerCredentials());
	builder.RegisterService(async_service_);

	thread_num_ = 2;

	for (int i = 0; i < thread_num_; i++)
	{
		cqs_.push_back(builder.AddCompletionQueue());

		auto cq = cqs_[i].get();

		threads_.push_back(
			std::thread(&StreamServer::DrawFromCq, this, cq));
	}

	server_ = builder.BuildAndStart();	

	return server_ != nullptr;
}

bool StreamServer::Start(const std::string& addr, int thread_nums, int max_connect)
{
	return StartInternal(addr, false, 3);
}

bool StreamServer::StartInternal(const std::string& addr, bool limit, int thread_num)
{
	if (async_service_ != nullptr) Shutdown();
	async_service_ = new Base::AsyncService();
	if (async_service_ == nullptr) return false;

	grpc::ServerBuilder builder;
	builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
	builder.RegisterService(async_service_);
	thread_num_ = thread_num;
	for (int i = 0; i < thread_num_; i++)
	{
		cqs_.push_back(builder.AddCompletionQueue());
		threads_.push_back(std::thread(&StreamServer::DrawFromCq, this, cqs_[i].get()));
	}

	server_ = builder.BuildAndStart();

	return server_ != nullptr;
}

bool StreamServer::RunWithLimit(int thread_nums)
{
	for (int i = 0; i < thread_num_; i++)
	{
		auto cq = cqs_[i].get();

		threads_.push_back(
			std::thread(&StreamServer::DrawFromCq, this, cq));
	}

	return true;
}

void StreamServer::DrawFromCq(grpc::ServerCompletionQueue* cq)
{
	if (cq == nullptr)
	{
		return;
	}

	NewConnect(cq);

	void* tag;
	bool ok;
	grpc::CompletionQueue::NextStatus status;
	do {

		tag = nullptr;
		ok = false;

		status = cq->AsyncNext(&tag, &ok, std::chrono::system_clock::now() + std::chrono::milliseconds(20));

		if (status == grpc::CompletionQueue::SHUTDOWN)
		{
			break;
		}

		if (status == grpc::CompletionQueue::GOT_EVENT)
		{
			auto connect_key = std::to_string((uintptr_t)tag);
			INFO_LOG("IN GOT_EVENT, tag:{}", (uintptr_t)tag);
			auto connect = GetConnect(connect_key);
			if (connect == nullptr)
			{
				ERROR_LOG("cannot find connect in map");
				continue;
			}

			BaseRequest request;
			auto connect_status = connect->Proceed(request);
			if (connect_status == EZCode::Connect_Get_Connected)
			{
				// NewConnect(cq);
			}
			else if (connect_status == EZCode::Connect_Work_Success)
			{
				INFO_LOG("outter len:{} read:{}", request.length(), (char*)request.buffer().c_str());
			}
			else if (connect_status == EZCode::Connect_Disconnected)
			{
				INFO_LOG("disconnect");
				if (!DeleteConnect(connect_key))
				{
					ERROR_LOG("delete connect failed, connect_key:{}", connect_key);
				}
				else
				{
					INFO_LOG("delete connect success, connect_key:{}", connect_key);
				}
			}
		}

		/*if (status == grpc::CompletionQueue::TIMEOUT)
		{
		}*/

	} while (!exit_);
}

void StreamServer::DrawFromCqWithOneConnect(grpc::ServerCompletionQueue* cq)
{

}




void StreamServer::NewConnect(grpc::ServerCompletionQueue* cq)
{
	auto connect = std::make_shared<ServerConnectImpl>(async_service_, cq);
	//error connect == nullptr
	std::string defaultKey = std::to_string((uintptr_t)(connect.get()));

	std::unique_lock<std::shared_mutex> lock(shared_mutex_);
	if (connect_pool_.find(defaultKey) != connect_pool_.end())
	{
		//warning
	}

	connect_pool_[defaultKey] = connect;
}


std::shared_ptr<ServerConnectImpl> StreamServer::GetConnect(const std::string& key)
{
	std::shared_lock<std::shared_mutex> lock(shared_mutex_);

	auto iter = connect_pool_.find(key);
	if (iter == connect_pool_.end())
	{
		return nullptr;
	}
	return iter->second;
}

std::shared_ptr<ServerConnectImpl> StreamServer::GetFirstConnect()
{
	
	std::shared_lock<std::shared_mutex> lock(shared_mutex_);

	INFO_LOG("map cnt:{}", connect_pool_.size());

	return connect_pool_.empty() ? nullptr : connect_pool_.begin()->second;
}


bool StreamServer::DeleteConnect(const std::string& key)
{
	std::unique_lock<std::shared_mutex> lock(shared_mutex_);
	return connect_pool_.erase(key) == 1;
}


