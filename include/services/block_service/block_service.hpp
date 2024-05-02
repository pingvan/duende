#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/block_service/block_service.hpp"
#include "services/block_service/block.grpc.pb.h"
#include "services/block_service/block.pb.h"
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using block::BlockRequest;
using block::BlockResponse;
using block::UnblockRequest;
using block::UnblockResponse;
using block::ShowAllBlockedRequest;
using block::ShowAllBlockedResponse;

class BlockServiceImpl final : public block::BlockService::Service {
    Status Block(
        ServerContext *context,
        const BlockRequest *request,
        BlockResponse *response
    ) override;

    Status Unblock(
        ServerContext *context,
        const UnblockRequest *request,
        UnblockResponse *response
    ) override;

    Status ShowAllBlocked(
        ServerContext *context,
        const ShowAllBlockedRequest *request,
        ShowAllBlockedResponse *response
    ) override;
};