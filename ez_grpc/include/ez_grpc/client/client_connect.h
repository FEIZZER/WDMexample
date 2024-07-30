#pragma once
#include "ez_grpc/common/connect.h"

#include <memory>


namespace ez_grpc {

	class ClientConnectInterface : public ConnectInterface
	{
	public:
		ClientConnectInterface() = default;
		virtual ~ClientConnectInterface() = default;

		virtual bool Request(void* buffer, unsigned int length,
			void** out_buffer = nullptr, unsigned int* out_length = nullptr) = 0;

		virtual void DisConnect() = 0;

	private:
		
	};
}