#include <string>

namespace grpc_cpp_test {
	class BasicServer
	{
	public:
		BasicServer();
		virtual ~BasicServer();

		int BuildServer();

		bool BuildServer(const int port);

		bool BuildServer(const std::string& ip, const int port);

		bool BuildServer(const std::string& ip, const std::string& port);

	private:
		bool BuildServerInternal(const std::string& addr_uri);

	private:
		int local_port_;
	};
}