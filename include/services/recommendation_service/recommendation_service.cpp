#include "recommendation_service.hpp"
#include <algorithm>
#include <functional>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include "components/database/services/profile_service/profile_service.hpp"
#include "components/database/services/recommendation_service/recommendation_service.hpp"
#include "components/database/services/authservice/authservice.hpp"


RecommendationServiceImpl::RecommendationServiceImpl() {
    auto rating_matrix = database::recommendation_service_database_api::getUserRatingMarix();
    films_simularity = ComputeCosineSimularity(Transpose(rating_matrix));
}

grpc::Status RecommendationServiceImpl::GetUsersRecommendations(grpc::ServerContext *context, const recommendation_service::UserId *request, recommendation_service::GetUsersRecommendationsResponse *response) {
    auto user_id = request->id();
    constexpr uint32_t recommendation_count = 15;
    auto top_users = RecommendUsers(user_id, recommendation_count);
    for (const auto &recommended_user_id : top_users) {
        auto preview = database::authservice_database_api::get_user_preview(recommended_user_id);
        auto preview_proto = response->add_user();
        preview.convert_to_proto(preview_proto);
    }
    return grpc::Status::OK;
}

grpc::Status RecommendationServiceImpl::GetFilmsRecommendations(grpc::ServerContext *context, const recommendation_service::UserId *request, recommendation_service::GetFilmsRecommendationsResponse *response) {
    auto user_id = request->id();
    constexpr uint32_t recommendation_count = 15;
    auto top_films = RecommendFilms(user_id, recommendation_count);
    for (const auto &recommended_film_id : top_films) {
        auto film = database::profile_service_database_api::get_film_preview(recommended_film_id);
        auto film_proto = response->add_film();
        film.convert_to_proto(film_proto);
    }
    return grpc::Status::OK;
}

grpc::Status RecommendationServiceImpl::GetFilmsRecommendationsForSeveralUsers(grpc::ServerContext *context, const recommendation_service::UserIds *request, recommendation_service::GetFilmsRecommendationsResponse *response) {
    const google::protobuf::RepeatedField<int> &users_ids = request->ids();
    auto users_id = std::vector<int>(users_ids.begin(), users_ids.end());
    auto recommended_film_id = RecommendFilmsForSeveralUsers(users_id);
    auto film = database::profile_service_database_api::get_film_preview(recommended_film_id);
    auto film_proto = response->add_film();
    film.convert_to_proto(film_proto);
    return grpc::Status::OK;
}

std::vector<std::vector<int>> RecommendationServiceImpl::Transpose(const std::vector<std::vector<int>> &matrix) {
    int num_rows = matrix.size();
    int num_cols = matrix[0].size();
    std::vector<std::vector<int>> transposed_matrix(num_cols, std::vector<int>(num_rows, 0));
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            transposed_matrix[j][i] = matrix[i][j];
        }
    }
    return transposed_matrix;
}

std::vector<double> RecommendationServiceImpl::AggregateUserRatings(const std::vector<std::vector<int>> &users_ratings) {
    size_t num_users = users_ratings.size();
    size_t num_movies = users_ratings[0].size(); 
    std::vector<double> aggregated_ratings(num_movies, 0);
    for (size_t i = 0; i < num_movies; ++i) {
        double sum = 0;
        int count = 0;
        for (size_t j = 0; j < num_users; ++j) {
            if (users_ratings[j][i] > 0) {
                sum += users_ratings[j][i];
                count++;
            }
        }
        if (count > 0) {
            aggregated_ratings[i] = sum / count;
        } else {
            aggregated_ratings[i] = 0;
        }
    }

    return aggregated_ratings;
}

std::vector<std::vector<double>> RecommendationServiceImpl::ComputeCosineSimularity(const std::vector<std::vector<int>> &ratings_matrix) {
    int num_rows = ratings_matrix.size();
    int num_cols = ratings_matrix[0].size();
    std::vector<std::vector<double>> similarity(num_rows, std::vector<double>(num_rows, 0.0));
    for (int i = 1; i < num_rows; i++) {
        for (int j = 1; j < num_rows; j++) {
            if (i != j) {
                double dot_product = 0.0;
                double norm_a = 0.0;
                double norm_b = 0.0;
                for (int k = 1; k < num_cols; k++) {
                    dot_product += ratings_matrix[i][k] * ratings_matrix[j][k];
                    norm_a += ratings_matrix[i][k] * ratings_matrix[i][k];
                    norm_b += ratings_matrix[j][k] * ratings_matrix[j][k];
                }
                if (norm_a != 0.0 && norm_b != 0.0) {
                    similarity[i][j] = dot_product / (std::sqrt(norm_a) * std::sqrt(norm_b));
                }
            }
        }
    }
    return similarity;
}

std::vector<double> RecommendationServiceImpl::ComputeCosineSimularityForOneUser(const int &user_id) {
    auto rating_matrix = database::recommendation_service_database_api::getUserRatingMarix();
    int num_rows = rating_matrix.size();
    int num_cols = rating_matrix[0].size();
    double user_norm = 0.0;
    for (int i = 1; i < num_cols; i++) {
        user_norm += rating_matrix[user_id][i] * rating_matrix[user_id][i];
    }
    if (user_norm == 0.0) {
        // case where we have no views - give rand users;
    }
    auto user_norm_sqrt = std::sqrt(user_norm);
    std::vector<double> simularity(num_rows);
    for (int i = 1; i < num_rows; i++) {
        if (i != user_id) {
            double dot_product = 0.0;
            double norm_a = 0.0;
            for (int j = 1; j < num_cols; j++) {
                dot_product += rating_matrix[user_id][j] * rating_matrix[i][j];
                norm_a += rating_matrix[i][j] * rating_matrix[i][j];
            }
            if (norm_a != 0.0) {
                simularity[i] = dot_product / (user_norm_sqrt * std::sqrt(norm_a));
            }
        }
    }
    return simularity;
}

std::vector<int> RecommendationServiceImpl::RecommendUsers(const uint32_t user_id, const uint32_t reccomendation_count) {
    auto user_scores = ComputeCosineSimularityForOneUser(user_id);
    std::sort(user_scores.begin(), user_scores.end(), std::greater<>());
    std::vector<int> top_users;
    for (int i = 1; i < reccomendation_count && i < user_scores.size(); i++) {
        top_users.push_back(i);
    }
    return top_users;
}

std::vector<int> RecommendationServiceImpl::RecommendFilms(const uint32_t user_id, const uint32_t reccomendation_count) {
    auto rating_matrix = database::recommendation_service_database_api::getUserRatingMarix();
    auto size = films_simularity.size();
    std::vector<double> film_scores(size, 0.0);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < films_simularity[i].size(); j++) {
            film_scores[i] += films_simularity[i][j] * rating_matrix[user_id][j];
        }
    }
    std::vector<std::pair<double, int>> scored_films;
    for (int i = 0; i < film_scores.size(); i++) {
        scored_films.push_back({film_scores[i], i});
    }
    std::sort(scored_films.begin(), scored_films.end(), std::greater<>());
    std::vector<int> top_films;
    for (int i = 0; i < reccomendation_count && i < scored_films.size(); i++) {
        top_films.push_back(scored_films[i].second);
    }
    return top_films;
}

int RecommendationServiceImpl::RecommendFilmsForSeveralUsers(const std::vector<int> &users_ids) {
    auto rating_matrix = database::recommendation_service_database_api::getUsersRatingsVector(users_ids);
    auto combined_ratings = AggregateUserRatings(rating_matrix);
    auto size = films_simularity.size();
    std::vector<double> film_scores(size, 0.0);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < films_simularity[i].size(); j++) {
            film_scores[i] += films_simularity[i][j] * combined_ratings[j];
        }
    }
    std::vector<std::pair<double, int>> scored_films;
    for (int i = 0; i < film_scores.size(); i++) {
        scored_films.push_back({film_scores[i], i});
    }
    return std::max_element(scored_films.begin(), scored_films.end())->second;
}