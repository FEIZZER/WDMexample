#pragma once
#ifndef GRPCBASESERVICE
#define GRPCBASESERVICE

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <Poco/Thread.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "work_thread.h"

class GrpcBaseService : public WorkThread
{
public:
	GrpcBaseService(const void* request_buffer, int32_t request_length, 
					void** reply_buffer, int32_t* reply_length, 
					BaseReply* reply_, grpc::ServerAsyncResponseWriter<BaseReply>* responder_):

					request_buffer(request_buffer), request_length(request_length), 
					reply_buffer(reply_buffer), reply_length(reply_length),
					reply_(reply_), responder_(responder_)
					{};
	~GrpcBaseService() {};

	virtual bool GRPCRequestHandle(_In_ const void* request_buffer, _In_ int32_t request_length,
									_Out_ void** reply_buffer, _Out_ int32_t* reply_length) 
	{
		std::cout << "In GRPCRequestHandle()" << std::endl;
		*reply_buffer = const_cast<void*>(request_buffer);
		*reply_length = request_length;
		return true;
	};

	void run()
	{
		std::cout << "In run()" << std::endl;
		GRPCRequestHandle(request_buffer, request_length, reply_buffer, reply_length);
		reply_->set_buffer(*reply_buffer, *reply_length);
		int cnt = 1;
		while (cnt <= 5)
		{
			Sleep(1000);
			std::cout << (ULONG_PTR)this << " is handle: " << cnt << std::endl;
			cnt++;
		}
		// And we are done! Let the gRPC runtime know we've finished, using the
		// memory address of this instance as the uniquely identifying tag for
		// the event.
		responder_->Finish(*reply_, Status::OK, this);
	}

	void Start() 
	{
		std::cout << "In Start()" << std::endl;
		std::cout << "IsRunning" << this->InitThread() << std::endl;
	}

public:
	void** reply_buffer;
	int32_t* reply_length;

private:
	const void* request_buffer;
	int32_t request_length;

	// What we send back to the client.
	BaseReply* reply_;
	// The means to get back to the client.
	grpc::ServerAsyncResponseWriter<BaseReply>* responder_;
};




#endif // !GRPCBASESERVICE