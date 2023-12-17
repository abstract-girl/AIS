#include "item.h"
#include "database.h"
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{
    void Item::init()
    {}

    void Item::save_to_mysql()
    {
           try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Item (name,category,price,description,availability) VALUES(?, ?, ?, ?, ?)",
                use(_name),
                use(_category),
                use(_price),
                use(_description),
                use(_availability);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::optional<Item> Item::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Item item;

            select << "SELECT id, name, category, price, description, availability FROM Item WHERE id=?",
                into(item._id),
                into(item._name),
                into(item._category),
                into(item._price),
                into(item._description),
                into(item._availability),
                use(id),
                range(0, 1); 

            if (select.execute() > 0)
            {
                return item;
            }
        }
        catch (const Poco::Exception& ex)
        {
            std::cerr << "Item::read_by_id failed: " << ex.displayText() << std::endl;
        }
        return {};        
    }

    std::vector<Item> Item::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Item> result;
            Item a;
            select << "SELECT id, name, category, price, description, availability FROM Item", 
                into(a._id),
                into(a._name),
                into(a._category),
                into(a._price),
                into(a._description),
                into(a._availability),
                range(0, 1); //  iterate over result set one row at a time
            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    Poco::JSON::Object::Ptr Item::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("category", _category);
        root->set("price", _price);
        root->set("description", _description);
        root->set("availability", _availability); 

        return root;
    }
        // Getters
    long Item::get_id() const {
        return _id;
    }

    const std::string& Item::get_name() const {
        return _name;
    }

    const std::string& Item::get_category() const {
        return _category;
    }

    double Item::get_price() const {
        return _price;
    }

    const std::string& Item::get_description() const {
        return _description;
    }

    bool Item::get_availability() const {
        return _availability;
    }

    long& Item::id() {
        return _id;
    }

    std::string& Item::name() {
        return _name;
    }

    std::string& Item::category() {
        return _category;
    }

    double& Item::price() {
        return _price;
    }

    std::string& Item::description() {
        return _description;
    }

    bool& Item::availability() {
        return _availability;
    }

}
