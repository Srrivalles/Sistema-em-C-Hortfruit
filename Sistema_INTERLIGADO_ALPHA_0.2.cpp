#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100
#define VALIDADE_REEMBOLSO 3600 // 1 hora em segundos
#define ARQUIVO_ESTOQUE "produtos.txt"
#define ARQUIVO_COMPRAS "compras.txt"

// Estrutura para armazenar os dados dos produtos
typedef struct {
    char nome[50];
    float preco;
    int quantidade;
} Produto;

// Estrutura para armazenar dados dos clientes e reembolsos
typedef struct {
    char codigo_reembolso[20];
    float total;
    time_t timestamp; // Para controlar o tempo do ticket
} Cliente;

Produto produtos[MAX_PRODUTOS];
Cliente clientes[MAX_CLIENTES];
int total_produtos = 0;
int total_clientes = 0;

// Funções de estoque (módulo de logística)
void carregar_produtos();
void salvar_produtos();
void cadastrarProduto();
void listarProdutos();
void atualizarEstoque();
void relatorioEstoqueBaixo();
void excluirProduto();

// Funções de compras e reembolso (módulo de compras)
void realizar_compra();
void verificar_reembolso();
void registrar_compra(const char *codigo_reembolso, float total);
float calcular_desconto(float total, float desconto);
void gerar_codigo_reembolso(char *codigo);
int verificar_reembolso_valido(time_t timestamp);

// Funções de login, menu e utilitários
void exibirMenuPrincipal();
void exibirMenuCompras();
void exibirMenuLogistica();
void limparBuffer();
int verificar_login(int tipo);

int main() {
    setlocale(LC_ALL, "Portuguese");

    carregar_produtos();

    int opcao_principal;
    do {
        system("cls"); // Limpa a tela antes de exibir o menu principal
        exibirMenuPrincipal();
        scanf("%d", &opcao_principal);
        limparBuffer();

        if (opcao_principal == 1) {
            // Sistema de Compras
            if (!verificar_login(1)) {
                printf("Acesso ao sistema de compras negado.\n");
                continue;
            }

            int opcao_compras;
            do {
                system("cls"); // Limpa a tela
                exibirMenuCompras();
                scanf("%d", &opcao_compras);
                limparBuffer();

                switch (opcao_compras) {
                    case 1:
                        realizar_compra();
                        salvar_produtos(); // Salva o estoque após a compra
                        break;
                    case 2:
                        verificar_reembolso();
                        break;
                    case 3:
                        printf("Voltando ao menu principal...\n");
                        break;
                    default:
                        printf("Opção inválida! Tente novamente.\n");
                }
                system("pause");  // Pausa após cada ação
            } while (opcao_compras != 3);
        } else if (opcao_principal == 2) {
            // Sistema de Logística (estoque)
            if (!verificar_login(2)) {
                printf("Acesso ao sistema de logística negado.\n");
                continue;
            }

            int opcao_logistica;
            do {
                system("cls"); // Limpa a tela
                exibirMenuLogistica();
                scanf("%d", &opcao_logistica);
                limparBuffer();

                switch (opcao_logistica) {
                    case 1:
                        cadastrarProduto();
                        salvar_produtos();
                        break;
                    case 2:
                        listarProdutos();
                        break;
                    case 3:
                        atualizarEstoque();
                        salvar_produtos();
                        break;
                    case 4:
                        relatorioEstoqueBaixo();
                        break;
                    case 5:
                        excluirProduto();
                        salvar_produtos();
                        break;
                    case 6:
                        printf("Voltando ao menu principal...\n");
                        break;
                    default:
                        printf("Opção inválida! Tente novamente.\n");
                }
                system("pause");  // Pausa após cada ação
            } while (opcao_logistica != 6);
        } else if (opcao_principal == 3) {
            printf("Saindo do sistema...\n");
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
        system("pause");
    } while (opcao_principal != 3);

    return 0;
}

// Funções de estoque (logística)
void carregar_produtos() {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }

    while (fscanf(arquivo, "%[^,],%f,%d\n", produtos[total_produtos].nome, &produtos[total_produtos].preco, &produtos[total_produtos].quantidade) != EOF) {
        total_produtos++;
    }
    fclose(arquivo);
}

void salvar_produtos() {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estoque para escrita.\n");
        return;
    }

    for (int i = 0; i < total_produtos; i++) {
        fprintf(arquivo, "%s,%.2f,%d\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
    fclose(arquivo);
}

void cadastrarProduto() {
    if (total_produtos < MAX_PRODUTOS) {
        printf("Cadastrar novo produto:\n");
        printf("Nome: ");
        fgets(produtos[total_produtos].nome, 50, stdin);
        produtos[total_produtos].nome[strcspn(produtos[total_produtos].nome, "\n")] = 0;

        printf("Preço: ");
        scanf("%f", &produtos[total_produtos].preco);
        limparBuffer();

        printf("Quantidade: ");
        scanf("%d", &produtos[total_produtos].quantidade);
        limparBuffer();

        total_produtos++;
        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

void listarProdutos() {
    printf("Produtos cadastrados:\n");
    for (int i = 0; i < total_produtos; i++) {
        printf("%d - %s: R$%.2f (Quantidade: %d)\n", i + 1, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
}

void atualizarEstoque() {
    char nome[50];
    int quantidade;

    printf("Nome do produto para atualizar: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    for (int i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Quantidade a adicionar/remover: ");
            scanf("%d", &quantidade);
            produtos[i].quantidade += quantidade;
            printf("Estoque atualizado!\n");
            return;
        }
    }
    printf("Produto não encontrado.\n");
}

void relatorioEstoqueBaixo() {
    printf("Produtos com estoque baixo (menos de 5 unidades):\n");
    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade < 5) {
            printf("%s: %d unidades\n", produtos[i].nome, produtos[i].quantidade);
        }
    }
}

void excluirProduto() {
    char nome[50];
    printf("Nome do produto a excluir: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    for (int i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            for (int j = i; j < total_produtos - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            total_produtos--;
            printf("Produto excluído.\n");
            return;
        }
    }
    printf("Produto não encontrado.\n");
}

// Funções de compras e reembolso
void realizar_compra() {
    float total_compra = 0.0;
    int opcao, quantidade;
    char codigo_reembolso[20];

    listarProdutos();

    while (1) {
        printf("Selecione o produto (0 para finalizar): ");
        scanf("%d", &opcao);
        if (opcao == 0) break;

        printf("Quantidade: ");
        scanf("%d", &quantidade);

        if (opcao > 0 && opcao <= total_produtos && quantidade <= produtos[opcao - 1].quantidade) {
            produtos[opcao - 1].quantidade -= quantidade;
            total_compra += produtos[opcao - 1].preco * quantidade;
        } else {
            printf("Produto ou quantidade inválidos.\n");
        }
    }

    if (total_compra > 0) {
        gerar_codigo_reembolso(codigo_reembolso);
        printf("Compra realizada! Total: R$%.2f\n", total_compra);
        printf("Código de reembolso: %s\n", codigo_reembolso);
        registrar_compra(codigo_reembolso, total_compra);
    }
}

void verificar_reembolso() {
    char codigo[20];
    printf("Digite o código de reembolso: ");
    fgets(codigo, 20, stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].codigo_reembolso, codigo) == 0) {
            if (verificar_reembolso_valido(clientes[i].timestamp)) {
                printf("Reembolso válido! Total a reembolsar: R$%.2f\n", clientes[i].total);
                return;
            } else {
                printf("Desculpe, o período de reembolso expirou.\n");
                return;
            }
        }
    }
    printf("Código de reembolso não encontrado.\n");
}

void registrar_compra(const char *codigo_reembolso, float total) {
    strcpy(clientes[total_clientes].codigo_reembolso, codigo_reembolso);
    clientes[total_clientes].total = total;
    clientes[total_clientes].timestamp = time(NULL);
    total_clientes++;
}

void gerar_codigo_reembolso(char *codigo) {
    sprintf(codigo, "RB%05d", total_clientes + 1);
}

int verificar_reembolso_valido(time_t timestamp) {
    time_t agora = time(NULL);
    return difftime(agora, timestamp) <= VALIDADE_REEMBOLSO;
}

// Funções de menu
void exibirMenuPrincipal() {
    printf("Menu Principal\n");
    printf("1. Sistema de Compras\n");
    printf("2. Sistema de Logística\n");
    printf("3. Sair\n");
    printf("Escolha uma opção: ");
}

void exibirMenuCompras() {
    printf("Sistema de Compras\n");
    printf("1. Realizar compra\n");
    printf("2. Verificar reembolso\n");
    printf("3. Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
}

void exibirMenuLogistica() {
    printf("Sistema de Logística (Estoque)\n");
    printf("1. Cadastrar produto\n");
    printf("2. Listar produtos\n");
    printf("3. Atualizar estoque\n");
    printf("4. Relatório de estoque baixo\n");
    printf("5. Excluir produto\n");
    printf("6. Voltar ao menu principal\n");
    printf("Escolha uma opção: ");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Função de login com controle de tipo
int verificar_login(int tipo) {
    char usuario[20], senha[20];
    printf("Login\n");
    printf("Usuário: ");
    fgets(usuario, 20, stdin);
    usuario[strcspn(usuario, "\n")] = 0;
    printf("Senha: ");
    fgets(senha, 20, stdin);
    senha[strcspn(senha, "\n")] = 0;

    if (tipo == 1) { // Sistema de Compras
        if (strcmp(usuario, "compras") == 0 && strcmp(senha, "compras123") == 0) {
            return 1;
        }
    } else if (tipo == 2) { // Sistema de Logística (estoque)
        if (strcmp(usuario, "logistica") == 0 && strcmp(senha, "logistica123") == 0) {
            return 1;
        }
    }
    return 0;
}

