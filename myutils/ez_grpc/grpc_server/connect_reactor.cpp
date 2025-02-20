#include "connect_reactor.h"

using namespace ez;

connect_reactor::connect_reactor(){

}

connect_reactor::~connect_reactor(){

}

void connect_reactor::set_status(reactor_status& status){
	std::lock_guard<std::shared_mutex> lock(status_mutex_);
	status_ = status;
}

reactor_status connect_reactor::get_status(){
	std::shared_lock<std::shared_mutex> lock(status_mutex_);
	return status_;
}