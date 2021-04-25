#include "run.h"

#include "bitcode.h"
#include "utils.h"


void run(int argc, char *argv[]) {
    // Check arguments
    if (argc < 1) {
        fprintf(stderr, "Usage: run <inputfile>\n");
        exit(1);
    }

    // Open input file
    FILE *input_file = open_file_or_std(argv[0], "rb", stdin);

    // The stack
    int8_t stack[255];
    int8_t stack_amount = 0;

    // The registers
    int8_t regs[] = {
        0, // ip
        0, 0, 0, 0 // a, b, c, d
    };

    // Read file to stack
    while (! feof(input_file)) {
        char c = (char) fgetc(input_file);
        stack[stack_amount++] = c;
    }

    // Remove EOF character
    stack_amount--;

    // Close input file
    fclose(input_file);


    #define get_dest(dest, loc) \
        switch (loc) { \
            case REGA: \
                dest = &regs[1]; \
                break; \
            case REGB: \
                dest = &regs[2]; \
                break; \
            case REGC: \
                dest = &regs[3]; \
                break; \
            case REGD: \
                dest = &regs[4]; \
                break; \
            default: \
                fprintf(stderr, "Unable to find destination"); \
                exit(1); \
        }

    int8_t *ip = &regs[0];
    while (*ip < stack_amount) {
        int8_t cmd = stack[*ip];

        debugf("%3i/%i %c\n", *ip, stack_amount, cmd);

        if (cmd == MOV) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2], stack[*ip+3], stack[*ip+4] };
            *ip += 4;

            int8_t *dest;
            assert(args[0] == REG && "First argument to MOV has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                int8_t *from;
                get_dest(from, args[3]);

                *dest = *from;
            }
            else if (args[2] == BYTE) {
                *dest = args[3];
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[2]);
                exit(1);
            }
        }

        else if (cmd == ADD) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2], stack[*ip+3], stack[*ip+4] };
            *ip += 4;

            int8_t *dest;
            assert(args[0] == REG && "First argument to ADD has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                int8_t *from;
                get_dest(from, args[3]);

                *dest += *from;
            }
            else if (args[2] == BYTE) {
                *dest += args[3];
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[2]);
                exit(1);
            }
        }

        else if (cmd == SUB) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2], stack[*ip+3], stack[*ip+4] };
            *ip += 4;

            int8_t *dest;
            assert(args[0] == REG && "First argument to SUB has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                int8_t *from;
                get_dest(from, args[3]);

                *dest -= *from;
            }
            else if (args[2] == BYTE) {
                *dest -= args[3];
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[2]);
                exit(1);
            }
        }

        else if (cmd == MUL) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2], stack[*ip+3], stack[*ip+4] };
            *ip += 4;

            int8_t *dest;
            assert(args[0] == REG && "First argument to MUL has to be register");
            get_dest(dest, args[1]);

            if (args[2] == REG) {
                int8_t *from;
                get_dest(from, args[3]);

                *dest *= *from;
            }
            else if (args[2] == BYTE) {
                *dest *= args[3];
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[2]);
                exit(1);
            }
        }

        else if (cmd == SQ) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2] };
            *ip += 2;

            int8_t *dest;
            assert(args[0] == REG && "First argument to SQ has to be register");
            get_dest(dest, args[1]);

            *dest *= *dest;
        }

        else if (cmd == INC) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2] };
            *ip += 2;

            int8_t *dest;
            assert(args[0] == REG && "First argument to INC has to be register");
            get_dest(dest, args[1]);

            *dest += 1;
        }

        else if (cmd == DEBUG) {
            int8_t args[] = { stack[*ip+1], stack[*ip+2] };
            *ip += 2;

            if (args[0] == REG) {
                int8_t *dest;
                get_dest(dest, args[1]);

                char regc = args[1] - REGA + 'A';

                printf("REG%c %i\n", regc, *dest);
            }
            else if (args[0] == BYTE) {
                printf("0x%02x\n", args[1]);
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[0]);
                exit(1);
            }
        }

        else {
            fprintf(stderr, "Failed to exec cmd %2x\n", cmd);
            exit(1);
        }

        (*ip)++;
    }
}
