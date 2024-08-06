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
	for (int i = 0; i < thread_num_; i++)
	{
		threads_[i].join();
		INFO_LOG("thread: {} has exited", i);
	}

	server_->Shutdown();
	INFO_LOG("server_->Shutdown");

	void* ignored_tag;
	bool ignored_ok;
	for (int i = 0; i < thread_num_; i++)
	{
		cqs_[i]->Shutdown();
		while (cqs_[i]->Next(&ignored_tag, &ignored_ok)) {}
		INFO_LOG("cq: {} has empty", i);
	}

	thread_num_ = 0;
	threads_.clear();
	cqs_.clear();

	delete async_service_;
}


bool StreamServer::Init()
{
	Base::AsyncService* async_service = new Base::AsyncService();
	if (async_service == nullptr)
	{
		return;
	}

	grpc::ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
	builder.RegisterService(async_service);

	for (int i = 0; i < 2; i++)
	{
		cqs_.push_back(builder.AddCompletionQueue());

		threads_.push_back(
			std::thread(&StreamServer::DrawFromCompletionQueue, this, cqs_[i]));
	}

	server_ = builder.BuildAndStart();	
}



void StreamServer::DrawFromCompletionQueue(std::unique_ptr<grpc::ServerCompletionQueue> cq)
{
	if (cq == nullptr)
	{
		return;
	}

	new ServerConnect(async_service_, cq.get());

	void* tag;
	bool ok;
	grpc::CompletionQueue::NextStatus status;
	do {
		
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