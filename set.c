#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000

// ��������� ��� �������� �������� ���������.
typedef struct SetEntry {
    char* value;
    struct SetEntry* next;
} SetEntry;

//������ ���������.
typedef struct {
    SetEntry** entries;
} Set;

// ������� ��� ���������� ���-���� �������� (�� ������, �� �� O(1)...)...
unsigned int hash(const char* value) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(value); i++) {
        hash = hash * 19 + value[i];
    }
    return hash % TABLE_SIZE;
}

// ������� ��� �������� ������ �������� ���������.
SetEntry* set_new_entry(const char* value) {
    SetEntry* entry = malloc(sizeof(SetEntry));
    entry->value = strdup(value);
    entry->next = NULL;
    return entry;
}

// ������� ��� �������� ������ ���������.
Set* new_set() {
    Set* set = malloc(sizeof(Set));
    set->entries = calloc(TABLE_SIZE, sizeof(SetEntry*));
    return set;
}

//������� ��� ���������� �������� � ���������.
void sadd(Set* set, const char* value) {
    unsigned int index = hash(value);
    if (set->entries[index] == NULL) {
        set->entries[index] = set_new_entry(value);
    }
    else {
        SetEntry* entry = set->entries[index];
        while (entry != NULL) {
            if (strcmp(entry->value, value) == 0) {  //������������ ��������. 
                return;
            }
            if (entry->next == NULL) {
                break;
            }
            entry = entry->next;
        }
        entry->next = set_new_entry(value);
    }
}

//������� ��� �������� �������� �� ���������.
void srem(Set* set, const char* value) {
    unsigned int index = hash(value);
    SetEntry* entry = set->entries[index];
    SetEntry* prev = NULL;
    while (entry != NULL) {
        if (strcmp(entry->value, value) == 0) {
            if (prev == NULL) {
                set->entries[index] = entry->next;
            }
            else {
                prev->next = entry->next;
            }
            free(entry->value);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

// ������� ��� �������� ������� �������� � ���������.
int sismember(Set* set, const char* value) {
    unsigned int index = hash(value);
    SetEntry* entry = set->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->value, value) == 0) {
            return 1;
        }
        entry = entry->next;
    }
    return 0;
}

//������� ��� ������������ ������, ������� ����������.
void free_set(Set* set) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        SetEntry* entry = set->entries[i];
        while (entry != NULL) {
            SetEntry* prev = entry;
            entry = entry->next;
            free(prev->value);
            free(prev);
        }
    }
    free(set->entries);
    free(set);
}

void saveSetToFile(Set* set, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing");
        exit(1);
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        SetEntry* entry = set->entries[i];
        while (entry != NULL) {
            fprintf(file, "%s\n", entry->value);
            entry = entry->next;
        }
    }

    fclose(file);
}

void loadSetFromFile(Set* set, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        saveSetToFile(set, filename);
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // ������� ������ ����� ������.
        sadd(set, buffer);
    }

    fclose(file);
}
