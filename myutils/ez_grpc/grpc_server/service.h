#pragma once
#include "proto/ez.pb.h"
#include "proto/ez.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "ez/log.hpp"
#include "ez/signal.hpp"
#include "connect_reactor.h"
#include "client_stream_reactor.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>

using namespace ez;
using namespace grpc;

namespace ez {
template<typename T, 
typename = std::enable_if_t<std::is_base_of_v<connect_reactor,T>>>
class base_service {
public:
    base_service() {
        start_clean();
    }
    virtual ~base_service() {
        stop_clean();
    }

protected:
    void add_connect(T* object) {
        std::lock_guard<std::shared_mutex> lock(connections_mutex_);
        connections_.insert(object);
    }
    void del_connect() {
    }

protected:
    ez::signal          exit_signal_;
    std::thread         clean_thread_;

    std::set<T*>        connections_;
    std::shared_mutex   connections_mutex_;

private:
    void start_clean() {
        clean_thread_ = std::thread(&base_service::clean, this);
    }

    void stop_clean() {
        exit_signal_.notify_all();
        if (clean_thread_.joinable()) {
            clean_thread_.join();
        }
    }

    void clean() {
        while (true) {
            if (exit_signal_.wait_for(std::chrono::seconds(5))) {
                break;
            }
            std::lock_guard<std::shared_mutex> lock(connections_mutex_);
            for (auto connect : connections_) {
                if (connect->get_status() == reactor_status::done) {
                    delete connect;
                    connections_.erase(connect);
                }
            }
        }
    }
};

class UnaryService final : public Communication::CallbackService
{
public:
    ServerUnaryReactor* UnaryCommunicate(CallbackServerContext* context, const Package* request, Package* response) override
    {

    }
private:

};



class client_stream_service : public base_service<client_stream_reactor>, public Communication::CallbackService
{
public:
    virtual ~client_stream_service() {
    }
    client_stream_service() = default;

    ServerReadReactor<Package>* ClientStreamCommunicate(CallbackServerContext* context, Package* response) override {
        auto connect = new client_stream_reactor(context, response);
        add_connect(connect);
        return connect;
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

}