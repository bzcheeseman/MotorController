#ifndef COMPILER_H
#define COMPILER_H

#define _XOPEN_SOURCE 500

/*
 * TODO: Documentation for all these functions
 * TODO: Add GCode parser, input GCode table into map
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

struct entry_s {
  char *key;
  char *value;
  struct entry_s *next;
};

typedef struct entry_s entry_t;

typedef struct {
  entry_t **entries;
  int size;
} map_t ;

/**
 *
 * @param size
 * @return
 */
map_t *mapCtor(int size);

/**
 *
 * @param map
 */
void mapDtor(map_t* map);

/**
 *
 * @param map
 * @param key
 * @return
 */
int hashEntry(map_t *map, char *key);

/**
 *
 * @param key
 * @param value
 * @return
 */
entry_t *newEntry(char *key, char *value);

/**
 *
 * @param map
 * @param key
 * @param value
 */
void addEntry(map_t *map, char *key, char *value);

/**
 *
 * @param map
 * @param key
 * @return
 */
char *getEntry(map_t *map, char *key);

/**
 * Handles parsing each line of the GCode.
 */
// char *parseLine(char *input);

#endif //COMPILER_H
