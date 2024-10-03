import customtkinter
from PIL import Image
from tkinter import messagebox

# Dicionário de credenciais (usuário e senha) para cada sistema
credenciais = {
    "compras": {"usuario": "compras_admin", "senha": "compras123"},
    "logistica": {"usuario": "logistica_admin", "senha": "logistica123"},
    "graal": {"usuario": "graal_admin", "senha": "graal123"}
}

# Função para verificar login e navegar para o sistema escolhido
def verificar_login(usuario, senha, sistema):
    if sistema in credenciais:
        credencial_sistema = credenciais[sistema]
        if usuario == credencial_sistema["usuario"] and senha == credencial_sistema["senha"]:
            mostrar_pagina(sistema)
        else:
            messagebox.showerror("Erro", "Usuário ou senha incorretos!")
    else:
        messagebox.showerror("Erro", "Sistema não encontrado!")

# Função para criar a página de login
def criar_pagina_login(sistema):
    limpar_janela()

    label_usuario = customtkinter.CTkLabel(janela, text="Usuário")
    label_usuario.grid(row=0, column=1, padx=10, pady=10, sticky="ew")
    entry_usuario = customtkinter.CTkEntry(janela)
    entry_usuario.grid(row=0, column=2, padx=10, pady=10, sticky="ew")

    label_senha = customtkinter.CTkLabel(janela, text="Senha")
    label_senha.grid(row=1, column=1, padx=10, pady=10, sticky="ew")
    entry_senha = customtkinter.CTkEntry(janela, show="*")
    entry_senha.grid(row=1, column=2, padx=10, pady=10, sticky="ew")

    # Função que será chamada quando o usuário pressionar Enter
    def ao_pressionar_enter(event):
        verificar_login(entry_usuario.get(), entry_senha.get(), sistema)

    # Vinculando a tecla Enter para submeter o login
    janela.bind('<Return>', ao_pressionar_enter)

    botao_login = customtkinter.CTkButton(janela, text="Login", 
                    command=lambda: verificar_login(entry_usuario.get(), entry_senha.get(), sistema))
    botao_login.grid(row=2, column=1, columnspan=2, padx=10, pady=20, sticky="ew")

    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.grid(row=3, column=1, columnspan=2, padx=10, pady=10, sticky="ew")

# Função para a página do sistema de compras
def pagina_compras():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Compras")
    label.grid(row=0, column=1, columnspan=2, padx=10, pady=20, sticky="ew")
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.grid(row=1, column=1, columnspan=2, padx=10, pady=10, sticky="ew")

# Função para a página do sistema de logística
def pagina_logistica():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Logística")
    label.grid(row=0, column=1, columnspan=2, padx=10, pady=20, sticky="ew")
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.grid(row=1, column=1, columnspan=2, padx=10, pady=10, sticky="ew")

# Função para a página do sistema de pesagem Graal
def pagina_graal():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Pesagem Graal")
    label.grid(row=0, column=1, columnspan=2, padx=10, pady=20, sticky="ew")
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.grid(row=1, column=1, columnspan=2, padx=10, pady=10, sticky="ew")

# Função para limpar a janela (remover widgets existentes)
def limpar_janela():
    for widget in janela.winfo_children():
        widget.grid_forget()

# Função para exibir a página principal
def mostrar_pagina_principal():
    limpar_janela()

    largura_janela = janela.winfo_width()

    # Carregando as imagens usando CTkImage (redimensionando para torná-las responsivas)
    img_compras = customtkinter.CTkImage(Image.open("desconto-em-compras.png"), size=(largura_janela // 6, largura_janela // 6))
    img_log = customtkinter.CTkImage(Image.open("inventario.png"), size=(largura_janela // 6, largura_janela // 6))
    img_pesagem = customtkinter.CTkImage(Image.open("perda-de-peso.png"), size=(largura_janela // 6, largura_janela // 6))

    # Definindo título
    titulo = customtkinter.CTkLabel(janela, text="Sistema do Hortfruit", font=("Arial", 14, "bold"))
    titulo.grid(row=0, column=1, columnspan=2, padx=10, pady=14, sticky="ew")

    # Criando botões com imagens
    botao1 = customtkinter.CTkButton(janela, text="Compras", image=img_compras, compound="top", 
                                     command=lambda: criar_pagina_login("compras"))
    botao1.grid(row=1, column=1, padx=10, pady=10, sticky="ew")

    botao2 = customtkinter.CTkButton(janela, text="Logística", image=img_log, compound="top", 
                                     command=lambda: criar_pagina_login("logistica"))
    botao2.grid(row=1, column=2, padx=10, pady=10, sticky="ew")

    botao3 = customtkinter.CTkButton(janela, text="Pesagem", image=img_pesagem, compound="top", 
                                     command=lambda: criar_pagina_login("graal"))
    botao3.grid(row=2, column=1, columnspan=2, padx=10, pady=10, sticky="ew")

    # Configurar grid para redimensionamento dinâmico e manter o conteúdo centralizado
    janela.grid_rowconfigure(0, weight=1)
    janela.grid_rowconfigure(1, weight=1)
    janela.grid_rowconfigure(2, weight=1)
    janela.grid_columnconfigure(0, weight=1)  # Coluna da esquerda para centralizar o conteúdo
    janela.grid_columnconfigure(1, weight=1)
    janela.grid_columnconfigure(2, weight=1)
    janela.grid_columnconfigure(3, weight=1)  # Coluna da direita para centralizar o conteúdo

# Função para mostrar páginas específicas
def mostrar_pagina(sistema):
    if sistema == "compras":
        pagina_compras()
    elif sistema == "logistica":
        pagina_logistica()
    elif sistema == "graal":
        pagina_graal()

# Criação da janela principal
janela = customtkinter.CTk()
janela.title("Sistemas do Hortfruit")
janela.geometry("800x600")

# Exibir a página principal na inicialização
mostrar_pagina_principal()

janela.mainloop()
