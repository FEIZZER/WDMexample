#include "base.grpc.pb.h"

#include <string>
#include <memory>
#include <map>

#include <grpcpp/grpcpp.h>


namespace ez_grpc {

	class Client
	{
	public:
		Client() = default;
		virtual ~Client() = default;

		bool CreateConnect();

	private:
		bool CreateConnectInternal(const std::string& key, );

	private:

	};
}
