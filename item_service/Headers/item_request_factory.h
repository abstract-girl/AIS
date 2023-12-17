
#ifndef ITEMS_REQUEST_FACTORY_H
#define ITEMS_REQUEST_FACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "item_handler.h" 
#include <iostream>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class ItemsRequestFactory : public HTTPRequestHandlerFactory
{
public:
    ItemsRequestFactory(const std::string& format) : _format(format) {}

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
    {
        std::cout << "Items request: " << request.getURI() << std::endl;
        if (hasSubstr(request.getURI(), "/items/all") || 
            hasSubstr(request.getURI(),"/items/create"))
            return new ItemsHandler(_format);
        return nullptr;
    }

private:
    std::string _format;
};

#endif // ITEMS_REQUEST_FACTORY_H
