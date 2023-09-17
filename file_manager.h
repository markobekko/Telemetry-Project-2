#pragma once

#include <stdio.h>

// Path of the candump.log file
extern const char FILE_PATH[];

/**
 * @brief Opens the file with append mode
 *
 * @param filename name of the file
 * @return int 0 if ok, -1 if there are some errors
 */
FILE* open_file(const char* filename);

/**
 * @brief Return the number of rows in the candump.log file
 *
 * @return -1 if there are some errors, everything else means it's successful
 */
int get_rows_of_file();

/**
 * @brief Returns how many times in the file candump.log "Start" and "Stop" appear
 *
 * @return -1 if there are some errors, everything else means it's successful
 */
int get_start_stop_occurences();
