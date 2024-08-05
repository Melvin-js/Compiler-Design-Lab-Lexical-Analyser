#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contain(char x[15], char y[20][20], int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(y[i], x) == 0) {
            return -1;
        }
    }
    return 1;
}

int is_keyword(char buffer[]) {
    switch (buffer[0]) {
        case 'i':
            if (strcmp(buffer, "int") == 0 || strcmp(buffer, "if") == 0)
                return 1;
            break;
        case 'e':
            if (strcmp(buffer, "else") == 0)
                return 1;
            break;
        case 'v':
            if (strcmp(buffer, "void") == 0)
                return 1;
            break;
        case 'r':
            if (strcmp(buffer, "return") == 0)
                return 1;
            break;
        case 'm':
            if (strcmp(buffer, "main") == 0)
                return 1;
            break;
        
    }
    return 0;
}

void handle_keyword(char buffer[], char keyword[20][20], int *key_count) {
    if (is_keyword(buffer)) {
        if (contain(buffer, keyword, *key_count) == 1) {
            strcpy(keyword[*key_count], buffer);
            (*key_count)++;
        }
    }
}

void main() {
    char buffer[15], identifier[20][20], keyword[20][20], operator[20][20], constant[20][20];
    char c;
    FILE *fp1, *fp2;
    int i = 0, id_count = 0, key_count = 0, op_count = 0, con_count = 0;

    fp1 = fopen("input.txt", "r");
    if (fp1 == NULL) {
        printf("Unable to open file\n");
        return;
    }

    while ((c = fgetc(fp1)) != EOF) {

    start:
        if (c == '\n' || c == '\t' || c == '{' || c == ' ' || c == '(' || c == '}' || c == ')' || c == ';') {
            continue;
        }
        if (c >= '0' && c <= '9') {
            i = 0;
            buffer[i++] = c;
            while ((c = fgetc(fp1)) >= '0' && c <= '9' && c != ';') {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            i = 0;
            if (contain(buffer, constant, con_count) == 1) {
                strcpy(constant[con_count], buffer);
                con_count++;
            }
            goto start;

        } else if (c == 'i' || c == 'e' || c == 'v' || c == 'r') {
            i = 0;
            buffer[i++] = c;
            c = fgetc(fp1);
            while (c != ' ' && c != '(' && c != '{' && c != '}' && c != '=' && c != ';' && c != '\n' && c != '\t' && c != EOF) {
                buffer[i++] = c;
                c = fgetc(fp1);
            }
            buffer[i] = '\0';
            i = 0;
            handle_keyword(buffer, keyword, &key_count);
            if (c == '=') {
                goto opertr;
            }
        } else if (c == '=') {
        opertr:
            buffer[i++] = c;
            c = fgetc(fp1);
            if (c == '=' || c == '+' || c == '-') {
                buffer[i++] = c;
                buffer[i] = '\0';
                i = 0;
                if (contain(buffer, operator, op_count) == 1) {
                    strcpy(operator[op_count], buffer);
                    op_count++;
                }

            } else {
                buffer[i] = '\0';
                i = 0;
                if (contain(buffer, operator, op_count) == 1) {
                    strcpy(operator[op_count], buffer);
                    op_count++;
                }
                goto start;
            }
        } else {
        identify:
            buffer[i++] = c;
            while ((c = fgetc(fp1)) != ' ' && c != '\n' && c != '(' && c != ')' && c != ';' && c != ',' && c != '\t' && c != '{' && c != '}' && c != EOF && c != '=') {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            i = 0;
            if (contain(buffer, identifier, id_count) == 1) {
                strcpy(identifier[id_count], buffer);
                id_count++;
            }
            if (c == '=') {
                goto opertr;
            }
        }
    }

    fclose(fp1);
    printf("\nKeywords\n----------------------------\n");
    for (int j = 0; j < key_count; j++) {
        printf("%s\n", keyword[j]);
    }

    printf("\nIdentifiers\n----------------------------\n");
    fp2 = fopen("SymbolTable.txt", "w");
    for (int j = 0; j < id_count; j++) {
        printf("%s\n", identifier[j]);
        fprintf(fp2, "%s\n", identifier[j]);
    }
    fclose(fp2);

    printf("\nConstants\n----------------------------\n");
    for (int j = 0; j < con_count; j++) {
        printf("%s\n", constant[j]);
    }

    printf("\nOperators\n----------------------------\n");
    for (int j = 0; j < op_count; j++) {
        printf("%s\n", operator[j]);
    }
}

/*
OUTPUT

Keywords
----------------------------
main
void
int
if
else

Identifiers
----------------------------
x
a

Constants
----------------------------
0
1

Operators
----------------------------
=
==

*/