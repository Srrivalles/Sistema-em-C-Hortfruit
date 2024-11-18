#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

#define MAX_REEMBOLSOS 5
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
    char produtos[5][50]; // Para armazenar os últimos 5 produtos relacionados ao reembolso
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

char historico_reembolsos[MAX_REEMBOLSOS][20]; 
int total_reembolsos = 0;

// Funções de gerenciamento de produtos
void cadastrarProduto();
void listarProdutos();
void atualizarEstoque();
void relatorioEstoqueBaixo();
void alterarProduto();
void carregarProdutos();
void salvarProdutos();

// Funções de compras e reembolso
void realizarCompra();
void gerarCodigoReembolso(char *codigo);
void registrarCompra(const char *codigo_reembolso, float total);
void verificarReembolso();
int verificarReembolsoValido(time_t timestamp);
void adicionarReembolsoAoHistorico(const char* codigo_reembolso);
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
void LP();
void PA();


//fazendo o código ficar portável para linux e macOS sem "cls' e "pause"

void LP(){
	//system("clear");   // se quiser usar no linux ou mac, remova as barras.
	 system("cls");       
}
void PA(){
	printf("\nPressione 'ENTER' para continuar...");
	getchar();
}
// Implementações das funções
void cadastrarProduto() {
    if (total_produtos < MAX_PRODUTOS) {
        struct Produto novo_produto;

        printf("\nDigite o nome do produto (Ou digite '0' para Cancelar): ");
        fgets(novo_produto.nome, sizeof(novo_produto.nome), stdin);
        novo_produto.nome[strcspn(novo_produto.nome, "\n")] = 0; // Remove o '\n'

        // Converte o nome do produto para maiúsculas
        for (int i = 0; novo_produto.nome[i] != '\0'; i++) {
            novo_produto.nome[i] = toupper((unsigned char)novo_produto.nome[i]);
        }

        // Verifica se o usuário deseja cancelar
        if (strcmp(novo_produto.nome, "0") == 0) {
            LP();
            return; // Cancela e volta ao menu anterior
        }

        printf("Digite o código do produto: ");
        fgets(novo_produto.codigo, sizeof(novo_produto.codigo), stdin);
        novo_produto.codigo[strcspn(novo_produto.codigo, "\n")] = 0; // Remove o '\n'

        // Converte o código do produto para maiúsculas
        for (int i = 0; novo_produto.codigo[i] != '\0'; i++) {
            novo_produto.codigo[i] = toupper((unsigned char)novo_produto.codigo[i]);
        }

        printf("Digite o preço do produto: ");
        scanf("%f", &novo_produto.preco);
        limparBuffer();

        printf("Digite a quantidade do produto: ");
        scanf("%d", &novo_produto.quantidade);
        limparBuffer();

        printf("Este produto é vendido por peso? (1 para sim, 0 para não): ");
        scanf("%d", &novo_produto.vendido_por_peso);
        limparBuffer();

        // Armazena o novo produto e incrementa o contador de produtos
        produtos[total_produtos] = novo_produto;
        total_produtos++;
        salvarProdutos(); // Salva após cadastrar
        LP();
        printf("\nPRODUTO CADASTRADO COM SUCESSO!\n");
    } else {
        printf("Limite de produtos atingido.\n");
    }
}


void listarProdutos() {
	LP();
    int i;
    printf("\nProdutos Disponiveis: \n");
    for (i = 0; i < total_produtos; i++) {
        printf("------------------------------------------------------------\n");
		printf(" Nome do Produto:  %s\n", produtos[i].nome);
		printf(" Código:   %s\n", produtos[i].codigo);
		printf(" Preço:    R$ %.2f\n", produtos[i].preco);
		printf(" Unidades:  %d %s\n", produtos[i].quantidade,
		       produtos[i].vendido_por_peso ? "(Vendido em KG)" : "(Unidades)");


    }
}

void atualizarEstoque() {
	LP();
    char nome[50];
    int quantidade;
    listarProdutos();

    printf("\n\nDigite o nome do produto para atualizar o estoque: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;
    
    int i;
    for (i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
        	system("cls");
            printf("\n\n\n\n\n\n\nQuantidade Atual: %d\n", produtos[i].quantidade);
            printf("\nDigite a Nova Quantidade de Produtos: ");
            scanf("%d", &quantidade);
            limparBuffer();
            produtos[i].quantidade = quantidade;
            LP();
            printf("\n\n\n\n\n\n\Estoque Atualizado com Sucesso para %s!\n", produtos[i].nome);
            PA();
            LP();
            salvarProdutos(); // Salva após atualizar
            return;
        }
    }
    LP();
    printf("\n\n\n\n\nProduto não encontrado.\n");
    PA();
    LP();
    
}

void relatorioEstoqueBaixo() {
    LP();
    int i;
    printf("Produtos com estoque baixo (Menor que 10 unidades): \n");
    for (i = 0; i < total_produtos; i++) {
        if (produtos[i].quantidade < 10) {
    printf("%s  ||  Código: %s  ||  Unidades Disponíveis: %d\n", produtos[i].nome, produtos[i].codigo, produtos[i].quantidade);


        }
    }
}


void alterarProduto() {
    LP();
    char codigo[20];
    listarProdutos();

    printf("\n\nCódigo do Produto a Ser Alterado: ");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < total_produtos; i++) {
        if (strcmp(produtos[i].codigo, codigo) == 0) {
            encontrado = 1;

            printf("\nProduto encontrado: %s\n\n", produtos[i].nome);
            printf("Insira os novos dados do produto:\n");

            printf("Nome do Produto: ");
            fgets(produtos[i].nome, sizeof(produtos[i].nome), stdin);
            produtos[i].nome[strcspn(produtos[i].nome, "\n")] = 0;

            printf("Preço do Produto: R$ ");
            scanf("%f", &produtos[i].preco);
            limparBuffer();

            printf("Quantidade em Estoque: ");
            scanf("%d", &produtos[i].quantidade);
            limparBuffer();

            printf("\nProduto é vendido por peso? (1 para Sim, 0 para Não): ");
            scanf("%d", &produtos[i].vendido_por_peso);
            limparBuffer();

            LP();
            printf("\nProduto alterado com sucesso!\n");
            salvarProdutos(); // Salva as alterações feitas no produto
            return;
        }
    }

    if (!encontrado) {
        LP();
        printf("\nProduto não encontrado.\n");
    }
}


void finalizarCompra() {
    LP();
    if (total_carrinho == 0) {
        printf("\nNenhum produto no carrinho para finalizar.\n");
        return; // Retorna se não há produtos no carrinho
    }

    float total = 0.0f;
    printf("                   Resumo da Compra:\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < total_carrinho; i++) {
        if (carrinho[i].peso > 0) {
            printf(" Produto: %s\n", carrinho[i].produto.nome);
            printf(" Peso: %.2f kg\n", carrinho[i].peso);
            printf(" Preço: R$ %.2f\n\n", carrinho[i].produto.preco * carrinho[i].peso);
            total += carrinho[i].produto.preco * carrinho[i].peso;
        } else {
            printf(" Produto: %s\n", carrinho[i].produto.nome);
            printf(" Quantidade: %d\n", carrinho[i].quantidade);
            printf(" Preço: R$ %.2f\n", carrinho[i].produto.preco * carrinho[i].quantidade);
            total += carrinho[i].produto.preco * carrinho[i].quantidade;
        }
    }
    printf(" Total da compra: R$ %.2f\n", total);
    printf("----------------------------------------------------------\n");
    printf("\nCompra finalizada com sucesso! Obrigado pela sua compra!\n");
}

void adicionarAoCarrinho() {
    LP();
    char codigo[20];
    int quantidade;

    while (1) {
        listarProdutos(); // Exibe os produtos disponíveis

        printf("\nCódigo do produto para adicionar ao carrinho (ou 0 para finalizar): ");
        fgets(codigo, sizeof(codigo), stdin);
        codigo[strcspn(codigo, "\n")] = 0;

        if (strcmp(codigo, "0") == 0) {
            LP();
            printf("\nProdutos adicionados ao Carrinho!\n");

            // Finaliza a compra e gera o código de reembolso
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
                gerarCodigoReembolso(codigo_reembolso); // Gera o código de reembolso
                registrarCompra(codigo_reembolso, total); // Registra a compra

                //printf("\nCompra realizada com sucesso! Total: R$%.2f\n", total);
                finalizarCompra();
                printf("\nCódigo de reembolso: %s\n", codigo_reembolso); // Exibe o código de reembolso
                total_carrinho = 0; // Limpa o carrinho após a compra
            } else {
                LP();
                printf("\n\n\n\n\n\n\nCarrinho vazio. Adicione produtos antes de finalizar a compra.\n\n");
                PA();
                LP();
            }
            break; // Finaliza a adição de produtos ao carrinho
        }

        int encontrado = 0;
        for (int i = 0; i < total_produtos; i++) {
            if (strcmp(produtos[i].codigo, codigo) == 0) {
                encontrado = 1;
                LP();
                printf("----------------------------------------------------------\n");
                printf("           Produto encontrado:\n");
                printf("----------------------------------------------------------\n");
                printf(" Nome do Produto: %s\n", produtos[i].nome);
                printf(" Preço:    R$ %.2f\n", produtos[i].preco);
                printf(" Estoque:   %d\n", produtos[i].quantidade);
                printf("----------------------------------------------------------\n");

                // Verifica o estoque e adiciona o produto ao carrinho
                if (produtos[i].quantidade > 0) {
                    printf("\nQuantidade a adicionar ao carrinho (ou 0 para cancelar): ");
                    scanf("%d", &quantidade);
                    limparBuffer();

                    if (quantidade > 0 && quantidade <= produtos[i].quantidade) {
                        carrinho[total_carrinho].produto = produtos[i];
                        carrinho[total_carrinho].quantidade = quantidade;
                        carrinho[total_carrinho].peso = 0;
                        total_carrinho++;
                        produtos[i].quantidade -= quantidade; // Atualiza o estoque
                        printf("\n\nProduto adicionado ao carrinho!\n\n");
                        PA();
                    } else {
                        printf("\n\nQuantidade inválida ou insuficiente.\n\n");
                        PA();
                    }
                } else {
                    LP();
                    printf("\n\nProduto sem estoque disponível.\n");
                    PA();
                }

                salvarProdutos(); // Salva o novo estoque após a adição
                break;
            }
        }

        if (!encontrado) {
        	LP();
            printf("\n\n\n\n\n\n\n\nProduto não encontrado.\n\n");
            PA();
            LP();
        }
    }
}

void realizarCompra() {
    LP();
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
    	LP();
        printf("\nCarrinho vazio. Adicione produtos antes de finalizar a compra.\n\n");
        PA();
    	LP();
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

    // Adiciona os produtos do carrinho ao cliente
    for (int i = 0; i < total_carrinho && i < 5; i++) {
        strcpy(novo_cliente.produtos[i], carrinho[i].produto.nome); // Armazena os nomes dos produtos
    }

    clientes[total_clientes++] = novo_cliente;
    adicionarReembolsoAoHistorico(codigo_reembolso); // Adiciona o código de reembolso ao histórico
}


int verificarReembolsoValido(time_t timestamp) {
    return difftime(time(NULL), timestamp) <= VALIDADE_REEMBOLSO;
}

void adicionarReembolsoAoHistorico(const char* codigo_reembolso) {
    // Desloca os reembolsos para abrir espaço para o novo no final do array
    if (total_reembolsos < MAX_REEMBOLSOS) {
        strcpy(historico_reembolsos[total_reembolsos], codigo_reembolso);
        total_reembolsos++;
    } else {
        for (int i = 1; i < MAX_REEMBOLSOS; i++) {
            strcpy(historico_reembolsos[i - 1], historico_reembolsos[i]);
        }
        strcpy(historico_reembolsos[MAX_REEMBOLSOS - 1], codigo_reembolso);
    }
}

void verificarReembolso() {
    LP();

    // Exibe os últimos cinco códigos de reembolso e produtos associados
    printf("\nÚltimos %d códigos de reembolso e produtos associados:\n", 
           total_reembolsos < MAX_REEMBOLSOS ? total_reembolsos : MAX_REEMBOLSOS);
    for (int i = 0; i < total_reembolsos; i++) {
        printf("- Código: %s\n", historico_reembolsos[i]);
        // Exibe os produtos relacionados a esse código de reembolso
        for (int j = 0; j < total_clientes; j++) {
            if (strcmp(clientes[j].codigo_reembolso, historico_reembolsos[i]) == 0) {
                printf("  Produtos: ");
                for (int k = 0; k < 5 && strlen(clientes[j].produtos[k]) > 0; k++) {
                    printf("%s ", clientes[j].produtos[k]);
                }
                printf("\n");
                break;
            }
        }
    }

    // Solicita o código de reembolso ao usuário
    printf("\nDigite o código de reembolso: ");
    char codigo[20];
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = 0;

    // Procura pelo código nos clientes registrados
    int encontrado = 0;
    for (int i = 0; i < total_clientes; i++) {
        if (strcmp(clientes[i].codigo_reembolso, codigo) == 0) {
            encontrado = 1;

            if (clientes[i].utilizado) {
                printf("\nEste código de reembolso já foi utilizado.\n");
                PA();
                LP();
                return;
            }

            if (!verificarReembolsoValido(clientes[i].timestamp)) {
                printf("\nO código de reembolso expirou.\n");
                PA();
                LP();
                return;
            }

            printf("\nReembolso válido!\n");
            clientes[i].utilizado = 1; // Marca o código como utilizado
            PA();
            LP();
            return;
        }
    }

    if (!encontrado) {
        printf("\nCódigo de reembolso inválido.\n");
        PA();
        LP();
    }
}


// Funções de login e gerenciamento de usuários
void exibirMenuPrincipal() {
    LP();
    int opcao;
    while (1) {
        printf("\n\n\n\n\n\n ________________________\n");
        printf("|     Menu Principal:    |\n");
        printf("|------------------------|\n");
        printf("| 1. Login               |\n");
        printf("| 2. Cadastrar Usuário   |\n");
        printf("| 0. Sair                |\n");
        printf("|------------------------|\n");
        printf("| Escolha uma opção:     | ");
        printf("             \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        scanf("%d", &opcao);
        printf("|________________________|\n");
        limparBuffer();

        switch (opcao) {
            case 1:
                if (verificarLogin()) {
                    printf("\n\nLogado com sucesso!\n"); 
                    PA();
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
                LP();
                printf("\nOpção inválida.\n\n");
                PA();
                LP();
        }
    }
}


void exibirMenuEscolha() {
    int opcao;
    LP();
    
    while (1) {
    	printf("\n\n\n\n\n\n __________________________\n");
        printf("|     Menu do Caixa:       |\n");
        printf("|--------------------------|\n");
        printf("| 1. Adicionar ao Carrinho |\n");
        printf("| 2. Gerenciar Produtos    |\n");
        printf("| 3. Verificar Reembolso   |\n");
        printf("| 0. Sair                  |\n");
        printf("|--------------------------|\n");
        printf("| Escolha uma opção:       | ");
        printf("             \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        scanf("%d", &opcao);
        printf("|__________________________|\n");
        limparBuffer();
        
        switch (opcao) {
            case 1:
                adicionarAoCarrinho();
                break;
            case 2:
                exibirMenuLogistica();
                break;
            case 3:
                 verificarReembolso();
                break;
            case 0:
                LP();
                return;
            default:
                LP();
                printf("Opção inválida.\n\n");
                PA();
    			LP();
        }
    }
}

void exibirMenuLogistica() {
    int opcao;
	LP();
    while (1) {
    	printf("\n\n\n\n\n\n _______________________________\n");
    	printf("|       Menu de Logística       |\n");
    	printf("|-------------------------------|\n");
        printf("| 1. Cadastrar Produto      	|\n");
        printf("| 2. Listar Produtos        	|\n");
        printf("| 3. Atualizar Estoque      	|\n");
        printf("| 4. Relatório de Estoque Baixo |\n");
        printf("| 5. Alterar Produto         	|\n");
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
                alterarProduto();
                break;
            case 0:
                LP();
                return;
            default:
                LP();
                printf("Opção inválida.\n\n");
                PA();
				LP();
        }
    }
}

void limparBuffer() {
    while (getchar() != '\n');
}

int verificarLogin() {
	LP();
    char username[20], password[20];
    printf("\n\nLogando...\n");
    printf("\n\n\n\nNome de usúario: ");
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
    LP();
    printf("\n\n\n\n\n\n\n\nLogin Falhou.\n");
    PA();
    LP();
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

      
        LP();
        usuarios[total_usuarios++] = novo_usuario;
        salvarUsuarios(); // Salva após cadastrar
    } else {
        printf("Limite de usuários atingido.\n");
    }
    LP();
    printf("\n\n\n\n\n\n\n\n\n\nCADASTRADO COM SUCESSO!\n\n");
    PA();
    LP();
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
        int i, j;
        for (i = 0; i < total_produtos; i++) {
            for (j = 0; produtos[i].nome[j] != '\0'; j++) {
                produtos[i].nome[j] = toupper((unsigned char)produtos[i].nome[j]);
            }

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
    
    setlocale(LC_ALL, "Portuguese");
    carregarProdutos();
    carregarUsuarios();
    exibirMenuPrincipal();
    return 0;
}
