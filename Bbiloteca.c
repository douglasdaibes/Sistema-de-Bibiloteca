#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LIVROS 100
#define MAX_USUARIOS 100
#define MAX_EMPRESTIMOS 100
#define MAX_EMPRESTIMOS_POR_USUARIO 5  // Novo limite de empréstimos por usuário

struct Livro {
    char titulo[100];
    char autor[100];
    char isbn[20];
    int ano_publicacao;
    int status; // 0 - disponível, 1 - emprestado
};

struct Usuario {
    char nome[100];
    int id;
    char contato[50];
    int num_emprestimos;
};

struct Emprestimo {
    int id_usuario;
    char isbn_livro[20];
    char data_emprestimo[11]; // formato: dd/mm/aaaa
    char data_devolucao[11];   // formato: dd/mm/aaaa
};

struct Livro livros[MAX_LIVROS];
struct Usuario usuarios[MAX_USUARIOS];
struct Emprestimo emprestimos[MAX_EMPRESTIMOS];

int num_livros = 0;
int num_usuarios = 0;
int num_emprestimos = 0;

void cadastrarLivro() {
    if (num_livros >= MAX_LIVROS) {
        printf("Limite de livros atingido.\n");
        return;
    }
    struct Livro livro;
    printf("Digite o título do livro: ");
    scanf(" %[^\n]", livro.titulo);
    printf("Digite o autor do livro: ");
    scanf(" %[^\n]", livro.autor);
    printf("Digite o ISBN do livro: ");
    scanf(" %[^\n]", livro.isbn);
    printf("Digite o ano de publicação: ");
    scanf("%d", &livro.ano_publicacao);
    livro.status = 0; // disponível
    livros[num_livros++] = livro;
    printf("Livro cadastrado com sucesso!\n");
}

void consultarLivro() {
    char isbn[20];
    printf("Digite o ISBN do livro: ");
    scanf(" %[^\n]", isbn);
    for (int i = 0; i < num_livros; i++) {
        if (strcmp(livros[i].isbn, isbn) == 0) {
            printf("Título: %s\nAutor: %s\nISBN: %s\nAno de Publicação: %d\nStatus: %s\n",
                   livros[i].titulo, livros[i].autor, livros[i].isbn,
                   livros[i].ano_publicacao, livros[i].status == 0 ? "Disponível" : "Emprestado");
            return;
        }
    }
    printf("Livro não encontrado.\n");
}

void cadastrarUsuario() {
    if (num_usuarios >= MAX_USUARIOS) {
        printf("Limite de usuários atingido.\n");
        return;
    }
    struct Usuario usuario;
    printf("Digite o nome do usuário: ");
    scanf(" %[^\n]", usuario.nome);
    printf("Digite o ID do usuário: ");
    scanf("%d", &usuario.id);
    printf("Digite o contato do usuário: ");
    scanf(" %[^\n]", usuario.contato);
    usuario.num_emprestimos = 0;
    usuarios[num_usuarios++] = usuario;
    printf("Usuário cadastrado com sucesso!\n");
}

void emprestarLivro() {
    int id_usuario;
    char isbn[20], data_emprestimo[11], data_devolucao[11];
    
    printf("Digite o ID do usuário: ");
    scanf("%d", &id_usuario);
    
    // Verifica se o usuário existe
    int usuario_idx = -1;
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].id == id_usuario) {
            usuario_idx = i;
            break;
        }
    }
    if (usuario_idx == -1) {
        printf("Usuário não encontrado.\n");
        return;
    }

    // Verifica se o usuário atingiu o limite de empréstimos
    if (usuarios[usuario_idx].num_emprestimos >= MAX_EMPRESTIMOS_POR_USUARIO) {
        printf("Usuário atingiu o limite máximo de empréstimos.\n");
        return;
    }

    printf("Digite o ISBN do livro: ");
    scanf(" %[^\n]", isbn);
    
    // Verifica se o livro existe e está disponível
    int livro_idx = -1;
    for (int i = 0; i < num_livros; i++) {
        if (strcmp(livros[i].isbn, isbn) == 0 && livros[i].status == 0) {
            livro_idx = i;
            break;
        }
    }
    if (livro_idx == -1) {
        printf("Livro não encontrado ou não disponível.\n");
        return;
    }
    
    printf("Digite a data de empréstimo (dd/mm/aaaa): ");
    scanf(" %[^\n]", data_emprestimo);
    printf("Digite a data de devolução (dd/mm/aaaa): ");
    scanf(" %[^\n]", data_devolucao);

    // Registra o empréstimo
    struct Emprestimo emprestimo;
    emprestimo.id_usuario = id_usuario;
    strcpy(emprestimo.isbn_livro, isbn);
    strcpy(emprestimo.data_emprestimo, data_emprestimo);
    strcpy(emprestimo.data_devolucao, data_devolucao);
    emprestimos[num_emprestimos++] = emprestimo;

    // Atualiza o status do livro
    livros[livro_idx].status = 1;
    
    // Atualiza o número de empréstimos do usuário
    usuarios[usuario_idx].num_emprestimos++;

    printf("Empréstimo realizado com sucesso!\n");
}

void consultarEmprestimos() {
    printf("Empréstimos registrados:\n");
    if (num_emprestimos == 0) {
        printf("Nenhum empréstimo registrado.\n");
        return;
    }

    for (int i = 0; i < num_emprestimos; i++) {
        int usuario_idx = -1;
        int livro_idx = -1;

        // Encontra o índice do usuário
        for (int j = 0; j < num_usuarios; j++) {
            if (usuarios[j].id == emprestimos[i].id_usuario) {
                usuario_idx = j;
                break;
            }
        }

        // Encontra o índice do livro
        for (int j = 0; j < num_livros; j++) {
            if (strcmp(livros[j].isbn, emprestimos[i].isbn_livro) == 0) {
                livro_idx = j;
                break;
            }
        }

        if (usuario_idx == -1 || livro_idx == -1) {
            printf("Erro no empréstimo %d\n", i + 1);
            continue;
        }

        printf("Empréstimo %d:\n", i + 1);
        printf("Usuário: %s (ID: %d)\n", usuarios[usuario_idx].nome, usuarios[usuario_idx].id);
        printf("Livro: %s\n", livros[livro_idx].titulo);
        printf("ISBN: %s\n", livros[livro_idx].isbn);
        printf("Data de Empréstimo: %s\n", emprestimos[i].data_emprestimo);
        printf("Data de Devolução: %s\n\n", emprestimos[i].data_devolucao);
    }
}

int main() {
    int opcao;

    do {
        printf("\nBiblioteca - Menu\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Consultar Livro\n");
        printf("3. Cadastrar Usuário\n");
        printf("4. Emprestar Livro\n");
        printf("5. Consultar Empréstimos\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarLivro(); break;
            case 2: consultarLivro(); break;
            case 3: cadastrarUsuario(); break;
            case 4: emprestarLivro(); break;
            case 5: consultarEmprestimos(); break;
            case 6: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 6);

    return 0;
}
