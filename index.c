#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Tamanho de caracteres
#define TAM 50

// Redefinir todas as formatações de texto
#define RESET    "\033[0m"

// Cores Padrão
#define VERMELHO "\033[91m"
#define VERDE    "\033[92m"
#define AMARELO  "\033[93m"
#define AZUL     "\033[94m"

// Cores em Negrito
#define NEGRITO_BRANCO   "\033[1;97m"

typedef struct{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct{
    int hora;
    int minuto;
}Horario;

typedef struct{
    Data data;
    Horario inicio;
    Horario fim;
    char descricao[TAM];
    char local[TAM];
}Evento;


void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void enter(){
    printf("\n" NEGRITO_BRANCO "Pressione Enter para continuar..." RESET);
    limpar_buffer();
}

int main(){
    
    // Compatibilidade dos caracteres especiais
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    Evento *info = NULL;

    int opcao;
    bool erro = false;

    do{
        printf("\033[H\033[2J");

        printf("\n\n");
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║" NEGRITO_BRANCO "                 AGENDA PESSOAL                   " RESET "║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║  1. Cadastrar novo evento                        ║\n");
        printf("║  2. Mostrar todos os eventos                     ║\n");
        printf("║  3. Buscar eventos por data                      ║\n");
        printf("║  4. Buscar eventos por descrição                 ║\n");
        printf("║  5. Remover evento (por data e hora)             ║\n");
        printf("║  6. Sair do programa                             ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");

        if(erro){
            printf("\n" VERMELHO "Ops! Essa opção não está no menu, tente novamente." RESET "\n");
            erro = false;
        }

        printf("\n" AZUL "Escolha a sua opção: " RESET);

        if(scanf("%d", &opcao) != 1){
            opcao = -1;
        }

        limpar_buffer();

        switch(opcao){

            case 1:

                enter();
                break;
            
            case 2:
                enter();
                break;
            
            case 3:
                enter();
                break;
            
            case 4:
                enter();
                break;
            
            case 5:
                enter();
                break;
            
            case 6:
                printf("\n" NEGRITO_BRANCO "Obrigado por usar o sistema! Volte sempre." RESET "\n\n");
                return 0;
            default:
                erro = true;
                break;
        }

    }while(true);
}