#pragma once

typedef struct {
    void *key;
    void *value;
} HashMapPair;

typedef struct PebbleHashMap_t {
    HashMapPair *head;
    struct PebbleHashMap_t *tail;
} PebbleHashMap;

PebbleHashMap* hash_new();
void hash_add(PebbleHashMap *hashmap, void *key, void *value);
int hash_has(PebbleHashMap *hashmap, void *key);
void * hash_get(PebbleHashMap *hashmap, void *key);
void hash_remove(PebbleHashMap *hashmap, void *key);
void hash_free(PebbleHashMap *hashmap);