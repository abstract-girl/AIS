#include "config.h"

Config::Config()
{
        _host = std::getenv("DB_HOST");
        _port = std::getenv("DB_PORT");
        _login = std::getenv("DB_LOGIN");
        _password = std::getenv("DB_PASSWORD");
        _database = std::getenv("DB_DATABASE");

        const char* cache_servers = std::getenv("CACHE");
        _cache_servers = cache_servers ? std::make_optional(std::string(cache_servers)) : std::nullopt;;

        const char* queue_host = std::getenv("QUEUE_HOST");
        _queue_host = queue_host ? std::make_optional(std::string(queue_host)) : std::nullopt;

        const char* queue_topic = std::getenv("QUEUE_TOPIC");
        _queue_topic = queue_topic ? std::make_optional(std::string(queue_topic)) : std::nullopt;
        
        const char* queue_group_id = std::getenv("QUEUE_GROUP_ID");
        _queue_group_id = queue_group_id ? std::make_optional(long(std::stol(queue_group_id))) : std::nullopt;

}

Config &Config::get()
{
    static Config _instance;
    return _instance;
}

std::optional<long>&Config::queue_group_id()
{
    return _queue_group_id;
}

const std::optional<long>&Config::get_queue_group_id() const
{
    return _queue_group_id;
}
std::optional<std::string>&Config::queue_host()
{
    return _queue_host;
}

std::optional<std::string>&Config::queue_topic()
{
    return _queue_topic;
}

const std::optional<std::string>&Config::get_queue_host() const
{
    return _queue_host;
}

const std::optional<std::string>&Config::get_queue_topic() const
{
    return _queue_topic;
}

const std::string &Config::get_port() const
{
    return _port;
}

const std::optional<std::string> &Config::get_cache_servers() const
{
    return _cache_servers;
}

std::optional<std::string> &Config::cache_servers()
{
    return _cache_servers;
}

const std::string &Config::get_host() const
{
    return _host;
}

const std::string &Config::get_login() const
{
    return _login;
}

const std::string &Config::get_password() const
{
    return _password;
}
const std::string &Config::get_database() const
{
    return _database;
}

std::string &Config::port()
{
    return _port;
}

std::string &Config::host()
{
    return _host;
}

std::string &Config::login()
{
    return _login;
}

std::string &Config::password()
{
    return _password;
}

std::string &Config::database()
{
    return _database;
}