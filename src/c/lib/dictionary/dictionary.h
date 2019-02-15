#pragma once

typedef struct {
    char *key;
    void *value;
} KVPair;

typedef struct PebbleDictionary_t {
    KVPair *head;
    struct PebbleDictionary_t *tail;
} PebbleDictionary;

PebbleDictionary* dict_new();
void dict_add(PebbleDictionary *dictionary, const char *key, void *value);
int dict_has(PebbleDictionary *dictionary, const char *key);
void * dict_get(PebbleDictionary *dictionary, const char *key);
void dict_remove(PebbleDictionary *dictionary, const char *key);
void dict_free(PebbleDictionary *dictionary);