#ifndef BENCHMARK_H
#define BENCHMARK_H


#include <time.h>

#include "vector.h"


#define long_time(time) ((long) 1e9 * (long) time.tv_sec + (long) time.tv_nsec)


#ifdef BENCH
	#define FUNC_START()  bench_start_func(__FUNCTION__);
	#define FUNC_END()  bench_end_func(__FUNCTION__);
#else
	#define FUNC_START()
	#define FUNC_END()
#endif


typedef struct benchmark_state {
	const char *func_name;

	long actual_time;
	long effective_time;

	unsigned int amount;

	struct timespec actual_start;
	struct timespec effective_start;
} benchmark_state_t;


void bench_prepare(void);
void bench_write_states(void);

benchmark_state_t *bench_get_or_create_state_by_name(const char func_name[]);
void bench_apply_starttime(benchmark_state_t *state, struct timespec start);
void bench_apply_endtime(benchmark_state_t *state, struct timespec end);

void bench_start_func(const char func_name[]);
void bench_end_func(const char func_name[]);

void benchmark_state_init(benchmark_state_t *state);


#endif
