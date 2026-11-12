/*
     ____ ____ ____ ____   ____ ____ ____ ____
    ||R |||i |||a |||n || ||L |||u |||i |||z ||
    ||__|||__|||__|||__|| ||__|||__|||__|||__||
    |/__\|/__\|/__\|/__\| |/__\|/__\|/__\|/__\|

     ____ ____ ____ ____ ____ ____ ____ ____ ____   ____ ____ ____ ____
    ||G |||u |||i |||l |||h |||e |||r |||m |||e || ||P |||i |||l |||z ||
    ||__|||__|||__|||__|||__|||__|||__|||__|||__|| ||__|||__|||__|||__||
    |/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|/__\| |/__\|/__\|/__\|/__\|
*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Tamanho dos caracteres
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

// Tipo Data
typedef struct{
    int dia;
    int mes;
    int ano;
}Data;

// Tipo Horario
typedef struct{
    int hora;
    int minuto;
}Horario;


// Tipo Evento
typedef struct{
    Data data;
    Horario inicio;
    Horario fim;
    char descricao[TAM];
    char local[TAM];
}Evento;

// ---------- Utilitários ----------

void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void enter(){
    printf("\n" NEGRITO_BRANCO "Pressione Enter para continuar..." RESET);
    limpar_buffer();
}

// ---------- Opções do menu ----------


// 1.
void cadastrar_evento(Evento, **info, *contador){
    (*contador)++;

    Evento *temp = realloc(info, sizeof(Evento) * (*contador));

    if (temp == NULL) {
        printf("\n" VERMELHO "Erro ao alocar memória!" RESET "\n");
        (*contador)--;
        return;
    }

    *info = temp;

    Evento *novo = &((*info)[*contador - 1]);

    bool horario_valido = false;

    do {
        printf("Insira a data (DD MM AAAA): ");
        scanf("%d %d %d", &novo->data.dia, &novo->data.mes, &novo->data.ano);

        printf("Insira o horário de início (HH MM): ");
        scanf("%d %d", &novo->inicio.hora, &novo->inicio.minuto);

        printf("Insira o horário de término (HH MM): ");
        scanf("%d %d", &novo->fim.hora, &novo->fim.minuto);

        int inicio_minutos = novo->inicio.hora * 60 + novo->inicio.minuto;
        int fim_minutos = novo->fim.hora * 60 + novo->fim.minuto;

        if (fim_minutos <= inicio_minutos) {
            printf("\n" VERMELHO "O término deve ser após o início. Tente novamente." RESET "\n\n");
            continue;
        }

        // Checa conflitos com os eventos anteriores
        bool conflito = false;
        for (int i = 0; i < *contador - 1; i++) {
            if ((*info)[i].data.dia == novo->data.dia && 
                (*info)[i].data.mes == novo->data.mes && 
                (*info)[i].data.ano == novo->data.ano) {

                int ex_inicio = (*info)[i].inicio.hora * 60 + (*info)[i].inicio.minuto;
                int ex_fim = (*info)[i].fim.hora * 60 + (*info)[i].fim.minuto;

                if (inicio_minutos < ex_fim && fim_minutos > ex_inicio) {
                    conflito = true;
                    break;
                }
            }
        }

        if (conflito) {
            printf("\n" VERMELHO "Conflito de horários! Insira outro horário." RESET "\n\n");
        } else {
            horario_valido = true;
        }

    } while (!horario_valido);
}

int main(){
    
    // Compatibilidade dos caracteres especiais
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    login();

    Evento *info = NULL;

    int opcao;
    bool erro = false;


    int contador = 0;

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
                cadastrar_evento(Evento, &info, &contador);
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
                free(info);
                return 0;
            default:
                erro = true;
                break;
        }

    }while(true);
}