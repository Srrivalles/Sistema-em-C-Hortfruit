#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100
#define MAX_USUARIOS 100
#define MAX_HISTORICO 100
#define VALIDADE_REEMBOLSO 3600
#define ARQUIVO_ESTOQUE "produtos.txt"
#define ARQUIVO_COMPRAS "compras.txt"
#define ARQUIVO_CLIENTES "clientes.txt"
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_BACKUP_ESTOQUE "backup_produtos.txt"
#define ARQUIVO_BACKUP_USUARIOS "backup_usuarios.txt"
#define ARQUIVO_HISTORICO "historico_compras.txt"

// Estruturas
typedef struct {
    char codigo[20];
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct {
    char codigo_reembolso[20];
    float total;
    time_t timestamp;
} Cliente;

typedef struct {
    char username[20];
    char password[20];
    int tipo;
} Usuario;

typedef struct {
    char codigo_produto[20];
    char nome_produto[50];
    int quantidade;
    float preco_unitario;
    float preco_total;
    time_t timestamp;
} HistoricoCompra;

// Variáveis globais
Produto produtos[MAX_PRODUTOS];
Cliente clientes[MAX_CLIENTES];
Usuario usuarios[MAX_USUARIOS];
HistoricoCompra historico_compras[MAX_HISTORICO];
int total_produtos = 0;
int total_clientes = 0;
int total_usuarios = 0;
int total_historico = 0;

// Funções
void carregar_produtos();
void salvar_produtos();
void cadastrarProduto();
void listarProdutos();
void atualizarEstoque();
void relatorioEstoqueBaixo();
void excluirProduto();
void realizar_compra();
void verificar_reembolso();
void registrar_compra(const char *codigo_reembolso, float total);
void gerar_codigo_reembolso(char *codigo);
void gerar_codigo_produto(char *codigo, int index);
int verificar_reembolso_valido(time_t timestamp);
void exibirMenuPrincipal();
void exibirMenuCompras();
void exibirMenuLogistica();
void limparBuffer();
int verificar_login(int tipo);
void cadastrar_usuario();
int login_usuario();
void carregar_usuarios();
void salvar_usuarios();
void backup_dados();
void salvar_historico();
void carregar_historico();

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    carregar_produtos();
    carregar_usuarios();
    carregar_historico();

    int opcao_principal;
    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        exibirMenuPrincipal();
        scanf("%d", &opcao_principal);
        limparBuffer();

        if (opcao_principal == 1) {
            if (login_usuario() == 1) {
                int opcao_compras;
                do {
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    exibirMenuCompras();
                    scanf("%d", &opcao_compras);
                    limparBuffer();

                    if (opcao_compras == 1) realizar_compra();
                    else if (opcao_compras == 2) verificar_reembolso();
                    else if (opcao_compras != 3) printf("Opção inválida.\n");

                    system("pause");
                } while (opcao_compras != 3);
            }
        } else if (opcao_principal == 2) {
            if (login_usuario() == 2) {
                int opcao_logistica;
                do {
#ifdef _WIN32
                    system("cls");
#else
                    system("clear");
#endif
                    exibirMenuLogistica();
                    scanf("%d", &opcao_logistica);
                    limparBuffer();

                    switch (opcao_logistica) {
                        case 1: cadastrarProduto(); break;
                        case 2: listarProdutos(); break;
                        case 3: atualizarEstoque(); break;
                        case 4: relatorioEstoqueBaixo(); break;
                        case 5: excluirProduto(); break;
                        case 6: break;
                        default: printf("Opção inválida.\n");
                    }
                    system("pause");
                } while (opcao_logistica != 6);
            }
        } else if (opcao_principal == 3) {
            cadastrar_usuario();
        } else if (opcao_principal != 4) {
            printf("Opção inválida.\n");
        }
        system("pause");
    } while (opcao_principal != 4);

    salvar_produtos();
    salvar_usuarios();
    backup_dados();
    salvar_historico();
    return 0;
}

// Função para registrar compra no histórico
void registrar_compra(const char *codigo_reembolso, float total) {
    if (total_clientes < MAX_CLIENTES) {
        strcpy(clientes[total_clientes].codigo_reembolso, codigo_reembolso);
        clientes[total_clientes].total = total;
        clientes[total_clientes].timestamp = time(NULL);
        total_clientes++;
        salvar_produtos();
        printf("Compra registrada com sucesso.\n");
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Função para salvar histórico de compras
void salvar_historico() {
    FILE *arquivo = fopen(ARQUIVO_HISTORICO, "w");
    if (!arquivo) {
        printf("Erro ao salvar o histórico de compras.\n");
        return;
    }
    for (int i = 0; i < total_historico; i++) {
        fprintf(arquivo, "%s,%s,%d,%.2f,%.2f,%ld\n", historico_compras[i].codigo_produto, historico_compras[i].nome_produto, 
                historico_compras[i].quantidade, historico_compras[i].preco_unitario, historico_compras[i].preco_total, 
                historico_compras[i].timestamp);
    }
    fclose(arquivo);
}

// Função para carregar histórico de compras
void carregar_historico() {
    FILE *arquivo = fopen(ARQUIVO_HISTORICO, "r");
    if (!arquivo) {
        printf("Nenhum histórico de compras encontrado.\n");
        return;
    }
    while (fscanf(arquivo, "%[^,],%[^,],%d,%f,%f,%ld\n", historico_compras[total_historico].codigo_produto, 
                  historico_compras[total_historico].nome_produto, &historico_compras[total_historico].quantidade, 
                  &historico_compras[total_historico].preco_unitario, &historico_compras[total_historico].preco_total, 
                  &historico_compras[total_historico].timestamp) != EOF) {
        total_historico++;
    }
    fclose(arquivo);
}

// Funções de gerenciamento de produtos
void carregar_produtos() {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de estoque.\n");
        return;
    }
    while (fscanf(arquivo, "%[^,],%[^,],%f,%d\n", produtos[total_produtos].codigo, produtos[total_produtos].nome, &produtos[total_produtos].preco, &produtos[total_produtos].quantidade) != EOF) {
        total_produtos++;
    }
    fclose(arquivo);
}

void salvar_produtos() {
    FILE *arquivo = fopen(ARQUIVO_ESTOQUE, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    for (int i = 0; i < total_produtos; i++) {
        fprintf(arquivo, "%s,%s,%.2f,%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
    fclose(arquivo);
}

void cadastrarProduto() {
    if (total_produtos < MAX_PRODUTOS) {
        printf("Nome do produto: ");
        fgets(produtos[total_produtos].nome, sizeof(produtos[total_produtos].nome), stdin);
        produtos[total_produtos].nome[strcspn(produtos[total_produtos].nome, "\n")] = 0;

        printf("Preço: ");
        scanf("%f", &produtos[total_produtos].preco);
        limparBuffer();

        printf("Quantidade: ");
        scanf("%d", &produtos[total_produtos].quantidade);
        limparBuffer();

        gerar_codigo_produto(produtos[total_produtos].codigo, total_produtos);
        
        total_produtos++;
        printf("Produto cadastrado! Código: %s\n", produtos[total_produtos - 1].codigo);
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

void gerar_codigo_produto(char *codigo, int index) {
    snprintf(codigo, 20, "PROD-%03d", index + 1); // Gera um código no formato PROD-001, PROD-002, etc.
}

void listarProdutos() {
    printf("Produtos cadastrados:\n");
    for (int i = 0; i < total_produtos; i++) {
        printf("%d - %s (Código: %s): R$%.2f (Quantidade: %d)\n", i + 1, produtos[i].nome, produtos[i].codigo, produtos[i].preco, produtos[i].quantidade);
    }
}

void atualizarEstoque() {
    char nome[50];
    int quantidade;

    printf("Nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
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
    printf("Produtos com estoque baixo:\n");
    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade < 5) {
            printf("%s (Código: %s): %d unidades\n", produtos[i].nome, produtos[i].codigo, produtos[i].quantidade);
        }
    }
}

void excluirProduto() {
    char nome[50];
    printf("Nome do produto a excluir: ");
    fgets(nome, sizeof(nome), stdin);
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
        if (opcao > total_produtos || opcao < 1) {
            printf("Produto inválido!\n");
            continue;
        }
        printf("Quantidade: ");
        scanf("%d", &quantidade);
        if (quantidade > produtos[opcao - 1].quantidade) {
            printf("Quantidade disponível: %d\n", produtos[opcao - 1].quantidade);
            continue;
        }
        total_compra += produtos[opcao - 1].preco * quantidade;
        produtos[opcao - 1].quantidade -= quantidade;
    }

    if (total_compra > 0) {
        gerar_codigo_reembolso(codigo_reembolso);
        registrar_compra(codigo_reembolso, total_compra);
        printf("Compra realizada! Código de reembolso: %s\n", codigo_reembolso);
    } else {
        printf("Nenhuma compra realizada.\n");
    }
}

void gerar_codigo_reembolso(char *codigo) {
    time_t agora;
    struct tm *info;
    time(&agora);
    info = localtime(&agora);
    snprintf(codigo, 20, "REEMB-%02d%02d%02d%02d", info->tm_year % 100, info->tm_mon + 1, info->tm_mday, total_clientes + 1);
}

void verificar_reembolso() {
    char codigo_reembolso[20];
    printf("Código de reembolso: ");
    scanf("%s", codigo_reembolso);

    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].codigo_reembolso, codigo_reembolso) == 0) {
            if (verificar_reembolso_valido(clientes[i].timestamp)) {
                printf("Reembolso de R$%.2f aprovado!\n", clientes[i].total);
            } else {
                printf("Reembolso expirado!\n");
            }
            return;
        }
    }
    printf("Código de reembolso não encontrado.\n");
}

int verificar_reembolso_valido(time_t timestamp) {
    time_t agora = time(NULL);
    return difftime(agora, timestamp) <= VALIDADE_REEMBOLSO;
}

// Funções de gerenciamento de usuários
void carregar_usuarios() {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    if (!arquivo) return;

    while (fscanf(arquivo, "%[^,],%[^,],%d\n", usuarios[total_usuarios].username, usuarios[total_usuarios].password, &usuarios[total_usuarios].tipo) != EOF) {
        total_usuarios++;
    }
    fclose(arquivo);
}

void salvar_usuarios() {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "w");
    for (int i = 0; i < total_usuarios; i++) {
        fprintf(arquivo, "%s,%s,%d\n", usuarios[i].username, usuarios[i].password, usuarios[i].tipo);
    }
    fclose(arquivo);
}

void cadastrar_usuario() {
    if (total_usuarios < MAX_USUARIOS) {
        printf("Nome de usuário: ");
        scanf("%s", usuarios[total_usuarios].username);
        printf("Senha: ");
        scanf("%s", usuarios[total_usuarios].password);
        printf("Tipo (1: Cliente, 2: Logística): ");
        scanf("%d", &usuarios[total_usuarios].tipo);
        total_usuarios++;
        salvar_usuarios();
    } else {
        printf("Limite de usuários atingido.\n");
    }
}

int login_usuario() {
    char username[20], password[20];
    printf("Nome de usuário: ");
    scanf("%s", username);
    printf("Senha: ");
    scanf("%s", password);

    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) {
            return usuarios[i].tipo;
        }
    }
    printf("Login inválido.\n");
    return 0;
}

// Funções de exibição de menus e utilitários
void exibirMenuPrincipal() {
    printf("Menu Principal:\n");
    printf("1. Compras\n");
    printf("2. Logística\n");
    printf("3. Cadastrar Usuário\n");
    printf("4. Sair\n");
}

void exibirMenuCompras() {
    printf("Menu de Compras:\n");
    printf("1. Realizar Compra\n");
    printf("2. Verificar Reembolso\n");
    printf("3. Voltar\n");
}

void exibirMenuLogistica() {
    printf("Menu de Logística:\n");
    printf("1. Cadastrar Produto\n");
    printf("2. Listar Produtos\n");
    printf("3. Atualizar Estoque\n");
    printf("4. Relatório de Estoque Baixo\n");
    printf("5. Excluir Produto\n");
    printf("6. Voltar\n");
}

void limparBuffer() {
    while (getchar() != '\n');
}

// Função para backup dos dados
void backup_dados() {
    FILE *arquivo_produtos = fopen(ARQUIVO_BACKUP_ESTOQUE, "w");
    FILE *arquivo_usuarios = fopen(ARQUIVO_BACKUP_USUARIOS, "w");
    if (!arquivo_produtos || !arquivo_usuarios) {
        printf("Erro ao criar arquivos de backup.\n");
        return;
    }

    for (int i = 0; i < total_produtos; i++) {
        fprintf(arquivo_produtos, "%s,%s,%.2f,%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
    for (int i = 0; i < total_usuarios; i++) {
        fprintf(arquivo_usuarios, "%s,%s,%d\n", usuarios[i].username, usuarios[i].password, usuarios[i].tipo);
    }

    fclose(arquivo_produtos);
    fclose(arquivo_usuarios);
    printf("Backup realizado com sucesso!\n");
}
