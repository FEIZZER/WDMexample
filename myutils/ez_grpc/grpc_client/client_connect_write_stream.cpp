#include "client_connect_write_stream.h"
#include "ez/log.hpp"
using namespace ez;

client_connect_write_stream::~client_connect_write_stream()
{
	disconnect();
}

client_connect_write_stream::client_connect_write_stream(unsigned long port) 
	: client_connect_write_stream("127.0.0.1", port) {}

client_connect_write_stream::client_connect_write_stream(const std::string& ip, unsigned long port)
	: client_connect_base(ip, port), exit_(false)
{
	if (!is_connected())
	{
		EZ_ERROR("connect to {} failed", ip + ":" + std::to_string(port));
		return;
	}
	stub_->async()->ClientStreamCommunicate(&client_context_, &response_, this);
	AddHold();
	StartCall();
	write_thread_ = std::thread(&client_connect_write_stream::write_loop, this);
	
	ip_ = ip;
	port_ = port;
	target_addr_ = ip + ":" + std::to_string(port_);
}

void client_connect_write_stream::write_loop()
{
	EZ_INFO("connect to:{} start write loop...", target_addr_);
	while (!exit_.load())
	{
		if (!queue_.pop_front(request_))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}
		StartWrite(&request_);
		request_.Clear();
	}
	EZ_INFO("connect to:{} write loop exit", target_addr_);
}




bool client_connect_write_stream::request(const void* buffer, unsigned int buffer_len,
	void** out_buffer /*= nullptr*/, unsigned int* out_len /*= nullptr*/)
{
	Package request;
	request.set_buffer(buffer, buffer_len);
	request.set_length(buffer_len);
	return queue_.push_back(std::move(request));
}


void client_connect_write_stream::disconnect()
{
	EZ_INFO("disconnect to:{}", target_addr_);
	exit_.store(true);
	if (write_thread_.joinable())
	{
		write_thread_.join();
	}

	StartWritesDone();
}


void client_connect_write_stream::OnWriteDone(bool ok) {
	EZ_INFO("");
}
void client_connect_write_stream::OnDone(const grpc::Status& status) {
	EZ_INFO("");
}
void client_connect_write_stream::OnReadInitialMetadataDone(bool ok) {
	EZ_INFO("");
}
void client_connect_write_stream::OnWritesDoneDone(bool ok) {
	EZ_INFO("");
}
