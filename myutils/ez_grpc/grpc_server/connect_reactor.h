#pragma once
#include "proto/ez.pb.h"
#include "proto/ez.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <mutex>
#include <shared_mutex>

namespace ez {

enum class reactor_status {
	created = 0,
	connected,
	finished,
	done,
};

class connect_reactor {
public:
	connect_reactor();
	virtual ~connect_reactor();

	void set_status(reactor_status& status);
	reactor_status get_status();

protected:
	reactor_status		status_;
	std::shared_mutex	status_mutex_;
};

}