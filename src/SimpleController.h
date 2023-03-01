#pragma once

#include "drogon/HttpController.h"

using namespace drogon;

namespace controllers {
class SimpleController : public drogon::HttpController<SimpleController, true> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SimpleController::health, "/health", Get);
    ADD_METHOD_TO(SimpleController::getTickets, "/tickets", Get);
    ADD_METHOD_TO(SimpleController::flight, "/flight/{1}", Get);
    ADD_METHOD_TO(SimpleController::newSpacecraft, "/spacecraft", Post);
    METHOD_LIST_END

    void health(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void getTickets(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void flight(const drogon::HttpRequestPtr &req,
                std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                int search_flight_num);
    void newSpacecraft(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
}  // namespace controllers
