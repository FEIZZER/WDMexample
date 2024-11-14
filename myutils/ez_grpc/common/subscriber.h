#pragma once
#include "ez_grpc/common/subscriber.h"

#include <mutex>
#include <shared_mutex>
#include <set>


namespace ez_grpc {
	class SubscriberImpl: public Subscriber{
	public:
		SubscriberImpl() = default;

		~SubscriberImpl();

		virtual bool AddHandler(std::shared_ptr<Handler> handler);

		virtual bool RemoveHandler(std::shared_ptr<Handler> handler);

		virtual bool ClearHandler();

		virtual void Notify();

	private:
		std::shared_mutex shared_mutex_;
		std::set<std::shared_ptr<ez_grpc::Handler>> handlers_;

	};
}