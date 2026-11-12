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

int main(){
    
    // Compatibilidade dos caracteres especiais
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    Evento info;

    int opcao;
    bool erro = false;

    do{
        printf("\033[H\033[2J");

        printf("\n\n");
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║                    AGENDA PESSOAL                ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║  1. Cadastrar novo evento                        ║\n");
        printf("║  2. Mostrar todos os eventos                     ║\n");
        printf("║  3. Mostrar todos eventos de um data                          ║\n");
        printf("║  4. Mostrar todos os eventos                     ║\n");
        printf("║  5. Remover um evento                            ║\n");
        printf("║  6. Sair do programa                             ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        
        if(erro){
            printf("");
            erro = false;
        }

        if(scanf("%d", &opcao) != 1){
            opcao = -1;
        }

        switch(opcao){

            case 1:
                break;
            
            case 2:
                break;
            
            case 3:
                break;
            
            case 4:
                break;
            
            case 5:
                break;
            
            case 6:
                printf("\n" NEGRITO_BRANCO "Obrigado por usar o sistema! Volte sempre." RESET "\n");
                return 0;
            default:
                erro = true;
                break;
        }

    }while(true);
}