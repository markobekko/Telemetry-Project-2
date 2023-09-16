#pragma once

#include "fake_receiver.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_CHARS_ID 3

static int number_chars_id = 0;

int parse_message(char message[MAX_CAN_MESSAGE_SIZE], int bytes_received, time_t current_timestamp);

FILE* open_file(const char* filename);

int check_if_start(char message[MAX_CAN_MESSAGE_SIZE]);

int check_if_stop(char message[MAX_CAN_MESSAGE_SIZE]);

void print_log(FILE* current_file, char id[], char payload[]);

void processID(char currentID[4], time_t current_timestamp);

void printData();
