#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define MAX_PRODUTOS 100

// Estrutura para armazenar os dados dos produtos
struct Produto {
    char nome[50];
    float preco;
    int quantidade;
};

// Funções
void carregarProdutos(struct Produto produtos[], int *contador);
void salvarProdutos(struct Produto produtos[], int contador);
void cadastrarProduto(struct Produto produtos[], int *contador);
void listarProdutos(struct Produto produtos[], int contador);
void atualizarEstoque(struct Produto produtos[], int contador);
void relatorioEstoqueBaixo(struct Produto produtos[], int contador);
void excluirProduto(struct Produto produtos[], int *contador);
void exibirMenu();
void limparBuffer();
int verificarSaida(char *input);

// Função principal
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    struct Produto produtos[MAX_PRODUTOS];
    int contador = 0;
    int opcao;

    // Carrega os produtos do arquivo na inicialização
    carregarProdutos(produtos, &contador);

    do {
        system("cls"); // Limpa a tela antes de exibir o menu
        exibirMenu();  // Chama a função de exibição de menu
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer após leitura de número

        switch (opcao) {
            case 1:
                cadastrarProduto(produtos, &contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa antes de voltar ao menu
                break;
            case 2:
                listarProdutos(produtos, contador);
                system("pause");  // Pausa após listar os produtos
                break;
            case 3:
                atualizarEstoque(produtos, contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa após atualizar o estoque
                break;
            case 4:
                relatorioEstoqueBaixo(produtos, contador);
                system("pause");  // Pausa após gerar o relatório
                break;
            case 5:
                excluirProduto(produtos, &contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa após excluir o produto
                break;
            case 6:
                printf("Obrigado por usar o sistema! Ate a proxima!\n");
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa antes de encerrar o programa
                break;
            default:
                printf("Oops! Parece que você digitou uma opção invalida. Tente novamente.\n");
                system("pause");  // Pausa se o usuário escolher uma opção inválida
        }
    } while (opcao != 6);

    return 0;
}

// Função para carregar os produtos do arquivo
void carregarProdutos(struct Produto produtos[], int *contador) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de dados encontrado. Vamos começar do zero!\n");
        return;
    }
    while (fscanf(arquivo, "%s %f %d", produtos[*contador].nome, &produtos[*contador].preco, &produtos[*contador].quantidade) != EOF) {
        (*contador)++;
    }
    fclose(arquivo);
}

// Função para salvar os produtos no arquivo
void salvarProdutos(struct Produto produtos[], int contador) {
    FILE *arquivo = fopen("produtos.txt", "w");
    if (arquivo == NULL) {
        printf("Houve um problema ao tentar salvar os dados. Tente novamente mais tarde.\n");
        return;
    }
    for (int i = 0; i < contador; i++) {
        fprintf(arquivo, "%s %.2f %d\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
    fclose(arquivo);
}

// Função para cadastrar um novo produto
void cadastrarProduto(struct Produto produtos[], int *contador) {
    if (*contador < MAX_PRODUTOS) {
        printf("Vamos cadastrar um novo produto!\n");
        printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

        printf("Nome do produto: ");
        fgets(produtos[*contador].nome, 50, stdin);
        produtos[*contador].nome[strcspn(produtos[*contador].nome, "\n")] = 0;  // Remove o caractere '\n'

        // Verifica se o usuário quer voltar
        if (verificarSaida(produtos[*contador].nome)) {
            printf("Voltando ao menu principal...\n");
            return;
        }

        printf("\n Preço do produto: ");
        scanf("%f", &produtos[*contador].preco);
        limparBuffer(); // Limpa o buffer após a leitura do preço

        printf("Quantidade do produto: ");
        scanf("%d", &produtos[*contador].quantidade);
        limparBuffer(); // Limpa o buffer após a leitura da quantidade

        (*contador)++;
        printf("Produto cadastrado com sucesso! \n");
    } else {
        printf("Desculpe, mas ja atingimos o limite de produtos!\n");
    }

    printf("\n");
}

// Função para listar os produtos cadastrados
void listarProdutos(struct Produto produtos[], int contador) {
    printf("Aqui está a lista de produtos cadastrados:\n");
    printf("\n");

    for (int i = 0; i < contador; i++) {
        printf("Produto: %s, Preço: R$ %.2f, Quantidade: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    printf("\n");
}

// Função para atualizar o estoque de um produto
void atualizarEstoque(struct Produto produtos[], int contador) {
    char nome[50];
    int quantidade;

    printf("Vamos atualizar o estoque de um produto!\n");
    printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usuário quer voltar
    if (verificarSaida(nome)) {
        printf("Voltando ao menu principal...\n");
        return;
    }

    for (int i = 0; i < contador; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Quantidade a adicionar ou remover (use numeros negativos para remover):\n ");
            scanf("%d", &quantidade);
            limparBuffer(); // Limpa o buffer após a leitura da quantidade
            produtos[i].quantidade += quantidade;
            printf("Estoque atualizado! \n");
            printf("\n");
            return;
        }
    }
    printf("Desculpe, mas nao conseguimos encontrar o produto que voce mencionou.\n");
    printf("\n");
}

// Função para gerar relatório de produtos com estoque baixo
void relatorioEstoqueBaixo(struct Produto produtos[], int contador) {
    printf("Produtos com estoque baixo (menos de 5 unidades):\n");
    printf("\n");

    int encontrou = 0;
    for (int i = 0; i < contador; i++) {
        if (produtos[i].quantidade < 5) {
            printf("Produto: %s, Quantidade: %d\n", produtos[i].nome, produtos[i].quantidade);
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("Todos os produtos estao com o estoque saudavel! \n");
    }

    printf("\n");
}

// Função para excluir um produto
void excluirProduto(struct Produto produtos[], int *contador) {
    char nome[50];
    int encontrado = 0;

    printf("Vamos excluir um produto!\n");
    printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto a excluir: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usuário quer voltar
    if (verificarSaida(nome)) {
        printf("Voltando ao menu principal...\n");
        return;
    }

    for (int i = 0; i < *contador; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            encontrado = 1;
            // Move todos os produtos após o produto excluído uma posição para frente
            for (int j = i; j < *contador - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            (*contador)--;
            printf("Produto excluido com sucesso! \n");
            printf("\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Desculpe, mas nao encontramos o produto que voce deseja excluir.\n");
    }

    printf("\n");
}

// Função para exibir o menu principal
void exibirMenu() {
    printf("\n");
    printf("====================================\n");
    printf("        Sistema de Hortfruit        \n");
    printf("====================================\n");
    printf("1. Cadastrar produto\n");
    printf("2. Listar produtos\n");
    printf("3. Atualizar estoque\n");
    printf("4. Relatorio de baixa quantidade\n");
    printf("5. Excluir produto\n");
    printf("6. Sair\n");
    printf("====================================\n");
    printf("Escolha uma opcao: \n");
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Função para verificar se o usuário deseja voltar ao menu
int verificarSaida(char *input) {
    return strcmp(input, "-+nao quero continuar+-") == 0;
}

