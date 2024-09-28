import customtkinter
from tkinter import messagebox

# Função para verificação do login
def verificar_login(usuario, senha, sistema):
    if usuario == "admin" and senha == "1234":
        if sistema == "compras":
            janela_compras()
        elif sistema == "logistica":
            janela_logistica()
        elif sistema == "graal":
            janela_graal()
    else:
        messagebox.showerror("Erro", "Usuário ou senha incorretos!")

# Função para criar a janela de login
def criar_janela_login(sistema):
    janela_login = customtkinter.CTkToplevel()
    janela_login.title("Login")
    janela_login.geometry("400x200")

    label_usuario = customtkinter.CTkLabel(janela_login, text="Usuário")
    label_usuario.pack(pady=10)
    entry_usuario = customtkinter.CTkEntry(janela_login)
    entry_usuario.pack(pady=10)

    label_senha = customtkinter.CTkLabel(janela_login, text="Senha")
    label_senha.pack(pady=10)
    entry_senha = customtkinter.CTkEntry(janela_login, show="*")
    entry_senha.pack(pady=10)

    botao_login = customtkinter.CTkButton(janela_login, text="Login", 
                    command=lambda: verificar_login(entry_usuario.get(), entry_senha.get(), sistema))
    botao_login.pack(pady=20)

# Função para a janela do sistema de compras
def janela_compras():
    janela_compras = customtkinter.CTkToplevel()
    janela_compras.title("Sistema de Compras")
    janela_compras.geometry("600x400")
    label = customtkinter.CTkLabel(janela_compras, text="Bem-vindo ao Sistema de Compras")
    label.pack(pady=20)

# Função para a janela do sistema de logística
def janela_logistica():
    janela_logistica = customtkinter.CTkToplevel()
    janela_logistica.title("Sistema de Logística")
    janela_logistica.geometry("600x400")
    label = customtkinter.CTkLabel(janela_logistica, text="Bem-vindo ao Sistema de Logística")
    label.pack(pady=20)

# Função para a janela do sistema de pesagem Graal
def janela_graal():
    janela_graal = customtkinter.CTkToplevel()
    janela_graal.title("Sistema de Pesagem Graal")
    janela_graal.geometry("600x400")
    label = customtkinter.CTkLabel(janela_graal, text="Bem-vindo ao Sistema de Pesagem Graal")
    label.pack(pady=20)

# Funções dos botões principais para abrir as janelas de login
def abrir_compras():
    criar_janela_login("compras")

def abrir_logistica():
    criar_janela_login("logistica")

def abrir_graal():
    criar_janela_login("graal")

# Criação da janela principal
janela = customtkinter.CTk()
janela.title("Sistemas do Hortfruit")
janela.geometry("800x400")

# Inserção de componentes na janela principal
titulo = customtkinter.CTkLabel(janela, text="Sistema de Compras", font=("Arial", 14, "bold"))
titulo.pack(pady=14)
botao1 = customtkinter.CTkButton(janela, text="Entrar em Compras", command=abrir_compras)
botao1.pack(pady=10)

titulo2 = customtkinter.CTkLabel(janela, text="Sistema de Logística", font=("Arial", 14, "bold"))
titulo2.pack(pady=10)
botao2 = customtkinter.CTkButton(janela, text="Entrar em Logística", command=abrir_logistica)
botao2.pack(pady=14)

titulo3 = customtkinter.CTkLabel(janela, text="Sistema de Pesagem Graal", font=("Arial", 14, "bold"))
titulo3.pack(pady=10)
botao3 = customtkinter.CTkButton(janela, text="Entrar em Graal", command=abrir_graal)
botao3.pack(pady=14)

janela.mainloop()
