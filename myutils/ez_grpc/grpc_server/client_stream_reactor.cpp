#include "client_stream_reactor.h"
#include "ez/log.hpp"

using namespace ez;
using namespace grpc;

client_stream_reactor::client_stream_reactor(CallbackServerContext* context, Package* response)
    :context_(context), response_(response) {

    context_->AddInitialMetadata("key", "value");
    StartSendInitialMetadata();
    StartRead(&request_);
}

client_stream_reactor::~client_stream_reactor() {
}

void client_stream_reactor::OnReadDone(bool ok) {
    if (ok) {
        const char* buffer = request_.buffer().c_str();
        auto buffer_len = request_.length();


        request_.Clear();
        StartRead(&request_);
    }
    /* when client is closed, 
       or grpc_server calling shutdown(), wait all reactors deleted, shutdown continue.
    */
    else {
        EZ_INFO("read data failed, finish this connect from server");
        disconnect();
    }
}

void client_stream_reactor::OnDone() {
    std::lock_guard<std::shared_mutex> status_lock(status_mutex_);
    status_ = reactor_status::done;
}

void client_stream_reactor::disconnect() {
    std::lock_guard<std::shared_mutex> status_lock(status_mutex_);
    if (status_ != reactor_status::done && status_ != reactor_status::finished) {
        status_ = reactor_status::finished;
        Finish(grpc::Status::OK);
    }
}