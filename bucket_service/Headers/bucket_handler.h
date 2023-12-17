#ifndef BUCKET_HANDLER_H
#define BUCKET_HANDLER_H


#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
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
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../helper.h"
#include "../../database/bucket.h"

class BucketHandler : public HTTPRequestHandler
{
private:
    std::string _format;

public:
    BucketHandler(const std::string& format) : _format(format) {}

    void sendError(const std::string &message, HTTPServerResponse& response) {
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setContentType("application/json");
        std::ostream& ostr = response.send();
        Poco::JSON::Object errorObj;
        errorObj.set("error", message);
        Poco::JSON::Stringifier::stringify(errorObj, ostr);
        return;
    }

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
    {
        HTMLForm form(request, request.stream());
        try
        {
             if (hasSubstr(request.getURI(), "/buckets/create") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)) {

                assertIfExists(form, "user_id");
                
                long user_id = std::stol(form.get("user_id", "0"));
                
                std::optional<database::Bucket> newBucket = database::Bucket::create(user_id);

                if (!newBucket.has_value()) {
                    sendError("Couldn't create a new bucket for the user.", response);
                    return;
                } 

                auto bucketValue = newBucket.value();
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& ostr = response.send();
                Poco::JSON::Object::Ptr root = bucketValue.toJSON();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;

             } else if (hasSubstr(request.getURI(), "/buckets/add_item") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)) {
                
                assertIfExists(form, "user_id");
                assertIfExists(form, "item_id");

                long user_id = std::stol(form.get("user_id", "0"));
                long item_id = std::stol(form.get("item_id", "0"));

                std::optional<database::Bucket> bucket = database::Bucket::get_bucket_for_user(user_id);
                
                if (!bucket.has_value()) {
                    sendError("Bucket not found", response);
                    return;
                }

                auto bucketValue = bucket.value();
                long bucket_id = bucketValue.get_id();

                if (!bucketValue.add_item(item_id, bucket_id)) {
                    sendError("Couldn't add the item into bucket", response);
                    return;   
                }
    
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& ostr = response.send();
                Poco::JSON::Object::Ptr root = bucketValue.toJSON();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;

             } else if (hasSubstr(request.getURI(), "/buckets/search") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)) {
                
                assertIfExists(form, "user_id");
                long user_id = std::stol(form.get("user_id", ""));

                std::optional<database::Bucket> bucket = database::Bucket::get_bucket_for_user(user_id);
                
                if (!bucket.has_value()) {
                    sendError("Bucket not found with the provided user ID.", response);
                    return;
                } 

                auto bucketValue = bucket.value();
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& ostr = response.send();
                Poco::JSON::Object::Ptr root = bucketValue.toJSON();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
             }
        }
        catch (const std::invalid_argument &e) {
            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
            root->set("type", "/errors/not_found");
            root->set("title", "Internal exception");
            root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
            root->set("detail", e.what()); 
            root->set("instance", "/service");
            std::ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);
        }
        catch (...)
        {
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request not found");
        root->set("instance", "/bucket");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

};

#endif // BUCKET_HANDLER_H
