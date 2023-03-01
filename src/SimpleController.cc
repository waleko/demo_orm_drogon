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
    // replaces: select * from tickets;
    auto allTickets = ticketMapper.findAll();
    // convert to json array
    auto arr = Json::Value();
    for (auto &ticket : allTickets) {
        arr.append(ticket.toJson());
        LOG_DEBUG << *ticket.getPaxName() << " on a flight #"
                  << *ticket.getFlightNum();
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
    // replaces: select * from flights where num = ?;
    auto flight = flightMapper.findByPrimaryKey(
        search_flight_num);
    auto flightJson = flight.toJson();

    // add spacecraft and planet objects to the json by corresponding ids
    std::promise<void> spacecraftPromise, planetPromise;
    auto spacecraftFuture = spacecraftPromise.get_future();
    auto planetFuture = planetPromise.get_future();

    // replaces: select s.* from spacecrafts s join flights f on s.id =
    //  f.spacecraft_id where f.num = ?;
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
    // replaces: select p.* from planets p join flights f on p.id = f.planet_id
    //  where f.num = ?;
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

    // get db client and mapper
    auto dbClient = drogon::app().getDbClient();
    auto spacecraftMapper = Mapper<Spacecrafts>(dbClient);

    std::promise<Json::Value> spacecraftPromise;
    auto spacecraftFuture = spacecraftPromise.get_future();
    // replaces: insert into spacecrafts values (?, ?, ?);
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
