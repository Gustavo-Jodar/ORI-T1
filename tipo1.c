#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

//Funções
//Lê o input de um novo registro
int read_data(record *);
//Imprime uma string
void mostra_registro(record *);
//Função que escreve record no arquivo
void escreve_arquivo(record *);
//função que lê um record do arquivo
void ler_arquivo(record *);
//função para buscar um registro
void buscar_reg(record *);
//buscar sequencial por key
void buscar_key(record *);

int main()
{
    record registro;
    record *p_registro = &registro;
    int opcao;
    int continuar = 1;
    while (opcao != 5)
    {
        printf("\n\n===========================================\n");
        printf("1-Escrever registro\n");
        printf("2-Ler todos os registros\n");
        printf("3-Buscar registro por ordem de escrito\n");
        printf("4-Buscar registro por key\n");
        printf("5-Terminar\n");
        printf("===========================================\n");
        printf("Opção:");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            while (continuar == 1)
            {
                read_data(p_registro);
                escreve_arquivo(p_registro);
                printf("\nAdicionar mais um registro? Digite 1 para continuar.\n");
                printf("Continuar: ");
                scanf("%d", &continuar);
            }
            break;
        case 2:
            ler_arquivo(p_registro);
            break;
        case 3:
            buscar_reg(p_registro);
            break;
        case 4:
            buscar_key(p_registro);
            break;
        case 5:
            printf("Terminando...\n");
            break;
        default:
            printf("Opção inválida! Digite novamente.\n");
            break;
        }
    }
}

// função para ler os dados de um rgistro
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

void escreve_arquivo(record *registro)
{
    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "ab");

    fwrite(registro, TAM_RECORD, 1, arquivo);

    fclose(arquivo);
}

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

void buscar_reg(record *registro)
{
    int num_reg;
    printf("Qual registro deseja buscar: ");
    scanf("%d", &num_reg);

    FILE *arquivo;

    char arquivo_name[9] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    int contador = 1;
    while (fread(registro, TAM_RECORD, 1, arquivo))
    {
        if (contador == num_reg)
        {
            mostra_registro(registro);

            fclose(arquivo);
            return;
        }
        contador++;
    }

    fclose(arquivo);
    printf("Registro inexistente.\n");
}

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