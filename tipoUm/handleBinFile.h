#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
//Infos no arquivo:

#define TAM_KEY 4
#define TAM_LAST_NAME 11
#define TAM_FIRST_NAME 11
#define TAM_ADDRESS 20
#define TAM_CITY 16
#define TAM_STATE 3
#define TAM_ZIP 10
#define TAM_PHONE 16

#define TAM_RECORD TAM_LAST_NAME + TAM_FIRST_NAME + TAM_ADDRESS + TAM_CITY + TAM_STATE + TAM_ZIP + TAM_PHONE + TAM_KEY

#define TAM_INDICE TAM_FIRST_NAME + 5 //tamanho do indice = tamanho do primeiro nome + tamanho de int

//struct para armazenar os indices (FIRST_NAME | posicao relativa no arquivo binario)
typedef struct
{
    char first_n[TAM_FIRST_NAME]; //chave de busca ordenada por FIRST_NAME
    int posicao;                  //posicao referente no arquivo binario

} indice;

//variáveis globais
typedef struct
{
    int key;
    char last_n[TAM_LAST_NAME];
    char first_n[TAM_FIRST_NAME];
    char address[TAM_ADDRESS];
    char city[TAM_CITY];
    char state[TAM_STATE];
    char zip[TAM_ZIP];
    char phone[TAM_PHONE];

} record;

//Lê o input de um novo registro
int read_data(record *registro)
{
    printf("key:");
    scanf("%d", &registro->key);
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

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "ab+");

    fwrite(registro, TAM_RECORD, 1, arquivo);

    fclose(arquivo);

    return (tam_arq(arquivo_name) / (sizeof(record) - 1)) - 1;
}

//função que lê todos os registros do arquivo
void ler_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    int contador = 1;
    if (arquivo != NULL)
    {
        fseek(arquivo, 0, SEEK_SET);
        while (fread(registro, TAM_RECORD, 1, arquivo))
        {
            printf("============= %dº pessoa =========\n", contador);
            mostra_registro(registro);
            contador++;
        }
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }

    fclose(arquivo);
}

//função para buscar um registro pela ordem de adicionado
void buscar_reg(record *registro, int offSet, int use)
{
    if (!use)
    {
        printf("Qual registro deseja buscar: ");
        scanf("%d", &offSet);
    }

    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb+");

    fseek(arquivo, (offSet - 1) * (sizeof(record) - 1), SEEK_SET);
    if (fread(registro, TAM_RECORD, 1, arquivo))
    {
        mostra_registro(registro);

        fclose(arquivo);
        return;
    }
    fclose(arquivo);
    printf("Registro inexistente.\n");
}

//buscar sequencial por key
void buscar_key(record *registro)
{
    int key;
    printf("Digite uma key:");
    scanf("%d", &key);

    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

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

//verifica se duas strings são iguais
//reotna 1 caso seja, 0 caso não sejam
int compara_strings(char n1[], char n2[])
{
    for (int i = 0; n1[i] != '\0'; i++)
    {
        if (n2[i] != n1[i])
        {
            return 0;
        }
    }
    return 1;
}

//função que busca sequencialmente um índice pelo primeiro nome no arquivo de índice
//e dps retorna o registro que aquele índice referencia
void busca_first_name(record *registro)
{
    char first_name[TAM_FIRST_NAME];
    printf("Digite o nome: ");
    scanf("%s", first_name);

    indice *p_indice = malloc(sizeof(indice));

    FILE *arquivoIndice;
    char arquivo_indice[11] = "Indices.bin";
    char arquivo_name[9] = "Dados.bin";

    arquivoIndice = fopen(arquivo_indice, "rb+");

    if (arquivoIndice != NULL)
    {
        int encontrado = 0;
        while (fread(p_indice, TAM_INDICE, 1, arquivoIndice))
        {
            if (compara_strings(first_name, p_indice->first_n))
            {
                printf("posicao = %d\n", p_indice->posicao);

                buscar_reg(registro, p_indice->posicao + 1, 1);

                encontrado = 1;
            }
        }
        if (!encontrado)
            printf("Registro não encontrado!\n");
        fclose(arquivoIndice);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
    free(p_indice);
}