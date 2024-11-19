#ifndef HASHTABLE_H
#define HASHTABLE_H

#define INITIAL_TABLE_SIZE 10
#define MAX_INPUT_SIZE 100  // Tamanho máximo de entrada
#define LOAD_FACTOR_THRESHOLD 0.7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura da tabela
typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

//Protótipos das funcoes
void insert(const char *key, const char *value);
char* search(const char *key);
void delete(const char *key);
void initialize_table();
void resize_table();

// Declaracao de variaveis
Node **hash_table;  // Tabela hash
int TABLE_SIZE = INITIAL_TABLE_SIZE;  // Tamanho inicial da tabela
int element_count = 0;  // Número de elementos na tabela

#endif
