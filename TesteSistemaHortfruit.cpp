#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define LARGURA_TERMINAL 50  // Usado para centralizar textos

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
int verificarSaida(char *input);  // Fun��o para verificar se o usu�rio quer sair
void centralizarTexto(char *texto);  // Fun��o para centralizar o texto

// Fun��o principal
main() {
    struct Produto produtos[MAX_PRODUTOS];
    int contador = 0;
    int opcao;

    // Carrega os produtos do arquivo na inicializa��o
    carregarProdutos(produtos, &contador);

    do {
        exibirMenu();  // Chama a fun��o de exibi��o de menu
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer ap�s leitura de n�mero

        switch (opcao) {
            case 1:
                cadastrarProduto(produtos, &contador);
                salvarProdutos(produtos, contador); // Salva ap�s cada cadastro
                break;
            case 2:
                listarProdutos(produtos, contador);
                break;
            case 3:
                atualizarEstoque(produtos, contador);
                salvarProdutos(produtos, contador); // Salva ap�s cada atualiza��o de estoque
                break;
            case 4:
                relatorioEstoqueBaixo(produtos, contador);
                break;
            case 5:
                excluirProduto(produtos, &contador);
                salvarProdutos(produtos, contador); // Salva ap�s a exclus�o
                break;
            case 6:
                centralizarTexto("Saindo do sistema...");
                salvarProdutos(produtos, contador); // Salva antes de sair
                break;
            default:
                centralizarTexto("Opcao invalida! Tente novamente.");
        }
    } while (opcao != 6);

    return 0;
}

// Fun��o para carregar os produtos do arquivo
void carregarProdutos(struct Produto produtos[], int *contador) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        centralizarTexto("Nenhum arquivo de dados encontrado. Criando um novo...");
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
        centralizarTexto("Erro ao abrir o arquivo para escrita.");
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
        centralizarTexto("Cadastro de Produto");
        printf("\nDigite '-+nao quero continuar+-' para voltar ao menu.\n");

        printf("Nome do produto: ");
        fgets(produtos[*contador].nome, 50, stdin);  // Usa fgets para capturar a string completa
        produtos[*contador].nome[strcspn(produtos[*contador].nome, "\n")] = 0;  // Remove o caractere '\n'

        // Verifica se o usu�rio quer voltar
        if (verificarSaida(produtos[*contador].nome)) {
            centralizarTexto("Voltando ao menu principal...");
            return;
        }

        printf("Preco do produto: ");
        scanf("%f", &produtos[*contador].preco);
        limparBuffer(); // Limpa o buffer ap�s a leitura do pre�o

        printf("Quantidade do produto: ");
        scanf("%d", &produtos[*contador].quantidade);
        limparBuffer(); // Limpa o buffer ap�s a leitura da quantidade

        (*contador)++;
        centralizarTexto("Produto cadastrado com sucesso!");
    } else {
        centralizarTexto("Limite de produtos atingido!");
    }

    printf("\n");
}

// Fun��o para listar os produtos cadastrados
void listarProdutos(struct Produto produtos[], int contador) {
    centralizarTexto("Lista de Produtos");
    printf("\n");

    for (int i = 0; i < contador; i++) {
        printf("Produto: %s, Preco: R$ %.2f, Quantidade: %d\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    printf("\n");
}

// Fun��o para atualizar o estoque de um produto
void atualizarEstoque(struct Produto produtos[], int contador) {
    char nome[50];
    int quantidade;

    centralizarTexto("Atualizar Estoque");
    printf("\nDigite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usu�rio quer voltar
    if (verificarSaida(nome)) {
        centralizarTexto("Voltando ao menu principal...");
        return;
    }

    for (int i = 0; i < contador; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Quantidade a adicionar ou remover (use numeros negativos para remover): ");
            scanf("%d", &quantidade);
            limparBuffer(); // Limpa o buffer ap�s a leitura da quantidade
            produtos[i].quantidade += quantidade;
            centralizarTexto("Estoque atualizado!");
            printf("\n");
            return;
        }
    }
    centralizarTexto("Produto nao encontrado!");
    printf("\n");
}

// Fun��o para gerar relat�rio de produtos com estoque baixo
void relatorioEstoqueBaixo(struct Produto produtos[], int contador) {
    centralizarTexto("Produtos com Estoque Baixo (menos de 5 unidades)");
    printf("\n");

    for (int i = 0; i < contador; i++) {
        if (produtos[i].quantidade < 5) {
            printf("Produto: %s, Quantidade: %d\n", produtos[i].nome, produtos[i].quantidade);
        }
    }

    printf("\n");
}

// Fun��o para excluir um produto
void excluirProduto(struct Produto produtos[], int *contador) {
    char nome[50];
    int encontrado = 0;

    centralizarTexto("Excluir Produto");
    printf("\nDigite '-+nao quero continuar+-' para voltar ao menu.\n");

    printf("Nome do produto a excluir: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remove o caractere '\n'

    // Verifica se o usu�rio quer voltar
    if (verificarSaida(nome)) {
        centralizarTexto("Voltando ao menu principal...");
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
            centralizarTexto("Produto excluido com sucesso!");
            printf("\n");
            break;
        }
    }

    if (!encontrado) {
        centralizarTexto("Produto nao encontrado!");
    }

    printf("\n");
}

// Fun��o para exibir o menu principal
void exibirMenu() {
    printf("\n");
    centralizarTexto("====================================");
    centralizarTexto("        Sistema de Hortfruit        ");
    centralizarTexto("====================================");
    centralizarTexto("1. Cadastrar produto");
    centralizarTexto("2. Listar produtos");
    centralizarTexto("3. Atualizar estoque");
    centralizarTexto("4. Relat�rio de baixa quantidade");
    centralizarTexto("5. Excluir produto");
    centralizarTexto("6. Sair");
    centralizarTexto("====================================");
    printf("Escolha uma opcao: ");
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

// Fun��o para centralizar o texto na tela
void centralizarTexto(char *texto) {
    int espacos = (LARGURA_TERMINAL - strlen(texto)) / 2;
    for (int i = 0; i < espacos; i++) {
        printf(" ");
    }
    printf("%s\n", texto);
}

