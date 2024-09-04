#pragma once
#include "ez_grpc/common/handler.h"

#include <memory>

namespace ez_grpc {

	class Subscriber 
	{
	public:
		virtual bool AddHandler(std::shared_ptr<Handler> handler) = 0;

		virtual bool RemoveHandler(std::shared_ptr<Handler> handler) = 0;

		virtual bool ClearHandler() = 0;

		virtual void Notify() = 0;
	};

}
