// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: services/authservice/authservice.proto
#ifndef GRPC_services_2fauthservice_2fauthservice_2eproto__INCLUDED
#define GRPC_services_2fauthservice_2fauthservice_2eproto__INCLUDED

#include "services/authservice/authservice.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

namespace authservice {

class AuthService final {
 public:
  static constexpr char const* service_full_name() {
    return "authservice.AuthService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Login(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::authservice::LoginResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>> AsyncLogin(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>>(AsyncLoginRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>> PrepareAsyncLogin(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>>(PrepareAsyncLoginRaw(context, request, cq));
    }
    virtual ::grpc::Status Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::authservice::SignupResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>> AsyncSignup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>>(AsyncSignupRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>> PrepareAsyncSignup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>>(PrepareAsyncSignupRaw(context, request, cq));
    }
    virtual ::grpc::Status Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::authservice::LogoutResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>> AsyncLogout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>>(AsyncLogoutRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>> PrepareAsyncLogout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>>(PrepareAsyncLogoutRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void Login(::grpc::ClientContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Login(::grpc::ClientContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>* AsyncLoginRaw(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LoginResponse>* PrepareAsyncLoginRaw(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>* AsyncSignupRaw(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::SignupResponse>* PrepareAsyncSignupRaw(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>* AsyncLogoutRaw(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::authservice::LogoutResponse>* PrepareAsyncLogoutRaw(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status Login(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::authservice::LoginResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>> AsyncLogin(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>>(AsyncLoginRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>> PrepareAsyncLogin(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>>(PrepareAsyncLoginRaw(context, request, cq));
    }
    ::grpc::Status Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::authservice::SignupResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>> AsyncSignup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>>(AsyncSignupRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>> PrepareAsyncSignup(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>>(PrepareAsyncSignupRaw(context, request, cq));
    }
    ::grpc::Status Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::authservice::LogoutResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>> AsyncLogout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>>(AsyncLogoutRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>> PrepareAsyncLogout(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>>(PrepareAsyncLogoutRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void Login(::grpc::ClientContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response, std::function<void(::grpc::Status)>) override;
      void Login(::grpc::ClientContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response, std::function<void(::grpc::Status)>) override;
      void Signup(::grpc::ClientContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response, std::function<void(::grpc::Status)>) override;
      void Logout(::grpc::ClientContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
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
    ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>* AsyncLoginRaw(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::authservice::LoginResponse>* PrepareAsyncLoginRaw(::grpc::ClientContext* context, const ::authservice::LoginRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>* AsyncSignupRaw(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::authservice::SignupResponse>* PrepareAsyncSignupRaw(::grpc::ClientContext* context, const ::authservice::SignupRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>* AsyncLogoutRaw(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::authservice::LogoutResponse>* PrepareAsyncLogoutRaw(::grpc::ClientContext* context, const ::authservice::LogoutRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Login_;
    const ::grpc::internal::RpcMethod rpcmethod_Signup_;
    const ::grpc::internal::RpcMethod rpcmethod_Logout_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Login(::grpc::ServerContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response);
    virtual ::grpc::Status Signup(::grpc::ServerContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response);
    virtual ::grpc::Status Logout(::grpc::ServerContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Login() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestLogin(::grpc::ServerContext* context, ::authservice::LoginRequest* request, ::grpc::ServerAsyncResponseWriter< ::authservice::LoginResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Signup() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSignup(::grpc::ServerContext* context, ::authservice::SignupRequest* request, ::grpc::ServerAsyncResponseWriter< ::authservice::SignupResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Logout() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestLogout(::grpc::ServerContext* context, ::authservice::LogoutRequest* request, ::grpc::ServerAsyncResponseWriter< ::authservice::LogoutResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Login<WithAsyncMethod_Signup<WithAsyncMethod_Logout<Service > > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Login() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::authservice::LoginRequest, ::authservice::LoginResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::authservice::LoginRequest* request, ::authservice::LoginResponse* response) { return this->Login(context, request, response); }));}
    void SetMessageAllocatorFor_Login(
        ::grpc::MessageAllocator< ::authservice::LoginRequest, ::authservice::LoginResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::authservice::LoginRequest, ::authservice::LoginResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Login(
      ::grpc::CallbackServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Signup() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::authservice::SignupRequest, ::authservice::SignupResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::authservice::SignupRequest* request, ::authservice::SignupResponse* response) { return this->Signup(context, request, response); }));}
    void SetMessageAllocatorFor_Signup(
        ::grpc::MessageAllocator< ::authservice::SignupRequest, ::authservice::SignupResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::authservice::SignupRequest, ::authservice::SignupResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Signup(
      ::grpc::CallbackServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Logout() {
      ::grpc::Service::MarkMethodCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::authservice::LogoutRequest, ::authservice::LogoutResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::authservice::LogoutRequest* request, ::authservice::LogoutResponse* response) { return this->Logout(context, request, response); }));}
    void SetMessageAllocatorFor_Logout(
        ::grpc::MessageAllocator< ::authservice::LogoutRequest, ::authservice::LogoutResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(2);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::authservice::LogoutRequest, ::authservice::LogoutResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Logout(
      ::grpc::CallbackServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_Login<WithCallbackMethod_Signup<WithCallbackMethod_Logout<Service > > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Login() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Signup() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Logout() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Login() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestLogin(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Signup() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSignup(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Logout() {
      ::grpc::Service::MarkMethodRaw(2);
    }
    ~WithRawMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestLogout(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Login() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Login(context, request, response); }));
    }
    ~WithRawCallbackMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Login(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Signup() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Signup(context, request, response); }));
    }
    ~WithRawCallbackMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Signup(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Logout() {
      ::grpc::Service::MarkMethodRawCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Logout(context, request, response); }));
    }
    ~WithRawCallbackMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* Logout(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Login() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::authservice::LoginRequest, ::authservice::LoginResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::authservice::LoginRequest, ::authservice::LoginResponse>* streamer) {
                       return this->StreamedLogin(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Login() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Login(::grpc::ServerContext* /*context*/, const ::authservice::LoginRequest* /*request*/, ::authservice::LoginResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedLogin(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::authservice::LoginRequest,::authservice::LoginResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Signup : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Signup() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::authservice::SignupRequest, ::authservice::SignupResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::authservice::SignupRequest, ::authservice::SignupResponse>* streamer) {
                       return this->StreamedSignup(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Signup() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Signup(::grpc::ServerContext* /*context*/, const ::authservice::SignupRequest* /*request*/, ::authservice::SignupResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSignup(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::authservice::SignupRequest,::authservice::SignupResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Logout : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Logout() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::internal::StreamedUnaryHandler<
          ::authservice::LogoutRequest, ::authservice::LogoutResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::authservice::LogoutRequest, ::authservice::LogoutResponse>* streamer) {
                       return this->StreamedLogout(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Logout() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Logout(::grpc::ServerContext* /*context*/, const ::authservice::LogoutRequest* /*request*/, ::authservice::LogoutResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedLogout(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::authservice::LogoutRequest,::authservice::LogoutResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Login<WithStreamedUnaryMethod_Signup<WithStreamedUnaryMethod_Logout<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Login<WithStreamedUnaryMethod_Signup<WithStreamedUnaryMethod_Logout<Service > > > StreamedService;
};

}  // namespace authservice


#endif  // GRPC_services_2fauthservice_2fauthservice_2eproto__INCLUDED
