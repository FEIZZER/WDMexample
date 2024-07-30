#include "base.grpc.pb.h"


namespace ez_grpc 
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