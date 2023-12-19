#include "http_web_server.h"
#include <iostream>

int main(int argc, char*argv[]) 
{
    debug("The user app has started.");
    HTTPWebServer app;
    debug("HTTPWebServer created successfully");
    try
{
    app.run(argc, argv);
}
catch(const std::exception& e)
{
    debug("HTTPWebServer failed");
    std::cerr << "Caught exception: " << e.what() << std::endl;
}

    // return app.run(argc, argv);
}