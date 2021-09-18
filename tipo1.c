#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Infos no arquivo:

#define TAM_LAST_NAME 11
#define TAM_FIRST_NAME 11
#define TAM_ADDRESS 20
#define TAM_CITY 16
#define TAM_STATE 3
#define TAM_ZIP 10
#define TAM_PHONE 16

#define TAM_RECORD TAM_LAST_NAME + TAM_FIRST_NAME + TAM_ADDRESS + TAM_CITY + TAM_STATE + TAM_ZIP + TAM_PHONE
//variáveis globais
typedef struct
{
    char last_n[TAM_LAST_NAME];
    char first_n[TAM_FIRST_NAME];
    char address[TAM_ADDRESS];
    char city[TAM_CITY];
    char state[TAM_STATE];
    char zip[TAM_ZIP];
    char phone[TAM_PHONE];

} record;

//Funções
//Lê o input de um novo registro
int read_data(record *);
//Imprime uma string
void mostra_registro(record *);
//Função que escreve record no arquivo
void escreve_arquivo(record *);
//função que lê um record do arquivo
void ler_arquivo(record *);

int main()
{
    record registro;
    record *p_registro = &registro;
    int opcao;
    printf("1 - escrever\n2 - ler\nOpcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        read_data(p_registro);
        escreve_arquivo(p_registro);
        break;

    case 2:
        ler_arquivo(p_registro);
        mostra_registro(p_registro);
        break;
    default:
        printf("flw");
        break;
    }
}

// função para ler os dados de um rgistro
int read_data(record *registro)
{
    printf("Last name:");
    scanf("%s", registro->last_n);
    printf("First name:");
    scanf("%s", registro->first_n);
    printf("Address:");
    scanf("%s", registro->address);
    printf("City:");
    scanf("%s", registro->city);
    printf("state:");
    scanf("%s", registro->state);
    printf("ZIP (CEP):");
    scanf("%s", registro->zip);
    printf("Phone:");
    scanf("%s", registro->phone);

    return 1;
}

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
    mostra_campo(registro->last_n);
    mostra_campo(registro->first_n);
    mostra_campo(registro->address);
    mostra_campo(registro->city);
    mostra_campo(registro->state);
    mostra_campo(registro->zip);
    mostra_campo(registro->phone);
}

void escreve_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "w");

    fwrite(registro, TAM_RECORD, 1, arquivo);
}

void ler_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "r");

    fread(registro->last_n, TAM_LAST_NAME, 1, arquivo);
    fread(registro->first_n, TAM_FIRST_NAME, 1, arquivo);
    fread(registro->address, TAM_ADDRESS, 1, arquivo);
    fread(registro->city, TAM_CITY, 1, arquivo);
    fread(registro->state, TAM_STATE, 1, arquivo);
    fread(registro->zip, TAM_ZIP, 1, arquivo);
    fread(registro->phone, TAM_PHONE, 1, arquivo);
}
