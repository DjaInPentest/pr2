#ifndef STACK_H
#define STACK_H

typedef struct StackElement {
    char* value;
    struct StackElement* next;
} StackElement;

typedef struct {
    StackElement* head;
} Stack;

Stack* createStack();
void addStack(Stack* stack, const char* value);
void delStack(Stack* stack);
void printStack(Stack* stack);
void saveStackToFile(Stack* stack, const char* filename);
void loadStackFromFile(Stack* stack, const char* filename);

#endif // STACK_H
