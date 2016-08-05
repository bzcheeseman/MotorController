#ifndef COMPILER_H
#define COMPILER_H

#define _XOPEN_SOURCE 500

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

map_t *mapCtor(int size);
void mapDtor(map_t* map);
int hashEntry(map_t *map, char *key);
entry_t *newEntry(char *key, char *value);
void addEntry(map_t *map, char *key, char *value);
char *getEntry(map_t *map, char *key);

/**
 * Handles parsing each line of the GCode.
 */
// char *parseLine(char *input);

#endif //COMPILER_H
