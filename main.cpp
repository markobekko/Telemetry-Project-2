#include <stdio.h>
#include <iostream>
using namespace std;

extern "C"{
    #include "fake_receiver.h"
    #include "machine_state.h"
    #include "file_manager.h"
}

#define MAX_ID_LENGTH 4

int get_rows_of_file();

int main(void){
    time_t current_timestamp = ((unsigned long)time(NULL));
    int bytes_received;
    cout << "Welcome to Project 2" << endl;
    char message[MAX_CAN_MESSAGE_SIZE];

    open_can(FILE_PATH);

    //message_stats list = init_size();

    for(int i = 0; i < get_rows_of_file(); i++){
        bytes_received = can_receive(message);
        parse_message(message, bytes_received, current_timestamp);
    }

    //count_ids(&list);

    close_can();

    printf("Statistics:\n");
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the file.\n");
        return 1;
    }
;
    fclose(file);
    printData();

    return 0;
}


