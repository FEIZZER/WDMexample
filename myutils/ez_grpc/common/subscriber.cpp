#include "subscriber.h"

using namespace ez_grpc;

SubscriberImpl::~SubscriberImpl()
{
	ClearHandler();
}

bool SubscriberImpl::AddHandler(std::shared_ptr<Handler> handler)
{
	std::unique_lock<std::shared_mutex> lock;
	handlers_.insert(std::move(handler));
}

bool SubscriberImpl::RemoveHandler(std::shared_ptr<Handler> handler)
{
	std::unique_lock<std::shared_mutex> lock;
	handlers_.erase(handler);
}

bool SubscriberImpl::ClearHandler()
{
	std::unique_lock<std::shared_mutex> lock;
	handlers_.clear();
	return true;
}

void SubscriberImpl::Notify()
{
	std::shared_lock<std::shared_mutex> lock;

	for (auto handler : handlers_)
	{
	}

}