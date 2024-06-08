#include <grpcpp/grpcpp.h>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "services/recommendation_service/recommendation_service.grpc.pb.h"
#include "services/recommendation_service/recommendation_service.pb.h"

struct RecommendationServiceImpl
    : public recommendation_service::RecommendationService::Service {
    RecommendationServiceImpl();

    grpc::Status GetUsersRecommendations(
        grpc::ServerContext *context,
        const recommendation_service::UserId *request,
        recommendation_service::GetUsersRecommendationsResponse *response
    ) override;
    grpc::Status GetFilmsRecommendations(
        grpc::ServerContext *context,
        const recommendation_service::UserId *request,
        recommendation_service::GetFilmsRecommendationsResponse *response
    ) override;
    grpc::Status GetFilmsRecommendationsForSeveralUsers(
        grpc::ServerContext *context,
        const recommendation_service::UserIds *request,
        recommendation_service::GetFilmsRecommendationsResponse *response
    ) override;

    std::vector<std::vector<int>> Transpose(
        const std::vector<std::vector<int>> &martix
    );

    std::vector<double> AggregateUserRatings(
        const std::vector<std::vector<int>> &users_ratings
    );
    std::vector<std::vector<double>> ComputeCosineSimularity(
        const std::vector<std::vector<int>> &ratings_matrix
    );
    std::vector<double> ComputeCosineSimularityForOneUser(const int &user_id);

    std::vector<int>
    RecommendUsers(const uint32_t user_id, const uint32_t reccomendation_count);
    std::vector<int>
    RecommendFilms(const uint32_t user_id, const uint32_t reccomendation_count);
    int RecommendFilmsForSeveralUsers(const std::vector<int> &users_ids);

    std::vector<std::vector<double>> films_simularity;
};