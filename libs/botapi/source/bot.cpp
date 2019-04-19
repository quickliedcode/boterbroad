#include <bot.h>

TelegramBot::TelegramBot(string tok) : httpsSocket("api.telegram.org"), token(tok){
    json result = method("/getUpdates", "").toJSON();
    if(result["ok"] == false)
        throw runtime_error("Invalid bot token");
}

HTTPResponse TelegramBot::method(const string& method, const string& args){
    string q = "GET https://api.telegram.org/bot";
    q.append(token);
    q.append(method);
    q.append(" HTTP/1.1\r\nHOST:api.telegram.org\r\nConnection: Keep-Alive\r\n\r\n");
    return httpsSocket.query(q);
}

