#include <bot.h>

#define api_token "753935921:AAHGztWq2zxoD0zc1fG_-6OVB91MD6SHiVY"
#define start_message "Приветствую, хихикающий прохожий. Пришли мне смешные сообщения для формата Галереи, и," \
                        " если нам понравится пост - мы обязательно его выложим! Можно пересылать боту сообщения! " \
                        "Если я не отвечаю - не волнуйся. Сообщения все-равно попадают в очередь"
#define accept_message "Сообщение принято. Надеюсь, оно действительно смешное"

int main(){
    TelegramBot gbot(api_token);
    gbot.regAnswer("/start", start_message);
    while(1){

        cout << endl << endl << " ------ here 1 ------ " << endl << endl;

        json answer = gbot.getUpdates();

        cout << endl << endl << " ------ here 2 ------ " << endl << endl;

        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer)){

                cout << endl << endl << " ------ here 3 ------ " << endl << endl;

                gbot.sendMessage(answer["result"][0]["message"]["chat"]["id"], accept_message);

                cout << endl << endl << " ------ here 4 ------ " << endl << endl;

                int message_id = 0;
                int from_chat_id = 0;
                if(!answer["result"][0]["message"].is_null()){
                    message_id = answer["result"][0]["message"]["message_id"];
                    from_chat_id = answer["result"][0]["message"]["chat"]["id"];
                }else{
                    message_id = answer["result"][0]["channel_post"]["message_id"];
                    from_chat_id = answer["result"][0]["channel_post"]["chat"]["id"];
                }

                gbot.forwardMessage("@growbucket", from_chat_id, message_id);
            }

        gbot.refresh();
    }
}
