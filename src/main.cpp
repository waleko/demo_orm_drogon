#include "SimpleController.h"
#include "drogon/drogon.h"

int main() {
    auto &app = drogon::app();
    app.createDbClient("postgresql", "127.0.0.1", 5432, "postgres", "postgres",
                       "postgres");
    app.addListener("0.0.0.0", 8080);
    app.run();
}
