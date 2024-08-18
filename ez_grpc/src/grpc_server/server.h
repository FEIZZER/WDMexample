#include "service.h"

#include "grpc++/grpc++.h"

#include <memory>
#include <string>

namespace ez_grpc {

	class Server
	{
	public:
		Server();
		virtual ~Server();

		int BuildServer(ez_request_handler handler);

		bool BuildServer(const int port, ez_request_handler handler);

		bool BuildServer(const std::string& ip, const int port);

		bool BuildServer(const std::string& ip, const std::string& port);

	private:

		bool BuildServerInternal(const std::string& addr_uri, int& selected_port, ez_request_handler handler);

	private:

		int local_port_;

		BasicServiceImpl* basic_service_;
		std::unique_ptr<grpc::Server> server_;
	};
}