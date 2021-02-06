#ifndef LEXER_H
#define LEXER_H

typedef struct

{
    int column;
    char current_char;
    char *code;

} lexer_t;

enum groups
{
    OPERATOR,
    NUMBER,
    TYPE,
    NAME,
    STATEMENT,
    SPACE,
    EOL,
    ARRAY
};

enum tokens
{
    T_FOR,
    T_WHILE,
    T_BREAK,
    T_CONTINUE,
    T_IMPORT,
    T_IF,
    T_ELSE,
    T_ELIF,
    T_TRUE,
    T_FALSE,
    T_RETURN,
    T_FUN,
    T_INVALID

};

lexer_t *lexer_create(char *code);

char lexer_get_current_char(lexer_t *lexer);
char lexer_get_next_char(lexer_t *lexer);

int lexer_get_current_column(lexer_t *lexer);
int lexer_get_current_line(lexer_t *lexer);

int lexer_classify_string(lexer_t* lexer);
#endif
