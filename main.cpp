#include <stdio.h>
#include <iostream>
using namespace std;

extern "C"{
    #include "fake_receiver.h"
    #include "machine_state.h"
    #include "file_manager.h"
}

int main(void){
    time_t current_timestamp = ((unsigned long)time(NULL));
    int bytes_received;
    char message[MAX_CAN_MESSAGE_SIZE];

    open_can(FILE_PATH);

    for(int i = 0; i < get_rows_of_file(); i++){
        bytes_received = can_receive(message);
        parse_message(message, bytes_received, current_timestamp);
    }

    close_can();

    return 0;
}


