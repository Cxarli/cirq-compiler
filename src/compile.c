#include "compile.h"

#include "bitcode.h"
#include "utils.h"
#include "benchmark.h"


void compile(int argc, char *argv[]) {
    FUNC_START();

    // Check amount of arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: compile <inputfile> <outputfile>\n");
        FUNC_END();
        exit(1);
    }


    // Open file streams
    FILE *input_file = open_file_or_std(argv[0], "r", stdin);
    FILE *output_file = open_file_or_std(argv[1], "wb", stdout);


    // Read file
    char *line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, input_file)) >= 0) {
        // Remove newline
        // TODO: Remove all leading and trailing whitespace
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Ignore empty lines and comments
        // NOTE: Comments only work at the beginning of the line
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        debugf("\n%s\n", line);

        // Split line in parts
        char *str, *tofree;
        str = tofree = strdup(line);

        // NOTE: No more than 10 tokens per line
        char *tokens[10];
        size_t amount_tokens = 0;

        char *token;
        while ((token = strsep(&str, " ")) != NULL) {
            tokens[amount_tokens++] = token;
        }

        // Get command
        char *cmd = tokens[0];
        debugf("cmd: %s\n", cmd);

        // Convert command to bitcode
        uint8_t bitcode = 0;
        #define cmp(stra, strb, bc)  else if (strcmp(stra, strb) == 0) bitcode = bc

        if (0) {}
        cmp(cmd, "mov", MOV);
        cmp(cmd, "add", ADD);
        cmp(cmd, "sub", SUB);
        cmp(cmd, "mul", MUL);
        cmp(cmd, "sq", SQ);
        cmp(cmd, "inc", INC);
        cmp(cmd, "debug", DEBUG);
        cmp(cmd, "exit", EXIT);
        cmp(cmd, "nop", NOP);

        else {
            fprintf(stderr, "Failed to convert token `%s`\n", cmd);
            FUNC_END();
            exit(1);
        }

        // Write bitcode
        debugf("%02x ", bitcode);
        fwrite(&bitcode, 1, sizeof(bitcode), output_file);


        // Convert tokens to bitcodes
        for (size_t i=1; i < amount_tokens; i++) {
            token = tokens[i];
            bitcode = 0;

            // Default type is register
            uint8_t type = REG;

            if (0) {}
            cmp(token, "regip", REGIP);
            cmp(token, "rega", REGA);
            cmp(token, "regb", REGB);
            cmp(token, "regc", REGC);
            cmp(token, "regd", REGD);

            else if (isnumber(token)) {
                type = BYTE;

                // NOTE: Does weird things with negative numbers
                bitcode = (uint8_t) tonumber(token);
            }
            else if (strlen(token) == 0) {
                // Ignore empty tokens
                continue;
            }
            else {
                fprintf(stderr, "Failed to convert token `%s`\n", token);
                FUNC_END();
                exit(1);
            }


            // Write type and bitcode
            debugf("%04x ", type << 8 | bitcode);
            fwrite(&type, 1, sizeof(type), output_file);
            fwrite(&bitcode, 1, sizeof(bitcode), output_file);
        }

        debugf("\b\n");
        free(tofree);
    }


    free(line);

    fclose(input_file);
    fclose(output_file);

    FUNC_END();
}
