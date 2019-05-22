#include <bot.h>

#define api_token "753935921:AAHGztWq2zxoD0zc1fG_-6OVB91MD6SHiVY"
#define start_message "Приветствую, хихикающий прохожий. Пришли мне смешные сообщения для формата Галереи, и," \
                        " если нам понравится пост - мы обязательно его выложим! Можно пересылать боту сообщения! " \
                        "Если я не отвечаю - не волнуйся. Сообщения все-равно попадают в очередь"
#define accept_message "Сообщение принято. Спасибо за активность"
#define accept_block_message "Блок сообщений принят. Спасибо за активность"


static int sender_id;
static int count_of_messages = 0;

int main(){
    TelegramBot gbot(api_token);
    gbot.regAnswer("/start", start_message);
    while(1){
        json answer = gbot.getUpdates();

        if(answer["result"].size())
            if(!gbot.sendKnownPhrase(answer)){      

                sender_id = answer["result"][0]["message"]["from"]["id"];

                bool close_block = false;
                json peek_answer = gbot.peekUpdates(2);
                if(!peek_answer["result"].size() || peek_answer["result"][0]["message"]["from"]["id"] != sender_id)
					close_block = true;
				count_of_messages++;




                int message_id = 0;
                int from_chat_id = 0;
                if(!answer["result"][0]["message"].is_null()){
                    message_id = answer["result"][0]["message"]["message_id"];
                    from_chat_id = answer["result"][0]["message"]["chat"]["id"];
                }else{
                    message_id = answer["result"][0]["channel_post"]["message_id"];
                    from_chat_id = answer["result"][0]["channel_post"]["chat"]["id"];
                }

                if(from_chat_id == int(-1001401603714))
                    continue;


                if(answer["result"][0]["message"]["forward_sender_name"].is_null() && answer["result"][0]["message"]["forward_from_chat"].is_null() && answer["result"][0]["message"]["forward_from"].is_null()){
                    try {
                        if(answer["result"][0]["message"]["text"].is_null()){
                            int count_of_photos = answer["result"][0]["message"]["photo"].size();
                            gbot.sendPhoto("@growbucket", answer["result"][0]["message"]["photo"][count_of_photos-1]["file_id"]);
                        }
                        else
                            gbot.sendMessage("@growbucket", answer["result"][0]["message"]["text"]);

                        if(close_block){
                            if(!answer["result"][0]["message"]["from"]["last_name"].is_null())
                                gbot.sendMessage("@growbucket", "🏅 Отправил: <a href='tg://user?id=" + to_string(int(answer["result"][0]["message"]["from"]["id"]))+ "'>" + string(answer["result"][0]["message"]["from"]["first_name"]) + " " + string(answer["result"][0]["message"]["from"]["last_name"]) + "</a>");
                            else
                                gbot.sendMessage("@growbucket", "🏅 Отправил: <a href='tg://user?id=" + to_string(int(answer["result"][0]["message"]["from"]["id"]))+ "'>" + string(answer["result"][0]["message"]["from"]["first_name"]) + "</a>");
                            gbot.sendMessage(int(answer["result"][0]["message"]["chat"]["id"]), accept_message);
                        }
                    } catch (...) {
                        gbot.sendMessage("@growbucket", "Ошибка бота на ветке сообщений. JSON: " + answer.dump());
                    }
                }
                else{
                    try {
                        gbot.forwardMessage("@growbucket", from_chat_id, message_id);
                        if(close_block){
                            if(!answer["result"][0]["message"]["from"]["last_name"].is_null())
                                gbot.sendMessage("@growbucket", "🏓 Переслал: <a href='tg://user?id=" + to_string(int(answer["result"][0]["message"]["from"]["id"]))+ "'>" + string(answer["result"][0]["message"]["from"]["first_name"]) + " " + string(answer["result"][0]["message"]["from"]["last_name"]) + "</a>");
                            else{
                                gbot.sendMessage("@growbucket", "🏓 Переслал: <a href='tg://user?id=" + to_string(int(answer["result"][0]["message"]["from"]["id"]))+ "'>" + string(answer["result"][0]["message"]["from"]["first_name"]) + "</a>");
                            }
							if(count_of_messages > 1)
								gbot.sendMessage(int(answer["result"][0]["message"]["chat"]["id"]), accept_block_message);
							else
								gbot.sendMessage(int(answer["result"][0]["message"]["chat"]["id"]), accept_message);
							count_of_messages = 0;

                        }
                    } catch (...) {
                        gbot.sendMessage("@growbucket", "Ошибка бота на ветке пересылки. JSON: " + answer.dump());
                    }

                }




            }
        gbot.refresh();
    }
}
