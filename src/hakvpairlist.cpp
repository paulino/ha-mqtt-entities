#include"hakvpairlist.h"

HAKVPairList::HAKVPairList(const char *key, const char *value)
{
    this->key = key;
    this->value = value;
    this->next = NULL;
}

/** Append new element following the list */
void HAKVPairList::append(const char *key, const char *value)
{    
    if(this->key == NULL)
    {
        this->key = key;
        this->value = value;
    }
    else
    {
        HAKVPairList *pair = new HAKVPairList(key,value);
        HAKVPairList *last = this->next;
        while(this->next != NULL)
            last = last->next;
        last->next = pair;
    }
}