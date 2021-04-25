#include "utils.h"
#include "compile.h"
#include "run.h"
#include "test.h"
#include "benchmark.h"


int main(int argc, char *argv[]) {
    #ifdef BENCH
		bench_prepare();
		atexit(bench_write_states);
	#endif

	FUNC_START();


    // Remove program name
    argv++;
    argc--;

    // Check if we have enough arguments
    if (argc == 0) {
        goto no_valid_action;
    }

    // Get action
    char *action = argv[0];
    // Remove action from arguments
    argv++;
    argc--;


    if (strcmp(action, "run") == 0) {
        run(argc, argv);
    }
    else if (strcmp(action, "compile") == 0) {
        compile(argc, argv);
    }
    else if (strcmp(action, "test") == 0) {
        test();
    }
    else {
        goto no_valid_action;
    }

    FUNC_END();
    return 0;


    no_valid_action:
    fprintf(stderr, "No (valid) action specified\nValid actions are:\n\trun\n\tcompile\n\ttest\n");
    FUNC_END();
    exit(1);
}
