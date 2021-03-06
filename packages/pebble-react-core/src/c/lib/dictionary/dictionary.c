#include <pebble.h>

#include "dictionary.h"

typedef void (*DictForeachCall)(char *key, void *value);

static DictForeachCall callDictForeach = NULL;

PebbleDictionary *dict_new()
{
    PebbleDictionary *dictionary = (PebbleDictionary *)malloc(sizeof(PebbleDictionary));
    dictionary->head = NULL;
    dictionary->tail = NULL;
    return dictionary;
}

void dict_add(PebbleDictionary *dictionary, const char *key, void *value)
{
    if (dictionary == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Trying to add %s: %p on null dictionary", key, value);
        return;
    }
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
        dictionary->tail = dict_new();
        dictionary = dictionary->tail;
    }
    dictionary->head = (KVPair *)malloc(sizeof(KVPair));
    dictionary->head->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(dictionary->head->key, key);
    dictionary->head->value = value;
}

void dict_foreach(PebbleDictionary *dictionary, void *callback)
{
    if (dictionary == NULL)
    {
        return;
    }
    if (dictionary->head == NULL)
    {
        return;
    }
    if (dictionary->head->key == NULL)
    {
        return;
    }
    callDictForeach = (DictForeachCall)callback;
    callDictForeach(dictionary->head->key, dictionary->head->value);
    dict_foreach(dictionary->tail, callback);
}

int dict_has(PebbleDictionary *dictionary, const char *key)
{
    if (dictionary == NULL)
        return 0;
    if (dictionary->head == NULL)
        return 0;
    if (dictionary->head->key == NULL)
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
    if (dictionary == NULL)
        return 0;
    if (dictionary->head == NULL)
        return 0;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0) {
            return dictionary->head->value;
        }
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
            if (previous != NULL)
            {
                previous->tail = dictionary->tail;
                free(dictionary->head->key);
                dictionary->head->key = NULL;
                free(dictionary->head);
                dictionary->head = NULL;
                free(dictionary);
            }
            else
            {
                if (dictionary->tail != NULL)
                {
                    PebbleDictionary *next = dictionary->tail;
                    free(dictionary->head->key);
                    free(dictionary->head);
                    dictionary->head = next->head;
                    dictionary->tail = next->tail;
                    free(next);
                }
                else
                {
                    free(dictionary->head->key);
                    dictionary->head->key = NULL;
                    free(dictionary->head);
                    dictionary->head = NULL;
                }
            }

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

    if (dictionary->head != NULL)
    {
        if (dictionary->head->key != NULL)
            free(dictionary->head->key);
        free(dictionary->head);
    }

    dict_free(dictionary->tail);
    free(dictionary);
}