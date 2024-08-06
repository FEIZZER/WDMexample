#pragma once
#include "ez_grpc/client/client_connect.h"

#include <memory>

#include "base.grpc.pb.h"



namespace ez_grpc {

	class ClientConnectInternal : public ez_grpc::ClientConnectInterface
	{
	public:
		ClientConnectInternal(const std::string& strIp, int port);
		virtual ~ClientConnectInternal();

		bool IsConnected();

		bool Request(void* buffer, unsigned int length, 
			void** out_buffer = nullptr, unsigned int* out_length = nullptr);

		void DisConnect();

	private:

		std::unique_ptr<Base::Stub> stub_;

	};


	class ClientConnectStream : public ez_grpc::ClientConnectInterface
	{
	public:
		ClientConnectStream(const std::string& strIp, int port);
		~ClientConnectStream() = default;

		bool Request(void* buffer, unsigned int length,
			void** out_buffer = nullptr, unsigned int* out_length = nullptr);

		bool IsConnected();

		void DisConnect();


	private:
		std::unique_ptr<Base::Stub> stub_;
		grpc::ClientContext context_;
		std::unique_ptr<grpc::ClientReaderWriter<BaseRequest, BaseReply>> stream_;
	};

}