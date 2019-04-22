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
        json answer = gbot.getUpdates();

        cout << "size: " << answer["result"].size() << endl;

        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer)){      
                gbot.sendMessage(answer["result"][0]["message"]["chat"]["id"], accept_message);

                int message_id = 0;
                int from_chat_id = 0;
                if(!answer["result"][0]["message"].is_null()){
                    message_id = answer["result"][0]["message"]["message_id"];
                    from_chat_id = answer["result"][0]["message"]["chat"]["id"];
                }else{
                    message_id = answer["result"][0]["channel_post"]["message_id"];
                    from_chat_id = answer["result"][0]["channel_post"]["chat"]["id"];
                }

                if(answer["result"][0]["forward_from"].is_null())
                    gbot.sendMessage("@growbucket", string(answer["result"][0]["message"]["text"]));
                else
                    gbot.forwardMessage("@growbucket", from_chat_id, message_id);
            }
        gbot.refresh();
    }
}
