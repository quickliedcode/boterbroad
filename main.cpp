#include <bot.h>

#define api_token "805012211:AAH8Z7IYj2FAu_lzUcYD5lStdJl9gN4aP7c"

int main(){
    TelegramBot gb(api_token);
    HTTPResponse response;
    string arg = "?timeout=60&limit=3&offset=0";
    string offset;
    while(1){
        response = gb.method("/getUpdates", arg)
        cout << response.getText();
    }
    gb.refresh();
}
