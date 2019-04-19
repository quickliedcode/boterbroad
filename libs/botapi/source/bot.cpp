#include <bot.h>

TelegramBot::TelegramBot(string tok) : httpsSocket("api.telegram.org"){
    string q = "GET https://api.telegram.org/bot";
    q.append(tok);
    q.append("/getUpdates HTTP/1.1\r\nHOST:api.telegram.org\r\nConnection: Keep-Alive\r\n\r\n\0");
    HTTPResponse str = httpsSocket.query(q);
    json result = str.toJSON();
    if(result["ok"] == false)
        throw runtime_error("Invalid bot token");
}
