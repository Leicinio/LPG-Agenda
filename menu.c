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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

bool exite_evento(int qtdEventos){
    if(qtdEventos == 0){
        printf(VERMELHO "\nVocê precisa criar um novo evento!" RESET "\n");
        return false;
    }

    return true;
}

void imprimir_descricao_destacada(Evento *listaEventos, int i, char *busca) {
    int tam_desc = (int)strlen(listaEventos[i].descricao);
    char alvo = (char)tolower((unsigned char)*busca);

    printf("Descrição: ");
    for (char *p = listaEventos[i].descricao; p < listaEventos[i].descricao + tam_desc; p++) {
        char atual = (char)tolower((unsigned char)*p);
        if (atual == alvo) {
            printf(VERDE "%c" RESET, *p);
        } else {
            printf("%c", *p);
        }
    }
    printf("\n");
}

//Retorna 0 se evento1 for anterior ao evento2, 1 se evento1 for no mesmo dia/mes/ano do evento2, 2 se evento1 for posterior ao evento2
int compara_data(Evento evento1, Evento evento2){
    if (evento1.data.ano < evento2.data.ano)
        return 0;
    if (evento1.data.ano > evento2.data.ano)
        return 2;

    if (evento1.data.mes < evento2.data.mes)
        return 0;
    if (evento1.data.mes > evento2.data.mes)
        return 2;

    if (evento1.data.dia < evento2.data.dia)
        return 0;
    if (evento1.data.dia > evento2.data.dia)
        return 2;

    if (evento1.inicio.hora < evento2.inicio.hora)
        return 0;
    if (evento1.inicio.hora > evento2.inicio.hora)
        return 2;

    if (evento1.inicio.minuto < evento2.inicio.minuto)
        return 0;
    if (evento1.inicio.minuto > evento2.inicio.minuto)
        return 2;

    return 1;
}

// ---------- Opções do menu ----------


// 1. (alternativa, avaliar melhor opcao depois)
void add_evento(Evento **listaEventos, int *qtdEventos){
    Evento novo;

    printf("\nInsira a data (DD MM AAAA): ");
    scanf("%d %d %d", &novo.data.dia, &novo.data.mes, &novo.data.ano);

    limpar_buffer();

    printf("\nInsira o horário de início (HH MM): ");
    scanf("%d %d", &novo.inicio.hora, &novo.inicio.minuto);

    limpar_buffer();

    printf("\nInsira o horário de término (HH MM): ");
    scanf("%d %d", &novo.fim.hora, &novo.fim.minuto);

    limpar_buffer();

    printf("\nInsira a descrição: ");
    scanf("%[^\n]", novo.descricao);

    limpar_buffer();

    printf("\nInsira o local: ");
    scanf("%[^\n]", novo.local);

    limpar_buffer();

    //Verificar conflito

    //Adicionar a lista
    Evento *a = realloc(*listaEventos, sizeof(Evento) * (*qtdEventos + 1));
    if(a == NULL){
        printf(AMARELO "\nErro de memória! Nada foi registrado." RESET);
    }else{

        *listaEventos = a;
        bool adicionou = false;

        for(int i = 0; i < *qtdEventos; i++){
            if(compara_data((*listaEventos)[i], novo) == 1){
                printf(VERMELHO "\nOps! Encontramos um conflitos entre datas ou horarios" RESET);

            }else if(compara_data((*listaEventos)[i], novo) == 2){
                for(int j = *qtdEventos - 1; j >= i; j--){
                    (*listaEventos)[j+1] = (*listaEventos)[j];
                }
                (*listaEventos)[i] = novo;
                (*qtdEventos)++;
                adicionou = true;
                printf(VERDE "\nEvento adicionado com sucesso!\n" RESET);
                break;
            }
        }
        if(!adicionou){
            printf(VERDE "\nEvento adicionado com sucesso!\n" RESET);
            (*listaEventos)[(*qtdEventos)] = novo;
            (*qtdEventos)++;
        }   
    }

}

//2. Mostra todos os eventos já adicionados
void mostrar_todos_eventos(Evento *listaEventos, int qtdEventos){
    for(int i = 0; i < qtdEventos; i++){
        printf("\n");
        printf("┌───────────────────────┐\n");
        printf("│       " NEGRITO_BRANCO "EVENTO %02d" RESET "       │\n", i + 1);
        printf("└───────────────────────┘\n");
        printf("%02d/%02d/%04d | %02d:%02d - %02d:%02d\n",
               listaEventos[i].data.dia, listaEventos[i].data.mes, listaEventos[i].data.ano,
               listaEventos[i].inicio.hora, listaEventos[i].inicio.minuto,
               listaEventos[i].fim.hora, listaEventos[i].fim.minuto);
        printf("Descrição: %s\n", listaEventos[i].descricao);
        printf("Local: %s\n", listaEventos[i].local);
    }
}

//3. Busca eventos por meio de uma data
void busca_data(Evento *listaEventos, int qtdEventos){
    int dia, mes, ano;
    int referencia = 0;

    printf("\nInsira uma data para achar os eventos (DD MM AAAA): ");
    scanf("%d %d %d", &dia, &mes, &ano);

    limpar_buffer();

    for(int i = 0; i < qtdEventos; i++){
        if(listaEventos[i].data.dia == dia && listaEventos[i].data.mes == mes && listaEventos[i].data.ano == ano){
            referencia++;
            printf("\n");
            printf("┌───────────────────────┐\n");
            printf("│       " NEGRITO_BRANCO "EVENTO %02d" RESET "       │\n", i + 1);
            printf("└───────────────────────┘\n");
            printf(VERDE "%02d/%02d/%04d" RESET " | %02d:%02d - %02d:%02d\n",
                listaEventos[i].data.dia, listaEventos[i].data.mes, listaEventos[i].data.ano,
                listaEventos[i].inicio.hora, listaEventos[i].inicio.minuto,
                listaEventos[i].fim.hora, listaEventos[i].fim.minuto);
            printf("Descrição: %s\n", listaEventos[i].descricao);
            printf("Local: %s\n", listaEventos[i].local);
        }
    }

    if(referencia == 0){
        printf(AMARELO "\nDesculpa, mas não foi encontrado nenhum evento com essa data!\n" RESET);
    }
}

//4. Busca eventos a partir de uma descrição
void busca_descricao(Evento *listaEventos, int qtdEventos){
    char descricao[TAM];
    char busca_min[TAM];
    int referencia = 0;

    printf("\nInsira uma descrição para achar os eventos: ");
    scanf("%[^\n]", descricao);

    int tam_busca = (int)strlen(descricao);
    for(int k = 0; k < tam_busca; k++){
        busca_min[k] = (char)tolower((unsigned char)descricao[k]);
    }
    busca_min[tam_busca] = '\0';

    for(int i = 0; i < qtdEventos; i++){
        char desc_min[TAM];
        int tam_desc = (int)strlen(listaEventos[i].descricao);

        for(int k = 0; k < tam_desc; k++){
            desc_min[k] = (char)tolower((unsigned char)listaEventos[i].descricao[k]);
        }
        desc_min[tam_desc] = '\0';

        if(strstr(desc_min, busca_min) != NULL){
            referencia++;
            printf("\n");
            printf("┌───────────────────────┐\n");
            printf("│       " NEGRITO_BRANCO "EVENTO %02d" RESET "       │\n", i + 1);
            printf("└───────────────────────┘\n");
            printf("%02d/%02d/%04d | %02d:%02d - %02d:%02d\n",
                listaEventos[i].data.dia, listaEventos[i].data.mes, listaEventos[i].data.ano,
                listaEventos[i].inicio.hora, listaEventos[i].inicio.minuto,
                listaEventos[i].fim.hora, listaEventos[i].fim.minuto);
            imprimir_descricao_destacada(listaEventos, i, descricao);
            printf("Local: %s\n", listaEventos[i].local);
        }
    }

    if(referencia == 0){
        printf(AMARELO "\nDesculpa, mas não foi encontrado nenhum evento com essa descrição!\n" RESET);
    }

    limpar_buffer();
}

// 1.
/*
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
*/
int main(){
    
    // Compatibilidade dos caracteres especiais
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    int opcao;
    bool erro = false;

    Evento *listaEventos = malloc(sizeof(Evento));
    int qtdEventos = 0;


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
                add_evento(&listaEventos, &qtdEventos);
                enter();
                break;
            
            case 2:
                if(exite_evento(qtdEventos)){
                    mostrar_todos_eventos(listaEventos, qtdEventos);
                }
                enter();
                break;
            
            case 3:
                if(exite_evento(qtdEventos)){
                    busca_data(listaEventos, qtdEventos);
                }
                enter();
                break;
            
            case 4:
                if(exite_evento(qtdEventos)){
                    busca_descricao(listaEventos, qtdEventos);
                }
                enter();
                break;
            
            case 5:
                if(exite_evento(qtdEventos)){
                    
                }
                enter();
                break;
            
            case 6:
                printf("\n" NEGRITO_BRANCO "Obrigado por usar o sistema! Volte sempre." RESET "\n\n");
                free(listaEventos);
                return 0;
            default:
                erro = true;
                break;
        }

    }while(true);
}