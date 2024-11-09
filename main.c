#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Brainfuck interpreter written in C.

    This is a simple project made purely for fun and because i hate my life.
    It uses offsets to reference the pointers and makes sure that
    the input consists exclusively of a single line of BF code.

    Please don'kill me if I made some terrible mistakes, I wrote this in like
   3-4 hours

   Also, I've been watching a DSA course so I implemented a stack for
   the brackets matching :)
*/

struct Node {
    size_t offset;
    struct Node *prev;
};

size_t GetOffset(struct Node *);
struct Node *push(struct Node *, size_t);
struct Node *pop(struct Node *);

int main(int argc, char **argv) {

    // checks if there's any input and if there is, if it's just the code or
    // something else
    if (argc == 1) {
        printf("No input, exiting...");
        return 0;
    } else if (argc > 2) {
        printf("Too many input, please insert just a single line of code.\n ");
        return -1;
    }

    // takes the input length and allocates the memory for the data pointer
    size_t instrlen = strlen(*(argv + 1));
    unsigned char *datap = calloc(1024, sizeof(unsigned char));
    if (!datap) {
        printf("Allocation Error");
        return -1;
    }

    // initialises the instruction pointer and the offsets
    char *instrp = *(argv + 1);
    size_t instr_offset = 0;
    size_t data_offset = 0;

    struct Node *stack = NULL;

    while (instr_offset < instrlen) {
        switch (*(instrp + instr_offset)) {
        case '>':
            if (data_offset < 1023)
                data_offset++;
            break;
        case '<':
            if (data_offset > 0)
                data_offset--;
            break;
        case '+':
            (*(datap + data_offset))++;
            break;
        case '-':
            if (*(datap + data_offset) > 0)
                (*(datap + data_offset))--;
            break;
        case ',':
            printf("\nInput: ");
            // Use scanf like a chad
            scanf("%c", (datap + data_offset));
            break;
        case '.':
            printf("%c", *(datap + data_offset));
            break;
        case ']':
            if (*(datap + data_offset) != 0) {
                instr_offset = GetOffset(stack);
            } else {
                stack = pop(stack);
            }
            break;
        case '[':
            if (*(datap + data_offset) == 0) {
                // Could have used the stack for this as well instead of this
                // shitty while loop, but I'm too lazy. I'll keep it like this
                // for now.
                while (instr_offset < instrlen &&
                       *(instrp + instr_offset) != ']') {
                    instr_offset++;
                }
            } else {
                stack = push(stack, instr_offset);
            }
            break;

        default:
            break;
        }
        instr_offset++;
    }
    free(datap);

    return 0;
}

size_t GetOffset(struct Node *node) { return node->offset; }

struct Node *push(struct Node *node, size_t offset) {
    struct Node *new = malloc(sizeof(struct Node *));
    new->prev = node;
    new->offset = offset;

    return new;
}

struct Node *pop(struct Node *node) {
    struct Node *temp;
    temp = node->prev;
    free(node);

    return temp;
}
