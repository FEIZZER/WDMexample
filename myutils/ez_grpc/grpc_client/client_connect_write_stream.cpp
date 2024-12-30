#include "client_connect_write_stream.h"
#include "ez_log/ez_log.hpp"
using namespace ez_grpc;

client_connect_write_stream::~client_connect_write_stream()
{
	
}

client_connect_write_stream::client_connect_write_stream(unsigned long port) 
	: client_connect_write_stream("0.0.0.0", port) {}

client_connect_write_stream::client_connect_write_stream(const std::string& ip, unsigned long port)
	: client_connect_base(ip, port), exit_(false)
{
	if (!is_connected())
	{
		return;
	}

	stub_->async()->ClientStreamCommunicate(&client_context_, &response_, this);
	AddHold();
	NextWrite();
	StartCall();
}


void client_connect_write_stream::OnWriteDone(bool ok) {
	NextWrite();
}
void client_connect_write_stream::OnDone(const grpc::Status& status) {
	INFO("");
}

bool client_connect_write_stream::request(const void* buffer, unsigned int buffer_len,
	void** out_buffer /*= nullptr*/, unsigned int* out_len /*= nullptr*/)
{
	Package request;
	request.set_buffer(buffer, buffer_len);
	request.set_length(buffer_len);
	return queue_.push_back(request);
}