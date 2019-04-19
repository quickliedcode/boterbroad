#include <bot.h>

#define api_token "805012211:AAH8Z7IYj2FAu_lzUcYD5lStdJl9gN4aP7c"

int main(){
    TelegramBot gbot(api_token);
    while(1){

        json answer = gbot.getUpdates();
        gbot.regAnswer("/start", "text");
        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer))
                gbot.sendMessage(answer["result"][0]["message"]["chat"]["id"], "привет");
        cout << answer << endl;
        gbot.refresh();

    }

}
