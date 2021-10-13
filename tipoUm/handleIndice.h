/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - ??????

*/

#include "handleBinFile.h"

//funcao que copia uma string em n2 para n1
void copy_name(char n1[], char n2[])
{
    int i;
    for (i = 0; n2[i] != '\0'; i++)
    {
        n1[i] = n2[i];
    }
    n1[i] = '\0';
}

//funcao para comparar alfabeticamente dois nomes
//retorna 1 se n1 vier antes
//retorna 2 se n2 vier antes
int compara_nomes(char n1[], char n2[])
{
    for (int i = 0; n1[i] != '\0'; i++)
    {
        if (n1[i] < n2[i] && n2[i] != '\0')
            return 1;
        if (n1[i] > n2[i] && n2[i] != '\0')
            return 2;
        if (n2[i] == '\0')
            return 2;
    }
    return 1;
}

//função que verifica se duas strings são identicas
int nomes_iguais(char n1[], char n2[])
{
    for (int i = 0; n1[i] != '\0'; i++)
    {
        if (n1[i] != n2[i])
            return 0;
    }
    return 1;
}

//função que printa a estrutura de indíce
void printIndice(indice *i)
{
    printf("=========\n");
    printf("Nome: %s\n", i->first_n);
    printf("Posicao: %d\n", i->posicao);
}

//função que lê o conteúdo do arquivo de índices
void lerIndices()
{
    indice *indice = malloc(sizeof(indice));

    FILE *arquivoIndice;
    char arquivo_name[11] = "Indices.bin";
    arquivoIndice = fopen(arquivo_name, "rb");

    if (arquivoIndice != NULL)
    {
        while (fread(indice, TAM_INDICE, 1, arquivoIndice))
        {
            printIndice(indice);
        }
        fclose(arquivoIndice);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
}

//função que acha a posicao que deve ser inserido o novo indice
int findPosition(char nome[])
{
    FILE *arquivoIndice;
    char arquivo_name[11] = "Indices.bin";
    arquivoIndice = fopen(arquivo_name, "rb");

    //indice auxiliar
    indice *p_aux = malloc(sizeof(indice));

    int posicao = 0;

    //se aquivo existir
    if (arquivoIndice != NULL)
    {
        //determina a posição a ser inserido
        while (fread(p_aux, TAM_INDICE, 1, arquivoIndice))
        {
            if (compara_nomes(p_aux->first_n, nome) == 2)
            {
                fclose(arquivoIndice);
                return posicao;
            }
            posicao++;
        }
        posicao++;
        fclose(arquivoIndice);
        free(p_aux);
        return posicao;
    }
    else
    {
        free(p_aux);
        return -1;
    }
}

//função que avança um numero específico de registros (contando a partir do fim)
void avancaRegistrosFrente(int qntd_avancar)
{
    //abrindo arquivo de indices e ajustando ponteiro para o final
    char arquivo_name[11] = "Indices.bin";
    FILE *arquivoIndice;
    arquivoIndice = fopen(arquivo_name, "rb+");
    fseek(arquivoIndice, 0, SEEK_END);

    //indice auxiliar
    indice *p_aux = malloc(sizeof(indice));

    //avançando registros
    for (int i = 0; i < qntd_avancar; i++)
    {
        fseek(arquivoIndice, -1 * sizeof(indice), SEEK_CUR);
        fread(p_aux, TAM_INDICE, 1, arquivoIndice);
        fwrite(p_aux, TAM_INDICE, 1, arquivoIndice);
        fseek(arquivoIndice, -2 * sizeof(indice), SEEK_CUR);
    }
    fclose(arquivoIndice);
    free(p_aux);
}

//funão que adiciona um indice de forma ordenada no arquivo
//escolhendo o local a ser inserido e ajustando a posição dos demais
void escreveIndiceOrdenado(char nome[], int posicao)
{
    char arquivo_name[11] = "Indices.bin";

    //novo indice
    indice *p_novo = malloc(sizeof(indice));
    copy_name(p_novo->first_n, nome);
    p_novo->posicao = posicao;

    //posicao a ser inserido o novo indice
    int i = findPosition(nome);

    //arquivo nao existe OU está no último a ser inserido
    if (i == -1 || tam_arq(arquivo_name) < i * sizeof(indice))
    {
        //criando arquivo para escrita
        FILE *arquivoIndice;
        arquivoIndice = fopen(arquivo_name, "ab");
        //inserindo na última posição
        fwrite(p_novo, TAM_INDICE, 1, arquivoIndice);
        fclose(arquivoIndice);
        free(p_novo);
        return;
    }

    //arquivo existe e registro não está no final
    //avançando registros em uma posicao para adicionar ordenadamente
    int qntd_avancar = (tam_arq(arquivo_name) - (sizeof(indice) * i)) / sizeof(indice);
    avancaRegistrosFrente(qntd_avancar);

    //adicionando registro na posicao correta p/ ordenação
    FILE *arquivoIndice;
    arquivoIndice = fopen(arquivo_name, "rb+");
    fseek(arquivoIndice, i * sizeof(indice), SEEK_SET);
    fwrite(p_novo, TAM_INDICE, 1, arquivoIndice);

    fclose(arquivoIndice);
    free(p_novo);
}

//retorna 2 se nomes forem iguais
//1 se o nome procurado for maior
//0 se o nome procurado for menor
int verificaIndice(int i, char nome[], indice *p_aux)
{
    FILE *arquivoIndice;
    arquivoIndice = fopen("Indices.bin", "rb+");
    fseek(arquivoIndice, i * sizeof(indice), SEEK_SET);

    fread(p_aux, TAM_INDICE, 1, arquivoIndice);
    fclose(arquivoIndice);

    if (nomes_iguais(p_aux->first_n, nome))
    {
        return 2;
    }
    else if (compara_nomes(nome, p_aux->first_n) == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int buscaBinariaIndice(char nome[], indice *p_aux)
{
    int begin = 0;
    int end = tam_arq("Indices.bin") / sizeof(indice);
    while (begin <= end)
    {

        int i = (begin + end) / 2;               /* Calcula o meio do sub-vetor */
        if (verificaIndice(i, nome, p_aux) == 2) // achou nome
        {
            return i;
        }

        if (verificaIndice(i, nome, p_aux) == 1) //nome está a direita
        {
            begin = i + 1;
        }
        else // nome está a esquerda
        {
            end = i;
        }
    }
    return -1;
}

void busca_bin_first_name(record *registro)
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
        int i = buscaBinariaIndice(first_name, p_indice);
        if (i == -1)
            printf("Registro não encontrado!\n");
        else
        {
            printf("Registro encontrado!\n\n");

            buscar_reg(registro, p_indice->posicao, 1);
        }
        fclose(arquivoIndice);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
    free(p_indice);
}