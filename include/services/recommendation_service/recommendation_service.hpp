#include <grpcpp/grpcpp.h>
#include <condition_variable>
#include <vector>
#include <mutex>

#include "services/recommendation_service/recommendation_service.grpc.pb.h"
#include "services/recommendation_service/recommendation_service.pb.h"

struct RecommendationServiceImpl : public recommendation_service::RecommendationService::Service {
    grpc::Status GetUsersRecommendations(grpc::ServerContext *context, const recommendation_service::UserId *request, recommendation_service::GetUsersRecommendationsResponse *response) override;
    grpc::Status GetFilmsRecommendations(grpc::ServerContext *context, const recommendation_service::UserId *request, recommendation_service::GetFilmsRecommendationsResponse *response) override;
// private:

    void Init();

    std::vector<std::vector<int>> transpose(const std::vector<std::vector<int>> &martix);
    std::vector<std::vector<double>> computeCosineSimularity(const std::vector<std::vector<int>> &ratings_matrix);
    std::vector<int> recommendUsers(const uint32_t user_id, const uint32_t reccomendation_count);
    std::vector<int> recommendFilms(const uint32_t user_id, const uint32_t reccomendation_count);

    bool copying = false;
    std::mutex m;
    std::condition_variable cv;

    std::vector<std::vector<int>> rating_matrix;
    std::vector<std::vector<double>> users_simularity;
    std::vector<std::vector<double>> films_simularity;
};