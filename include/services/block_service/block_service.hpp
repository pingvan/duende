#pragma once

#include <grpcpp/grpcpp.h>
#include "components/database/services/block_service/block_service.hpp"
#include "services/block_service/block.grpc.pb.h"
#include "services/block_service/block.pb.h"
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using block::StreamRequest;
using block::StreamResponse;
using block::BlockRequest;
using block::UnblockRequest;
using block::ShowAllBlockedRequest;

class BlockServiceImpl final : public BlockService::Service {
    Status BlockStream(
        ServerContext *context,
        ServerReaderWriter<StreamResponse, StreamRequest> *stream
    ) override;
};