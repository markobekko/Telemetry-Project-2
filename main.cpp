#include <stdio.h>
#include <iostream>
using namespace std;

const char FILE_PATH[] = "/home/marko/Desktop/Eagle/project_2/candump.log";

extern "C"{
    #include "fake_receiver.h"
    #include "machine_state.h"
}

int get_rows_of_file();

int main(void){
    int bytes_received;
    cout << "Welcome to Project 2" << endl;
    char message[MAX_CAN_MESSAGE_SIZE];

    open_can(FILE_PATH);
    //bytes_received = can_receive(message);

    //for(int i = 0; i < 6; i++){
    //    printf("\n%c", message[i]);
    //}
    //parse_message(message, bytes_received);

    for(int i = 0; i < get_rows_of_file(); i++){
        bytes_received = can_receive(message);
        parse_message(message, bytes_received);
    }

    close_can();

    return 0;
}

int get_rows_of_file(){
    FILE *file = fopen(FILE_PATH, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int lineCount = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    if (ch != '\n' && lineCount > 0) {
        lineCount++;
    }

    fclose(file);

    return lineCount;
}
