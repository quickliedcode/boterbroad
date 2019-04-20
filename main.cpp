#include <bot.h>

#define api_token "805012211:AAH8Z7IYj2FAu_lzUcYD5lStdJl9gN4aP7c"
#define start_message "Приветствую, хихикающий прохожий. Пришли мне смешные сообщения для формата Галереи, и," \
                        " если нам понравится пост - мы обязательно его выложим! Можно пересылать боту сообщения! " \
                        "Если я не отвечаю - не волнуйся. Сообщения все-равно попадают в очередь"
#define accept_message "Сообщение принято. Надеюсь, оно действительно смешное"

int main(){
    TelegramBot gbot(api_token);
    gbot.regAnswer("/start", start_message);
    while(1){

        json answer = gbot.getUpdates();
        cout << answer << endl;
        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer)){
                gbot.sendMessage(answer["result"][0]["message"]["chat"]["id"], accept_message);
            }

        gbot.refresh();

    }

}
