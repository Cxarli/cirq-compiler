#include "utils.h"
#include "compile.h"
#include "run.h"


void test(void);
void test(void) {
    assert(tonumber("b10") == 0b10);
    assert(tonumber("o10") == 010);
    assert(tonumber("d-10") == -10);
    assert(tonumber("h10") == 0x10);

    assert(tonumber("h09") == 0x09);
    assert(tonumber("h7f") == 0x7f);
    assert(tonumber("b101010") == 0b101010);
    assert(tonumber("o76") == 076);

    printf("tests ok\n");
}




int main(int argc, char *argv[]) {
    // Remove program name
    argv++;
    argc--;

    if (argc > 0) {
        // Get action
        char *action = argv[0];
        // Remove action from arguments
        argv++;
        argc--;

        if (strcmp(action, "run") == 0) {
            run(argc, argv);
            return 0;
        }
        else if (strcmp(action, "compile") == 0) {
            compile(argc, argv);
            return 0;
        }
        else if (strcmp(action, "test") == 0) {
            test();
            return 0;
        }
    }

    fprintf(stderr, "No (valid) action specified\nValid actions are:\n\trun\n\tcompile\n\ttest\n");
    exit(1);
}
