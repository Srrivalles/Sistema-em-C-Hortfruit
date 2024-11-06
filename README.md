# Sistema de Gestão de Vendas e Reembolsos

Este sistema foi desenvolvido para gerenciar vendas, reembolsos e o controle de produtos em um ambiente comercial. Com uma interface simples, ele permite que usuários registrem compras, verifiquem reembolsos e gerenciem produtos e clientes de forma eficiente.

## Funcionalidades

### Gerenciamento de Produtos
- **Cadastrar Produto**: Permite a inclusão de novos produtos no sistema, especificando nome, código, preço e quantidade.
- **Listar Produtos**: Visualiza todos os produtos cadastrados.
- **Atualizar Estoque**: Modifica a quantidade disponível de um produto.
- **Relatório de Estoque Baixo**: Gera um relatório dos produtos com estoque abaixo de um limite pré-definido.
- **Alterar Produto**: Atualiza as informações de um produto já cadastrado.
- **Carregar e Salvar Produtos**: Funções para carregar produtos de um arquivo e salvar alterações.

### Compras e Reembolsos
- **Realizar Compra**: Permite que o usuário registre uma compra, adicionando produtos ao carrinho.
- **Gerar Código de Reembolso**: Cria um código único para cada reembolso solicitado.
- **Registrar Compra**: Armazena informações sobre a compra realizada.
- **Verificar Reembolso**: Valida se um código de reembolso é válido e se ainda pode ser utilizado.
- **Verificar Validade do Reembolso**: Garante que o reembolso esteja dentro do período de validade.
- **Adicionar Reembolso ao Histórico**: Registra o código de reembolso utilizado.

### Login e Gerenciamento de Usuários
- **Login**: Autentica usuários no sistema, permitindo acesso às funcionalidades.
- **Cadastrar Usuário**: Adiciona novos usuários com diferentes níveis de acesso (Caixa, Logística).
- **Carregar e Salvar Usuários**: Funções para carregar usuários de um arquivo e salvar alterações.

### Carrinho de Compras
- **Adicionar ao Carrinho**: Permite que o usuário adicione produtos ao carrinho, especificando a quantidade ou peso, se aplicável.
- **Totalização**: Calcula o total da compra no carrinho, considerando produtos por unidade e por peso.

## Estruturas de Dados

O sistema utiliza as seguintes estruturas para gerenciar informações:

- **Produto**: Armazena dados dos produtos (nome, código, preço, quantidade, se vendido por peso).
- **Cliente**: Guarda informações de reembolsos e produtos relacionados.
- **Usuário**: Registra os dados de usuários que podem acessar o sistema.
- **Item do Carrinho**: Estrutura que representa um item adicionado ao carrinho de compras.

## Definições de Limites
- **Máximos**:
  - Produtos: 100
  - Clientes: 100
  - Usuários: 50
  - Itens no Carrinho: 50
  - Reembolsos: 5
- **Validade do Reembolso**: 30 minutos (1800 segundos).

## Como Executar o Sistema

1. **Compilar o código**: Use um compilador de C, como `gcc`, para compilar o arquivo.
2. **Executar**: Após a compilação, execute o programa gerado.
3. **Interagir com o Menu**: Siga as instruções na tela para navegar pelas diferentes funcionalidades do sistema.

## Contribuições

Sinta-se à vontade para contribuir com melhorias ou correções. Qualquer sugestão é bem-vinda!

## Licença

Este projeto é licenciado sob a [Licença MIT](LICENSE).
