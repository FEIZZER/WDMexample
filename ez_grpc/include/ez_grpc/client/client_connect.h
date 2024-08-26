#pragma once
#include "ez_grpc/common/connect.h"

#include <memory>


namespace ez_grpc {

	class ClientConnect : public Connect
	{
	public:
		ClientConnect() = default;
		virtual ~ClientConnect() = default;

		virtual bool Request(void* buffer, unsigned int length,
			void** out_buffer = nullptr, unsigned int* out_length = nullptr) = 0;

		virtual bool IsConnected() = 0;

		virtual void DisConnect() = 0;

	private:
		
	};
}