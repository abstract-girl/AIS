
#ifndef BUCKET_REQUEST_FACTORY_H
#define BUCKET_REQUEST_FACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "bucket_handler.h" 
#include <iostream>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class BucketRequestFactory : public HTTPRequestHandlerFactory
{
public:
    BucketRequestFactory(const std::string& format) : _format(format) {}

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
    {
        std::cout << "Bucket request: " << request.getURI() << std::endl;
        if (hasSubstr(request.getURI(), "/buckets/create") || 
            hasSubstr(request.getURI(),"/buckets/add_item") || 
            hasSubstr(request.getURI(),"/buckets/search"))
            return new BucketHandler(_format);
        return nullptr;
    }

private:
    std::string _format;
};

#endif // BUCKET_REQUEST_FACTORY_H
