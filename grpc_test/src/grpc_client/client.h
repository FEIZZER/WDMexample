#include "base.grpc.pb.h"

#include <grpcpp/grpcpp.h>

namespace grpc_cpp_test {

	class Client
	{
	public:
		Client() = default;
		virtual ~Client() = default;

		bool Request(int port, const void* buffer, const int length);

	private:
		std::unique_ptr<Base::Stub> stub_;
	};
}
