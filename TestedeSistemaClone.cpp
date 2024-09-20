#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100

// Estrutura para armazenar os dados dos produtos
struct Produto {
    char nome[50];
    float preco;
    int quantidade;
};

// Fun��es
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

// Fun��o principal
int main() {
    struct Produto produtos[MAX_PRODUTOS];
    int contador = 0;
    int opcao;

    // Carrega os produtos do arquivo na inicializa��o
    carregarProdutos(produtos, &contador);

    do {
        system("cls"); // Limpa a tela antes de exibir o menu
        exibirMenu();  // Chama a fun��o de exibi��o de menu
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer ap�s leitura de n�mero

        switch (opcao) {
            case 1:
                cadastrarProduto(produtos, &contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa antes de voltar ao menu
                break;
            case 2:
                listarProdutos(produtos, contador);
                system("pause");  // Pausa ap�s listar os produtos
                break;
            case 3:
                atualizarEstoque(produtos, contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa ap�s atualizar o estoque
                break;
            case 4:
                relatorioEstoqueBaixo(produtos, contador);
                system("pause");  // Pausa ap�s gerar o relat�rio
                break;
            case 5:
                excluirProduto(produtos, &contador);
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa ap�s excluir o produto
                break;
            case 6:
                printf("Obrigado por usar o sistema! Ate a proxima!\n");
                salvarProdutos(produtos, contador);
                system("pause");  // Pausa antes de encerrar o programa
                break;
            default:
                printf("Oops! Parece que voc� digitou uma op��o invalida. Tente novamente.\n");
                system("pause");  // Pausa se o usu�rio escolher uma op��o inv�lida
        }
    } while (opcao != 6);

    return 0;
}

// Fun��o para carregar os produtos do arquivo
void carregarProdutos(struct Produto produtos[], int *contador) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de dados encontrado. Vamos come�ar do zero!\n");
        return;
    }
    while (fscanf(arquivo, "%s %f %d", produtos[*contador].nome, &produtos[*contador].preco, &produtos[*contador].quantidade) != EOF) {
        (*contador)++;
    }
    fclose(arquivo);
}

// Fun��o para salvar os produtos no arquivo
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

// Fun��o para cadastrar um novo produto
void cadastrarProduto(struct Produto produtos[], int *contador) {
    if (*contador < MAX_PRODUTOS) {
        printf("Vamos cadastrar um novo produto!\n");
        printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

        printf("Nome do produto: ");
        fgets(produtos[*contador].nome, 50, stdin);
        produtos[*contador].nome[strcspn(produtos[*contador].nome, "\n")] = 0;  // Remove o caractere '\n'

        // Verifica se o usu�rio quer voltar
        if (verificarSaida(produtos[*contador].nome)) {
            printf("Voltando ao menu principal...\n");
            return;
        }

        printf("\n Pre�o do produto: ");
        scanf("%f", &produtos[*contador].preco);
        limparBuffer(); // Limpa o buffer ap�s a leitura do pre�o

        printf("Quantidade do produto: ");
        scanf("%d", &produtos[*contador].quantidade);
        limparBuffer(); // Limpa o buffer ap�s a leitura da quantidade

        (*contador)++;
        printf("Produto cadastrado com sucesso! \n");
    } else {
        printf("Desculpe, mas ja atingimos o limite de produtos!\n");
    }

    printf("\n");
}

// Fun��o para listar os produtos cadastrados
void listarProdutos(struct Produto produtos[], int contador) {
    printf("Aqui est� a lista de produtos cadastrados:\n");
    printf("\n");

    for (int i = 0; i < contador; i++) {
        printf("Produto: %s, Pre�o: R$ %.2f, Quantidade: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    printf("\n");
}

// Fun��o para atualizar o estoque de um produto
void atualizarEstoque(struct Produto produtos[], int contador) {
    char nome[50];
    int quantidade;

    printf("Vamos atualizar o estoque de um produto!\n");
    printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usu�rio quer voltar
    if (verificarSaida(nome)) {
        printf("Voltando ao menu principal...\n");
        return;
    }

    for (int i = 0; i < contador; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Quantidade a adicionar ou remover (use numeros negativos para remover):\n ");
            scanf("%d", &quantidade);
            limparBuffer(); // Limpa o buffer ap�s a leitura da quantidade
            produtos[i].quantidade += quantidade;
            printf("Estoque atualizado! \n");
            printf("\n");
            return;
        }
    }
    printf("Desculpe, mas nao conseguimos encontrar o produto que voce mencionou.\n");
    printf("\n");
}

// Fun��o para gerar relat�rio de produtos com estoque baixo
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

// Fun��o para excluir um produto
void excluirProduto(struct Produto produtos[], int *contador) {
    char nome[50];
    int encontrado = 0;

    printf("Vamos excluir um produto!\n");
    printf("Digite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto a excluir: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usu�rio quer voltar
    if (verificarSaida(nome)) {
        printf("Voltando ao menu principal...\n");
        return;
    }

    for (int i = 0; i < *contador; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            encontrado = 1;
            // Move todos os produtos ap�s o produto exclu�do uma posi��o para frente
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

// Fun��o para exibir o menu principal
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

// Fun��o para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Fun��o para verificar se o usu�rio deseja voltar ao menu
int verificarSaida(char *input) {
    return strcmp(input, "-+nao quero continuar+-") == 0;
}

