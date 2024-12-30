#pragma once
#include "proto/ez.pb.h"
#include "proto/ez.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include "ez_log/ez_log.hpp"

using namespace grpc;

class UnaryService final : public Communication::CallbackService
{
public:
    ServerUnaryReactor* UnaryCommunicate(CallbackServerContext* context, const Package* request, Package* response) override
    {

    }
private:

};


class ClientStreamService final : public Communication::CallbackService
{
public:
    ServerReadReactor<Package>* ClientStreamCommunicate(CallbackServerContext* context, Package* response) override 
    {
        class Recorder: public grpc::ServerReadReactor<Package>
        {
        public:
            Recorder()
            {
                StartRead(&request_);
            }
            ~Recorder() {}

            void OnReadDone(bool ok) override 
            {
                if (ok)
                {
                    const char* buffer = request_.buffer().c_str();
                    auto buffer_len = request_.length();
                    INFO("read data len:{} buffer:{}", buffer_len, buffer);
                    request_.Clear();
                    StartRead(&request_);
                }
                else
                {
                    INFO("read data not ok");
                    Finish(grpc::Status::OK);
                }
            }

            void OnDone() override {
                INFO("");
                delete this;
            }

            void OnCancel() override {
                INFO("");
            }


        private:
            Package request_;
        };

        return new Recorder();
    }
};


class ServerStreamService final : public Communication::CallbackService
{
public:
    ServerWriteReactor<Package>* ServerStreamCommunicate(CallbackServerContext* context, const Package* request) override
    {
        return nullptr;
    }

private:

};


class BidirectionStreamService final : public Communication::CallbackService
{
public:              
    ServerBidiReactor< ::Package, ::Package>* BidirectionStreamCommunicate(CallbackServerContext* context) override
    {
        return nullptr;
    }
};
