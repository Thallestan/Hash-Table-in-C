#include "HashTable.h"


// Funcao para validar se o cpf ja foi cadastrado
int cpf_ja_cadastrado(const char *cpf) {
    DadosPessoa *result = search(cpf);
    return result != NULL;
}

// Função para validar se uma string representa uma data válida no formato DD/MM/AAAA
int validar_data(const char *data) {
    if (data == NULL || strlen(data) != 10) {
        return 0; // Data é nula ou não tem o tamanho correto
    }
    
    // Verifica o formato DD/MM/AAAA
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') {
                return 0; // Caracteres nas posições 2 e 5 devem ser '/'
            }
        } else {
            if (!isdigit(data[i])) {
                return 0; // Os outros caracteres devem ser dígitos
            }
        }
    }

    // Converte partes da data para inteiros
    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');
    int ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 + (data[8] - '0') * 10 + (data[9] - '0');

    // Verifica os valores de dia, mês e ano
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) {
        return 0; // Valores inválidos para dia ou mês
    }

    // Verifica os dias válidos para cada mês
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return 0; // Abril, Junho, Setembro e Novembro têm no máximo 30 dias
    }

    if (mes == 2) {
        // Verifica se é um ano bissexto
        int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
        if ((bissexto && dia > 29) || (!bissexto && dia > 28)) {
            return 0; // Fevereiro tem 29 dias em anos bissextos e 28 dias nos demais anos
        }
    }

    return 1;
}

// Função para validar se o nome é composto apenas por letras
int validar_nome(const char *nome) {
    if (nome == NULL || strlen(nome) == 0) {
        return 0; // Nome é nulo ou vazio
    }
    
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') {
            return 0; // Contém caracteres que não são letras ou espaços
        }
    }

    return 1; // Nome válido
}

// Função para calcular o primeiro dígito verificador do CPF
int calcular_dv1(const char *cpf) {
    int pesos[] = {10, 9, 8, 7, 6, 5, 4, 3, 2};
    int soma = 0;

    for (int i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * pesos[i];
    }

    int dv1 = 11 - (soma % 11);
    if (dv1 >= 10) {
        dv1 = 0;
    }

    return dv1;
}

// Função para calcular o segundo dígito verificador do CPF
int calcular_dv2(const char *cpf, int dv1) {
    int pesos[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
    int soma = 0;

    for (int i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * pesos[i];
    }
    soma += dv1 * pesos[9];

    int dv2 = 11 - (soma % 11);
    if (dv2 >= 10) {
        dv2 = 0;
    }

    return dv2;
}

// Função para validar o CPF
int validar_cpf(const char *cpf) {
    if (strlen(cpf) != 11) {
        return 0; // CPF inválido
    }

    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            return 0; // CPF contém caracteres não numéricos
        }
    }

    int dv1 = calcular_dv1(cpf);
    int dv2 = calcular_dv2(cpf, dv1);

    if (cpf[9] - '0' == dv1 && cpf[10] - '0' == dv2) {
        return 1; // CPF válido
    }

    return 0; // CPF inválido
}

// Função de hash (djb2)
unsigned int hash_function(const char *nome) {
    unsigned int hash = 5381;
    int c;
    while ((c = *nome++)) {
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

// Insere o cadastro na tabela
void insert(const char *cpf, const char *nome, const char *data_nascimento) {
    unsigned int index = hash_function(cpf);

    Node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) { 
            strcpy(current->value.nome, nome);
            strcpy(current->value.data_nascimento, data_nascimento);
            strcpy(current->value.cpf, cpf);
                return;
            } else {
                return;
            }
        current = current->next;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->cpf, cpf);
    strcpy(new_node->value.nome, nome);
    strcpy(new_node->value.data_nascimento, data_nascimento);
    strcpy(new_node->value.cpf, cpf);
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
            insert(current->cpf, current->value.nome, current->value.data_nascimento);
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(old_table);
}




// Busca um valor pelo CPF
DadosPessoa *search(const char *cpf) {
    unsigned int index = hash_function(cpf);

    Node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) {
            return &current->value;
        }
        current = current->next;
    }

    return NULL;
}



// Remove um cadastro da tabela
void delete(const char *cpf) {
    unsigned int index = hash_function(cpf);

    Node *current = hash_table[index];
    Node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) {
            if (prev == NULL) {
                hash_table[index] = current->next;
            } else {
                prev->next = current->next;
            }
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
        printf("Indice %d: ", i);
        Node *current = hash_table[i];
        while (current != NULL) {
            printf("( CPF = %s, Nome = %s, Data Nascimento = %s ) -> ",
                   current->cpf, current->value.nome, current->value.data_nascimento);
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
            fprintf(file, "%s %s %s\n", current->cpf, current->value.nome, current->value.data_nascimento);
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

    char cpf[12], nome[MAX_INPUT_SIZE], data_nascimento[11];
    while (fscanf(file, "%s %s %s\n", cpf, nome, data_nascimento) != EOF) {
        insert(cpf, nome, data_nascimento);
    }

    fclose(file);
    printf("Dados carregados do arquivo %s.\n", filename);
}




