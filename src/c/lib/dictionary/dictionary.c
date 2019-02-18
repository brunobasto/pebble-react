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
        PebbleDictionary *next = dict_new();
        dictionary->tail = next;
        dictionary = dictionary->tail;
    }
    dictionary->head = (KVPair *)malloc(sizeof(KVPair));
    dictionary->head->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(dictionary->head->key, key);
    dictionary->head->value = value;
}

int dict_has(PebbleDictionary *dictionary, const char *key)
{
    if (dictionary == NULL)
        return 0;
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
    if (dictionary == NULL)
        return 0;
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

    PebbleDictionary *current = dictionary;

    while (current != NULL)
    {
        if (strcmp(current->head->key, key) == 0)
        {
            if (current->tail != NULL)
            {
                PebbleDictionary *temp = current->tail;
                current->head = current->tail->head;
                current->tail = temp->tail;
                free(temp);
            }
            return;
        }
        current = current->tail;
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

    PebbleDictionary *tail = dictionary->tail;
    free(dictionary);
    dict_free(tail);
}