#include <grpcpp/grpcpp.h>

class Test : public grpc::CallbackGenericService
{
public:
	Test();
	~Test();

private:

};
