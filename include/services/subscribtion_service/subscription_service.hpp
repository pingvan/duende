#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/subscription_service/subscription_service.hpp"
#include "services/subscription_service/subscription.grpc.pb.h"
#include "services/subscription_service/subscription.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

class SubscriptionServiceImpl final : public SubscriptionService::Service {
    Status SubscriptionStream(
        ServerContext *context,
        ServerReaderWriter<StreamResponse, StreamRequest> *stream
    ) override;
};