#include <bot.h>

TelegramBot::TelegramBot(const string& tok) : httpsSocket("api.telegram.org"), token(tok), offset(0){
    json result = getUpdates();
    if(result["ok"] == false)
        throw runtime_error("Invalid bot token");
}

HTTPResponse TelegramBot::method(const string& method, const string& args){
    string q = "GET https://api.telegram.org/bot";
    q.append(token);
    q.append(method);
    q.append(args);
    q.append(" HTTP/1.1\r\nHOST:api.telegram.org\r\nConnection: Keep-Alive\r\n\r\n");

    cout << endl << endl << "meth: " << q << endl << endl;

    return httpsSocket.query(q);
}
