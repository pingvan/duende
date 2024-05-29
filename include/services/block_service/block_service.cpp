#include "block_service.hpp"
#include "components/database/services/authservice/authservice.hpp"

Status BlockServiceImpl::Block(
    ServerContext *context,
    const BlockRequest *request,
    BlockResponse *response
) {
    database::connector db_connector;
    if (db_connector.is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::ALREADY_EXISTS, "User is already blocked");
    } else {
        db_connector.block_user(request->blocker_id(), request->blockie_id());
        response->set_message("User blocked");
    }
    return Status::OK;
}

Status BlockServiceImpl::Unblock(
    ServerContext *context,
    const UnblockRequest *request,
    UnblockResponse *response
) {
    database::connector db_connector;
    if (!db_connector.is_blocked(request->blocker_id(), request->blockie_id())) {
        return Status(grpc::NOT_FOUND, "User is not blocked");
    } else {
        db_connector.unblock_user(request->blocker_id(), request->blockie_id());
        response->set_message("User unblocked");
    }
    return Status::OK;
}

Status BlockServiceImpl::ShowAllBlocked(
    ServerContext *context,
    const ShowAllBlockedRequest *request,
    ShowAllBlockedResponse *response
) {
    database::connector db_connector;
    std::vector<int> blocked_users = db_connector.get_all_blocked(request->blockie_id());
    for (int blockie_id : blocked_users) {
        auto preview = database::authservice_database_api::get_user_preview(blockie_id);
        response->add_blockie_id(blockie_id);
    }

    return Status::OK;
}