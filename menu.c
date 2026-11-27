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

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  

#include "login.h"

#ifdef _WIN32
#include <windows.h>
#endif

FILE *arquivo;

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

static void remover_quebra_linha(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
}

static void extrair_apos_dois_pontos(const char *linha, char *destino, size_t tam_destino) {
    const char *pos = strchr(linha, ':');
    if (pos != NULL) {
        pos++; 
        while (*pos == ' ') {
            pos++; 
        }
        snprintf(destino, tam_destino, "%s", pos);
    } else {
        snprintf(destino, tam_destino, "%s", linha);
    }
}

bool le_arquivo(FILE *arquivo, Evento *dado) {
    char linha[256];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    if (sscanf(linha, "Data: %d/%d/%d",
               &dado->data.dia, &dado->data.mes, &dado->data.ano) != 3) {
        return false;
    }

    if (fgets(linha, sizeof(linha), arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    if (sscanf(linha, "Horario de inicio e fim: %d:%d - %d:%d",
               &dado->inicio.hora, &dado->inicio.minuto,
               &dado->fim.hora, &dado->fim.minuto) != 4) {
        return false;
    }

    if (fgets(linha, sizeof(linha), arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    extrair_apos_dois_pontos(linha, dado->descricao, TAM);

    if (fgets(linha, sizeof(linha), arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    extrair_apos_dois_pontos(linha, dado->local, TAM);

    return true;
}


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
        printf("\n" VERMELHO "Você precisa criar um novo evento!" RESET "\n");
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
int compara_todos(Evento evento1, Evento evento2){
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

int compara_data(Evento evento1, Evento evento2){
    if(evento1.data.ano < evento2.data.ano)
        return 0;
    if(evento1.data.ano > evento2.data.ano)
        return 2;
    
    if(evento1.data.mes < evento2.data.mes)
        return 0;
    if(evento1.data.mes > evento2.data.mes)
        return 2;

    if(evento1.data.dia < evento2.data.dia)
        return 0;
    if(evento1.data.dia > evento2.data.dia)
        return 2;

    return 1;
}

void insira_data(Evento *cadastrar){

    bool deu_certo = false;
    const int total_dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dia, mes, ano;
    do{

        printf("\nInsira a data (DD MM AAAA): ");
        if(scanf("%d %d %d", &dia, &mes, &ano ) != 3){
            limpar_buffer();
            printf("\n" AMARELO "Ops! Formato inválido. Digite dia, mês e ano separados por espaços" RESET "\n");
            continue;
        }

        limpar_buffer();

        if(ano < 1){
            printf("\n" AMARELO  "Ops! O ano precisa ser maior que zero. Tente novamente!" RESET "\n");
            continue;
        }

        if(mes < 1 || mes > 12){
            printf("\n" AMARELO "Ops! O mês precisa estar entre 1 e 12. Tente novamente!" RESET "\n");
            continue;
        }

        if(dia < 1 || dia > total_dias_mes[mes - 1]){
            printf("\n" AMARELO "Ops! Esse dia não existe no mês informado. Tente novamente!" RESET "\n");
            continue;
        }

        cadastrar->data.dia = dia;
        cadastrar->data.mes = mes;
        cadastrar->data.ano = ano;

        deu_certo = true;


    }while(!deu_certo);

}

void insira_hora_inicio_fim(Evento *cadastrar){
    
    bool deu_certo = false;
    // LOOP inserir horario de inicio
    do{

        int hora_inicio, minuto_inicio;

        printf("\nInsira o horário de inicio (HH MM): ");
        if(scanf("%d %d", &hora_inicio, &minuto_inicio) != 2){
            limpar_buffer();
            printf("\n" AMARELO "Ops! Digite apenas números para o horário de início!" RESET "\n");
            continue;
        }

        limpar_buffer();

        if(hora_inicio < 0 || hora_inicio > 23){
            printf("\n" AMARELO "Ops! A hora de início deve ser entre 0 e 23. Tente novamente!" RESET "\n");
            continue;
        }
            
        if(minuto_inicio < 0 || minuto_inicio > 59){
            printf("\n" AMARELO "Ops! Os minutos de início devem ser entre 0 e 59. Tente novamente!" RESET "\n");
            continue;
        }

        cadastrar->inicio.hora = hora_inicio;
        cadastrar->inicio.minuto = minuto_inicio;

        deu_certo = true;

    }while(!deu_certo);

    deu_certo = false;

    // LOOP inserir horario de fim
    do{

        int hora_fim, minuto_fim;

        printf("\nInsira o harario de fim (HH MM): ");
        if(scanf("%d %d", &hora_fim, &minuto_fim) != 2){
            limpar_buffer();
            printf("\n" AMARELO "Ops! Digite apenas números para o horário de fim!" RESET "\n");
            continue;
        }

        limpar_buffer();

        if(hora_fim < 0 || hora_fim > 23){
            printf("\n" AMARELO "Ops! A hora de fim deve ser entre 0 e 23. Tente novamente!" RESET "\n");
            continue;
        }

        if(minuto_fim < 0 || minuto_fim > 59){
            printf("\n" AMARELO "Ops! Os minutos de fim devem ser entre 0 e 59. Tente novamente!" RESET "\n");
            continue;
        }

        cadastrar->fim.hora = hora_fim;
        cadastrar->fim.minuto = minuto_fim;

        deu_certo = true;

    }while(!deu_certo);

}

void insira_descricao_local(Evento *cadastrar){

    bool deu_certo = false;

    do{
        
        char descricao[TAM] = "";

        printf("\nInsira uma descrição: ");
        if(fgets(descricao, TAM, stdin) != NULL){
            descricao[strcspn(descricao, "\n")] = '\0';
        }else{
            printf("\n" VERMELHO "Erro ao ler a descrição. Tente novamente!" RESET "\n");
            continue;
        }

        if(descricao[0] == '\0'){
            printf("\n" AMARELO "Ei! A descrição não pode ficar vazia. Escreva o que vai acontecer!" RESET "\n");
            continue;
        }

        size_t len = strlen(descricao);

        for(size_t i = 0; i < len; i++){
            cadastrar->descricao[i] = descricao[i];
        }
        
        cadastrar->descricao[len] = '\0';

        deu_certo = true;

    }while(!deu_certo);

    deu_certo = false;

    do{

        char local[TAM] = "";

        printf("\nInsira um local: ");
        if(fgets(local, TAM, stdin) != NULL){
            local[strcspn(local, "\n")] = '\0';
        }else{
            printf("\n" VERMELHO "Erro ao ler o local. Tente novamente!" RESET "\n");
            continue;
        }

        if(local[0] == '\0'){
            printf("\n" AMARELO "Ei! O local não pode ficar vazio. Informe onde será o evento!" RESET "\n");
            continue;
        }

        size_t len = strlen(local);

        for(size_t i = 0; i < len; i++){
            cadastrar->local[i] = local[i];
        }

        cadastrar->local[len] = '\0';

        deu_certo = true;


    }while(!deu_certo);
}

// ---------- Opções do menu ----------


// 1. (alternativa, avaliar melhor opcao depois)
void add_evento(Evento **listaEventos, int *qtdEventos){
    Evento cadastrar;

    insira_data(&cadastrar);
    
    insira_hora_inicio_fim(&cadastrar);

    insira_descricao_local(&cadastrar);

    //Verificar conflito
    Evento *a = realloc(*listaEventos, sizeof(Evento) * (*qtdEventos + 1));
    if(a == NULL){
        printf("\n" VERMELHO "Erro de memória! Nada foi registrado." RESET "\n");
        return;
    }else{

        *listaEventos = a;
        bool adicionou = false;

        for(int i = 0; i < *qtdEventos; i++){
            if(compara_todos((*listaEventos)[i], cadastrar) == 1){
                printf("\n" VERMELHO "Ops! Encontramos um conflitos entre datas ou horarios" RESET "\n");

            }else if(compara_todos((*listaEventos)[i], cadastrar) == 2){
                for(int j = *qtdEventos - 1; j >= i; j--){
                    (*listaEventos)[j+1] = (*listaEventos)[j];
                }
                (*listaEventos)[i] = cadastrar;
                (*qtdEventos)++;
                adicionou = true;
                printf("\n" VERDE "Evento adicionado com sucesso!" RESET "\n");
                break;
            }
        }
        if(!adicionou){
            printf("\n" VERDE "Evento adicionado com sucesso!" RESET "\n");
            (*listaEventos)[(*qtdEventos)] = cadastrar;
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
    Evento busca;

    int referencia = 0;

    insira_data(&busca);

    for(int i = 0; i < qtdEventos; i++){
        if(compara_data(listaEventos[i],busca) == 1){
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
        printf("\n" AMARELO "Desculpa, mas não foi encontrado nenhum evento com essa data!" RESET "\n");
    }
}

//4. Busca eventos a partir de uma descrição
void busca_descricao(Evento *listaEventos, int qtdEventos){
    char descricao[TAM];
    char busca_min[TAM];
    int referencia = 0;

    printf("\nInsira uma descrição: ");
    if(fgets(descricao, TAM, stdin) != NULL){
        descricao[strcspn(descricao, "\n")] = '\0';
    }else{
        descricao[0] = '\0';
    }

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
        printf("\n" AMARELO "Desculpa, mas não foi encontrado nenhum evento com essa descrição!" RESET "\n");
    }

}

// 5. Remove eventos
void remover_eventos(Evento **listaEventos, int *qtdEventos){

    Evento busca;
    
    bool existe = false;
    int salva_indice = 0;

    printf("\nInsira uma data (DD MM AAAA): ");
    scanf("%d %d %d", &busca.data.dia, &busca.data.mes, &busca.data.ano);

    limpar_buffer();

    printf("\nInsira um horario (HH MM): ");
    scanf("%d %d", &busca.inicio.hora, &busca.inicio.minuto);
    
    limpar_buffer();

    for(int i = 0; i < *qtdEventos; i++){
        if(compara_todos((*listaEventos)[i], busca) == 1){
            existe = true;
            salva_indice = i;        
            break;
        }
    }
    
    if(!existe){
        printf("\n" AMARELO "Deculpa, mas a data ou hora de inicio informada não existe dentro da AGENDA!" RESET "\n");
        return;
    }

    for(int i = salva_indice + 1; i < *qtdEventos; i++){
        (*listaEventos)[i - 1] = (*listaEventos)[i];
    }

    (*qtdEventos)--;
    if(*qtdEventos == 0){
        free(*listaEventos);
        *listaEventos = NULL;
        return;
    }

    Evento *temp  = realloc(*listaEventos, sizeof(Evento) * (*qtdEventos));
    if(temp != NULL){
        *listaEventos = temp;
    }

    printf("\n" VERDE "Evento removido com sucesso!" RESET "\n");

}

int main(){
    
    // Compatibilidade dos caracteres especiais
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    
    Evento *listaEventos = NULL;

    int opcao;
    int qtdEventos = 0;

    bool erro = false;
        
    char *endereco_do_arquivo = usuario();

    arquivo = fopen(endereco_do_arquivo , "r");

    if(arquivo == NULL){
        printf("\n" AMARELO "" RESET);
        return 0;
    }

    if(fscanf(arquivo, "Quantidade de eventos: %d", &qtdEventos) != 1 ){
        printf("\n" AMARELO "" RESET);
        return 0;
    }

    int ch;
    while ((ch = fgetc(arquivo)) != '\n' && ch != EOF);

    Evento *temp = realloc(listaEventos, sizeof(Evento) * (qtdEventos));
    if(temp != NULL){
        listaEventos = temp;
    }

    for(int i = 0; i < qtdEventos; i++){
        le_arquivo(arquivo, &listaEventos[i]);
    }

    fclose(arquivo);

    do{

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
            printf("\n" VERMELHO "Ops! Essa opção não está no menu, tente novamente" RESET "\n");
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
                    remover_eventos(&listaEventos, &qtdEventos);
                }
                enter();
                break;
            
            case 6:
                arquivo = fopen(endereco_do_arquivo, "w");
                if (arquivo == NULL) {
                    printf("\n" VERMELHO "Erro ao abrir o arquivo para salvar!" RESET "\n");
                    break;
                }

                fprintf(arquivo, "Quantidade de eventos: %d\n", qtdEventos);
                for(int i = 0; i < qtdEventos; i++){
                    fprintf(arquivo, "Data: %d/%d/%d\n", listaEventos[i].data.dia, listaEventos[i].data.mes, listaEventos[i].data.ano);    
                    fprintf(arquivo, "Horario de inicio e fim: %d:%d - %d:%d\n", listaEventos[i].inicio.hora, listaEventos[i].inicio.minuto, listaEventos[i].fim.hora, listaEventos[i].fim.minuto);
                    fprintf(arquivo, "Descrição: %s\n", listaEventos[i].descricao);
                    fprintf(arquivo, "Local: %s\n", listaEventos[i].local);
                }
                printf("\n" NEGRITO_BRANCO "Obrigado por usar o sistema! Volte sempre" RESET "\n\n");
                free(listaEventos);
                fclose(arquivo);
                return 0;
            default:
                erro = true;
                break;
        }

    }while(true);
}
