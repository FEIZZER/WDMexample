#include "base.grpc.pb.h"


namespace grpc_cpp_test 
{

	class BasicServiceImpl final : public Base::Service
	{
	public:
		BasicServiceImpl() = default;
		virtual ~BasicServiceImpl() = default;

		grpc::Status BaseTransmit(grpc::ServerContext* context, const ::BaseRequest* request, BaseReply* response) override;

	private:

	};

}