#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100
#define MAX_USUARIOS 50
#define VALIDADE_REEMBOLSO 3600 // 1 hora em segundos
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"
#define MAX_CARRINHO 50 // Capacidade do carrinho de compras

struct Produto {
    char nome[50];
    char codigo[20];
    float preco;
    int quantidade;
    int vendido_por_peso; // 1 se o produto é vendido por peso, 0 caso contrário
};

struct Cliente {
    char codigo_reembolso[20];
    float total;
    time_t timestamp;
};

struct Usuario {
    char username[20];
    char password[20];
    int tipo; // 1: Caixa, 2: Logística
};

struct Produto produtos[MAX_PRODUTOS];
struct Cliente clientes[MAX_CLIENTES];
struct Usuario usuarios[MAX_USUARIOS];
struct Produto carrinho[MAX_CARRINHO]; // Carrinho de compras
int total_carrinho = 0; // Total de produtos no carrinho

int total_produtos = 0;
int total_clientes = 0;
int total_usuarios = 0;

// Funções de gerenciamento de produtos
void cadastrarProduto();
void listarProdutos();
void atualizarEstoque();
void relatorioEstoqueBaixo();
void excluirProduto();
void carregarProdutos();
void salvarProdutos();

// Funções de compras e reembolso
void realizarCompra();
void adicionarAoCarrinho();
void finalizarCompra();
void gerarCodigoReembolso(char *codigo);
void registrarCompra(const char *codigo_reembolso, float total);
void verificarReembolso();
int verificarReembolsoValido(time_t timestamp);
float balancaVirtual();

// Funções de login, menu e utilitários
void exibirMenuPrincipal();
void exibirMenuEscolha();
void exibirMenuCompras();
void exibirMenuLogistica();
void limparBuffer();
int verificarLogin();
void cadastrarUsuario();
void carregarUsuarios();
void salvarUsuarios();

// Implementações das funções
void cadastrarProduto() {
    if (total_produtos < MAX_PRODUTOS) {
        struct Produto novo_produto;
        printf("Digite o nome do produto: ");
        fgets(novo_produto.nome, sizeof(novo_produto.nome), stdin);
        novo_produto.nome[strcspn(novo_produto.nome, "\n")] = 0;

        printf("Digite o código do produto: ");
        fgets(novo_produto.codigo, sizeof(novo_produto.codigo), stdin);
        novo_produto.codigo[strcspn(novo_produto.codigo, "\n")] = 0;

        printf("Digite o preço do produto: ");
        scanf("%f", &novo_produto.preco);
        limparBuffer();

        printf("Digite a quantidade do produto: ");
        scanf("%d", &novo_produto.quantidade);
        limparBuffer();

        printf("Este produto é vendido por peso? (1 para sim, 0 para não): ");
        scanf("%d", &novo_produto.vendido_por_peso);
        limparBuffer();

        produtos[total_produtos] = novo_produto;
        total_produtos++;
        printf("Produto cadastrado com sucesso!\n");
        salvarProdutos(); // Salva após cadastrar
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

void listarProdutos() {
    int i;
    printf("Lista de Produtos:\n");
    for (i = 0; i < total_produtos; i++) {
        printf("%s (Código: %s): R$%.2f, %d unidades%s\n",
               produtos[i].nome, produtos[i].codigo, produtos[i].preco, produtos[i].quantidade,
               produtos[i].vendido_por_peso ? " (vendido por peso)" : "");
    }
}

void atualizarEstoque() {
    char nome[50];
    int quantidade;

    printf("Digite o nome do produto para atualizar o estoque: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int i;
    for (i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            printf("Quantidade atual: %d\n", produtos[i].quantidade);
            printf("Nova quantidade: ");
            scanf("%d", &quantidade);
            limparBuffer();
            produtos[i].quantidade = quantidade;
            printf("Estoque atualizado com sucesso para %s!\n", produtos[i].nome);
            salvarProdutos(); // Salva após atualizar
            return;
        }
    }
    printf("Produto não encontrado.\n");
}

void relatorioEstoqueBaixo() {
    int i;
    printf("Produtos com estoque baixo (menor que 10 unidades):\n");
    for (i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade < 10) {
            printf("%s (Código: %s): %d unidades\n", produtos[i].nome, produtos[i].codigo, produtos[i].quantidade);
        }
    }
}

void excluirProduto() {
    char codigo[20];

    printf("Digite o código do produto a ser excluído: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    int i;
    for (i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].codigo, codigo) == 0) {
            int j;
            for (j = i; j < total_produtos - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            total_produtos--;
            printf("Produto excluído com sucesso!\n");
            salvarProdutos(); // Salva após excluir
            return;
        }
    }
    printf("Produto não encontrado.\n");
}

// Funções de compras e reembolso
float balancaVirtual() {
    float peso;
    printf("Digite o peso (em kg): ");
    scanf("%f", &peso);
    limparBuffer();
    return peso;
}

void adicionarAoCarrinho() {
    if (total_carrinho < MAX_CARRINHO) {
        char codigo[20];
        printf("Digite o código do produto para adicionar ao carrinho: ");
        fgets(codigo, sizeof(codigo), stdin);
        codigo[strcspn(codigo, "\n")] = 0;

        for (int i = 0; i < total_produtos; i++) {
            if (strcmp(produtos[i].codigo, codigo) == 0) {
                if (produtos[i].quantidade > 0) {
                    carrinho[total_carrinho++] = produtos[i];
                    printf("Produto %s adicionado ao carrinho.\n", produtos[i].nome);
                    return;
                } else {
                    printf("Produto fora de estoque.\n");
                    return;
                }
            }
        }
        printf("Produto não encontrado.\n");
    } else {
        printf("Carrinho cheio. Não é possível adicionar mais produtos.\n");
    }
}

void finalizarCompra() {
    float total = 0.0;

    for (int i = 0; i < total_carrinho; i++) {
        total += carrinho[i].preco;
        // Reduzir a quantidade do produto no estoque
        for (int j = 0; j < total_produtos; j++) {
            if (strcmp(produtos[j].codigo, carrinho[i].codigo) == 0) {
                produtos[j].quantidade--;
                break;
            }
        }
    }

    char codigo_reembolso[20];
    gerarCodigoReembolso(codigo_reembolso);
    registrarCompra(codigo_reembolso, total);

    // Exibir o código de reembolso
    printf("Compra realizada com sucesso! Total: R$%.2f\n", total);
    printf("Código de reembolso: %s\n", codigo_reembolso);

    // Limpar o carrinho
    total_carrinho = 0;
}

void realizarCompra() {
    int opcao;
    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Adicionar produto ao carrinho\n");
        printf("2. Finalizar compra\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                adicionarAoCarrinho();
                break;
            case 2:
                finalizarCompra();
                return;
            case 3:
                return;
            default:
                printf("Opção inválida.\n");
        }
    }
}

void gerarCodigoReembolso(char *codigo) {
    sprintf(codigo, "REEM%05d", total_clientes + 1);
}

void registrarCompra(const char *codigo_reembolso, float total) {
    struct Cliente novo_cliente;
    strcpy(novo_cliente.codigo_reembolso, codigo_reembolso);
    novo_cliente.total = total;
    novo_cliente.timestamp = time(NULL);
    clientes[total_clientes++] = novo_cliente;
}

int verificarReembolsoValido(time_t timestamp) {
    return difftime(time(NULL), timestamp) <= VALIDADE_REEMBOLSO;
}

void verificarReembolso() {
    char codigo[20];
    printf("Digite o código de reembolso: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].codigo_reembolso, codigo) == 0) {
            if (verificarReembolsoValido(clientes[i].timestamp)) {
                printf("Reembolso válido! Total: R$%.2f\n", clientes[i].total);
            } else {
                printf("Reembolso inválido ou expirado.\n");
            }
            return;
        }
    }
    printf("Código de reembolso não encontrado.\n");
}

// Funções de login, menu e utilitários
void exibirMenuPrincipal() {
    printf("Bem-vindo ao sistema de vendas!\n");
}

void exibirMenuEscolha() {
    int tipo_usuario = verificarLogin();
    if (tipo_usuario == 1) {
        exibirMenuCompras();
    } else if (tipo_usuario == 2) {
        exibirMenuLogistica();
    } else {
        printf("Usuário não encontrado.\n");
    }
}

void exibirMenuCompras() {
    int opcao;
    while (1) {
        printf("\nMenu de Compras:\n");
        printf("1. Listar Produtos\n");
        printf("2. Realizar Compra\n");
        printf("3. Verificar Reembolso\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                listarProdutos();
                break;
            case 2:
                realizarCompra();
                break;
            case 3:
                verificarReembolso();
                break;
            case 4:
                return;
            default:
                printf("Opção inválida.\n");
        }
    }
}

void exibirMenuLogistica() {
    int opcao;
    while (1) {
        printf("\nMenu de Logística:\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Atualizar Estoque\n");
        printf("4. Relatório de Estoque Baixo\n");
        printf("5. Excluir Produto\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                atualizarEstoque();
                break;
            case 4:
                relatorioEstoqueBaixo();
                break;
            case 5:
                excluirProduto();
                break;
            case 6:
                return;
            default:
                printf("Opção inválida.\n");
        }
    }
}

void limparBuffer() {
    while (getchar() != '\n');
}

int verificarLogin() {
    char username[20], password[20];
    printf("Digite seu username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Digite sua senha: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) {
            return usuarios[i].tipo; // Retorna o tipo do usuário
        }
    }
    return -1; // Usuário não encontrado
}

void cadastrarUsuario() {
    if (total_usuarios < MAX_USUARIOS) {
        struct Usuario novo_usuario;
        printf("Digite o username: ");
        fgets(novo_usuario.username, sizeof(novo_usuario.username), stdin);
        novo_usuario.username[strcspn(novo_usuario.username, "\n")] = 0;

        printf("Digite a senha: ");
        fgets(novo_usuario.password, sizeof(novo_usuario.password), stdin);
        novo_usuario.password[strcspn(novo_usuario.password, "\n")] = 0;

        printf("Digite o tipo de usuário (1: Caixa, 2: Logística): ");
        scanf("%d", &novo_usuario.tipo);
        limparBuffer();

        usuarios[total_usuarios] = novo_usuario;
        total_usuarios++;
        printf("Usuário cadastrado com sucesso!\n");
        salvarUsuarios(); // Salva após cadastrar
    } else {
        printf("Limite de usuários atingido.\n");
    }
}

void carregarUsuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "r");
    if (file) {
        while (fscanf(file, "%s %s %d", usuarios[total_usuarios].username, usuarios[total_usuarios].password, &usuarios[total_usuarios].tipo) == 3) {
            total_usuarios++;
        }
        fclose(file);
    }
}

void salvarUsuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "w");
    if (file) {
        for (int i = 0; i < total_usuarios; i++) {
            fprintf(file, "%s %s %d\n", usuarios[i].username, usuarios[i].password, usuarios[i].tipo);
        }
        fclose(file);
    }
}

void carregarProdutos() {
    FILE *file = fopen(ARQUIVO_PRODUTOS, "r");
    if (file) {
        while (fscanf(file, "%[^;];%[^;];%f;%d;%d\n", produtos[total_produtos].nome, produtos[total_produtos].codigo,
                      &produtos[total_produtos].preco, &produtos[total_produtos].quantidade,
                      &produtos[total_produtos].vendido_por_peso) == 5) {
            total_produtos++;
        }
        fclose(file);
    }
}

void salvarProdutos() {
    FILE *file = fopen(ARQUIVO_PRODUTOS, "w");
    if (file) {
        for (int i = 0; i < total_produtos; i++) {
            fprintf(file, "%s;%s;%.2f;%d;%d\n", produtos[i].nome, produtos[i].codigo, produtos[i].preco,
                    produtos[i].quantidade, produtos[i].vendido_por_peso);
        }
        fclose(file);
    }
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    carregarUsuarios();
    carregarProdutos();
    exibirMenuPrincipal();
    exibirMenuEscolha();
    return 0;
}
