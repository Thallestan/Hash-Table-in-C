#include "HashTable.c" 
#include "Hashtable.h"


int main() {
    initialize_table();

    char key[MAX_INPUT_SIZE];
    char value[MAX_INPUT_SIZE];
    int choice;

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Inserir Nome-CPF;\n");
        printf("2. Buscar CPF pelo nome;\n");
        printf("3. Remover nome;\n");
        printf("4. Exibir tabela;\n");
        printf("5. Salvar tabela em arquivo;\n");
        printf("6. Carregar tabela de arquivo;\n");
        printf("7. Sair.\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);
        getchar();  // Limpa o caractere de nova linha do buffer

        switch (choice) {
            case 1:  // Inserir
                printf("Digite o nome: ");
                fgets(key, MAX_INPUT_SIZE, stdin);
                key[strcspn(key, "\n")] = '\0';

                printf("Digite o CPF: ");
                fgets(value, MAX_INPUT_SIZE, stdin);
                value[strcspn(value, "\n")] = '\0';

                insert(key, value);
                printf("Nome-CPF inseridos com sucesso!\n");
                break;

            case 2:  // Buscar
                printf("Digite o nome para buscar: ");
                fgets(key, MAX_INPUT_SIZE, stdin);
                key[strcspn(key, "\n")] = '\0';

                char *result = search(key);
                if (result) {
                    printf("CPF encontrado: %s\n", result);
                } else {
                    printf("Nome nao encontrada.\n");
                }
                break;

            case 3:  // Remover
                printf("Digite o nome para remover: ");
                fgets(key, MAX_INPUT_SIZE, stdin);
                key[strcspn(key, "\n")] = '\0';

                delete(key);
                printf("Nome removido (se existia).\n");
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
                printf("Encerrando...\n");
                return 0;

            default:
                printf("Numero invalido. Tente novamente.\n");
        }
    }

    return 0;
}