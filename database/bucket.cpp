#include "bucket.h"
#include "database.h"
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPMessage.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{
    void Bucket::init()
    {}

    std::optional<Bucket> Bucket::create(long user_id)
    {

        std::optional<Bucket> existingBucket = get_bucket_for_user(user_id);

        if (existingBucket.has_value()) {
            return existingBucket;
        }

        Bucket newBucket;
        Session session = Database::get().create_session();

        try {
            session.begin();

            Statement insert(session);
            insert << "INSERT INTO `Bucket` (user_id) VALUES (?)",
                use(user_id),  
                now;

            Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(newBucket._id),
                now;

            newBucket._user_id = user_id;

            session.commit();
        } catch (const Poco::Exception& ex) {
            std::cerr << "Bucket::create failed: " << ex.displayText() << std::endl;
            session.rollback();
            return {};
        }
        return newBucket;
    }

    bool Bucket::add_item(long item_id, long bucket_id) 
    {
        Session session = Database::get().create_session();
        try {
            // Create a session to interact with the database
            
            // Begin a transaction to ensure data integrity
            session.begin();

            // Prepare a statement to insert a new record into the Bucket_Service table
            Statement insert(session);
            insert << "INSERT INTO Bucket_Item (bucket_id, item_id) VALUES (?, ?)",
                use(bucket_id),    
                use(item_id),
                now;         

            session.commit();

            // Optionally, you could update the _list_of_services_ids vector, if needed
            _list_of_items_ids.push_back(item_id);
            return true;

        } catch (const Poco::Exception& ex) {
            // If an exception occurs, output the error and roll back the transaction
            std::cerr << "Bucket::add_service failed: " << ex.displayText() << std::endl;
            session.rollback();
            return false;
        }
    }

    std::optional<Bucket> Bucket::get_bucket_for_user(long user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Bucket bucket;

            select << "SELECT id, user_id FROM Bucket WHERE user_id=?",
                into(bucket._id),
                into(bucket._user_id),
                use(user_id),
                range(0, 1); // iterate over result set one row at a time

            if (select.execute() > 0)
            {
                bucket.populate_items();
                return bucket;
            }
        }
        catch (const Poco::Exception& ex)
        {
            std::cerr << "Bucket::get_bucket_for_user failed: " << ex.displayText() << std::endl;
        }
        return {};        
    }

    void Bucket::populate_items() {
        try {
            Session session = Database::get().create_session();
            Statement select(session);

            std::vector<long> itemIDs;

            // Query the Order_Service table to find all item IDs related to this order
            select << "SELECT item_id FROM Bucket_Item WHERE bucket_id=?",
                into(itemIDs),
                use(_id); // Using the Bucket's _id field

            // Execute the query
            select.execute();

            // Clear existing items IDs and repopulate
            _list_of_items_ids.clear();
            _list_of_items_ids.assign(itemIDs.begin(), itemIDs.end());

        } catch (const Poco::Exception& ex) {
            std::cerr << "Bucket::populate_items failed: " << ex.displayText() << std::endl;
        }
    }

    Bucket Bucket::fromJSON(const std::string &str)
    {
        Bucket bucket;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        // Assigning parsed values to Order's properties using the accessors
        bucket.id() = object->getValue<long>("id");
        bucket.user_id() = object->getValue<long>("user_id");

        // Convert JSON array of strings back to std::vector<long>
        Poco::JSON::Array::Ptr itemsArray = object->getArray("list_of_items_ids");
        std::vector<long>& listOfItemsIds = bucket.list_of_items_ids();
        listOfItemsIds.clear(); // Clear existing data if necessary
        if (itemsArray) {
            for (size_t i = 0; i < itemsArray->size(); ++i) {
                listOfItemsIds.push_back(itemsArray->getElement<long>(i));
            }
        }

        return bucket;
    }

    Poco::JSON::Object::Ptr Bucket::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        
        // Convert list of service IDs to a JSON array of strings
        Poco::JSON::Array::Ptr itemsArray = new Poco::JSON::Array();
        for (const auto& itemId : _list_of_items_ids) {
            itemsArray->add(itemId);
        }
        root->set("list_of_items_ids", itemsArray);

        return root;
    }

    // Getters
    long Bucket::get_id() const {
        return _id;
    }

    long Bucket::get_user_id() const {
        return _user_id;
    }

    std::vector<long> Bucket::get_list_of_items_ids() const {
        return _list_of_items_ids;
    }

    // Properties
    long& Bucket::id() {
        return _id;
    }

    long& Bucket::user_id() {
        return _user_id;
    }

    std::vector<long>& Bucket::list_of_items_ids() {
        return _list_of_items_ids;
    }
    
}
