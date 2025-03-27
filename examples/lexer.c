#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/jparena.h"

#define FILENAME "test.txt"
#define BUFFER_SIZE 100

typedef enum {
    INT, PLUS, MINUS, STAR, SLASH, LPAREN, RPAREN
} TokenType;

typedef struct token_t{
    TokenType type;
    char* value;
}Token;

JPArena arena = ARENA(80);

Token* make_token(TokenType type, char* value){
    Token* t = ALLOC(&arena, Token);
    t->type = type;
    t->value = value;
    return t;
}

void print_token(Token token){
    printf("Token(type=%d, value=\"%s\")\n", token.type, token.value);
}

char* lex_num(char* line, size_t* i){
    size_t buf_i = 0;
    char buff[BUFFER_SIZE];

    while(line[*i] >= '0' && line[*i] <= '9' && line[*i] != '\0'){
        buff[buf_i] = line[*i];
        buf_i++;
        (*i)++;
    }
    buff[buf_i] = '\0';
    char* alloced = arena_alloc(&arena, sizeof(char) * buf_i + 1);
    strcpy(alloced, buff);
    return alloced;
}

void lex_line(char* line){
    size_t i = 0;

    while(line[i] != '\0'){
        if(line[i] >= '0' && line[i] <= '9'){
            char* value = lex_num(line, &i);
            i--; // just to fix the misalignment created by the lex_num function
            Token* tk = make_token(INT, value);
            print_token(*tk);
        } else if(line[i] == '+') {
            Token* tk = make_token(PLUS, "+");
            print_token(*tk);
        } else if(line[i] == '-') {
            Token* tk = make_token(MINUS, "-");
            print_token(*tk);
        } else if(line[i] == '*') {
            Token* tk = make_token(STAR, "*");
            print_token(*tk);
        } else if(line[i] == '/') {
            Token* tk = make_token(SLASH, "/");
            print_token(*tk);
        } else if(line[i] == '(') {
            Token* tk = make_token(LPAREN, "(");
            print_token(*tk);
        } else if(line[i] == ')') {
            Token* tk = make_token(RPAREN, ")");
            print_token(*tk);
        }
        i++;
    }
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    char buff[BUFFER_SIZE];

    while(fgets(buff, BUFFER_SIZE, file) != NULL){
        lex_line(buff);
    }
    return 0;
}