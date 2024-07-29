// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: base.proto
#ifndef GRPC_base_2eproto__INCLUDED
#define GRPC_base_2eproto__INCLUDED

#include "base.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

class Base final {
 public:
  static constexpr char const* service_full_name() {
    return "Base";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::BaseReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>> AsyncBaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>>(AsyncBaseTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>> PrepareAsyncBaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>>(PrepareAsyncBaseTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::BaseReply>> ServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::BaseReply>>(ServerStreamTransmitRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::BaseReply>> AsyncServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::BaseReply>>(AsyncServerStreamTransmitRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::BaseReply>> PrepareAsyncServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::BaseReply>>(PrepareAsyncServerStreamTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientWriterInterface< ::BaseRequest>> ClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::BaseRequest>>(ClientStreamTransmitRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::BaseRequest>> AsyncClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::BaseRequest>>(AsyncClientStreamTransmitRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::BaseRequest>> PrepareAsyncClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::BaseRequest>>(PrepareAsyncClientStreamTransmitRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::BaseRequest, ::BaseReply>> StreamTransmit(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::BaseRequest, ::BaseReply>>(StreamTransmitRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>> AsyncStreamTransmit(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>>(AsyncStreamTransmitRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>> PrepareAsyncStreamTransmit(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>>(PrepareAsyncStreamTransmitRaw(context, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::BaseReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::BaseReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void ServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::grpc::ClientReadReactor< ::BaseReply>* reactor) = 0;
      virtual void ClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::ClientWriteReactor< ::BaseRequest>* reactor) = 0;
      virtual void StreamTransmit(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::BaseRequest,::BaseReply>* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>* AsyncBaseTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::BaseReply>* PrepareAsyncBaseTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::BaseReply>* ServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::BaseReply>* AsyncServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::BaseReply>* PrepareAsyncServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientWriterInterface< ::BaseRequest>* ClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::BaseRequest>* AsyncClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::BaseRequest>* PrepareAsyncClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderWriterInterface< ::BaseRequest, ::BaseReply>* StreamTransmitRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>* AsyncStreamTransmitRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::BaseRequest, ::BaseReply>* PrepareAsyncStreamTransmitRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::BaseReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BaseReply>> AsyncBaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BaseReply>>(AsyncBaseTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BaseReply>> PrepareAsyncBaseTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::BaseReply>>(PrepareAsyncBaseTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::BaseReply>> ServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::BaseReply>>(ServerStreamTransmitRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::BaseReply>> AsyncServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::BaseReply>>(AsyncServerStreamTransmitRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::BaseReply>> PrepareAsyncServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::BaseReply>>(PrepareAsyncServerStreamTransmitRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientWriter< ::BaseRequest>> ClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::BaseRequest>>(ClientStreamTransmitRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::BaseRequest>> AsyncClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::BaseRequest>>(AsyncClientStreamTransmitRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::BaseRequest>> PrepareAsyncClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::BaseRequest>>(PrepareAsyncClientStreamTransmitRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderWriter< ::BaseRequest, ::BaseReply>> StreamTransmit(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::BaseRequest, ::BaseReply>>(StreamTransmitRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>> AsyncStreamTransmit(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>>(AsyncStreamTransmitRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>> PrepareAsyncStreamTransmit(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>>(PrepareAsyncStreamTransmitRaw(context, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::BaseReply* response, std::function<void(::grpc::Status)>) override;
      void BaseTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::BaseReply* response, ::grpc::ClientUnaryReactor* reactor) override;
      void ServerStreamTransmit(::grpc::ClientContext* context, const ::BaseRequest* request, ::grpc::ClientReadReactor< ::BaseReply>* reactor) override;
      void ClientStreamTransmit(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::ClientWriteReactor< ::BaseRequest>* reactor) override;
      void StreamTransmit(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::BaseRequest,::BaseReply>* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::BaseReply>* AsyncBaseTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::BaseReply>* PrepareAsyncBaseTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReader< ::BaseReply>* ServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request) override;
    ::grpc::ClientAsyncReader< ::BaseReply>* AsyncServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::BaseReply>* PrepareAsyncServerStreamTransmitRaw(::grpc::ClientContext* context, const ::BaseRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientWriter< ::BaseRequest>* ClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response) override;
    ::grpc::ClientAsyncWriter< ::BaseRequest>* AsyncClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncWriter< ::BaseRequest>* PrepareAsyncClientStreamTransmitRaw(::grpc::ClientContext* context, ::BaseReply* response, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReaderWriter< ::BaseRequest, ::BaseReply>* StreamTransmitRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>* AsyncStreamTransmitRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::BaseRequest, ::BaseReply>* PrepareAsyncStreamTransmitRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_BaseTransmit_;
    const ::grpc::internal::RpcMethod rpcmethod_ServerStreamTransmit_;
    const ::grpc::internal::RpcMethod rpcmethod_ClientStreamTransmit_;
    const ::grpc::internal::RpcMethod rpcmethod_StreamTransmit_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status BaseTransmit(::grpc::ServerContext* context, const ::BaseRequest* request, ::BaseReply* response);
    virtual ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* context, const ::BaseRequest* request, ::grpc::ServerWriter< ::BaseReply>* writer);
    virtual ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerReader< ::BaseRequest>* reader, ::BaseReply* response);
    virtual ::grpc::Status StreamTransmit(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBaseTransmit(::grpc::ServerContext* context, ::BaseRequest* request, ::grpc::ServerAsyncResponseWriter< ::BaseReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestServerStreamTransmit(::grpc::ServerContext* context, ::BaseRequest* request, ::grpc::ServerAsyncWriter< ::BaseReply>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_ClientStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_ClientStreamTransmit() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_ClientStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::BaseRequest>* /*reader*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestClientStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::BaseReply, ::BaseRequest>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(2, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_StreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_StreamTransmit() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_StreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::BaseReply, ::BaseRequest>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(3, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_BaseTransmit<WithAsyncMethod_ServerStreamTransmit<WithAsyncMethod_ClientStreamTransmit<WithAsyncMethod_StreamTransmit<Service > > > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::BaseRequest, ::BaseReply>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::BaseRequest* request, ::BaseReply* response) { return this->BaseTransmit(context, request, response); }));}
    void SetMessageAllocatorFor_BaseTransmit(
        ::grpc::MessageAllocator< ::BaseRequest, ::BaseReply>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::BaseRequest, ::BaseReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* BaseTransmit(
      ::grpc::CallbackServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackServerStreamingHandler< ::BaseRequest, ::BaseReply>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::BaseRequest* request) { return this->ServerStreamTransmit(context, request); }));
    }
    ~WithCallbackMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerWriteReactor< ::BaseReply>* ServerStreamTransmit(
      ::grpc::CallbackServerContext* /*context*/, const ::BaseRequest* /*request*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_ClientStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_ClientStreamTransmit() {
      ::grpc::Service::MarkMethodCallback(2,
          new ::grpc::internal::CallbackClientStreamingHandler< ::BaseRequest, ::BaseReply>(
            [this](
                   ::grpc::CallbackServerContext* context, ::BaseReply* response) { return this->ClientStreamTransmit(context, response); }));
    }
    ~WithCallbackMethod_ClientStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::BaseRequest>* /*reader*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerReadReactor< ::BaseRequest>* ClientStreamTransmit(
      ::grpc::CallbackServerContext* /*context*/, ::BaseReply* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_StreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_StreamTransmit() {
      ::grpc::Service::MarkMethodCallback(3,
          new ::grpc::internal::CallbackBidiHandler< ::BaseRequest, ::BaseReply>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->StreamTransmit(context); }));
    }
    ~WithCallbackMethod_StreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::BaseRequest, ::BaseReply>* StreamTransmit(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  typedef WithCallbackMethod_BaseTransmit<WithCallbackMethod_ServerStreamTransmit<WithCallbackMethod_ClientStreamTransmit<WithCallbackMethod_StreamTransmit<Service > > > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_ClientStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_ClientStreamTransmit() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_ClientStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::BaseRequest>* /*reader*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_StreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_StreamTransmit() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_StreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBaseTransmit(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestServerStreamTransmit(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncWriter< ::grpc::ByteBuffer>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_ClientStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_ClientStreamTransmit() {
      ::grpc::Service::MarkMethodRaw(2);
    }
    ~WithRawMethod_ClientStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::BaseRequest>* /*reader*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestClientStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(2, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_StreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_StreamTransmit() {
      ::grpc::Service::MarkMethodRaw(3);
    }
    ~WithRawMethod_StreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(3, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->BaseTransmit(context, request, response); }));
    }
    ~WithRawCallbackMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* BaseTransmit(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackServerStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const::grpc::ByteBuffer* request) { return this->ServerStreamTransmit(context, request); }));
    }
    ~WithRawCallbackMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerWriteReactor< ::grpc::ByteBuffer>* ServerStreamTransmit(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_ClientStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_ClientStreamTransmit() {
      ::grpc::Service::MarkMethodRawCallback(2,
          new ::grpc::internal::CallbackClientStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, ::grpc::ByteBuffer* response) { return this->ClientStreamTransmit(context, response); }));
    }
    ~WithRawCallbackMethod_ClientStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ClientStreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReader< ::BaseRequest>* /*reader*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerReadReactor< ::grpc::ByteBuffer>* ClientStreamTransmit(
      ::grpc::CallbackServerContext* /*context*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_StreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_StreamTransmit() {
      ::grpc::Service::MarkMethodRawCallback(3,
          new ::grpc::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->StreamTransmit(context); }));
    }
    ~WithRawCallbackMethod_StreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamTransmit(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::BaseReply, ::BaseRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* StreamTransmit(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_BaseTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_BaseTransmit() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::BaseRequest, ::BaseReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::BaseRequest, ::BaseReply>* streamer) {
                       return this->StreamedBaseTransmit(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_BaseTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status BaseTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::BaseReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedBaseTransmit(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::BaseRequest,::BaseReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_BaseTransmit<Service > StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_ServerStreamTransmit : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithSplitStreamingMethod_ServerStreamTransmit() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::SplitServerStreamingHandler<
          ::BaseRequest, ::BaseReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerSplitStreamer<
                     ::BaseRequest, ::BaseReply>* streamer) {
                       return this->StreamedServerStreamTransmit(context,
                         streamer);
                  }));
    }
    ~WithSplitStreamingMethod_ServerStreamTransmit() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status ServerStreamTransmit(::grpc::ServerContext* /*context*/, const ::BaseRequest* /*request*/, ::grpc::ServerWriter< ::BaseReply>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedServerStreamTransmit(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::BaseRequest,::BaseReply>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_ServerStreamTransmit<Service > SplitStreamedService;
  typedef WithStreamedUnaryMethod_BaseTransmit<WithSplitStreamingMethod_ServerStreamTransmit<Service > > StreamedService;
};


#endif  // GRPC_base_2eproto__INCLUDED
