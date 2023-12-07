#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct StructureElement {
    char* value;
    struct StructureElement* next;
} StructureElement;

typedef struct {
    StructureElement* head;
} Structure;

Structure* createQueue() {
    Structure* queue = malloc(sizeof(Structure));
    queue->head = NULL;
    return queue;
}

void qpush(Structure* queue, const char* value) {
    StructureElement* newElement = (StructureElement*)malloc(sizeof(StructureElement));
    newElement->value = strdup(value);
    newElement->next = NULL;

    if (queue->head == NULL) {
        queue->head = newElement;
    }
    else {
        StructureElement* current = queue->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newElement;
    }
}

void qpop(Structure* queue) {
    if (queue == NULL || queue->head == NULL) {
        printf("The queue is empty or invalid");
        exit(1);
    }

    StructureElement* removed = queue->head;
    queue->head = queue->head->next;

    printf("POPed-> %s\n", removed->value);
    free(removed->value);
    free(removed);
}

void printQueue(Structure* queue) {
    for (StructureElement* print = queue->head; print != NULL; print = print->next) {
        printf("%s\n", print->value);
    }
    printf("\n");
}

void saveQueueToFile(Structure* queue, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing");
        exit(1);
    }

    StructureElement* current = queue->head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->value);
        current = current->next;
    }

    fclose(file);
}

void loadQueueFromFile(Structure* queue, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        saveQueueToFile(queue, filename);
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; //Ну пон, типо до новой строки.
        qpush(queue, buffer);
    }

    fclose(file);
}
