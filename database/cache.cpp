#include "cache.h"
#include "../config/config.h"

#include <exception>
#include <mutex>

#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>

namespace database
{
    std::mutex _mtx;
    Cache::Cache()
    {
        std::string host = Config::get().get_cache_servers().value().substr(0,
                                                                    Config::get().get_cache_servers().value().find(':'));
        std::string port = Config::get().get_cache_servers().value().substr(Config::get().get_cache_servers().value().find(':') + 1);

        std::cout << "cache host:" << host <<" port:" << port << std::endl;
        _stream = rediscpp::make_stream(host, port);
    }

    Cache Cache::get()
    {
        static Cache instance;
        return instance;
    }

    void Cache::put([[maybe_unused]] std::string id, [[maybe_unused]] const std::string &val)
    {
        std::lock_guard<std::mutex> lck(_mtx);
        rediscpp::value response = rediscpp::execute(*_stream, "set",
                                                     id,
                                                     val, "ex", "60");
    }

    bool Cache::get([[maybe_unused]] std::string id, [[maybe_unused]] std::string &val)
    {
        std::lock_guard<std::mutex> lck(_mtx);
        rediscpp::value response = rediscpp::execute(*_stream, "get", id);

        if (response.is_error_message())
            return false;
        if (response.empty())
            return false;

        val = response.as<std::string>();
        return true;
    }
}