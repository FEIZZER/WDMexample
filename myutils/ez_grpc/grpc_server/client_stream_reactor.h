#pragma once
#include "connect_reactor.h"


using namespace grpc;

namespace ez {

class client_stream_reactor :public connect_reactor, public grpc::ServerReadReactor<Package>
{
public:
    client_stream_reactor(CallbackServerContext* context, Package* response);
    virtual ~client_stream_reactor();

    void disconnect();


private:
    void OnReadDone(bool ok) override;

    void OnDone() override;

    // nothing todo
    // void OnSendInitialMetadataDone(bool ok) override;
    // void OnCancel() override;
    // static void clear_reactors();

private:
    CallbackServerContext* context_;
    Package* response_;
    Package request_;

    std::mutex       status_mutex_;
    std::atomic_bool finished_;


private:
    static std::recursive_mutex mutex_;
    static std::set<uintptr_t>  reactors_;
};
}