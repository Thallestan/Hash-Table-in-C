#include "HashTable.h"

// Função de hash (djb2)
unsigned int hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}

// Inicializa a tabela 
void initialize_table() {
    hash_table = (Node **)malloc(TABLE_SIZE * sizeof(Node *));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

// Insere um par Nome-CPF na tabela
void insert(const char *key, const char *value) {
    unsigned int index = hash_function(key);

    Node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);  // Atualiza o valor existente
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }

    // Se o nome não existe, cria um novo nó
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = hash_table[index];
    hash_table[index] = new_node;

    element_count++;
    if ((float)element_count / TABLE_SIZE > LOAD_FACTOR_THRESHOLD) {
        resize_table();
    }
}

// Redimensiona a tabela hash
void resize_table() {
    int old_size = TABLE_SIZE;
    TABLE_SIZE *= 2;
    Node **old_table = hash_table;

    hash_table = (Node **)malloc(TABLE_SIZE * sizeof(Node *));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    // Reinsere os elementos da tabela antiga
    for (int i = 0; i < old_size; i++) {
        Node *current = old_table[i];
        while (current != NULL) {
            insert(current->key, current->value);
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(old_table);
}



// Busca um valor pela chave
char *search(const char *key) {
    unsigned int index = hash_function(key);

    Node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

// Remove um par Nome-CPF da tabela
void delete(const char *key) {
    unsigned int index = hash_function(key);

    Node *current = hash_table[index];
    Node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                hash_table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            element_count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Exibe o conteúdo da tabela hash
void display_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node *current = hash_table[i];
        while (current != NULL) {
            printf("(%s: %s) -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Salva tabela em um arquivo
void save_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hash_table[i];
        while (current != NULL) {
            fprintf(file, "%s %s\n", current->key, current->value);
            current = current->next;
        }
    }

    fclose(file);
    printf("Dados salvos no arquivo %s.\n", filename);
}

// Carrega os dados de um arquivo na tabela
void load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo para carregar.\n");
        return;
    }

    char key[MAX_INPUT_SIZE], value[MAX_INPUT_SIZE];
    while (fscanf(file, "%s %s\n", key, value) != EOF) {
        insert(key, value);
    }

    fclose(file);
    printf("Dados carregados do arquivo %s.\n", filename);
}


