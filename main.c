#include "HashTable.c"
#include "HashTable.h"

int main() {
    initialize_table();

    char nome[MAX_INPUT_SIZE];
    char cpf[12];
    char data_nascimento[11];
    char confirmacao;
    int choice;

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Inserir cadastro;\n");
        printf("2. Buscar dados pelo CPF;\n");
        printf("3. Remover cadastro;\n");
        printf("4. Exibir tabela;\n");
        printf("5. Salvar tabela em arquivo;\n");
        printf("6. Carregar tabela de arquivo;\n");
        printf("7. Sair.\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);
        getchar();  // Limpa o caractere de nova linha do buffer

        switch (choice) {
            case 1:  // Inserir
                do {
                    printf("Digite o CPF: ");
                    fgets(cpf, 12, stdin);
                    cpf[strcspn(cpf, "\n")] = '\0';                    
                    printf("\nPressione enter para continuar...\n");
                    getchar();

                    if (!validar_cpf(cpf)) {
                        do {
                            printf("\nCPF invalido! Deseja tentar novamente? (s/n): ");
                            scanf(" %c", &confirmacao);
                            getchar();
                        } while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N');
                        if (confirmacao == 'n' || confirmacao == 'N') {
                            printf("\nOperacao cancelada pelo usuario.\n");
                            break;
                        }
                    }
                } while (!validar_cpf(cpf) && (confirmacao == 's' || confirmacao == 'S'));

                if (!validar_cpf(cpf)) {
                    break;
                }

                if (cpf_ja_cadastrado(cpf)) {
                    do {
                        printf("\nCPF ja cadastrado! Deseja atualizar o cadastro? (s/n): ");
                        scanf(" %c", &confirmacao);
                        getchar();
                    } while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N');

                    if (confirmacao == 'n' || confirmacao == 'N') {
                        printf("\nAtualizacao de cadastro cancelada pelo usuario.\n");
                        break;
                    }
                }

                do {
                    printf("Digite o nome: ");
                    fgets(nome, MAX_INPUT_SIZE, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    if (!validar_nome(nome)) {
                        do {
                            printf("\nNome invalido! Deseja tentar novamente? (s/n): ");
                            scanf(" %c", &confirmacao);
                            getchar();
                        } while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N');
                        if (confirmacao == 'n' || confirmacao == 'N') {
                            printf("\nOperacao cancelada pelo usuario.\n");
                            break;
                        }
                    }
                } while (!validar_nome(nome) && (confirmacao == 's' || confirmacao == 'S'));

                if (!validar_nome(nome)) {
                    break;
                }

                do {
                    printf("Digite a data de nascimento (DD/MM/AAAA): ");
                    fgets(data_nascimento, 11, stdin);
                    data_nascimento[strcspn(data_nascimento, "\n")] = '\0';

                    if (!validar_data(data_nascimento)) {
                        do {
                            printf("\nData de nascimento invalida! Deseja tentar novamente? (s/n): ");
                            scanf(" %c", &confirmacao);
                            getchar();
                        } while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N');
                        if (confirmacao == 'n' || confirmacao == 'N') {
                            printf("\nOperacao cancelada pelo usuario.\n");
                            break;
                        }
                    }
                } while (!validar_data(data_nascimento) && (confirmacao == 's' || confirmacao == 'S'));

                if (!validar_data(data_nascimento)) {
                    break;
                }

                // Mostra os dados inseridos e pede confirmação
                printf("\nPor favor, confirme os dados inseridos:\n");
                printf("CPF: %s\n", cpf);
                printf("Nome: %s\n", nome);
                printf("Data de Nascimento: %s\n", data_nascimento);
                do {
                    printf("Os dados estao corretos? (s/n): ");
                    scanf(" %c", &confirmacao);
                    getchar();
                } while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N');

                if (confirmacao == 's' || confirmacao == 'S') {
                    insert(cpf, nome, data_nascimento);
                    printf("\nDados inseridos com sucesso!\n");
                } else {
                    printf("\nInsercao cancelada pelo usuario.\n");
                }
                break;

            case 2:  // Buscar
                printf("Digite o CPF para buscar: ");
                fgets(cpf, 12, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                if (validar_cpf(cpf)) {
                    DadosPessoa *result = search(cpf);
                    if (result) {
                        printf("\nNome encontrado: %s, Data de Nascimento: %s, CPF: %s\n", result->nome, result->data_nascimento, result->cpf);
                    } else {
                        printf("CPF nao encontrado.\n");
                    }
                } else {
                    printf("CPF invalido: %s\n", cpf);
                }
                break;

            case 3:  // Remover
                printf("Digite o CPF para remover: ");
                fgets(cpf, 12, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                delete(cpf);
                printf("\nCPF removido (se existia).\n");
                break;

            case 4:  // Exibir tabela
                display_table();
                break;

            case 5:  // Salvar em arquivo
                save_to_file("hashtable_data.txt");
                break;

            case 6:  // Carregar de arquivo
                load_from_file("hashtable_data.txt");
                break;

            case 7:  // Sair
                printf("\nEncerrando...\n");
                return 0;

            default:
                printf("\nNumero invalido. Tente novamente.\n");
        }
    }

    return 0;
}
