#include <gtest/gtest.h>
#include <gmock/gmock.h>

// TEST(GetUserFullPage, Full) {
//     static std::string server_address = "0.0.0.0:50051";
//     auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
//     auto stub = authservice::AuthServiceData::NewStub(channel);
//     grpc::ClientContext context;
//     authservice::RequestWithUserId request;
//     common::UserPreview response;
//     request.set_user_id(1);
//     auto status = stub->GetUserPrewiew(&context, request, &response);
//     if (!status.ok()) {
//         std::cerr << "Error in handling\n";
//     }
//     std::basic_string<std::byte> byte_photo;
//     auto photo = response.photo();
//     std::transform(photo.begin(), photo.end(), std::back_inserter(byte_photo), [](char char_){return static_cast<std::byte>(char_);});
//     write_photo_binary("prev", byte_photo);
// }

// TEST(GetFull, prev) {
//     static std::string server_address = "0.0.0.0:50051";
//     auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
//     auto stub = authservice::AuthServiceData::NewStub(channel);
//     grpc::ClientContext context;
//     authservice::RequestWithUserId request;
//     common::UserFullPage response;
//     request.set_user_id(10);
//     auto status = stub->GetUserFullPage(&context, request, &response);
//     if (!status.ok()) {
//         std::cerr << "Error in handling\n";
//     }
//     std::basic_string<std::byte> byte_photo;
//     auto photo = response.preview().photo();
//     std::transform(photo.begin(), photo.end(), std::back_inserter(byte_photo), [](char char_){return static_cast<std::byte>(char_);});
//     write_photo_binary("full", byte_photo);
//     std::cout << response.preview().user_id() << std::endl;
//     std::cout << response.preview().login() << std::endl;
//     std::cout << response.quote() << std::endl;
//     std::cout << response.about() << std::endl;
//     for (int i = 0; i < response.favorite_genres_size() && i < 10; i++) {
//         std::cout << response.favorite_genres(i) << std::endl;
//     }
//     for (int i = 0; i < response.favorite_actors_size() && i < 10; i++) {
//         std::cout << response.favorite_actors(i).actor_id() << std::endl;
//         std::cout << response.favorite_actors(i).full_name() << std::endl;
//     }
//     for (int i = 0; i < response.watched_films_size() && i < 10; i++) {
//         std::cout << response.watched_films(i).preview().film_id() << std::endl;
//         std::cout << response.watched_films(i).preview().title() << std::endl;
//         std::cout << response.watched_films(i).ratio() << std::endl;
//     }
// }