#include "stream_server.h"

#include <memory>

#include <grpcpp/grpcpp.h>


#include "stream_service.h"
#include "flog.h"


using namespace ez_grpc;

StreamServer::~StreamServer()
{
	UnInit();
}

void StreamServer::UnInit()
{
	exit_ = true;
	
	INFO_LOG("thread_num_:{}", thread_num_);
	for (int i = 0; i < threads_.size(); i++)
	{
		threads_[i].join();
		INFO_LOG("thread: {} has exited", i);
		std::cout << "thread has exited" << i << std::endl;
	}

	if (server_ != nullptr)
	{
		server_->Shutdown(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
		std::cout << "after shutdown" << std::endl;
		server_.release();
	}
	if (cq_ != nullptr)
	{
		cq_->Shutdown();
		cq_ .release();
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
			std::thread(&StreamServer::DrawFromCompletionQueue, this, cq));
	}

	server_ = builder.BuildAndStart();	

	return server_ != nullptr;
}



void StreamServer::DrawFromCompletionQueue(grpc::ServerCompletionQueue* cq)
{
	if (cq == nullptr)
	{
		return;
	}

	new ServerConnect(async_service_, cq);

	void* tag;
	bool ok;
	grpc::CompletionQueue::NextStatus status;
	do {

		tag = nullptr;
		ok = false;

		status = cq->AsyncNext(&tag, &ok, std::chrono::system_clock::now() + std::chrono::milliseconds(20));

		if (status == grpc::CompletionQueue::GOT_EVENT)
		{
			((ServerConnect*)tag)->Proceed();
		}
		else if (status == grpc::CompletionQueue::SHUTDOWN)
		{
			break;
		}
		/*else if (status == grpc::CompletionQueue::TIMEOUT)
		{
		}*/

	} while (!exit_);



}