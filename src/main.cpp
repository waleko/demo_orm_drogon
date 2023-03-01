#include "SimpleController.h"
#include "drogon/drogon.h"

int main(int argc, char* argv[]) {
    char* host;
    if (argc < 2) {
        // default: localhost
        host = "127.0.0.1";
    } else {
        host = argv[1];
    }
    auto &app = drogon::app();
    app.createDbClient("postgresql", host, 5432, "postgres", "postgres",
                       "postgres");
    app.addListener("0.0.0.0", 8080);
    app.run();
}
