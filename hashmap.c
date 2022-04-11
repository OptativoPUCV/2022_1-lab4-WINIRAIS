#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  int pos = hash(key, map->capacity);
  for (int i = pos; i < map->capacity; i++) {
    if(searchMap(map,key) != NULL) return;
    if (map->buckets[i] == NULL) {
      Pair *hashElem = createPair(key, value);
      map->buckets[i] = hashElem;
      map->size = map->size+1;
      break;
    }
    if(i == map->capacity-1) i = 0;
  }
}

void enlarge(HashMap *map) {
  enlarge_called = 1;// no borrar (testing purposes)
  Pair** aux  = map->buckets;
  map -> capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity,sizeof(Pair*));
  map->size = 0; 
  for(int i = 0 ; i < (map->capacity/2) ; i++){
    if(aux[i]!= NULL && aux[i]->key != NULL ){
      insertMap(map, aux[i]->key, aux[i]->value);
    }
    
  }
}

HashMap *createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity);
  for (int i = 0; i < capacity; i++) {
    map->buckets[i] = NULL;
  }
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap *map, char *key) {
  for (int i = hash(key, map->capacity); i < map->capacity; i++) {
    if (map->buckets[i] == NULL) break;
    if (is_equal(map->buckets[i]->key, key) == 1) {
      map->current = i;
      map->buckets[i]->key = NULL;
      map->buckets[i]->value = NULL;
      map->size = map->size - 1;
      break;
    }
  }
}

Pair *searchMap(HashMap *map, char *key) {
  for (int i = hash(key, map->capacity); i < map->capacity; i++) {
    if (map->buckets[i] == NULL) {
      map->current = i;
      return NULL;
    }
    if (is_equal(map->buckets[i]->key, key) == 1) {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
Pair *firstMap(HashMap *map) {
  int i;
  for( i = 0 ; i < map->capacity ; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
    if(i == map->capacity) i = 0;
  }
  map->current = i;
  return NULL;
}

Pair *nextMap(HashMap *map) {
  for(int i = map->current + 1 ; i < map->capacity ; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
    if(i == map->capacity) i = 0;
  }
  return NULL; 
}
