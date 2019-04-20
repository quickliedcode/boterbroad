#include <bot.h>

#define api_token "805012211:AAH8Z7IYj2FAu_lzUcYD5lStdJl9gN4aP7c"

int main(){
    TelegramBot gbot(api_token);
    gbot.regAnswer("/start", "text");
    while(1){

        json answer = gbot.getUpdates();
        cout << answer << endl;
        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer)){
                cout << "333\n";
                gbot.sendMessage(answer["result"][0]["message"]["chat"]["id"], "привет");
            }

        gbot.refresh();

    }

}
