#include <pic.h>

void handle_timer(){
    pic_send_eoi(0);
}
