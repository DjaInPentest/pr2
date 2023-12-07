#ifndef SET_H
#define SET_H

#define TABLE_SIZE 1000

typedef struct SetEntry {
    char* value;
    struct SetEntry* next;
} SetEntry;

// Удалите одно из объявлений функции new_entry
SetEntry* set_new_entry(const char* value);

typedef struct {
    SetEntry** entries;
} Set;

unsigned int hash(const char* value);
Set* new_set();
void sadd(Set* set, const char* value);
void srem(Set* set, const char* value);
int sismember(Set* set, const char* value);
void free_set(Set* set);
void saveSetToFile(Set* set, const char* filename);
void loadSetFromFile(Set* set, const char* filename);

#endif /* SET_H */
