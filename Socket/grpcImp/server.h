/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "../proto/helloworld.grpc.pb.h"
#include "../proto/base.grpc.pb.h"

#include "work_thread.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

using GRPCProc = void (*) (void*, int32_t, void**, int32_t*);

class AsyncStreamTransmitImpl final : public WorkThread
{
public:
    AsyncStreamTransmitImpl(Base::AsyncService* service, grpc::ServerCompletionQueue* cq)
    {
        service_ = service;
        cq_ = cq;
        status_ = AsyncStreamTransmitImpl::CREATE;
        Proceed();
    }
    void run()
    {
        BaseRequest request;
        BaseReply reply;
        stream_.Read(&request, this);
    }
    void Proceed() {

        switch (status_)
        {
        case AsyncStreamTransmitImpl::CREATE:
            std::cout << "AsyncStreamTransmitImpl In Create" << std::endl;
            status_ = INIT_READ;
            service_->RequestStreamTransmit(&ctx_, &stream_, cq_, cq_, this);
            std::cout << "AsyncStreamTransmitImpl after RequestStreamTransmit" << std::endl;
            break;

        case AsyncStreamTransmitImpl::INIT_READ:
            std::cout << "AsyncStreamTransmitImpl In Init_Read" << std::endl;
            new AsyncStreamTransmitImpl(service_, cq_);
        case AsyncStreamTransmitImpl::READ:
            std::cout << "AsyncStreamTransmitImpl In Read" << std::endl;
            stream_.Read(&request_, this);
            std::cout << "AsyncStreamTransmitImpl after Read" << std::endl;
            status_ = WRITE;
            break;

        case AsyncStreamTransmitImpl::WRITE:
            std::cout << "AsyncStreamTransmitImpl In Write" << std::endl;
            reply_.set_buffer(" ok then");
            stream_.Write(reply_, this);
            // do write
            status_ = READ;
            break;

        case AsyncStreamTransmitImpl::FINISH:
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves
            delete this;
            break;

        default:
            break;
        }
    }
private:
  
    Base::AsyncService* service_;

    grpc::ServerCompletionQueue* cq_;

    ServerContext ctx_;

    BaseRequest request_;

    BaseReply reply_;

    grpc::ServerAsyncReaderWriter< ::BaseReply, ::BaseRequest> stream_
        = grpc::ServerAsyncReaderWriter<BaseReply, BaseRequest>(&ctx_);

    enum CallStatus { CREATE, INIT_READ, READ, WRITE, FINISH };
    CallStatus status_;

    GRPCProc grpc_proc_;
};


class AsyncServiceImpl final : public WorkThread {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    AsyncServiceImpl(Base::AsyncService* service, grpc::ServerCompletionQueue* cq, GRPCProc grpc_proc):
        service_(service), 
        cq_(cq), 
        grpc_proc_(grpc_proc),
        responder_(&ctx_), 
        status_(CREATE)
    {
        // Invoke the serving logic right away
        Proceed();
    }
    void run()
    {
        auto request_buffer = request_.buffer();
        void* reply_buffer = nullptr;
        int32_t reply_length = 0;
        grpc_proc_(
            const_cast<void*>(reinterpret_cast<const void*>(request_buffer.c_str())),
            request_buffer.size(),
            &reply_buffer,
            &reply_length);
        reply_.set_buffer(reply_buffer, reply_length);
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
        responder_.Finish(reply_, Status::OK, this);
    }
    void Proceed() {
        switch (status_)
        {
        case AsyncServiceImpl::CREATE:
            std::cout << "In CREATE" << std::endl;
            status_ = PROCESS;
            service_->RequestBaseTransmit(&ctx_, &request_, &responder_, cq_, cq_, this);
            std::cout << "AsyncServiceImpl after RequestBaseTransmit" << std::endl;
            break;

        case AsyncServiceImpl::PROCESS:
            new AsyncServiceImpl(service_, cq_, grpc_proc_);
            WorkThread::InitThread();
            status_ = FINISH;
            break;

        case AsyncServiceImpl::FINISH:
            std::cout << "In FINISH" << std::endl;
            GPR_ASSERT(status_ == FINISH);
            delete this;
            break;

        default:
            break;
        }
    }
private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    Base::AsyncService* service_;

    // The producer-consumer queue where for asynchronous server notifications.
    grpc::ServerCompletionQueue* cq_;

    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    BaseRequest request_;

    // What we send back to the client.
    BaseReply reply_;

    // The means to get back to the client.
    grpc::ServerAsyncResponseWriter<BaseReply> responder_;

    // The current serving state.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;

    GRPCProc grpc_proc_;
};

class BaseServiceImpl final : public Base::Service
{
public:

    BaseServiceImpl(GRPCProc grpc_proc)
    {
        grpc_proc_ = grpc_proc;
    }


    grpc::Status BaseTransmit(
        grpc::ServerContext* context, 
        const ::BaseRequest* request, 
        BaseReply* response) override {
        
        void* reply_buffer = nullptr;
        int32_t reply_length = 0;

        auto request_buffer = request->buffer();
        

        std::cout << "recv" << std::endl;

        //response->set_version(1);
        response->set_length(0);
        //response->set_buffer(reply_buffer, reply_length);

        return grpc::Status::OK;
    }
    
    //grpc::Status StreamTransmit(
    //    grpc::ServerContext* context, 
    //    grpc::ServerReaderWriter<BaseReply, BaseRequest>* stream) override
    //{
    //    //stream->Read()
    //    // stream->Write()
    //    return grpc::Status::OK;
    //}

    //grpc::Status ServerStreamTransmit(
    //    grpc::ServerContext* context, 
    //    const ::BaseRequest* request, 
    //    grpc::ServerWriter<BaseReply>* writer) override
    //{
    //    std::cout << "In ServerStreamTransmit" << std::endl;
    //    BaseReply* reply = new BaseReply;
    //    while (!context->IsCancelled())
    //    {
    //       Sleep(2000);
    //        request->buffer().c_str();
    //        request->length();
    //        reply->set_buffer(
    //            const_cast<void*>(reinterpret_cast<const void*>(request->buffer().c_str())),
    //            request->buffer().size());

    //        writer->Write(*reply);
    //    }

    //    return grpc::Status::OK;
    //}

    //grpc::Status ClientStreamTransmit(
    //    grpc::ServerContext* context,
    //    grpc::ServerReader<BaseRequest>* reader,
    //    BaseReply* response) override
    //{
    //    BaseRequest* request = new BaseRequest;
    //    while (reader->Read(request))
    //    {
    //        std::cout << "server receive stream: " 
    //            << request->buffer().c_str() << std::endl;
    //    }

    //    std::cout << "read done" << std::endl;

    //    #define value "ClientStreamTransmit Handle Done"
    //    response->set_buffer(value, sizeof(value));
    //    return grpc::Status::OK;
    //}

private:
    GRPCProc grpc_proc_;
    Poco::Event event_;
};

class ServerImp final
{
public:
    ~ServerImp() {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        server_cq_->Shutdown();
    }

    ServerImp(GRPCProc grpc_proc):
        grpc_proc_(grpc_proc)  {}

    void Run() {
        std::string server_address("0.0.0.0:50051");
        BaseServiceImpl service(grpc_proc_);

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        ServerBuilder builder;

        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // Register "service" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *synchronous* service.
        builder.RegisterService(&service);

        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server_->Wait();
    }

    void RunAsync()
    {
        std::string server_address("0.0.0.0:50051");
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        Base::AsyncService service_;
        builder.RegisterService(&service_);

        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        server_cq_ = builder.AddCompletionQueue();

        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Spawn a new CallData instance to serve new clients.
        new AsyncServiceImpl(&service_, server_cq_.get(), grpc_proc_);
        void* tag;  // uniquely identifies a request.
        bool ok;

        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            std::cout << "In StartHandleRequest before Next" << std::endl;
            GPR_ASSERT(server_cq_->Next(&tag, &ok));
            std::cout << "In StartHandleRequest after Next" << std::endl;
            if (ok) 
            {
                static_cast<AsyncServiceImpl*>(tag)->Proceed();
            }
            else
            {
                std::cout << "server_cq_->Next err" << std::endl;
            }
        }
    }
    void RunAsync2()
    {
        std::string server_address("0.0.0.0:50051");
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        Base::AsyncService service_;
        builder.RegisterService(&service_);

        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        server_cq_ = builder.AddCompletionQueue();

        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Spawn a new AsyncStreamTransmitImpl instance to serve new clients.
        new AsyncStreamTransmitImpl(&service_, server_cq_.get());
        void* tag;  // uniquely identifies a request.
        bool ok;

        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            if (!server_cq_->Next(&tag, &ok))
            {
                break;
            }
            
            if (ok)
            {
                static_cast<AsyncStreamTransmitImpl*>(tag)->Proceed();
            }
            else
            {
                std::cout << "server_cq_->Next err" << std::endl;
            }
        }
    }
private:
    std::unique_ptr<Server> server_;
    std::unique_ptr<grpc::ServerCompletionQueue> server_cq_;

    GRPCProc grpc_proc_;
};

class GrpcServer
{
public:
    void InitServer(GRPCProc grpc_proc)
    {
        std::string server_address("127.0.0.1:50051");
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        Base::AsyncService service_;
        builder.RegisterService(&service_);


        base_cq_ = builder.AddCompletionQueue(true);
        stream_cq_ = builder.AddCompletionQueue(true);

        server_ = builder.BuildAndStart();

        new AsyncServiceImpl(&service_, base_cq_.get(), grpc_proc);
        new AsyncStreamTransmitImpl(&service_, stream_cq_.get());
        
        std::thread(&GrpcServer::HandleResponse2, this, stream_cq_.get()).detach();
        std::thread(&GrpcServer::HandleResponse1, this, base_cq_.get()).join();
        

    }

private:
    void HandleResponse1(grpc::ServerCompletionQueue* cq)
    {
        void* tag;  // uniquely identifies a request.
        bool ok;

        while (true) {

            if (!cq->Next(&tag, &ok)) 
            {

            }

            if (ok)
            {
                static_cast<AsyncServiceImpl*>(tag)->Proceed();
            }
            else
            {
                std::cout << "server_cq_->Next err" << std::endl;
            }
        }
    }
    void HandleResponse2(grpc::ServerCompletionQueue* cq)
    {
        void* tag;  // uniquely identifies a request.
        bool ok;

        while (true) {

            if (!cq->Next(&tag, &ok))
            {
                std::cout << "cq->Next error" << std::endl;
            }

            if (ok)
            {
                static_cast<AsyncStreamTransmitImpl*>(tag)->Proceed();
            }
            else
            {
                std::cout << "server_cq_->Next err" << std::endl;
            }
        }
    }

private:
    Poco::Event* event_;
    void* out_buffer_;

    std::unique_ptr<Server> server_;
    std::unique_ptr<grpc::ServerCompletionQueue> base_cq_;
    std::unique_ptr<grpc::ServerCompletionQueue> stream_cq_;

    std::vector<std::thread> threads;
};


