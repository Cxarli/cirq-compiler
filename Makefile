# The path of the executable
OUTPUT_EXEC = build/main

# Use clang by default
CC = clang

# Flags for Clang
CFLAGS = -O0 -g -Weverything -DENABLE_DEBUG

# Disable annoying warnings
ANNOYING_WARNINGS = padded variadic-macros gnu-zero-variadic-macro-arguments gnu-folding-constant gnu-binary-literal
IGNORE_FLAGS = $(ANNOYING_WARNINGS:%=-Wno-%)

# Flags for GCC
GCC_FLAGS = -O0 -g -Wall -Wextra

# Default valgrind flags
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all -v

# Colors
RED := $(shell echo -ne "\e[0;31m")
LIGHT_RED := $(shell echo -ne "\e[0;91m")
GREEN := $(shell echo -ne "\e[0;32m")
LIGHT_GREEN := $(shell echo -ne "\e[0;92m")
YELLOW := $(shell echo -ne "\e[0;33m")
LIGHT_YELLOW := $(shell echo -ne "\e[0;93m")
BLUE := $(shell echo -ne "\e[0;34m")
LIGHT_BLUE := $(shell echo -ne "\e[0;94m")
MAGENTA := $(shell echo -ne "\e[0;35m")
LIGHT_MAGENTA := $(shell echo -ne "\e[0;95m")
CYAN := $(shell echo -ne "\e[0;36m")
LIGHT_CYAN := $(shell echo -ne "\e[0;96m")

WHITE := $(shell echo -ne "\e[0;97m")
LIGHT_GRAY := $(shell echo -ne "\e[0;37m")
DARK_GRAY := $(shell echo -ne "\e[0;90m")
BLACK := $(shell echo -ne "\e[0;30m")
RESET := $(shell echo -ne "\e[0m")

TAB := $(shell echo -ne "\t")

# All targets that are not files
.PHONY: help asan all shit release bench bench_rel bench_build valgrind valshit run clean remake cleanrun


# Show some help
help:
	@echo "make help      show this help"
	@echo "make all       build all files with ASAN on clang"
	@echo "make shit      build all files with ASAN on gcc"
	@echo "make release   build all files with -O3 on clang"
	@echo "make bench     build all files with ASAN on clang with benchmarking on"
	@echo "make bench_rel build all files with -O3 on clang with benchmarking on"
	@echo "make valgrind  build all files (clang), then run with valgrind"
	@echo "make valshit   build all files (gcc), then run with valgrind"
	@echo "make run       just run the executable"
	@echo "make clean     remove all build files"
	@echo "make remake    same as make clean; make all"
	@echo "make cleanrun  same as make remake, but runs formed executable directly after"


# ASAN
asan: CFLAGS += -fsanitize=address
asan: $(OUTPUT_EXEC)


# Clang + ASAN
all: asan


# GCC + ASAN
shit: CC = gcc
shit: CFLAGS = $(GCC_FLAGS)
shit: asan


release: CFLAGS = -O3 -DIGNORE_ASSERT
release: clean build/release

build/release: $(OUTPUT_EXEC)
	@mv $(OUTPUT_EXEC) build/release


# Debug + Clang + ASAN
bench_build: CFLAGS += -DBENCH
bench_build: remake


ARGS ?= test
bench:
	@rm -f /tmp/bench
	@touch /tmp/bench
	@make --no-print-directory bench_build
	@build/main $(ARGS)
	@rm -f /tmp/bench.tmp
	@sort -nr /tmp/bench
	@rm -f /tmp/bench


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valgrind: clean $(OUTPUT_EXEC)
	valgrind $(VALGRIND_FLAGS) $(OUTPUT_EXEC)


# Enable and run valgrind
# Don't call `all` since that would enable ASAN
valshit: CC = gcc
valshit: CFLAGS = $(GCC_FLAGS)
valshit: valgrind


# Just run the executable
run:
	$(OUTPUT_EXEC)


# Clean all build files
clean:
	@echo "${RED}rm -f $(OUTPUT_EXEC) build/*${RESET}"
	@rm -rf $(OUTPUT_EXEC) build/*


# Remove all files, then build again
remake: clean all


# Remove all files, build, then run
cleanrun: remake
	@echo -e "\n\n"
	$(OUTPUT_EXEC)


# Find all C files
C_FILES = $(shell find src -iname '*.c')
C_FILES_O = $(C_FILES:src/%.c=build/%.o)

# Build C files
$(C_FILES_O): build/%.o: src/%.c
	@echo "$(CC)${TAB}${YELLOW}$(CFLAGS) ${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo "${TAB}${LIGHT_CYAN}-c" $< "${TAB}${LIGHT_GREEN}-o" $@
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) -c $< -o $@


# Find all header files
HEADER_FILES = $(shell find src -iname '*.h')
HEADER_FILES_O = $(HEADER_FILES:src/%.h=build/%.o)

# Add header files to c object
$(HEADER_FILES_O): build/%.o: src/%.h


# Build main executable
$(OUTPUT_EXEC): $(C_FILES_O) $(HEADER_FILES)
	@echo "$(CC)${TAB}${YELLOW}$(CFLAGS) ${DARK_GRAY}$(IGNORE_FLAGS)"
	@echo "${TAB}${LIGHT_GREEN}$(C_FILES_O)"
	@echo "${TAB}${LIGHT_MAGENTA}-o $(OUTPUT_EXEC)"
	@echo "${RESET}"
	@$(CC)  $(CFLAGS) $(IGNORE_FLAGS) $(C_FILES_O) -o $(OUTPUT_EXEC)
