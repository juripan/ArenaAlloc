CC = gcc
C_FLAGS = -std=c11 -Wall -Wextra -pedantic

main:
	$(CC) $(C_FLAGS) src/main.c -o main

debug:
	$(CC) $(C_FLAGS) -g -fsanitize=address src/main.c -o main

clean:
	rm ./main