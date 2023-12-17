#include "http_web_server.h"
#include <iostream>
#include "../helper.h"

int main(int argc, char*argv[]) 
{
    debug("The Item service app has started.");
    HTTPWebServer app;
    return app.run(argc, argv);
}