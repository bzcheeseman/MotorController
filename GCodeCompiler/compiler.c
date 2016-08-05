#include "compiler.h"

map_t *mapCtor(int size){
  map_t *out = NULL;

  if (size < 1){
    return NULL;
  }

  out = malloc(sizeof(map_t));
  if (!out){
    return NULL;
  }

  out->entries = malloc(sizeof(entry_t*) * size);
  if (!out->entries){
    return NULL;
  }

  for (int i = 0; i < size; i++){
    out->entries[i] = NULL;
  }

  out->size = size;

  return out;
}

void mapDtor(map_t *map){
  free(map->entries);
  free(map);
}

int hashEntry(map_t *map, char *key){
  unsigned long int hash = 0;
  int i = 0;
  while (hash < ULONG_MAX && i < strlen(key)){
    hash = hash<<4;
    hash += key[i];
    i++;
  }

  return hash % map->size;
}

entry_t *newEntry(char *key, char *value){
  entry_t *new;

  new = malloc(sizeof(entry_t));
  if (!new){
    return NULL;
  }

  new->key = strdup(key);
  if (!new->key){
    return NULL;
  }

  new->value = strdup(value);
  if (!new->value){
    return NULL;
  }

  new->next = NULL;

  return new;
}

void addEntry(map_t *map, char *key, char *value){
  int bin = 0;
  entry_t *new = NULL;
  entry_t *next = NULL;
  entry_t *last = NULL;

  bin = hashEntry(map, key);
  next = map->entries[bin];
  while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0){
    last = next;
    next = next->next;
  }

  //already has a value there
  if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0){
    free(next->value);
    next->value = strdup(value);
  }
  //need to enter a new pair/entry
  else{
    new = newEntry(key, value);
    if (!new){
      return;
    }

    if (next == map->entries[bin]){ //beginning of the table
      new->next = next;
      map->entries[bin] = new;
    }
    else if (next == NULL){ //end of the table
      last->next = new;
    }
    else{ //middle of the table
      new->next = next;
      last->next = new;
    }
  }
}

char *getEntry(map_t *map, char *key){
  int bin = hashEntry(map, key);
  entry_t *entryPair;

  entryPair = map->entries[bin];

  while (entryPair != NULL && entryPair->key != NULL && strcmp(key, entryPair->key) > 0){
    entryPair = entryPair->next;
  }

  if (entryPair == NULL || entryPair->key == NULL || strcmp(key, entryPair->key) != 0){
    return NULL;
  }
  else{
    return entryPair->value;
  }
}

int main(){
  map_t *map = mapCtor(4);


  addEntry( map, "key1", "inky" );
	addEntry( map, "key2", "pinky" );
	addEntry( map, "key3", "blinky" );
	addEntry( map, "key4", "floyd" );

	printf( "%s\n", getEntry( map, "key1" ) );
	printf( "%s\n", getEntry( map, "key2" ) );
	printf( "%s\n", getEntry( map, "key3" ) );
	printf( "%s\n", getEntry( map, "key4" ) );

  return 0;
}
