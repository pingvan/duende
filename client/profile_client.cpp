#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <grpcpp/grpcpp.h>

#include "proto/profile.grpc.pb.h"
#include "proto/profile.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using profile::ProfileReply;
using profile::ProfileRequest;
using profile::ProfileService;

const std::string server_address = "127.0.0.1:1111";

class ProfileClient {
 public:
  ProfileClient(std::shared_ptr<Channel> channel)
      : stub_(ProfileService::NewStub(channel)) {}

  std::string ChangeNickname(const std::string& nickname) {
    ProfileRequest request;
    request.set_nickname(nickname);

    ProfileReply reply;

    ClientContext context;

    Status status = stub_->ChangeNickname(&context, request, &reply);

    if (status.ok()) {
      return "Nickname changed";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string ChangePhoto(const std::string& photo) {
    ProfileRequest request;
    request.set_photo(photo);

    ProfileReply reply;

    ClientContext context;

    Status status = stub_->ChangePhoto(&context, request, &reply);

    if (status.ok()) {
      return "Photo changed";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string ChangeQuote(const std::string& quote) {
    ProfileRequest request;
    request.set_quote(quote);

    ProfileReply reply;

    ClientContext context;

    Status status = stub_->ChangeQuote(&context, request, &reply);

    if (status.ok()) {
      return "Quote changed";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string ChangeBio(const std::string& bio) {
    ProfileRequest request;
    request.set_bio(bio);

    ProfileReply reply;

    ClientContext context;

    Status status = stub_->ChangeBio(&context, request, &reply);

    if (status.ok()) {
      return "Bio changed";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<ProfileService::Stub> stub_;
};

int main(int argc, char** argv) {
  ProfileClient new_client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
  std::cout << "......" << std::endl;
  return 0;
}