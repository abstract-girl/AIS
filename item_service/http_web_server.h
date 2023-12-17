#ifndef HTTPWEBSERVER_H
#define HTTPWEBSERVER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "Headers/item_request_factory.h"
#include "../database/user.h"
#include "../helper.h"

class HTTPWebServer : public Poco::Util::ServerApplication
{
public:
    int main(const std::vector<std::string> &/*args*/)
    {
        try
        {
            debug("The Item web server has started");
            database::Item::init();
            std::unique_ptr<HTTPRequestHandlerFactory> requestFactory = std::make_unique<ItemsRequestFactory>("");
            ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", 8080));
            HTTPServer srv(requestFactory.release(), svs, new HTTPServerParams);
            srv.start();
            waitForTerminationRequest();
            srv.stop();
        }
        catch (const Poco::Exception& exc)
        {
            debug("The item web server has failed.");
            std::cerr << exc.displayText() << std::endl;
            return Application::EXIT_SOFTWARE;
        }

        return Application::EXIT_OK;
    }
};
#endif // !HTTPWEBSERVER
