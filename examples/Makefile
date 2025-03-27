CC = gcc
C_FLAGS = -std=c11 -Wall -Wextra -pedantic

main:
	$(CC) $(C_FLAGS) lexer.c -o lexer

debug:
	$(CC) $(C_FLAGS) -g -fsanitize=address lexer.c -o lexer

clean:
	rm ./lexer