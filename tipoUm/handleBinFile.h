/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

//tamanhos de cada campo do registro
#define TAM_DELETADO 4
#define TAM_KEY 4
#define TAM_LAST_NAME 11
#define TAM_FIRST_NAME 11
#define TAM_ADDRESS 20
#define TAM_CITY 16
#define TAM_STATE 3
#define TAM_ZIP 10
#define TAM_PHONE 16

//tamanho total do registro
#define TAM_RECORD TAM_LAST_NAME + TAM_FIRST_NAME + TAM_ADDRESS + TAM_CITY + TAM_STATE + TAM_ZIP + TAM_PHONE + TAM_KEY + TAM_DELETADO + 1

//tamanho do indice
#define TAM_INDICE TAM_FIRST_NAME + TAM_KEY + TAM_DELETADO + 1 //tamanho do indice = tamanho do primeiro nome + tamanho de int

//função que retorna uma posicao livre armazenada no arquivo de posicoes livres ou -1 se nao tiver nenhuma
int getPosicaoLivre(void);

//struct para armazenar os indices (FIRST_NAME | posicao relativa no arquivo binario)
typedef struct
{
    int deletado;
    char first_n[TAM_FIRST_NAME]; //chave de busca ordenada por FIRST_NAME
    int posicao;                  //posicao referente no arquivo binario

} indice;

//struct do registro
typedef struct
{
    int deletado;
    int key;
    char last_n[TAM_LAST_NAME];
    char first_n[TAM_FIRST_NAME];
    char address[TAM_ADDRESS];
    char city[TAM_CITY];
    char state[TAM_STATE];
    char zip[TAM_ZIP];
    char phone[TAM_PHONE];

} record;

//função que verifica se a KEY escolhida já existe no arquivo
//retorna 1 se a key ainda nao existe
//retorna 0 se ela já existe
int verificaKey(int nova_key)
{
    FILE *arquivo;
    char arquivo_name[10] = "Dados.bin";
    arquivo = fopen(arquivo_name, "rb+");

    record *registro_aux = (record *)malloc(sizeof(record));
    int achou_igual = 0;

    if (arquivo != NULL)
    {
        while (fread(registro_aux, TAM_RECORD, 1, arquivo) && !achou_igual)
            if (registro_aux->key == nova_key && registro_aux->deletado != 1)
                achou_igual = 1;

        fclose(arquivo);
        free(registro_aux);

        if (!achou_igual)
            return 1;
        else
        {
            printf("Key já existente! Digite uma nova key.\n");
            return 0;
        }
    }
    else
    {
        free(registro_aux);
        return 1;
    }
}

//Lê o input de um novo registro
int read_data(record *registro)
{
    registro->deletado = 0;
    int key_unica = 0;
    while (!key_unica)
    {
        printf("key:");
        scanf("%d", &registro->key);
        key_unica = verificaKey(registro->key);
    }
    printf("Last name:");
    getchar();
    scanf("%[^\n]s", registro->last_n);
    printf("First name:");
    getchar();
    scanf("%[^\n]s", registro->first_n);
    getchar();
    printf("Address:");
    scanf("%[^\n]s", registro->address);
    getchar();
    printf("City:");
    scanf("%[^\n]s", registro->city);
    printf("state:");
    scanf("%s", registro->state);
    printf("ZIP (CEP):");
    scanf("%s", registro->zip);
    printf("Phone:");
    scanf("%s", registro->phone);

    return 1;
}

//funcao que calcula tamanho do arquivo
int tam_arq(char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

//Imprime um com de string
void mostra_campo(char data[])
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

//função para printar variável de um campo
void mostra_registro(record *registro)
{
    printf("Key: %d\n", registro->key);
    printf("Last name: ");
    mostra_campo(registro->last_n);
    printf("First name: ");
    mostra_campo(registro->first_n);
    printf("Address: ");
    mostra_campo(registro->address);
    printf("City: ");
    mostra_campo(registro->city);
    printf("State: ");
    mostra_campo(registro->state);
    printf("ZIP (CEP): ");
    mostra_campo(registro->zip);
    printf("Phone: ");
    mostra_campo(registro->phone);
}

//Função que escreve record no arquivo
//E retorna a posição que foi escrito o novo registro
int escreve_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb+");

    if (arquivo != NULL)
    {
        //verifica se existe uma posicao livre (algum registro excluído logicamente)
        int posicao_livre = getPosicaoLivre();

        if (posicao_livre != -1)
        {
            printf("\nESPAÇO LIVRE DETECTADO: %d\n", posicao_livre);
            fseek(arquivo, posicao_livre * sizeof(record), SEEK_SET);
            fwrite(registro, TAM_RECORD, 1, arquivo);
            fclose(arquivo);
            //retorna a posicao que ele foi inserido
            return posicao_livre;
        }
    }
    //se o arquivo ainda nao existe
    else
        arquivo = fopen(arquivo_name, "ab");

    //adiciona na ultima posicao (nao tinha espaços vazios)
    fseek(arquivo, 0, SEEK_END);
    fwrite(registro, TAM_RECORD, 1, arquivo);
    fclose(arquivo);
    //retorna a ultima posicao
    return (tam_arq(arquivo_name) / sizeof(record)) - 1;
}

//função que lê todos os registros do arquivo
void ler_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    int contador = 1;
    if (arquivo != NULL)
    {
        fseek(arquivo, 0, SEEK_SET);
        while (fread(registro, TAM_RECORD, 1, arquivo))
        {
            if (!registro->deletado)
            {
                printf("============= %dº pessoa =========\n", contador);
                mostra_registro(registro);
                contador++;
            }
        }
        fclose(arquivo);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
}

//função para buscar um registro pela sua posicao no arquivo
//tambem utilizada em outras partes do programa para adquirir um registro por um offset
//se a flag "use" estiver setada, ele usa o offset dado nos parâmetros, caso contrário o usuário que fornece esse offset
void buscar_reg(record *registro, int offSet, int use)
{

    if (!use)
    {
        printf("Qual registro deseja buscar: ");
        scanf("%d", &offSet);
    }

    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb+");

    fseek(arquivo, offSet * sizeof(record), SEEK_SET);
    if (fread(registro, TAM_RECORD, 1, arquivo))
    {
        mostra_registro(registro);

        fclose(arquivo);
        return;
    }
    fclose(arquivo);
    printf("Registro inexistente.\n");
}

//fuynção que busca sequencialmente por uma key
void buscar_key(record *registro)
{
    int key;
    printf("Digite uma key:");
    scanf("%d", &key);

    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    if (arquivo != NULL)
    {
        fseek(arquivo, 0, SEEK_SET);
        while (fread(registro, TAM_RECORD, 1, arquivo))
        {
            if (key == registro->key)
            {
                mostra_registro(registro);

                fclose(arquivo);
                return;
            }
        }

        fclose(arquivo);
        printf("Registro com key %d não encontrado!\n", key);
    }
}
