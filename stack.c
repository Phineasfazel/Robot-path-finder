#include <stdlib.h>
#include "stack.h"

Stack* create_stack(){
    Stack* stack = malloc(sizeof(Stack));
    stack->arr = malloc(sizeof(Position) * 1);
    stack->number_of_elements = 0;
    stack->capacity = 1;
    return stack;
}

// assigning more memory to the stack
void resize_stack(Stack* stack){
    // creating a new array with double the memory of the stack array
    Position *new_arr = malloc(sizeof(Position) * stack->capacity * 2);
    // copying the elements of the stack array into the new array with double the size
    for (int i = 0; i < stack->capacity; i++){
        new_arr[i] = stack->arr[i];
    }

    stack->capacity*=2;
    free(stack->arr);
    stack->arr = new_arr;
}

void push_onto_stack(Stack *stack, Position position){
    if (stack->capacity == stack->number_of_elements){
        resize_stack(stack);
    }
    stack->arr[stack->number_of_elements] = position;
    stack->number_of_elements++;
}

Position pop_from_stack(Stack *stack){
    if (stack->number_of_elements > 0){
        stack->number_of_elements--;
        return stack->arr[stack->number_of_elements];
    }
    Position empty = {-1, -1};
    return empty;
}

Position peek(Stack *stack){
    return stack->arr[stack->number_of_elements - 1];
}

void stack_free(Stack *stack){
    free(stack->arr);
    free(stack);
}