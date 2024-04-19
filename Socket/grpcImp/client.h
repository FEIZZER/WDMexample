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
        client_writer->Write(request);
        client_writer->WritesDone();
        client_writer->Finish();

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