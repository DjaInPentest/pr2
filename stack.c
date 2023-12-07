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

Structure* createStack() {
    Structure* stack = malloc(sizeof(Structure));
    stack->head = NULL;
    return stack;
}

void addStack(Structure* stack, const char* value) {
    StructureElement* newElement = (StructureElement*)malloc(sizeof(StructureElement));
    newElement->value = strdup(value);
    newElement->next = stack->head;
    stack->head = newElement;
}

void delStack(Structure* stack) {
    if (stack == NULL || stack->head == NULL) {
        printf("The stack is empty or invalid");
        exit(1);
    }

    StructureElement* removed = stack->head;
    stack->head = stack->head->next;
    printf("POPed-> %s\n", removed->value);
}

void printStack(Structure* stack) {
    for (StructureElement* print = stack->head; print != NULL; print = print->next) { //Короче, переходим по всему стеку с головы и выписываем его.
        printf("%s\n", print->value);
    }
    printf("\n");
}

void saveStackToFile(Structure* stack, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing");
        exit(1);
    }
    //То же самое, что принт, только сейв.
    StructureElement* current = stack->head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->value);
        current = current->next;
    }

    fclose(file);
}

void loadStackFromFile(Structure* stack, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        saveStackToFile(stack, filename);
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; //Убираем символ новой строки.
        addStack(stack, buffer);
    }

    fclose(file);
}