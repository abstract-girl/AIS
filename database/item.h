#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>
#include <optional>
#include "Poco/JSON/Object.h"
#include "Poco/DateTime.h"

namespace database
{
    class Item
    {
      private:
        long _id;
        std::string _name;
        std::string _category;
        bool _availability;
        std::string _description;
        double _price;

      public:

        long get_id() const;
        const std::string &get_name() const;
        const std::string &get_category() const;
        double get_price() const;
        bool get_availability() const;
        const std::string &get_description() const;

        long& id();
        std::string &name();
        std::string &category();
        double& price();
        std::string &description();
        bool& availability();

        static void init();
        void save_to_mysql();
        static std::optional<Item>  read_by_id(long id);
        static std::vector<Item> read_all();

        Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif // ITEM_H