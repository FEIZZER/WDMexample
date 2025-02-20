#pragma once
#include <atomic>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "proto/ez.pb.h"
#include "proto/ez.grpc.pb.h"


namespace ez {

class client_connect_base
{
public:
	client_connect_base(unsigned long port);
	client_connect_base(const std::string& ip, unsigned long port);
	virtual ~client_connect_base();

	bool is_connected();

	virtual bool Request(const Package& request, Package& reply);

protected:
	std::atomic_bool	connected_;
	std::string			target_ip_;
	unsigned long		target_port_;
	
	std::unique_ptr<Communication::Stub> stub_;
};

}