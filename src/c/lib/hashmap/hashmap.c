#include <pebble.h>

#include "hashmap.h"

PebbleHashMap *hash_new()
{
    PebbleHashMap *hashmap = (PebbleHashMap *)malloc(sizeof(PebbleHashMap));
    hashmap->head = NULL;
    hashmap->tail = NULL;
    return hashmap;
}

void hash_add(PebbleHashMap *hashmap, void *key, void *value)
{
    if (hash_has(hashmap, key))
    {
        hash_remove(hashmap, key);
    }

    if (hashmap->head != NULL)
    {
        while (hashmap->tail != NULL)
        {
            hashmap = hashmap->tail;
        }
        PebbleHashMap *next = hash_new();
        hashmap->tail = next;
        hashmap = hashmap->tail;
    }
    hashmap->head = (HashMapPair *)malloc(sizeof(HashMapPair));
    hashmap->head->key = key;
    hashmap->head->value = value;
}

int hash_has(PebbleHashMap *hashmap, void *key)
{
    if (hashmap->head == NULL)
        return 0;
    while (hashmap != NULL)
    {
        if (hashmap->head->key == key)
            return 1;
        hashmap = hashmap->tail;
    }
    return 0;
}

void *hash_get(PebbleHashMap *hashmap, void *key)
{
    if (hashmap->head == NULL)
        return 0;
    while (hashmap != NULL)
    {
        if (hashmap->head->key == key)
            return hashmap->head->value;
        hashmap = hashmap->tail;
    }
    return 0;
}

void hash_remove(PebbleHashMap *hashmap, void *key)
{
    if (hashmap->head == NULL)
        return;
    PebbleHashMap *previous = NULL;
    while (hashmap != NULL)
    {
        if (hashmap->head->key == key)
        {
            if (previous == NULL)
            {
                // free(hashmap->head->key);
                hashmap->head->key = NULL;
                if (hashmap->tail != NULL)
                {
                    PebbleHashMap *toremove = hashmap->tail;
                    hashmap->head->key = toremove->head->key;
                    hashmap->tail = toremove->tail;
                    free(toremove->head);
                    free(toremove);
                    return;
                }
            }
            else
            {
                previous->tail = hashmap->tail;
            }
            // free(hashmap->head->key);
            free(hashmap->head);
            free(hashmap);
            return;
        }
        previous = hashmap;
        hashmap = hashmap->tail;
    }
}

void hash_free(PebbleHashMap *hashmap)
{
    if (hashmap == NULL)
        return;
    free(hashmap->head->key);
    free(hashmap->head->value);
    free(hashmap->head);
    PebbleHashMap *tail = hashmap->tail;
    free(hashmap);
    hash_free(tail);
}