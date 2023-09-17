#include "machine_state.h"
#include "file_manager.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct data_message data_message_t;
static int idle_or_running = 0;
static int number_of_sessions = 0;
data_message_t *data = NULL;
int dataSize = 0;


int parse_message(char message[MAX_CAN_MESSAGE_SIZE], int bytes_received, time_t current_timestamp){
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
        print_data();
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
        process_id(id, current_timestamp);
        sprintf(filename, "run_%d.txt", number_of_sessions);
        current_file = open_file(filename);
        print_log(current_file, id, payload);
        fclose(current_file);
    }
    return 0;
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
    fprintf(current_file, "%s\n\n", payload);
    fprintf(current_file, "// logged message\n");
    fprintf(current_file, "%lu ", (unsigned long)time(NULL));
    fprintf(current_file, id);
    fprintf(current_file, "#");
    fprintf(current_file, "%s\n\n", payload);
}

void process_id(char current_id[4], time_t current_timestamp) {
    time_t current_time = ((unsigned long)time(NULL)) - current_timestamp;

    int found = 0;
    for (int i = 0; i < dataSize; i++) {
        if (strncmp(data[i].id, current_id, 3) == 0) { // Compare only the first 3 characters
            double time_diff = difftime(current_time - current_timestamp, data[i].last_time);
            data[i].total_time += time_diff;
            data[i].last_time = current_time - current_timestamp;
            data[i].count++;
            found = 1;
            break;
        }
    }

    if (!found) {
        dataSize++;
        data = (data_message_t*)realloc(data, sizeof(data_message_t) * dataSize);
        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(data[dataSize - 1].id, current_id, 3); // Copy only the first 3 characters
        data[dataSize - 1].id[3] = '\0'; // Null-terminate the string
        data[dataSize - 1].count = 1;
        data[dataSize - 1].total_time = 0;
        data[dataSize - 1].last_time = current_time - current_timestamp;
    }
}

void print_data(){
    char filename[20];
    unsigned long mean_time;
    FILE* csv_file;
    sprintf(filename, "csv_%d.csv", number_of_sessions);
    csv_file = open_file(filename);
    fprintf(csv_file,"ID, number_of_messages, mean_time\n");

    for (int i = 0; i < dataSize; i++) {
        printf("ID: %s\n", data[i].id);
        printf("Count: %d\n", data[i].count);

        if(data[i].count == 1){
            mean_time = data[i].total_time;
            printf("Mean time between appearances: %lu ms\n", mean_time);
        }
        else if (data[i].count > 1) {
            mean_time = data[i].total_time / (data[i].count - 1);
            printf("Mean time between appearances: %lu ms\n", mean_time);
        }
        fprintf(csv_file,"%s,%d,%lu\n", data[i].id, data[i].count, mean_time);
    }
    free(data);
    dataSize = 0;
    data = NULL;
    fclose(csv_file);
}

