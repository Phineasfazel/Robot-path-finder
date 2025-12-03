#ifndef STACK_H
#define STACK_H

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position *arr;
    int number_of_elements;
    int capacity;
} Stack;

Stack* create_stack();
void resize_stack(Stack *stack);
void push_onto_stack(Stack *stack, Position position);
Position pop_from_stack(Stack *stack);
Position peek(Stack *stack);
void stack_free(Stack *stack);

#endif