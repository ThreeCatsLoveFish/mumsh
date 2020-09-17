# Course: 2020FA VE482
# Author: Zhimin Sun

.PHONY: all normal memory release clean format test

CC=clang
CFLAGS=-std=gnu11\
       -O2\
	   -Wall\
	   -Wextra\
	   -Werror\
	   -pedantic\
	   -Wno-unused-result\
	   -I include/
MUMSHMC_FLAGS=-fsanitize=address\
              -fno-omit-frame-pointer\
			  -fsanitize=undefined\
			  -fsanitize=integer\
			  -I include/
OBJ=$(patsubst src/%.c, build/%.o, $(wildcard src/*.c))
OBJMC=$(patsubst src/%.c, build/mc_%.o, $(wildcard src/*.c))
MUMSH=mumsh
MUMSHMC=mumsh_memory_check
NUM=$(words $(OBJ))

all: normal memory
	@echo "Finish!"

normal: $(OBJ)
	@echo "Dependency ($(NUM)/$(NUM)) satisfied!"
	@echo "Building mumsh (normal version)..."
	@$(CC) $(CFLAGS) -o $(MUMSH) $^

build/%.o: src/%.c
	@echo "Building $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

memory: $(OBJMC)
	@echo "Dependency ($(NUM)/$(NUM)) satisfied!"
	@echo "Building mumsh (memory check version)..."
	@$(CC) $(MUMSHMC_FLAGS) -o $(MUMSHMC) $^

build/mc_%.o: src/%.c
	@echo "Building $@"
	@$(CC) $(MUMSHMC_FLAGS) -o $@ -c $<

format:
	clang-format $(wildcard include/*.h src/*.c)

release:
	@rm -f release/*.h release/*.c
	@cp include/*.h release/
	@cp src/*.c release/
	@tar -cvf release.tar -C release/ . > /dev/null
	@echo "Package release!"

test:
	@./mumsh

clean:
	rm -f build/* mumsh*
	rm -f release/*.h release/*.c release.tar
