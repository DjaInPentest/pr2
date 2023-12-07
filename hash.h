#ifndef HASH_H
#define HASH_H

typedef struct HashEntry {
    char* key;
    char* value;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    HashEntry** entries;
} HashTable;

unsigned int hash_function(const char* key);
HashEntry* new_entry(const char* key, const char* value);
HashTable* new_table();
void hset(HashTable* table, const char* key, const char* value);
void hdel(HashTable* table, const char* key);
char* hget(HashTable* table, const char* key);
void free_table(HashTable* table);
void saveTableToFile(HashTable* table, const char* filename);
void loadTableFromFile(HashTable* table, const char* filename);

#endif /* HASH_H */
