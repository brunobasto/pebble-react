#include <pebble.h>

#include "dictionary.h"

PebbleDictionary *dict_new()
{
    PebbleDictionary *dictionary = (PebbleDictionary *)malloc(sizeof(PebbleDictionary));
    dictionary->head = NULL;
    dictionary->tail = NULL;
    return dictionary;
}

void dict_add(PebbleDictionary *dictionary, const char *key, void *value)
{
    if (dict_has(dictionary, key))
    {
        dict_remove(dictionary, key);
    }

    if (dictionary->head != NULL)
    {
        while (dictionary->tail != NULL)
        {
            dictionary = dictionary->tail;
        }
        PebbleDictionary *next = dict_new();
        dictionary->tail = next;
        dictionary = dictionary->tail;
    }
    int key_length = strlen(key) + 1;
    // int value_length = strlen(value) + 1;
    dictionary->head = (KVPair *)malloc(sizeof(KVPair));
    dictionary->head->key = (char *)malloc(key_length * sizeof(char));
    // dictionary->head->value = (char *)malloc(value_length * sizeof(char));
    strcpy(dictionary->head->key, key);
    // strcpy(dictionary->head->value, value);
    dictionary->head->value = value;
}

int dict_has(PebbleDictionary *dictionary, const char *key)
{
    if (dictionary->head == NULL)
        return 0;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0)
            return 1;
        dictionary = dictionary->tail;
    }
    return 0;
}

void *dict_get(PebbleDictionary *dictionary, const char *key)
{
    if (dictionary->head == NULL)
        return 0;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0)
            return dictionary->head->value;
        dictionary = dictionary->tail;
    }
    return 0;
}

void dict_remove(PebbleDictionary *dictionary, const char *key)
{
    if (dictionary->head == NULL)
        return;
    PebbleDictionary *previous = NULL;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0)
        {
            if (previous == NULL)
            {
                free(dictionary->head->key);
                dictionary->head->key = NULL;
                if (dictionary->tail != NULL)
                {
                    PebbleDictionary *toremove = dictionary->tail;
                    dictionary->head->key = toremove->head->key;
                    dictionary->tail = toremove->tail;
                    free(toremove->head);
                    free(toremove);
                    return;
                }
            }
            else
            {
                previous->tail = dictionary->tail;
            }
            free(dictionary->head->key);
            free(dictionary->head);
            free(dictionary);
            return;
        }
        previous = dictionary;
        dictionary = dictionary->tail;
    }
}

void dict_free(PebbleDictionary *dictionary)
{
    if (dictionary == NULL)
        return;
    free(dictionary->head->key);
    free(dictionary->head);
    PebbleDictionary *tail = dictionary->tail;
    // free(dictionary);
    dict_free(tail);
}