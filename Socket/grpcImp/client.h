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

#include <grpcpp/grpcpp.h>
#include "../proto/helloworld.grpc.pb.h"
#include "../proto/base.grpc.pb.h"

#include "work_thread.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;


class AsyncRequestImpl : public WorkThread
{
public:
    AsyncRequestImpl(grpc::CompletionQueue* cq):
        cq_(cq) {}
    virtual ~AsyncRequestImpl() {}

public:
    // 客户端异步响应读取器
    std::unique_ptr<grpc::ClientAsyncResponseReader<BaseReply>> response_reader_;
    ClientContext context_;
    BaseReply reply_;
    Status status_;

private:
    grpc::CompletionQueue* cq_;

};



class ClientImpl : public WorkThread
{
public:
    ClientImpl(std::shared_ptr<Channel> channel)
        : stub_(Base::NewStub(channel)) {}

    std::string BaseTransmit(const char* buf, unsigned __int32 length)
    {
        BaseRequest request;
        BaseReply reply;
        ClientContext context;

        request.set_version(1);
        request.set_length(length);
        request.set_buffer(buf, length);

        Status status = stub_->BaseTransmit(&context, request, &reply);

        if (!status.ok())
        {
            // error
            std::cout
                << "err code: " << status.error_code() << std::endl
                << "err message: " << status.error_message() << std::endl
                << "err detail: " << status.error_details() << std::endl
                << std::endl;
        }
        return reply.buffer();
    }

    std::string AsyncTransmit(const char* buf, unsigned __int32 length)
    {
        BaseRequest request;
        BaseReply reply;
        ClientContext context;

        request.set_version(1);
        request.set_length(length);
        request.set_buffer(buf, length);

        AsyncRequestImpl* call = new AsyncRequestImpl(&cq_);
        call->response_reader_ =
            stub_->PrepareAsyncBaseTransmit(&call->context_, request, &cq_);

        call->response_reader_->StartCall();

        call->response_reader_->Finish(&call->reply_, &call->status_, static_cast<void*>(call));
        return "";
    }

    std::string ServerStreamTransmit(const char* buf, unsigned __int32 length)
    {
        ClientContext context;
        BaseRequest request;
        request.set_buffer(buf, length);
        auto client_reader(stub_->ServerStreamTransmit(&context, request));

        BaseReply reply;
        while (client_reader->Read(&reply))
        {
            std::cout << "client received stream: " << reply.buffer().c_str() << std::endl;
        }
        std::cout << "client exit" << std::endl;
        return "";
    }

    std::string ClientStreamTransmit(const char* buf, unsigned __int32 length)
    {
        ClientContext context;
        BaseReply reply;
        auto client_writer(stub_->ClientStreamTransmit(&context, &reply));

        BaseRequest request;
        request.set_buffer("client stream1", sizeof("client stream1"));
        while (true)
        {
            if (!client_writer->Write(request))
            {
                break;
            }
            Sleep(2000);
        }
        client_writer->WritesDone();
        client_writer->Finish();
        std::cout << "reply:" << reply.buffer().c_str() << std::endl;

        return "";
    }
    std::string StreamTransmit(const char* buf, unsigned __int32 length)
    {
        std::cout << "In StreamTransmit" << std::endl;
        ClientContext context;
        auto stream(stub_->StreamTransmit(&context));

        BaseRequest request;
        BaseReply reply;

        while (true)
        {
            if (!stream->Write(request))
            {
                std::cout << "stream write failed";
                break;
            }
            if (!stream->Read(&reply))
            {
                std::cout << "stream read failed";
                break;
            }
            else
            {
                std::cout << "stream read: " << reply.buffer().c_str() << std::endl;
            }
            Sleep(2000);
        }

        return "";
    }

    bool LoopReceiveResponse()
    {
        return WorkThread::InitThread();
    }

    void run()
    {
        std::cout << "In run()" << std::endl;
        void* tag;
        bool ok = false;

        while (cq_.Next(&tag, &ok))
        {
            AsyncRequestImpl* call = static_cast<AsyncRequestImpl*>(tag);
            if (!ok)
            {
                delete call;
                continue;
            }

            if (call->status_.ok())
            {
                std::cout << "received data:"
                    << call->reply_.buffer().c_str()
                    << std::endl;
            }
            else
            {
                std::cout << (ULONG_PTR)call << "err" << std::endl;
            }
            delete call;
        }
    }
private:
    std::unique_ptr<Base::Stub> stub_;
    grpc::CompletionQueue cq_;
};

class PersistentConnect : public WorkThread
{
public:
    PersistentConnect(std::string target)
    {
        stub_ = Base::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
        stream_ = stub_->StreamTransmit(&ctx_);
    };

    static std::unique_ptr<Base::Stub> CreateTemporaryConnect(std::string target)
    {
        return Base::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()));
    }

    bool Request(void* buf, unsigned __int32 length)
    {
        request_.set_buffer(buf, length);
        return stream_->Write(request_);
    }
    void CreateStream()
    {
        auto stream2(stub_->StreamTransmit(&ctx_));
    }
    void run()
    {
        while (true)
        {
            if (!stream_->Read(&reply_))
            {
                std::cout << "Server Done" << std::endl;
                break;
            }
            std::cout << "Read: " << reply_.buffer().c_str() << std::endl;
        }
    };
private:
    std::unique_ptr<Base::Stub> stub_ = nullptr;
    std::unique_ptr<grpc::ClientReaderWriter<BaseRequest, BaseReply>> stream_ = nullptr;
    ClientContext ctx_;
    BaseRequest request_;

    BaseReply reply_;
};

class GrpcClient
{
public:
    static std::unique_ptr<PersistentConnect> CreatePersistentConnect(std::string target)
    {
        auto newConnect = std::make_unique<PersistentConnect>(target);
        newConnect->InitThread();
        return newConnect;
    }

    static bool Request(const void* buffer, unsigned __int32 length)
    {
        auto stub = PersistentConnect::CreateTemporaryConnect("127.0.0.1:50051");
        ClientContext context;
        BaseRequest request;
        BaseReply reply;
        request.set_buffer(buffer, length);
        std::cout << "before stub->BaseTransmit" << std::endl;
        Status status = stub->BaseTransmit(&context, request, &reply);
        std::cout << "Get Reply:" << reply.buffer().c_str() << std::endl;
        return true;
    }
};