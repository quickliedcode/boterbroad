cmake_minimum_required(VERSION 3.10)
project(boterbroads)

set(CMAKE_BUILD_TYPE Debug)

include_directories(libs/botapi/header)

include_directories(libs/openssl/ssl)
include_directories(libs/openssl/crypto/err)
include_directories(libs/openssl/include)

include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include/)

include_directories(libs/openssl/include)
include_directories(libs/openssl/include/openssl)
include_directories(libs/json-develop/include/nlohmann)
include_directories(libs/json-develop/include)

link_directories(/home/ubuntu/bot/boterbroad/libs/openssl)
link_directories(/home/ubuntu/bot/boterbroad/libs/botapi)

subdirs(libs/botapi)

set(SRC main.cpp)
set(LIBS botapi ssl crypto dl)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIBS})
