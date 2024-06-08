#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <grpcpp/grpcpp.h>
#include <memory>
#include <fstream>
#include <string>
#include <filesystem>
#include <string_view>

#include "components/database/connection/database-connector.hpp"

#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.hpp"
#include "services/chat_service/chat_service.pb.h"
#include "services/profile_service/profile.grpc.pb.h"
#include "services/profile_service/profile.pb.h"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "common/common.pb.h"
#include "common/common.grpc.pb.h"

std::string ReadFile(const std::filesystem::path &path) {
    auto cert_path = std::filesystem::current_path().parent_path() / path;
    std::ifstream file(cert_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << cert_path << std::endl;
        std::exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::shared_ptr<grpc::ChannelCredentials> GetCredentials() {
  grpc::SslCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = ReadFile("certs/ca.crt");
  ssl_opts.pem_private_key = ReadFile("certs/client.key");
  ssl_opts.pem_cert_chain = ReadFile("certs/client.crt");
  return grpc::SslCredentials(ssl_opts);
}

const std::string server_address = "127.0.0.1:50051";
struct testClient {
    std::shared_ptr<grpc::Channel> channel;
    std::shared_ptr<authservice::AuthService::Stub> authservice_stub;
    std::shared_ptr<profile::ProfileService::Stub> profile_stub;
    testClient(std::shared_ptr<grpc::Channel> channel) : channel(channel),
     authservice_stub(authservice::AuthService::NewStub(channel)),
    profile_stub(profile::ProfileService::NewStub(channel))
     {}
};

testClient client(grpc::CreateChannel(server_address, GetCredentials()));

struct testConnector : ::testing::Test {
    static const std::string host;
    static const database::connector connector;
};

const std::string testConnector::host = "postgres://root:strongpassword@localhost:5432/duende";
const database::connector testConnector::connector = database::connector(host);

struct dataHolder {
    std::string_view username = "test_user";
    std::string_view email = "test_email@mail.com";
    std::string_view password = "test_password";

    std::string_view access_token;
    std::string_view refresh_token;
    uint32_t id;
};

dataHolder dh;

void check_db_state_after_registratioin() {

}

TEST(Integration, RegistrationTest) {
    grpc::ClientContext context;
    authservice::SignupRequest request;
    authservice::SignupResponse response;
    request.set_username(dh.username);
    request.set_email(dh.email);
    request.set_password(dh.password);
    request.set_password_confirmation(dh.password);
    auto status = client.authservice_stub->Signup(&context, request, &response);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(response.has_access_token());
    // ASSERT_EQ(response.id(), 1);
    ASSERT_TRUE(response.id() > 0);
    dh.access_token = response.access_token().token();
    // dh.refresh_token = response.tokens().refresh().token();
    dh.id = response.id();
}

TEST(Integration, LoginTestWintEmail) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    authservice::LoginResponse response;
    request.set_email(dh.email);
    request.set_password(dh.password);
    auto status = client.authservice_stub->Login(&context, request, &response);
    ASSERT_TRUE(status.ok());
    ASSERT_EQ(response.id(), dh.id);
}

TEST(Integration, LoginTestWithUsername) {
    grpc::ClientContext context;
    authservice::LoginRequest request;
    authservice::LoginResponse response;
    request.set_username(dh.username);
    request.set_password(dh.password);
    auto status = client.authservice_stub->Login(&context, request, &response);
    ASSERT_TRUE(status.ok());
    ASSERT_EQ(response.id(), dh.id);
}

TEST(Integration, ChangeUserNameTest) {
    grpc::ClientContext context;
    profile::UsernameRequest request;
    profile::ProfileReply response;
    dh.username = "new_test_username";
    request.set_id(dh.id);
    request.set_username(dh.username);
    auto status = client.profile_stub->ChangeUsername(&context, request, &response);
    ASSERT_TRUE(status.ok());
    //check db state
    std::cerr << response.message() << std::endl;
}

TEST(Integration, ChangeQuote) {
    grpc::ClientContext context;
    profile::QuoteRequest request;
    profile::ProfileReply response;
    std::string_view quote = "new_test_quote";
    request.set_id(dh.id);
    request.set_quote(quote.data());
    auto status = client.profile_stub->ChangeQuote(&context, request, &response);
    ASSERT_TRUE(status.ok());
    //check db state
    std::cerr << response.message() << std::endl;
}



