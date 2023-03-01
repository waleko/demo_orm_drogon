#include "SimpleController.h"
#include "../model/Flights.h"
#include "../model/Planets.h"
#include "../model/Spacecrafts.h"
#include "../model/Tickets.h"

using namespace drogon::orm;
using namespace controllers;
using namespace drogon_model::postgres;

/**
 * Health handler. Always returns OK
 */
void SimpleController::health(
    const HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    resp->setBody("OK");
    callback(resp);
}

/**
 * Gets the list of tickets in the database
 */
void SimpleController::getTickets(
    const HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get db client and mapper
    auto dbClient = drogon::app().getDbClient();
    auto ticketMapper = Mapper<Tickets>(dbClient);
    // retrieve all tickets from the database
    auto allTickets = ticketMapper.findAll();  // select * from tickets
    // convert to json array
    auto arr = Json::Value();
    for (auto &ticket : allTickets) {
        arr.append(ticket.toJson());
    }
    // return the json array to the user
    callback(HttpResponse::newHttpJsonResponse(arr));
}

/**
 * Gets all the information about the ticket.
 * Including all data about corresponding flight and spacecraft.
 */
void SimpleController::flight(
    const HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int search_flight_num) {
    // get db client and mapper
    auto dbClient = drogon::app().getDbClient();
    auto flightMapper = Mapper<Flights>(dbClient);

    // find the flight
    auto flight = flightMapper.findByPrimaryKey(
        search_flight_num);  // select * from tickets where id = ?
    auto flightJson = flight.toJson();

    // add spacecraft and planet objects to the json by corresponding ids
    std::promise<void> spacecraftPromise, planetPromise;
    auto spacecraftFuture = spacecraftPromise.get_future();
    auto planetFuture = planetPromise.get_future();
    flight.getSpacecrafts(
        dbClient,
        [&](const Spacecrafts &spacecraft) {
            flightJson["spacecraft"] = spacecraft.toJson();
            spacecraftPromise.set_value();
        },
        [&](const DrogonDbException &err) {
            spacecraftPromise.set_exception(
                std::make_exception_ptr(err.base()));
        });
    flight.getPlanets(
        dbClient,
        [&](const Planets &planet) {
            flightJson["planet"] = planet.toJson();
            planetPromise.set_value();
        },
        [&](const DrogonDbException &err) {
            planetPromise.set_exception(std::make_exception_ptr(err.base()));
        });
    spacecraftFuture.wait();
    planetFuture.wait();

    // send json to the user
    callback(HttpResponse::newHttpJsonResponse(flightJson));
}

/**
 * Adds new spacecraft to the database from the JSON body of the request
 */
void SimpleController::newSpacecraft(
    const HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // read and parse flight request from user input
    Json::Value spacecraftData;
    std::stringstream(req->bodyData()) >> spacecraftData;
    auto spacecraftReq = Spacecrafts(spacecraftData);

    auto dbClient = drogon::app().getDbClient();
    auto spacecraftMapper = Mapper<Spacecrafts>(dbClient);

    std::promise<Json::Value> spacecraftPromise;
    auto spacecraftFuture = spacecraftPromise.get_future();
    spacecraftMapper.insert(
        spacecraftReq,
        [&](const Spacecrafts &flight) {
            spacecraftPromise.set_value(flight.toJson());
        },
        [&](const DrogonDbException &err) {
            spacecraftPromise.set_exception(
                std::make_exception_ptr(err.base()));
        });
    spacecraftFuture.wait();

    callback(HttpResponse::newHttpJsonResponse(spacecraftFuture.get()));
}
