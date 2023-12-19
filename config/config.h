#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <optional>

class  Config{
    private:
        Config();
        std::string _host;
        std::string _port;
        std::string _login;
        std::string _password;
        std::string _database;
        std::optional<std::string> _cache_servers;

        std::optional<std::string> _queue_host;
        std::optional<std::string> _queue_topic;
        std::optional<long> _queue_group_id;

    public:
        static Config& get();

        std::string& port();
        std::string& host();
        std::string& login();
        std::string& password();
        std::string& database();
        std::optional<std::string>& cache_servers();

        std::optional<long>& queue_group_id();
        std::optional<std::string>& queue_host();
        std::optional<std::string>& queue_topic();


        const std::string& get_port() const ;
        const std::string& get_host() const ;
        const std::string& get_login() const ;
        const std::string& get_password() const ;
        const std::string& get_database() const ;
        const std::optional<std::string>& get_cache_servers() const;
        const std::optional<long>& get_queue_group_id() const;
        const std::optional<std::string>& get_queue_host() const ;
        const std::optional<std::string>& get_queue_topic() const ;
};

#endif