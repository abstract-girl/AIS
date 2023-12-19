#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class User{
        private:
            std::string _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _title;
            std::string _login;
            std::string _password;
            std::string _role;

        public:

            static User fromJSON(const std::string & str);

            const std::string &get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_email() const;
            const std::string &get_title() const;
            const std::string &get_login() const;
            const std::string &get_password() const;
            const std::string &get_role() const;

            std::string &id();
            std::string &first_name();
            std::string &last_name();
            std::string &email();
            std::string &title();
            std::string &login();
            std::string &password();
            std::string &role();

            void save_to_cache();
            static std::optional<User> read_from_cache_by_id(std::string id);
            
            static void init();
            static std::optional<User> read_by_id(std::string id);
            static std::optional<std::string> auth(std::string &login, std::string &password);
            static std::vector<User> read_all();
            static std::vector<User> search(std::string first_name,std::string last_name);
            void save_to_mysql();
            void send_to_queue();
            static void preload(const std::string & str);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif