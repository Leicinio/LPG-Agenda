#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define CRIAR_PASTA(dir) _mkdir(dir)
    #define GETCWD _getcwd
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #define CRIAR_PASTA(dir) mkdir(dir, 0777)
    #define GETCWD getcwd
#endif

FILE *arquivo_usuarios;

#define TAM 50                  // Tamanho do Char
#define TAM_CAMINHO 4096        // Tamanho do arquivo

#define RESET "\033[0m"

#define VERMELHO "\033[91m"
#define VERDE    "\033[92m"
#define AMARELO  "\033[93m"
#define AZUL     "\033[94m"

#define NEGRITO_BRANCO "\033[1;97m"

typedef struct{
    char nome[TAM];
    char senha[TAM];
}Usuarios;

static void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

static void enter(){
    printf("\n" NEGRITO_BRANCO "Pressione Enter para continuar..." RESET);
    limpar_buffer();
}

bool existe_usuarios(int qdtUsuarios){
    if(qdtUsuarios == 0){
        printf("\n" VERMELHO "Você precisa criar um novo usuário!"  RESET "\n");
        return false;
    }

    return true;
}

char *obter_pasta_principal() {
    char cwd[TAM_CAMINHO];
    if (GETCWD(cwd, sizeof(cwd)) == NULL) {
        return NULL;
    }

    char *caminho_final = malloc(TAM_CAMINHO);
    if (caminho_final == NULL) {
        return NULL;
    }

    snprintf(caminho_final, TAM_CAMINHO, "%s/Usuarios/loginUsuarios.txt", cwd);
    return caminho_final;
}

char *obter_caminho_do_usuario(const char *pasta, const char *arquivoTXT){
    char cwd[TAM_CAMINHO];
    if (GETCWD(cwd, sizeof(cwd)) == NULL) {
        return NULL;
    }

    char *caminho_final = malloc(TAM_CAMINHO);
    if (caminho_final == NULL) {
        return NULL;
    }

    snprintf(caminho_final, TAM_CAMINHO, "%s/Usuarios/%s/%s.txt", cwd, pasta, arquivoTXT);
    return caminho_final;

}

void cria_pasta_principal(){
    CRIAR_PASTA("Usuarios");    
}


// 1. Login do usuario para a Agenda
bool login_usuario(Usuarios *usuarios, int qtdUsuarios, char *retorna_caminho){
    char usuario[TAM] = "";
    char usuario_minusculo[TAM] = "";

    bool valida_usuario = false;

    printf("\nDigite o nome do seu usuario(ou digite Q para sair): ");
    if(fgets(usuario, TAM, stdin) == NULL){
        return false;
    }

    usuario[strcspn(usuario, "\n")] = '\0';

    if(usuario[0] == '\0'){
        printf("\n" AMARELO "O nome não pode ficar vazio!" RESET "\n");
        return false;
    }
    
    if((char)tolower((unsigned char)usuario[0]) == 'q'){
        return false;
    }

    for(int i = 0; i < qtdUsuarios; i++){
        if(strcmp(usuarios[i].nome, usuario) == 0){
            valida_usuario = true;
            break;
        }
    }

    if(!valida_usuario){
        printf("\n" VERMELHO "O usuário digitado não existe!" RESET "\n");
        return false;
    }
    
    size_t tamanho = strlen(usuario);

    for(size_t i = 0; i < tamanho; i++){
        usuario_minusculo[i] = (char)tolower((unsigned char)usuario[i]);
    }

    char cwd[TAM_CAMINHO];
    if(GETCWD(cwd, sizeof(cwd)) == NULL){
        printf("\n" VERMELHO "Erro ao buscar a pasta principal" RESET "\n");
        return false;
    }

    snprintf(retorna_caminho, TAM_CAMINHO, "%s/Usuarios/%s/%s.txt", cwd, usuario, usuario_minusculo);
    printf("\n" VERDE "Sucesso ao logar com o usuário" RESET "\n");
    return true;
}

// 2. Cadastra um novo usuário
bool cadastra_usuario(Usuarios **usuarios, int *qtdUsuarios, char *retorna_caminho) {
    char usuario[TAM] = "";
    char usuario_minusculo[TAM] = "";

    printf("\nDigite um nome para seu usuário(ou digite Q para sair): ");
    if (fgets(usuario, TAM, stdin) == NULL) {
        return false;
    }
    usuario[strcspn(usuario, "\n")] = '\0';

    if (usuario[0] == '\0') {
        printf("\n" AMARELO "O nome não pode ficar vazio!" RESET "\n");
        return false;
    }

    if((char)tolower((unsigned char)usuario[0]) == 'q'){
        return false;
    }

    size_t tamanho = strlen(usuario);
    for (size_t i = 0; i < tamanho; i++) {
        usuario_minusculo[i] = (char)tolower((unsigned char)usuario[i]);
    }
    usuario_minusculo[tamanho] = '\0';

    for (int i = 0; i < *qtdUsuarios; i++) {
        if(strcmp(usuario, (*usuarios)[i].nome) == 0){
            printf("\n" VERMELHO "O usuário digitado já existe!" RESET "\n");
            return false;
        }
    }

    char cwd[TAM_CAMINHO];
    if (GETCWD(cwd, sizeof(cwd)) == NULL) {
        printf("\n" VERMELHO "Erro ao buscar a pasta principal" RESET "\n");
        return false;
    }

    char pasta_user[TAM_CAMINHO];
    snprintf(pasta_user, sizeof(pasta_user), "%s/Usuarios/%s", cwd, usuario);
    CRIAR_PASTA(pasta_user);

    char caminho_arquivo[TAM_CAMINHO];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/Usuarios/%s/%s.txt",
             cwd, usuario, usuario_minusculo);

    FILE *arq_novo = fopen(caminho_arquivo, "w");
    if (arq_novo == NULL) {
        printf("\n" VERMELHO "Erro ao criar o arquivo do usuário!" RESET "\n");
        return false;
    }
    fprintf(arq_novo, "Quantidade de eventos: 0\n");
    fclose(arq_novo);

    Usuarios *temp = realloc(*usuarios, sizeof(Usuarios) * (*qtdUsuarios + 1));
    if (temp == NULL) {
        printf("\n" VERMELHO "Erro de memória ao salvar usuário!" RESET "\n");
        return false;
    }

    *usuarios = temp;
    snprintf((*usuarios)[*qtdUsuarios].nome, TAM, "%s", usuario);
    (*usuarios)[*qtdUsuarios].senha[0] = '\0';
    (*qtdUsuarios)++;

    snprintf(retorna_caminho, TAM_CAMINHO, "%s", caminho_arquivo);
    printf("\n" VERDE "Usuário criado com sucesso!" RESET "\n");
    return true;
}

char *usuario(void){

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    Usuarios *usuarios = NULL;

    int opcao;
    int qtdUsuarios = 0;
    
    bool erro = false;
    bool valida_login = false;

    char *retorna_caminho = malloc(TAM_CAMINHO);
    if(retorna_caminho == NULL)
        return NULL;

    retorna_caminho[0] = '\0';

    char *caminho_pasta = obter_pasta_principal();
    if (caminho_pasta == NULL) {
        printf("\n" VERMELHO "Erro ao obter o caminho da base de usuarios!" RESET "\n");
        free(retorna_caminho);
        return NULL;
    }

    cria_pasta_principal();

    FILE *check = fopen(caminho_pasta, "r");
    if (check == NULL) {
        FILE *novo = fopen(caminho_pasta, "w");
        if (novo == NULL) {
            printf("\n" VERMELHO "Erro ao criar loginUsuarios.txt!" RESET "\n");
            free(caminho_pasta);
            free(retorna_caminho);
            return NULL;
        }
        fprintf(novo, "Quantidade de usuários: %d\n", 0);
        fclose(novo);
    } else {
        fclose(check);
    }

    arquivo_usuarios = fopen(caminho_pasta, "r");
    if (arquivo_usuarios != NULL) {
        if (fscanf(arquivo_usuarios, "Quantidade de usuários: %d", &qtdUsuarios) == 1 && qtdUsuarios > 0) {
            Usuarios *temp = realloc(usuarios, sizeof(Usuarios) * qtdUsuarios);
            if (temp != NULL) {
                usuarios = temp;
                int ch;
                while ((ch = fgetc(arquivo_usuarios)) != '\n' && ch != EOF);

                for (int i = 0; i < qtdUsuarios; i++) {
                    if (fgets(usuarios[i].nome, TAM, arquivo_usuarios) != NULL) {
                        usuarios[i].nome[strcspn(usuarios[i].nome, "\n")] = '\0';
                    } else {
                        qtdUsuarios = i;
                        break;
                    }
                }
            } else {
                qtdUsuarios = 0;
            }
        } else {
            qtdUsuarios = 0;
        }
        fclose(arquivo_usuarios);
    }

    do{
        printf("\033[H\33[2J");

        printf("\n\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║" NEGRITO_BRANCO "           ACESSO DE USUÁRIO          " RESET "║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  1. Entrar                           ║\n");
        printf("║  2. Criar novo usuário               ║\n");
        printf("║  3. sair                             ║\n");
        printf("╚══════════════════════════════════════╝\n");

        if (erro) {
            printf("\n" VERMELHO "Ops! Essa opção não está no menu, tente novamente." RESET "\n");
            erro = false;
        }

        printf("\n" AZUL "Escolha a sua opção: " RESET);
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }

        limpar_buffer();

        switch(opcao){
            case 1:
                if(existe_usuarios(qtdUsuarios)){
                    valida_login = login_usuario(usuarios, qtdUsuarios, retorna_caminho);
                
                    if(!valida_login){
                        enter();
                        break;
                    }

                    enter();
                    break;
                }
                
                enter();
                break;

            case 2:
                valida_login = cadastra_usuario(&usuarios, &qtdUsuarios, retorna_caminho);

                if(!valida_login){
                    enter();
                    break;
                }

                enter();
                break;

            case 3:
                printf("\n" NEGRITO_BRANCO "Obrigado por usar o sistema! Volte sempre" RESET "\n");
                free(caminho_pasta);
                free(retorna_caminho);
                free(usuarios);
                return NULL;
            default:
                erro = true;
                break;

        }

    }while(!valida_login);

    arquivo_usuarios = fopen(caminho_pasta, "w");

    fprintf(arquivo_usuarios, "Quantidade de usuários: %d\n", qtdUsuarios);
    
    for(int i = 0; i < qtdUsuarios; i++){
        fprintf(arquivo_usuarios, "%s\n", usuarios[i].nome);
    }

    fclose(arquivo_usuarios);

    return retorna_caminho;

}


