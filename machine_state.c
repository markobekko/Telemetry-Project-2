#include "machine_state.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int idle_or_running = 0;
static int number_of_sessions = 0;

int parse_message(char message[MAX_CAN_MESSAGE_SIZE], int bytes_received){
    char id[(NUM_CHARS_ID + 1)];
    char payload[MAX_CAN_MESSAGE_SIZE];
    char filename[20];
    FILE* current_file;

    if(check_if_start(message) == 0 && idle_or_running == 0){
        idle_or_running = 1;
        number_of_sessions++;
        return 0;
    }
    if(check_if_start(message) == 0){
        return 0;
    }
    if(check_if_stop(message) == 0){
        idle_or_running = 0;
        return 0;
    }

    for(int i = 0; i < NUM_CHARS_ID; i++){
        id[i] = message[i];
    }
    id[NUM_CHARS_ID] = '\0';

    if(bytes_received - 4 < 0 )
        return -1;

    for(int i = 0; i < bytes_received - 4; i++){
        payload[i] = message[i + 4];
    }
    payload[bytes_received - 4] = '\0';
    if(bytes_received % 2 != 0){ // If payload not valid
        return -1;
    }

    if(idle_or_running == 1){  // Running
        sprintf(filename, "run_%d.txt", number_of_sessions);
        current_file = open_file(filename);
        print_log(current_file, id, payload);
        fclose(current_file);
    }
    return 0;
}
FILE* open_file(const char* filename) {
    FILE* file = fopen(filename, "a"); // Append mode

    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
}
int check_if_start(char message[MAX_CAN_MESSAGE_SIZE]){
    if(strstr(message, "// Start") != NULL)
        return 0;
    return -1;
}
int check_if_stop(char message[MAX_CAN_MESSAGE_SIZE]){
    if(strstr(message, "// Stop") != NULL)
        return 0;
    return -1;
}
void print_log(FILE* current_file, char id[], char payload[]){
    fprintf(current_file, "// received message\n");
    fprintf(current_file, id);
    fprintf(current_file, "#");
    fprintf(current_file, payload);
    fprintf(current_file, "\n\n");
    fprintf(current_file, "// logged message\n");
    fprintf(current_file, "%lu ", (unsigned long)time(NULL));
    fprintf(current_file, id);
    fprintf(current_file, "#");
    fprintf(current_file, payload);
    fprintf(current_file, "\n\n");
}


