#pragma once
#include "client.hpp"

struct Manager {
    client new_client;

    explicit Manager(const std::shared_ptr<grpc::Channel> &new_channel)
        : new_client(new_channel) {
    }

    Manager() = default;
    Manager(const Manager& other) = delete;
    Manager(Manager&& other) noexcept = default;
    Manager &operator=(const Manager&) = delete;
    Manager &operator=(Manager&&) noexcept = default;
};
