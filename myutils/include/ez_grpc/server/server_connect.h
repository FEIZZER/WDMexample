#pragma once
#include "ez_grpc/connect.h"


namespace ez_grpc {

	class sever_connect : public connect
	{
	public:
		sever_connect() = default;
		virtual ~sever_connect() = default;

		/*virtual bool Read() = 0;*/

		virtual bool Write(void* buffer, int length) = 0;

		// virtual bool IsConnected() = 0

		virtual void DisConnect() = 0;

	public:

	private:

	};
}