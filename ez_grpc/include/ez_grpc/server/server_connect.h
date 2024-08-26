#pragma once
#include "ez_grpc/common/connect.h"

#include <memory>

namespace ez_grpc {

	class ServerConnect : public Connect
	{
	public:
		ServerConnect() = default;
		virtual ~ServerConnect() = default;

		/*virtual bool Read() = 0;

		virtual bool Write() = 0;*/

		// virtual bool IsConnected() = 0

		virtual void DisConnect() = 0;

	private:

	};
}