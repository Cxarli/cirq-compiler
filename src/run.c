#include "run.h"

#include "bitcode.h"
#include "utils.h"
#include "benchmark.h"


int8_t *get_reg(int8_t loc, int8_t regs[5]);
int8_t get_value(int8_t type, int8_t value, int8_t regs[5]);


/// Turn bitcode into register address
int8_t *get_reg(int8_t loc, int8_t regs[5]) {
    FUNC_START();

    int8_t *reg = NULL;
    switch (loc) {
        case REGIP: reg = &regs[0]; break;

        case REGA: reg = &regs[1]; break;
        case REGB: reg = &regs[2]; break;
        case REGC: reg = &regs[3]; break;
        case REGD: reg = &regs[4]; break;

        default:
            fprintf(stderr, "Unable to find destination\n");
            FUNC_END();
            exit(1);
    }

    FUNC_END();
    return reg;
}


/// Get value based on type (REG or BYTE) and value (register bitcode or number)
int8_t get_value(int8_t type, int8_t value, int8_t regs[5]) {
    FUNC_START();

    if (type == REG) {
        value = *get_reg(value, regs);
    }
    else if (type == BYTE) {
        // value = value;
    }
    else {
        fprintf(stderr, "Unknown type %2x (%c)\n", type, type);
        FUNC_END();
        exit(1);
    }

    FUNC_END();
    return value;
}


void run(int argc, char *argv[]) {
    FUNC_START();

    // Check arguments
    if (argc < 1) {
        fprintf(stderr, "Usage: run <inputfile>\n");
        FUNC_END();
        exit(1);
    }

    // Open input file
    FILE *input_file = open_file_or_std(argv[0], "rb", stdin);

    // The stack
    const uint8_t STACK_SIZE = 255;
    int8_t stack[STACK_SIZE];
    uint8_t stack_amount = 0;

    // The registers
    int8_t regs[5] = {
        0, // ip
        0, 0, 0, 0, // a, b, c, d
    };

    // Read file to stack
    while (! feof(input_file)) {
        char c = (char) fgetc(input_file);
        stack[stack_amount++] = c;

        if (stack_amount == STACK_SIZE) {
            fprintf(stderr, "PROGRAM TOO BIG!!\n");
            break;
        }
    }

    // Remove EOF from stack
    stack[--stack_amount] = 0;

    // Close input file
    fclose(input_file);


    // Shortcut for instruction pointer
    uint8_t *ip = (uint8_t*) &regs[0];


    // Get array of `amount` arguments from the stack
    #define get_args(amount) \
        int8_t args[amount]; \
        for(size_t _i_##__LINE__ = 0; (_i_##__LINE__) < amount; (_i_##__LINE__)++) \
            args[(_i_##__LINE__)] = stack[++(*ip)];


    // Don't keep going after we reached the end of the stack
    while (*ip < STACK_SIZE) {
        // Get command
        int8_t cmd = stack[*ip];
        debugf("%3i/%i %c\n", *ip, stack_amount-1, cmd);

        // Check command and perform calculation
        if (cmd == MOV) {
            get_args(4);

            assert(args[0] == REG && "First argument to MOV has to be register");

            int8_t *dest = get_reg(args[1], regs);
            *dest = get_value(args[2], args[3], regs);
        }

        else if (cmd == ADD) {
            get_args(4);

            assert(args[0] == REG && "First argument to ADD has to be register");

            int8_t *dest = get_reg(args[1], regs);
            *dest += get_value(args[2], args[3], regs);
        }

        else if (cmd == SUB) {
            get_args(4);

            assert(args[0] == REG && "First argument to SUB has to be register");

            int8_t *dest = get_reg(args[1], regs);
            *dest -= get_value(args[2], args[3], regs);
        }

        else if (cmd == MUL) {
            get_args(4);

            assert(args[0] == REG && "First argument to MUL has to be register");

            int8_t *dest = get_reg(args[1], regs);
            *dest *= get_value(args[2], args[3], regs);
        }

        else if (cmd == SQ) {
            get_args(2);

            assert(args[0] == REG && "First argument to SQ has to be register");
            int8_t *dest = get_reg(args[1], regs);

            *dest *= *dest;
        }

        else if (cmd == INC) {
            get_args(2);

            assert(args[0] == REG && "First argument to INC has to be register");
            int8_t *dest = get_reg(args[1], regs);

            *dest += 1;
        }

        else if (cmd == DEBUG) {
            get_args(2);

            if (args[0] == REG) {
                int8_t *dest = get_reg(args[1], regs);

                // NOTE: Assuming the bitcodes of the registers follow each other
                // char regchar = (args[1] - REGA) + 'A';

                // NOTE: Assuming the registers follow each others in the `regs` array
                char regchar = (char)(dest - &regs[1]) + 'A';

                printf("REG%c %i\n", regchar, *dest);
            }
            else if (args[0] == BYTE) {
                printf("0x%02x\n", args[1]);
            }
            else {
                fprintf(stderr, "Unknown type %2x\n", args[0]);
                exit(1);
            }
        }

        else if (cmd == EXIT) {
            debugf("exit!\n");
            break;
        }

        else if (cmd == NOP) {
            // Ignore NOP
            *ip += 1;
            continue;
        }

        else {
            fprintf(stderr, "Failed to exec cmd 0x%02x (%c) at location 0x%02x\n", cmd, cmd<0x20 ? 0 : cmd, *ip);

            int left = max(0, *ip - 8), right = min(STACK_SIZE, *ip + 8);

            for (int i = left; i < right; i++) {
                fprintf(stderr, "%02x ", i);
            }
            fprintf(stderr, "\b\n");

            for (int i = left; i < right; i++) {
                fprintf(stderr, "%02x ", ((uint8_t)stack[i]));
            }
            fprintf(stderr, "\b\n");

            for (int i = left; i < *ip; i++) {
                fprintf(stderr, "   ");
            }
            fprintf(stderr, "^^\n");

            FUNC_END();
            exit(1);
        }

        (*ip)++;
    }


    FUNC_END();
}
