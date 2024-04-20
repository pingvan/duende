#include "block_service.hpp"

Status BlockServiceImpl::BlockStream(
    ServerContext *context,
    ServerReaderWriter<StreamResponse, StreamRequest> *stream
) {
    StreamRequest request;
    database::connector db_connector;
    while (stream->Read(&request)) {
        StreamResponse response;
        if (request.has_block_request()) {
            const BlockRequest &block_request = request.block_request();
            if (db_connector.is_blocked(block_request.blocker_id(), block_request.blockie_id())) {
                response.mutable_block_response()->set_message("User is already blocked");
            } else {
                db_connector.block_user(block_request.blocker_id(), block_request.blockie_id());
                response.mutable_block_response()->set_message("User blocked");
            }
        } else if (request.has_unblock_request()) {
            const UnblockRequest &unblock_request = request.unblock_request();
            if (!db_connector.is_blocked(unblock_request.blocker_id(), unblock_request.blockie_id())) {
                response.mutable_unblock_response()->set_message("User is not blocked");
            } else {
                db_connector.unblock_user(unblock_request.blocker_id(), unblock_request.blockie_id());
                response.mutable_unblock_response()->set_message("User unblocked");
            }
        } else if (request.has_show_all_blocked_request()) {
            const ShowAllBlockedRequest &show_all_blocked_request = request.show_all_blocked_request();
            std::vector<int> blocked_users = db_connector.get_all_blocked(show_all_blocked_request.blockie_id());
            ShowAllBlockedResponse *show_all_blocked_response = response.mutable_show_all_blocked_response();
            for (int blockie_id : blocked_users) {
                show_all_blocked_response->add_blockie_id(blockie_id);
            }
        }
        stream->Write(response);
    }
    return Status::OK;
}