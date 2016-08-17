#ifndef COMPILER_H
#define COMPILER_H

#define _XOPEN_SOURCE 500

/*
 * TODO: Add GCode parser, input GCode table into map
 * TODO: Documentation for structs
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
 * Constructs a map object with a given size.
 * Probably can't really resize the map dynamically - though I should fix that
 *
 * @param size Size of the map at construction
 * @return pointer to a new map.
 */
map_t *mapCtor(int size);

/**
 * Frees memory allocated to the map/pointer.
 *
 * @param map The map to destruct
 */
void mapDtor(map_t* map);

/**
 * Hashes the key of an entry that will be added to the map.  Mostly internal use.
 *
 * @param map Map to which an entry will be added
 * @param key The key of the entry to be added
 * @return The hash for the entry
 */
int hashEntry(map_t *map, char *key);

/**
 * Creates a new entry out of a key and value pair.  Mostly internal use.
 *
 * @param key Key of new entry
 * @param value Value of new entry
 * @return The new entry
 */
entry_t *newEntry(char *key, char *value);

/**
 * Adds an entry to the map.  External use for adding a key, value pair to the map.  Hashes the key, creates a new entry
 * and adds it to the map.
 *
 * @param map The map to which an entry will be added
 * @param key The key to this new entry
 * @param value The value of this new entry
 */
void addEntry(map_t *map, char *key, char *value);

/**
 * Gets a specified entry from the key provided.
 *
 * @param map The map in which this entry (to be found) resides
 * @param key The key of the entry we want.
 * @return The char* corresponding to the value of the entry corresponding to the key.
 */
char *getEntry(map_t *map, char *key);

/**
 * Handles parsing each line of the GCode.
 */
// char *parseLine(char *input);

#endif //COMPILER_H
