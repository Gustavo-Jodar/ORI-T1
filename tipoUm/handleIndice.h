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

int findPlace(indice vet[], char chave[], int Tam)
{
    int i = 0;
    while (compara_nomes(vet[i].first_n, chave) == 1 && i <= Tam)
    {
        i++;
    }
    return i;
}

void printIndice(indice *i)
{
    printf("=========\n");
    printf("Nome: %s\n", i->first_n);
    printf("Posicao: %d\n", i->posicao);
}

void lerIndices()
{
    indice *indice = malloc(sizeof(indice));

    FILE *arquivoIndice;
    char arquivo_name[11] = "Indices.bin";
    arquivoIndice = fopen(arquivo_name, "rb");

    if (arquivoIndice != NULL)
    {
        fseek(arquivoIndice, 0, SEEK_SET);
        while (fread(indice, TAM_INDICE, 1, arquivoIndice))
        {
            printIndice(indice);
        }
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }

    fclose(arquivoIndice);
}

void escreveIndice(char nome[], int posicao)
{
    indice *p_i1 = malloc(sizeof(indice));
    copy_name(p_i1->first_n, nome);
    p_i1->posicao = posicao;

    FILE *arquivoIndice;

    char arquivo_name[11] = "Indices.bin";

    arquivoIndice = fopen(arquivo_name, "ab");

    fwrite(p_i1, TAM_INDICE, 1, arquivoIndice);

    fclose(arquivoIndice);

    free(p_i1);
}