import customtkinter
from tkinter import messagebox

# Dicionário de credenciais (usuário e senha) para cada sistema
credenciais = {
    "compras": {"usuario": "compras_admin", "senha": "compras123"},
    "logistica": {"usuario": "logistica_admin", "senha": "logistica123"},
    "graal": {"usuario": "graal_admin", "senha": "graal123"}
}

# Função para verificar login e navegar para o sistema escolhido
def verificar_login(usuario, senha, sistema):
    # Verifica se o sistema existe nas credenciais
    if sistema in credenciais:
        # Obtém as credenciais do sistema específico
        credencial_sistema = credenciais[sistema]
        # Verifica se o usuário e senha fornecidos correspondem às credenciais
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
    label_usuario.pack(pady=10)
    entry_usuario = customtkinter.CTkEntry(janela)
    entry_usuario.pack(pady=10)

    label_senha = customtkinter.CTkLabel(janela, text="Senha")
    label_senha.pack(pady=10)
    entry_senha = customtkinter.CTkEntry(janela, show="*")
    entry_senha.pack(pady=10)

    # Função que será chamada quando o usuário pressionar Enter
    def ao_pressionar_enter(event):
        verificar_login(entry_usuario.get(), entry_senha.get(), sistema)

    # Vinculando a tecla Enter para submeter o login
    janela.bind('<Return>', ao_pressionar_enter)
    
    botao_login = customtkinter.CTkButton(janela, text="Login", 
                    command=lambda: verificar_login(entry_usuario.get(), entry_senha.get(), sistema))
    botao_login.pack(pady=20)
    
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.pack(pady=20)

# Função para a página do sistema de compras
def pagina_compras():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Compras")
    label.pack(pady=20)
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.pack(pady=20)

# Função para a página do sistema de logística
def pagina_logistica():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Logística")
    label.pack(pady=20)
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.pack(pady=20)

# Função para a página do sistema de pesagem Graal
def pagina_graal():
    limpar_janela()
    label = customtkinter.CTkLabel(janela, text="Bem-vindo ao Sistema de Pesagem Graal")
    label.pack(pady=20)
    botao_voltar = customtkinter.CTkButton(janela, text="<- Voltar", command=mostrar_pagina_principal)
    botao_voltar.pack(pady=20)

# Função para limpar a janela (remover widgets existentes)
def limpar_janela():
    for widget in janela.winfo_children():
        widget.pack_forget()

# Função para exibir a página principal
def mostrar_pagina_principal():
    limpar_janela()
    titulo = customtkinter.CTkLabel(janela, text="Sistema do Hortfruit", font=("Arial", 14, "bold"))
    titulo.pack(pady=14)

    botao1 = customtkinter.CTkButton(janela, text="Entrar em Compras", command=lambda: criar_pagina_login("compras"))
    botao1.pack(pady=10)

    botao2 = customtkinter.CTkButton(janela, text="Entrar em Logística", command=lambda: criar_pagina_login("logistica"))
    botao2.pack(pady=10)

    botao3 = customtkinter.CTkButton(janela, text="Entrar em Graal", command=lambda: criar_pagina_login("graal"))
    botao3.pack(pady=10)

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
janela.geometry("800x400")

# Exibir a página principal na inicialização
mostrar_pagina_principal()

janela.mainloop()
