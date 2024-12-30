#pragma once
#include "client_connect_base.h"
#include "ez_queue/ez_queue.h"

namespace ez_grpc {

class client_connect_write_stream : public client_connect_base, public grpc::ClientWriteReactor<Package>
{
public:
	client_connect_write_stream(unsigned long port);
	client_connect_write_stream(const std::string& ip, unsigned long port);
	~client_connect_write_stream();

	bool Request(const Package& request, Package& response) override;

    bool request(const void* buffer, unsigned int buffer_len, void** out_buffer = nullptr, unsigned int* out_len = nullptr);

    void disconnect();

public:
    void OnWriteDone(bool ok) override;
    void OnDone(const grpc::Status& s) override;
    
    // Status Await(RouteSummary* stats);
private:
    void NextWrite() {
        if (exit_.load()) {
            StartWritesDone();
            return; 
        }

        request_.Clear();
        while (!queue_.pop_front(request_)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        StartWrite(&request_);
    }
private:
    std::atomic_bool    exit_;
    Package             request_;
    Package             response_;
	ez_queue<Package>   queue_;
    grpc::ClientContext client_context_;
};

}