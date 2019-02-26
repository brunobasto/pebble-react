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
            if (previous != NULL)
            {
                previous->tail = hashmap->tail;
                free(hashmap->head);
                hashmap->head = NULL;
            }
            else
            {
                if (hashmap->tail != NULL)
                {
                    PebbleHashMap *next = hashmap->tail;
                    free(hashmap->head);
                    hashmap->head = next->head;
                    hashmap->tail = next->tail;
                    free(next);
                }
                else
                {
                    free(hashmap->head);
                    hashmap->head = NULL;
                }
            }

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
    if (hashmap->head != NULL) {
        free(hashmap->head);
    }
    if (hashmap->tail != NULL) {
        hash_free(hashmap->tail);
    }
    free(hashmap);
}