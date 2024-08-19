#ifndef __HAKVPAIRLIST_H__
#define __HAKVPAIRLIST_H__

#include <cstddef>

class HAKVPairList {
    protected:
        const char *key;
        const char *value;
        HAKVPairList *next;
    public:
        HAKVPairList(const char* key = NULL, const char *value = NULL);
        void append(const char *key, const char *value=NULL);
        inline const char *getKey() {return key;}
        inline const char *getValue() {return value;}
        inline HAKVPairList *getNext() {return next;}
};

#endif