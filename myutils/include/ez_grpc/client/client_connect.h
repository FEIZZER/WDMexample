#pragma once
#include "ez_grpc/connect.h"

#include <memory>


namespace ez_grpc {

	class client_connect : public connect
	{
	public:
		client_connect() = default;
		virtual ~client_connect() = default;

		virtual bool Request(void* buffer, unsigned int length,
			void** out_buffer = nullptr, unsigned int* out_length = nullptr) = 0;

		virtual bool IsConnected() = 0;

		virtual void DisConnect() = 0;

	private:
		
	};
}