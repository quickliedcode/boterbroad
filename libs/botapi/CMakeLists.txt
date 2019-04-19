project(botapi)

set(SRC source/bot.cpp header/bot.h)

add_library(${PROJECT_NAME} STATIC ${SRC})
