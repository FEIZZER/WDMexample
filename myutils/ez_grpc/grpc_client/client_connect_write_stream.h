#pragma once
#include "client_connect_base.h"
#include "ez/queue.hpp"

namespace ez {

class client_connect_write_stream : public client_connect_base, public grpc::ClientWriteReactor<Package>
{
public:
	client_connect_write_stream(unsigned long port);
	client_connect_write_stream(const std::string& ip, unsigned long port);
	~client_connect_write_stream();

    bool Request(const Package& request, Package& response) override {
        return true;
    };

    bool request(const void* buffer, unsigned int buffer_len, void** out_buffer = nullptr, unsigned int* out_len = nullptr);

    void disconnect();

public:
    void OnReadInitialMetadataDone(bool ok) override;
    void OnWritesDoneDone(bool ok) override;
    void OnWriteDone(bool ok) override;
    void OnDone(const grpc::Status& s) override;
    

    // Status Await(RouteSummary* stats);
private:
    void write_loop();

private:
    std::string         ip_;
    unsigned long       port_;
    std::string         target_addr_;

    std::atomic_bool    exit_;
    Package             request_;
    Package             response_;
	queue<Package>      queue_;
    grpc::ClientContext client_context_;
    std::thread         write_thread_;
};

}