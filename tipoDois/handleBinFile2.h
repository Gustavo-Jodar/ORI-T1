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
//Infos no arquivo:

#define QCAMPOS 7 //quantidade de campos
#define TAM_MAX_CAMPO 64 //tamanho maximo de um campo

//variáveis globais
typedef struct
{
    int key;
    char last_n[TAM_MAX_CAMPO];        //sobrenome da pessoa
    char first_n[TAM_MAX_CAMPO];      //nome da pessoa
    char address[TAM_MAX_CAMPO];  //endereco da pessoa
    char city[TAM_MAX_CAMPO];        //cidade onde a pessoa mora
    char state[TAM_MAX_CAMPO];      //estado onde a pessoa mora
    char zip[TAM_MAX_CAMPO];          //cep
    char phone[TAM_MAX_CAMPO];      //telefone
} record;

//Imprime campo.
void mostra_campo(char data[])
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

//função para printar os registros
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

// Le o campo do input e salva no record "registro"
int obterCampo(record * registro, int campo)
{
    char bufferAux[256];  //buffer utilizado para ler um campo do teclado

    getchar();
    scanf("%[^\n]s",bufferAux);   //obtendo dados de um campo da estrutura pessoa

    //switch para saber de qual campo é aquele dado e salvar no campo especifico do registro. 
    switch(campo) {
        case 0:
            strcpy(registro->last_n, bufferAux);
            break;
        case 1:
            strcpy(registro->first_n, bufferAux);
            break;
        case 2:
            strcpy(registro->address, bufferAux);
            break;
        case 3:
            strcpy(registro->city, bufferAux);
            break;
        case 4:
            strcpy(registro->state, bufferAux);
            break;
        case 5:
            strcpy(registro->zip, bufferAux);
            break;
        case 6:
            strcpy(registro->phone, bufferAux);
            break;
    }

    return strlen(bufferAux);
}

void read_data(FILE *arquivo)
{
    int tam_campos[QCAMPOS]; //variavel utilizada para guardar o tamanho dos campos
    int tam_reg = 0; //tamanho total do registro
    int i = 0; //iterador
    char NomesCampos[QCAMPOS][TAM_MAX_CAMPO] = {"Last name", "First name", "Address", "City", "state", "ZIP (CEP)", "Phone"};

    record registro; //estrutura para guardar o novo registro

    printf("Key: ");
    getchar();
    scanf("%d", &registro.key);

    //le entradas, guarda tamanho do campo e soma no tamanho do registro:
    for (i = 0; i < QCAMPOS; i++) { 
        //obtendo campos e carregando na estrutura pessoa
        printf("%s: ", NomesCampos[i]);
        tam_campos[i] = obterCampo(&registro, i);
        tam_reg+= tam_campos[i];
    }

    //gravando no arquivo tamanhos e os campos do registro
    fwrite(&tam_reg, sizeof(int), 1, arquivo); //grava tamanho do registro

    fwrite(&registro.key, sizeof(int), 1, arquivo);

    fwrite(&tam_campos[0], sizeof(int), 1, arquivo);
    fwrite(&registro.last_n, tam_campos[0], 1, arquivo);

    fwrite(&tam_campos[1], sizeof(int), 1, arquivo);
    fwrite(&registro.first_n, tam_campos[1], 1, arquivo);

    fwrite(&tam_campos[2], sizeof(int), 1, arquivo);
    fwrite(&registro.address, tam_campos[2], 1, arquivo);

    fwrite(&tam_campos[3], sizeof(int), 1, arquivo);
    fwrite(&registro.city, tam_campos[3], 1, arquivo);

    fwrite(&tam_campos[4], sizeof(int), 1, arquivo);
    fwrite(&registro.state, tam_campos[4], 1, arquivo);

    fwrite(&tam_campos[5], sizeof(int), 1, arquivo);
    fwrite(&registro.zip, tam_campos[5], 1, arquivo);

    fwrite(&tam_campos[6], sizeof(int), 1, arquivo);
    fwrite(&registro.phone, tam_campos[6], 1, arquivo);

    putchar('\n');    //pula uma linha no final
}

void escreve_arquivo()
{

    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "ab");

    if(NULL == arquivo) printf("Arquivo especificado nao existe.\n");

    read_data(arquivo);  //obter registro e escrever no arquivo

    fclose(arquivo);
}

int recuperar_registro(FILE *arquivo, record *registro){
    int tam_reg = 0; 
    int tam_campo = 0; 
    int i = 0, j =0; 

    if (fread(&tam_reg, sizeof(int), 1, arquivo)) { 
        //le o tamanho total do registro
        fread(&registro->key,sizeof(int), 1, arquivo);
        while (i < tam_reg) {
            fread(&tam_campo, sizeof(int), 1, arquivo); //le o tamanho do campo
            switch(j) { //salva o campo do arquivo no record registro para podermos imprimir eles. 
                case 0:
                    fread(registro->last_n, tam_campo, 1, arquivo);
                    registro->last_n[tam_campo] = '\0';
                    break;
                case 1:
                    fread(registro->first_n, tam_campo, 1, arquivo);
                    registro->first_n[tam_campo] = '\0';
                    break;
                case 2:
                    fread(registro->address, tam_campo, 1, arquivo);
                    registro->address[tam_campo] = '\0';
                    break;
                case 3:
                    fread(registro->city, tam_campo, 1, arquivo);
                    registro->city[tam_campo] = '\0';
                    break;
                case 4:
                    fread(registro->state, tam_campo, 1, arquivo);
                    registro->state[tam_campo] = '\0';
                    break;
                case 5:
                    fread(registro->zip, tam_campo, 1, arquivo);
                    registro->zip[tam_campo] = '\0';
                    break;
                case 6:
                    fread(registro->phone, tam_campo, 1, arquivo);
                    registro->phone[tam_campo] = '\0';
                    break;
            }
            j++; //passa para o proximo campo do registro. 
            i = i + tam_campo; //Soma o i para verificar se ainda nao terminamos de ler todo o registro
        }
        return 1;
    }
    else
        return 0;
}

//função que lê todos os registros do arquivo
void ler_arquivo()
{
    record registro; //armazena cada registro lido

    FILE *arquivo;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    int contador = 1;
    if (arquivo != NULL)
    {
        while (recuperar_registro(arquivo, &registro))
        {
            printf("============= %dº pessoa =========\n", contador);
            mostra_registro(&registro);
            contador++;
        }
        fclose(arquivo);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
}