cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (USER_SERVICE "user_service")
SET (BUCKET_SERVICE "bucket_service")
SET (ITEM_SERVICE "item_service")

set (STD_CXX "c++17")
set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${USER_SERVICE} user_service/main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/cache.cpp
                                 database/user.cpp)
                                 
add_executable(${BUCKET_SERVICE} bucket_service/main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/bucket.cpp)
                             
add_executable(${ITEM_SERVICE} item_service/main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/item.cpp)

target_include_directories(${USER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${USER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${USER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${USER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${USER_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

target_include_directories(${BUCKET_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${BUCKET_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${BUCKET_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${BUCKET_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                             
target_link_libraries(${BUCKET_SERVICE} PRIVATE 
                            ${CMAKE_THREAD_LIBS_INIT} 
                            ${Poco_LIBRARIES}
                            "PocoData"
                            "PocoDataMySQL"
                            "mysqlclient"
                            ZLIB::ZLIB)

target_include_directories(${ITEM_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${ITEM_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${ITEM_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ITEM_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                                                         
target_link_libraries(${ITEM_SERVICE} PRIVATE 
                            ${CMAKE_THREAD_LIBS_INIT} 
                            ${Poco_LIBRARIES}
                            "PocoData"
                            "PocoDataMySQL"
                            "mysqlclient"
                            ZLIB::ZLIB)
                             
