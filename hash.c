#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 1000

typedef struct HashEntry {
    char* key; //op1
    char* value; //op2
    struct HashEntry* next; //Некст для записи.
} HashEntry;

typedef struct {
    HashEntry** entries; //Все "ячейки".
} HashTable;

unsigned int hash_function(const char* key) { //Я решил не заморачиваться и от балды написал умножение каждого кода символа + текущее значение символа, ну, и поделить на размер таблицы).
    unsigned int hash = 0;
    for (int i = 0; i < strlen(key); i++) { //Я сделал исключение для O(n), потому что, вроде как, иначе сделать невозможно.
        hash = hash * 31 + key[i];
    }
    return hash % TABLE_SIZE;
}

HashEntry* new_entry(const char* key, const char* value) { //Когда создаём новую ячейку, берём операнды, выделяем под них память.
    HashEntry* entry = malloc(sizeof(HashEntry)); //1
    if (entry == NULL) {
        printf("Memory allocation failed\n"); //1                O(1+1+1+1+1+1+1+1+1)==O(1) :).
        return NULL; //1
    }

    entry->key = strdup(key);
    if (entry->key == NULL) {
        printf("Memory allocation failed for key\n");
        free(entry);
        return NULL;
    }

    entry->value = strdup(value);
    if (entry->value == NULL) {
        printf("Memory allocation failed for value\n");
        free(entry->key);
        free(entry);
        return NULL;
    }

    entry->next = NULL;
    return entry;
}

HashTable* new_table() { //Создаём табличку, выделяем память под размер таблицы. 
    HashTable* table = malloc(sizeof(HashTable));
    table->entries = calloc(TABLE_SIZE, sizeof(HashEntry*));
    return table;
}

void hset(HashTable* table, const char* key, const char* value) { //HSET, берём ключ из функции, если нет коллизии, добавим запись.
    unsigned int index = hash_function(key);
    printf("Index: %d\n", index); //Добавим вывод (дебаглог).

    if (table->entries[index] == NULL) {
        printf("Creating new entry\n"); //Добавим вывод (дебаглог).
        table->entries[index] = new_entry(key, value); //Записываем в табличку операнды (O(1+1+1)=O(1)).
    }
    else {
        printf("Collision detected\n"); //Добавим вывод (дебаглог). //В entries уже есть (не NULL), значит кто-то уже что-то записал, коллизия, метод цепочек, просто перезаписываем второй оп.
        HashEntry* entry = table->entries[index];
        while (entry != NULL) {
            if (strcmp(entry->key, key) == 0) {
                free(entry->value);
                entry->value = strdup(value);   //По факту, при коллизии проходимся по табличке, O(n), если ключи совпадают - перезаписываем. 
                return;
            }
            if (entry->next == NULL) {
                break;
            }
            entry = entry->next;
        }
        entry->next = new_entry(key, value); //Записываем когда будет next==Null.
    }
}


void hdel(HashTable* table, const char* key) { //Аналогично Hset, записываем значение ключа, 
    unsigned int index = hash_function(key);
    HashEntry* entry = table->entries[index];
    HashEntry* prev = NULL;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                table->entries[index] = entry->next;
            }
            else {
                prev->next = entry->next;
            }
            free(entry->key);
            free(entry->value);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

char* hget(HashTable* table, const char* key) {
    unsigned int index = hash_function(key);
    HashEntry* entry = table->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void free_table(HashTable* table) {             //Кстати, в main не используется. :) O(1)!!
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = table->entries[i];
        while (entry != NULL) {
            HashEntry* prev = entry;
            entry = entry->next;
            free(prev->key);
            free(prev->value);
            free(prev);
        }
    }
    free(table->entries);
    free(table);
}

void saveTableToFile(HashTable* table, const char* filename) {
    printf("Inside saveTableToFile\n");
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {                                       //Пусть не O(1), но в файл действительно записывается, хотя можно использовать другой аргумент записи чтобы дополнять.
        HashEntry* entry = table->entries[i];                                    //Но нужна будет куча костылей).
        while (entry != NULL) {
            printf("Writing to file: %s,%s\n", entry->key, entry->value); //Если с большими костылями попробовать записывать сразу через HSET, то можно будет O(1).
            fprintf(file, "%s,%s\n", entry->key, entry->value);
            entry = entry->next;
        }
    }

    fclose(file);
}

void loadTableFromFile(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        saveTableToFile(table, filename);  //Создание нового файла при отсутствии. (Пустого)).
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Убираем символ новой строки.

        char* key = strtok(buffer, ",");
        char* value = strtok(NULL, ",");

        hset(table, key, value); //Записываем ячейки, чтобы если была коллизия, можно было сразу узнать.
    }

    fclose(file);
}

