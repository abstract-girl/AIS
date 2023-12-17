#ifndef ITEMS_HANDLER_H
#define ITEMS_HANDLER_H

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

#include "../../database/item.h"

class ItemsHandler : public HTTPRequestHandler
{
private:
    std::string _format;


public:
    ItemsHandler(const std::string& format) : _format(format) {}

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
    {
        HTMLForm form(request, request.stream());
        try
        {
             if (hasSubstr(request.getURI(), "/items/create") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)) {

                assertIfExists(form, "name");
                assertIfExists(form, "category");
                assertIfExists(form, "description");
                assertIfExists(form, "price");
                assertIfExists(form, "availability");
                
                database::Item newItem;
                newItem.name() = form.get("name", "");
                newItem.category() = form.get("category", "");
                newItem.description() = form.get("description", "");

                std::string price_str = form.get("price", "");

                // Convert the string to a long                
                if (!price_str.empty()) {
                    newItem.price() = std::stol(price_str);
                }

                std::string is_available_str = form.get("availability", "");

                if (!is_available_str.empty()) {
                    if (is_available_str == "true" || is_available_str == "1") {
                        newItem.availability() = true;
                    } else if (is_available_str == "false" || is_available_str == "0") {
                        newItem.availability() = false;
                    }
                }

                newItem.save_to_mysql();

                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& ostr = response.send();
                Poco::JSON::Object::Ptr root = newItem.toJSON();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;


             } else if (hasSubstr(request.getURI(), "/items/all") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)) {
                std::vector<database::Item> items = database::Item::read_all();
                Poco::JSON::Array jsonItems;
                for (const auto& item : items) {
                    jsonItems.add(item.toJSON());
                }

                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.setContentType("application/json");
                std::ostream& ostr = response.send();
                Poco::JSON::Stringifier::stringify(jsonItems, ostr);
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
            root->set("instance", "/items");
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
        root->set("instance", "/items");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }
};

#endif // ITEMS_HANDLER_H
