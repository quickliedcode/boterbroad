#include <bot.h>

#define api_token "805012211:AAH8Z7IYj2FAu_lzUcYD5lStdJl9gN4aP7c"

int main(){
    TelegramBot gb(api_token);
    while(1){

        json answer = gb.getUpdates();
        gb.refresh();

    }

}
