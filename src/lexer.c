#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t *lexer_create(char *code)
{
    lexer_t *new_lexer = malloc(sizeof(lexer_t));

    new_lexer->column = 0;
    new_lexer->code = code;
    new_lexer->current_char = code[new_lexer->column];

    return new_lexer;
}

char lexer_get_current_char(lexer_t *lexer)
{
    return lexer->current_char;
}

char lexer_get_next_char(lexer_t *lexer)
{

    return lexer->code[lexer->column + 1];
}

char *tokens[] = {
    "for",
    "while",
    "break",
    "continue",
    "import",
    "if",
    "else",
    "elif",
    "true",
    "false",
    "return",
    "fun"

};

int classify_keyword(char *string)
{

    /* Messy code go brrr */
    if (strcmp(string, tokens[T_FOR]) == 0)
        return T_FOR;

    else if (strcmp(string, tokens[T_WHILE]) == 0)
        return T_WHILE;

    else if (strcmp(string, tokens[T_BREAK]) == 0)
        return T_BREAK;

    else if (strcmp(string, tokens[T_CONTINUE]) == 0)
        return T_CONTINUE;

    else if (strcmp(string, tokens[T_IMPORT]) == 0)
        return T_IMPORT;

    else if (strcmp(string, tokens[T_IF]) == 0)
        return T_IF;

    else if (strcmp(string, tokens[T_ELSE]) == 0)
        return T_ELSE;

    else if (strcmp(string, tokens[T_ELIF]) == 0)
        return T_ELIF;

    else if (strcmp(string, tokens[T_TRUE]) == 0)
        return T_TRUE;

    else if (strcmp(string, tokens[T_FALSE]) == 0)
        return T_FALSE;

    else if (strcmp(string, tokens[T_RETURN]) == 0)
        return T_RETURN;

    else if (strcmp(string, tokens[T_FUN]) == 0)
        return T_FUN;

    else
    {
        return T_INVALID;
    }
}

int lexer_classify_char(char character)
{
    int i;

    char *types[] = {
        "int",
        "string",
        "double",
        "long"

    };

    switch (character)
    {
    case ' ':

        printf("character: %c group: SPACE\n", character);
        return SPACE;

    case '+':
    case '*':
    case '-':
    case '/':
    case '=':

        printf("character: %c group: OPERATOR\n", character);
        return OPERATOR;

    case '{':
    case '}':

        printf("character: %c group: STATEMENT\n", character);
        return STATEMENT;

    case '[':
    case ']':
        printf("character: %c group: ARRAY\n", character);
        return ARRAY;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        printf("character: %c group: NUMBER\n", character);
        return NUMBER;
    case ';':
        printf("character: %c group: EOL\n", character);
        return EOL;

    default:
        return -1;
    }
}

int lexer_classify_string(lexer_t *lexer)
{

    char split_string[100];

    strcpy(split_string, lexer->code);

    char *ptr = strtok(split_string, " ");

    char **s_tokens = malloc(strlen(lexer->code) * sizeof(char *));
    int index = 0;

    while (ptr)
    {

        s_tokens[index] = ptr;
        index++;
        ptr = strtok(NULL, " ");
    }

    free(ptr);

    int j;
    for (j = 0; j < index; j++)
    {
        if (classify_keyword(s_tokens[j]) == T_INVALID)
        {
            int i;

            for (i = 0; i < strlen(s_tokens[j]); i++)
            {
                lexer->current_char = s_tokens[j][i];
                lexer_classify_char(s_tokens[j][i]);
                lexer->column++;
            }
        }

        else if (classify_keyword(s_tokens[j]) != -1)
        {
            printf("keyword: %s\n", s_tokens[j]);
        }
    }

    free(s_tokens);
    return 0;
}
