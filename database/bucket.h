#ifndef DATABASE_ORDER_H
#define DATABASE_ORDER_H

#include <string>
#include <vector>
#include <optional>
#include <Poco/DateTime.h>
#include <Poco/JSON/Object.h>

namespace database {

    class Bucket {
    private:
        long _id; // Unique identifier for the order
        long _user_id; // Identifier for the consumer who placed the order
        std::vector<long> _list_of_items_ids; // List of service identifiers included in the order

    public:

        // API Methods
        static std::optional<Bucket> create(long user_id); 
        bool add_item(long item_id, long bucket_id); 
        static std::optional<Bucket> get_bucket_for_user(long user_id);

        // Getters
        long get_id() const;
        long get_user_id() const;
        std::vector<long> get_list_of_items_ids() const;

        // Setters
        long& id();
        long& user_id();
        std::vector<long>& list_of_items_ids();

        static void init();
        // Serialization and Deserialization
        static Bucket fromJSON(const std::string &str);
        Poco::JSON::Object::Ptr toJSON() const;

        void populate_items();
        // Database Interaction
        // static std::vector<Bucket> read_all(); // Read all buckets from the database
    };

} // namespace database

#endif // DATABASE_ORDER_H
