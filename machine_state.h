#pragma once

#include "fake_receiver.h"
#include <stdio.h>
#include <time.h>

// defines the length of the ID, excluding the null terminator string
#define NUM_CHARS_ID 3

/** @struct data_message
 *  @brief Structure for saving the data of each unique ID
 *  @var data_message::id
 *  Member 'id' contains the ID of the message, plus the null terminator character
 *  @var data_message::count
 *  Member 'count' contains the total count of how many times the ID appears
 *  @var data_message::total_time
 *  Member 'total_time' contains the total time in unix timestamp between each message
 *  @var data_message::last_time
 *  Member 'last_time' contains the last time registered before the current one
 */
typedef struct data_message{
    char id[NUM_CHARS_ID + 1];
    int count;
    unsigned long total_time;
    time_t last_time;
} data_message_t;

/**
 * @brief Parses the message
 *
 * @param message string containing the message
 * @param bytes_received how many bytes is the message made of
 * @param current_timestamp timestamp when the program started
 * @return int 0 if ok, -1 if there are some errors
 */
int parse_message(char message[MAX_CAN_MESSAGE_SIZE], int bytes_received, time_t current_timestamp);

/**
 * @brief Checks if the message contains "Start"
 *
 * @param message string containing the message
 * @return int 0 if ok, -1 if there are some errors
 */
int check_if_start(char message[MAX_CAN_MESSAGE_SIZE]);

/**
 * @brief Checks if the message contains "Stop"
 *
 * @param message string containing the message
 * @return int 0 if ok, -1 if there are some errors
 */
int check_if_stop(char message[MAX_CAN_MESSAGE_SIZE]);

/**
 * @brief Prints to the file the log of the message
 *
 * @param current_file the file where we write the output
 * @param id the id of the message
 * @param payload the payload of the message
 */
void print_log(FILE* current_file, char id[], char payload[]);

/**
 * @brief Computes the statistics for the messages and saves them
 *
 * @param current_id the id of the message
 * @param current_timestamp timestamp when the program started
 */
void process_id(char current_id[4], time_t current_timestamp);

/**
 * @brief Prints to the file the csv table the data saved from process_id()
 *
 */
void print_data();
