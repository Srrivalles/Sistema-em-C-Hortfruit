#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>

#define MAX_PRODUTOS 100
#define MAX_CLIENTES 100
#define MAX_USUARIOS 50
#define MAX_CARRINHO 50
#define VALIDADE_REEMBOLSO 1800 // 30 min em segundos
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"

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
    int utilizado; // 0: não utilizado, 1: utilizado
};

struct Usuario {
    char username[20];
    char password[20];
    int tipo; // 1: Caixa, 2: Logística
};

struct ItemCarrinho {
    struct Produto produto;
    int quantidade; // Para produtos vendidos por unidade
    float peso;     // Para produtos vendidos por peso
};

struct ItemCarrinho carrinho[MAX_CARRINHO];
int total_carrinho = 0;

struct Produto produtos[MAX_PRODUTOS];
struct Cliente clientes[MAX_CLIENTES];
struct Usuario usuarios[MAX_USUARIOS];

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

        printf("Digite o nome do produto (Ou digite '0' para Cancelar): ");
        fgets(novo_produto.nome, sizeof(novo_produto.nome), stdin);
        novo_produto.nome[strcspn(novo_produto.nome, "\n")] = 0;
        if (strcmp(novo_produto.nome, "0") == 0) {
    	system("cls");
    	return; // Cancela e volta ao menu anterior
}


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
        salvarProdutos();  // Salva após cadastrar
        system("cls");
        printf("\nPRODUTO CADASTRADO COM SUCESSO!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}


void listarProdutos() {
	system("cls");
    int i;
    printf("\nProdutos Disponiveis: \n");
    for (i = 0; i < total_produtos; i++) {
        printf("------------------------------------------------------------\n");
		printf(" Nome do Produto:  %s\n", produtos[i].nome);
		printf(" Código:   %s\n", produtos[i].codigo);
		printf(" Preço:    R$ %.2f\n", produtos[i].preco);
		printf(" Unidades:  %d %s\n", produtos[i].quantidade,
		       produtos[i].vendido_por_peso ? "(Vendido em KG)" : "(Unidades)");
		printf("------------------------------------------------------------");


    }
}

void atualizarEstoque() {
	system("cls");
    char nome[50];
    int quantidade;
    listarProdutos();

    printf("\nDigite o nome do produto para atualizar o estoque: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int i;
    for (i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
        	system("cls");
            printf("\nQuantidade Atual: %d\n", produtos[i].quantidade);
            printf("\nDigite a Nova Quantidade de Produtos: ");
            scanf("%d", &quantidade);
            limparBuffer();
            produtos[i].quantidade = quantidade;
            system("cls");
            printf("\nEstoque Atualizado com Sucesso para %s!\n", produtos[i].nome);
            salvarProdutos(); // Salva após atualizar
            return;
        }
    }
    printf("\nProduto não encontrado.\n");
}

void relatorioEstoqueBaixo() {
    system("cls");
    int i;
    printf("\n\nProdutos com estoque baixo (Menor que 10 unidades): \n");
    for (i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade < 10) {
            printf("%s  ||  Código: %s  ||  Unidades Disponíveis: %d %s\n", produtos[i].nome, produtos[i].codigo, produtos[i].quantidade);
        }
    }
}


void excluirProduto() {
	system("cls");
    char codigo[20];
    listarProdutos();

    printf("\n\n\n\nCódigo do Produto a Ser Excluído do Estoque: ");
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
            system("cls");
            printf("\nProduto Excluído Com Sucesso!\n");
            salvarProdutos(); // Salva após excluir
            return;
        }
    }
    system("cls");
    printf("\nProduto não encontrado.\n");
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
    system("cls");
    char codigo[20];
    int quantidade;
    while (1) {
        listarProdutos(); // Exibe os produtos disponíveis

        printf("\nCódigo do produto para adicionar ao carrinho (ou 0 para finalizar): ");
        fgets(codigo, sizeof(codigo), stdin);
        codigo[strcspn(codigo, "\n")] = 0;

        if (strcmp(codigo, "0") == 0) {
            system("cls");
            printf("\nProdutos adicionados ao Carrinho!\n");
            break; // Finaliza a adição de produtos ao carrinho
        }

        int encontrado = 0;
        for (int i = 0; i < total_produtos; i++) {
            if (strcmp(produtos[i].codigo, codigo) == 0) {
                encontrado = 1;
                printf("----------------------------------------------------------\n");
                printf("           Produto encontrado:\n");
                printf("----------------------------------------------------------\n");
                printf(" Nome do Produto: %s\n", produtos[i].nome);
                printf(" Preço:    R$ %.2f\n", produtos[i].preco);
                printf(" Estoque:   %d\n", produtos[i].quantidade);
                printf("----------------------------------------------------------\n");

                // Verifica se o estoque do produto é maior que zero
                if (produtos[i].quantidade <= 0) {
                	system("cls");
                    printf("\nProduto sem estoque disponível.\n");
                    return; // Retorna ao menu sem adicionar o produto
                }

                // Verifica se o produto é vendido por peso
                if (produtos[i].vendido_por_peso) {
                    float peso = balancaVirtual();
                    if (peso > 0) {
                        if (peso <= produtos[i].quantidade) {
                            carrinho[total_carrinho].produto = produtos[i];
                            carrinho[total_carrinho].peso = peso;
                            carrinho[total_carrinho].quantidade = 0; // Não aplica quantidade
                            total_carrinho++;
                            produtos[i].quantidade -= (int)peso; // Atualiza o estoque
                            printf("\nProduto adicionado ao carrinho!\n");
                        } else {
                            printf("\nPeso excede o estoque disponível.\n");
                        }
                    }
                } else {
                    printf("\nQuantidade a adicionar ao carrinho (ou 0 para cancelar): ");
                    scanf("%d", &quantidade);
                    limparBuffer();

                    if (quantidade > 0 && quantidade <= produtos[i].quantidade) {
                        carrinho[total_carrinho].produto = produtos[i];
                        carrinho[total_carrinho].quantidade = quantidade;
                        carrinho[total_carrinho].peso = 0; // Não aplica peso
                        total_carrinho++;
                        produtos[i].quantidade -= quantidade; // Atualiza o estoque
                        printf("\nProduto adicionado ao carrinho!\n");
                    } else {
                        printf("\nQuantidade inválida ou insuficiente.\n");
                    }
                }
                salvarProdutos(); // Salva o novo estoque após a adição
                break;
            }
        }

        if (!encontrado) {
            printf("\nProduto não Encontrado.\n\n");
            system("pause");
    		system("cls");
        }
    }
}

void realizarCompra() {
    system("cls");
    float total = 0.0;

    for (int i = 0; i < total_carrinho; i++) {
        if (carrinho[i].peso > 0) {
            total += carrinho[i].produto.preco * carrinho[i].peso;
        } else {
            total += carrinho[i].produto.preco * carrinho[i].quantidade;
        }
    }

    if (total > 0) {
        char codigo_reembolso[20];
        gerarCodigoReembolso(codigo_reembolso);
        registrarCompra(codigo_reembolso, total);
        printf("\nCompra realizada com sucesso! Total: R$%.2f\n", total);
        printf("\nCódigo de reembolso: %s\n", codigo_reembolso);
        total_carrinho = 0; // Limpa o carrinho após a compra
    } else {
    	system("cls");
        printf("\nCarrinho vazio. Adicione produtos antes de finalizar a compra.\n\n");
        system("pause");
    	system("cls");
    }
}

void gerarCodigoReembolso(char *codigo) {
    sprintf(codigo, "COD-%03d", total_clientes + 1);
}

void registrarCompra(const char *codigo_reembolso, float total) {
    struct Cliente novo_cliente;
    strcpy(novo_cliente.codigo_reembolso, codigo_reembolso);
    novo_cliente.total = total;
    novo_cliente.timestamp = time(NULL);
    novo_cliente.utilizado = 0; // Inicializa como não utilizado
    clientes[total_clientes++] = novo_cliente;
}


int verificarReembolsoValido(time_t timestamp) {
    return difftime(time(NULL), timestamp) <= VALIDADE_REEMBOLSO;
}

void verificarReembolso() {
    char codigo[20];
    printf("\nDigite o código de reembolso: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    int i;
    for (i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].codigo_reembolso, codigo) == 0) {
            if (clientes[i].utilizado) {
            	system("cls");
                printf("\nEste código de reembolso já foi utilizado.\n");
                system("pause");
    			system("cls");
                return;
            }
            if (verificarReembolsoValido(clientes[i].timestamp)) {
                system("cls");
                printf("\nReembolso Válido: R$%.2f\n", clientes[i].total);
                // Restaurar a quantidade de produtos
                float valor_reembolsado = clientes[i].total;
                for (int j = 0; j < total_produtos; j++) {
                    if (valor_reembolsado >= produtos[j].preco) {
                        int quantidade_reembolsada = (int)(valor_reembolsado / produtos[j].preco);
                        produtos[j].quantidade += quantidade_reembolsada;
                        valor_reembolsado -= quantidade_reembolsada * produtos[j].preco;
                    }
                }
                // Salvar o estoque atualizado após o reembolso
                salvarProdutos();
                // Marcar o reembolso como utilizado
                clientes[i].utilizado = 1;
                printf("\nEstoque restaurado após o reembolso.\n");
            } else {
                printf("\nReembolso inválido ou expirado.\n");
            }
            return;
        }
    }
    system("cls");
    printf("\n\n\n\nCódigo de reembolso não encontrado.\n");
    system("pause");
    system("cls");
}


// Funções de login e gerenciamento de usuários
void exibirMenuPrincipal() {
    int opcao;
    char I;
    system("cls");
    while (1) {
        printf("\n\n\n\n\n\n\n\n\n ________________________\n");
        printf("| Menu Principal:        |\n");
        printf("|------------------------|\n");
        printf("| 1. Login               |\n");
        printf("| 2. Cadastrar Usuário  |\n");
        printf("| 0. Sair                |\n");
        printf("|------------------------|\n");
        printf("| Escolha uma opção:   | ");
        printf("             \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        scanf("%d", &opcao);
        printf("|________________________|\n");
        limparBuffer();

        switch (opcao) {
            case 1:
                if (verificarLogin()) {
                    exibirMenuEscolha();
                }
                break;
            case 2:
                cadastrarUsuario();
                break;
            case 0:
                printf("\n\nObrigado por usar nosso Sistema!");
                exit(0);
            default:
                system("cls");
                printf("\nOpção inválida.\n\n");
                system("pause");
    			system("cls");
        }
    }
}

void exibirMenuEscolha() {
    int opcao;
    system("cls");
    printf("\n ________________________\n");
    printf("   LOGADO COM SUCESSO!    \n");
    while (1) {
    	printf("\n\n\n\n\n\n\n\n\n __________________________\n");
        printf("| 1. Adicionar ao Carrinho |\n");
        printf("| 2. Finalizar Compra      |\n");
        printf("| 3. Gerenciar Produtos    |\n");
        printf("| 4. Verificar Reembolso   |\n");
        printf("| 0. Sair                  |\n");
        printf("|--------------------------|\n");
        printf("| Escolha uma opção:     | ");
        printf("             \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        scanf("%d", &opcao);
        printf("|_______________________|\n");
        limparBuffer();
        
        switch (opcao) {
            case 1:
                adicionarAoCarrinho();
                break;
            case 2:
                realizarCompra();
                break;
            case 3:
                exibirMenuLogistica();
                break;
            case 4:
                verificarReembolso();
                break;
            case 0:
                system("cls");
                return;
            default:
                system("cls");
                printf("Opção inválida.\n\n");
                system("pause");
    			system("cls");
        }
    }
}

void exibirMenuLogistica() {
    int opcao;
    system("cls");
    while (1) {
    	printf("\n\n\n\n\n\n\n\n\n _______________________________\n");
    	printf("|     Menu de Logística        |\n");
    	printf("|-------------------------------|\n");
        printf("| 1. Cadastrar Produto      	|\n");
        printf("| 2. Listar Produtos        	|\n");
        printf("| 3. Atualizar Estoque      	|\n");
        printf("| 4. Relatório de Estoque Baixo|\n");
        printf("| 5. Excluir Produto        	|\n");
        printf("| 0. Voltar                 	|\n");
        printf("|-------------------------------|\n");
        printf("| Escolha uma opção:        	| ");
        printf("             \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        scanf("%d", &opcao);
        printf("|_______________________________|\n");

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
            case 0:
                system("cls");
                return;
            default:
                system("cls");
                printf("Opção inválida.\n\n");
                system("pause");
    			system("cls");
        }
    }
}

void limparBuffer() {
    while (getchar() != '\n');
}

int verificarLogin() {
	system("cls");
    char username[20], password[20];
    printf("\nLogando...\n");
    printf("\nNome de usúario: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("\nDigite a senha: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    int i;
    for (i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) {
            return 1; // Login bem-sucedido
        }
    }
    system("cls");
    printf("\n\n\n\n\nLogin Falhou.\n");
    system("pause");
    system("cls");
    return 0; // Login falhou
}

void cadastrarUsuario() {
    if (total_usuarios < MAX_USUARIOS) {
        struct Usuario novo_usuario;
        printf("Digite o usúario: ");
        fgets(novo_usuario.username, sizeof(novo_usuario.username), stdin);
        novo_usuario.username[strcspn(novo_usuario.username, "\n")] = 0;

        printf("Digite a senha: ");
        fgets(novo_usuario.password, sizeof(novo_usuario.password), stdin);
        novo_usuario.password[strcspn(novo_usuario.password, "\n")] = 0;

        printf("\nTipo de usuário (1: Caixa, 2: Logística): ");
        scanf("%d", &novo_usuario.tipo);
        limparBuffer();
        system("cls");
        usuarios[total_usuarios++] = novo_usuario;
        salvarUsuarios(); // Salva após cadastrar
    } else {
        printf("Limite de usuários atingido.\n");
    }
    system("cls");
    printf("\nCADASTRADO COM SUCESSO!\n\n");
    system("pause");
    system("cls");
}

void carregarProdutos() {
    FILE *file = fopen(ARQUIVO_PRODUTOS, "r");
    if (file) {
        while (fscanf(file, "%[^;];%[^;];%f;%d;%d\n", produtos[total_produtos].nome,
                      produtos[total_produtos].codigo,
                      &produtos[total_produtos].preco,
                      &produtos[total_produtos].quantidade,
                      &produtos[total_produtos].vendido_por_peso) == 5) {
            total_produtos++;
        }
        fclose(file);
    }
}

void salvarProdutos() {
    FILE *file = fopen(ARQUIVO_PRODUTOS, "w");
    if (file) {
        int i;
        for (i = 0; i < total_produtos; i++) {
            fprintf(file, "%s;%s;%.2f;%d;%d\n", produtos[i].nome,
                    produtos[i].codigo,
                    produtos[i].preco,
                    produtos[i].quantidade,
                    produtos[i].vendido_por_peso);
        }
        fclose(file);
    }
}

void carregarUsuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "r");
    if (file) {
        while (fscanf(file, "%[^;];%[^;];%d\n", usuarios[total_usuarios].username,
                      usuarios[total_usuarios].password,
                      &usuarios[total_usuarios].tipo) == 3) {
            total_usuarios++;
        }
        fclose(file);
    }
}

void salvarUsuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "w");
    if (file) {
        int i;
        for (i = 0; i < total_usuarios; i++) {
            fprintf(file, "%s;%s;%d\n", usuarios[i].username,
                    usuarios[i].password,
                    usuarios[i].tipo);
        }
        fclose(file);
    }
}

int main() {
    system("color E4");
    setlocale(LC_ALL, "Portuguese");
    carregarProdutos();
    carregarUsuarios();
    exibirMenuPrincipal();
    return 0;
}
