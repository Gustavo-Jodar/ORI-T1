/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include "handleIndice.h"

void lerExcluidos()
{
    FILE *arquivoRemovidos;
    char arquivo_name[14] = "Removidos.bin";
    //abre apenas para escrita no fim e se nao existir cria
    arquivoRemovidos = fopen(arquivo_name, "rb+");

    int *p_aux = (int *)malloc(sizeof(int));

    if (arquivoRemovidos != NULL)
    {
        while (fread(p_aux, sizeof(int), 1, arquivoRemovidos))
        {
            printf("==============\n");
            printf("Vazio na posicao: %d\n", *p_aux);
        }
        fclose(arquivoRemovidos);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
}

//função que adiciona a posição de um registro vazio no arquivo de espaços vazios
//E marca o índice que apontava para aquele registro no arquivo de indices como excluído
void adicionaRegRemovido(int posicao, char nome[])
{
    FILE *arquivoRemovidos;
    char arquivo_name[14] = "Removidos.bin";
    int *p_posicao = &posicao;
    int *p_aux = (int *)malloc(sizeof(int));

    arquivoRemovidos = fopen(arquivo_name, "rb+");
    //abre apenas para escrita no fim e criando o arquivo
    if (arquivoRemovidos == NULL)
    {
        arquivoRemovidos = fopen(arquivo_name, "ab");
        //adiciona a posicao do item logicamente excluído no arquivo de indices excluídos
        fwrite(p_posicao, sizeof(int), 1, arquivoRemovidos);
        fclose(arquivoRemovidos);
        return;
    }

    //se o arquivo já existe procura por um -1 ou o final do arquivo para inserir uma nova posicao vazia
    while (fread(p_aux, sizeof(int), 1, arquivoRemovidos))
    {
        if (*p_aux == -1)
        {
            printf("Achou um -1");
            fseek(arquivoRemovidos, -1 * sizeof(int), SEEK_CUR);
            break;
        }
    }
    free(p_aux);
    fwrite(p_posicao, sizeof(int), 1, arquivoRemovidos);
    fclose(arquivoRemovidos);
}

void marcaRegistroEIndiceComoExcluido(int posicao_indice, indice *p_indice)
{
    FILE *arquivoIndice;
    FILE *arquivoDados;
    char arquivo_indice[12] = "Indices.bin";
    char arquivo_name[10] = "Dados.bin";

    //MARCANDO O INDICE COMO DELETADO
    arquivoIndice = fopen(arquivo_indice, "rb+");
    fseek(arquivoIndice, posicao_indice * sizeof(indice), SEEK_SET);
    fread(p_indice, TAM_INDICE, 1, arquivoIndice);
    printf("Indice lido:\n");
    printIndice(p_indice);
    p_indice->deletado = 1;
    fseek(arquivoIndice, -1 * sizeof(indice), SEEK_CUR);
    fwrite(p_indice, TAM_INDICE, 1, arquivoIndice);
    fclose(arquivoIndice);

    //adiciona a posição do registro deletado no arquivo de excluidos
    adicionaRegRemovido(p_indice->posicao, p_indice->first_n);

    //retira indice excluido do arquivo de indices
    refatoraArquivoIndice(posicao_indice);

    //marcando o registro como deletado
    record *p_record = (record *)malloc(sizeof(record));
    arquivoDados = fopen(arquivo_name, "rb+");
    fseek(arquivoDados, p_indice->posicao * sizeof(record), SEEK_SET);
    fread(p_record, TAM_RECORD, 1, arquivoDados);
    printf("\nO registro a ser deletado:\n");
    mostra_registro(p_record);
    printf("\nFoi excluído logicamente com sucesso!\n");
    p_record->deletado = 1;
    fseek(arquivoDados, -1 * sizeof(record), SEEK_CUR);
    fwrite(p_record, TAM_RECORD, 1, arquivoDados);
    fclose(arquivoDados);
    free(p_record);
}

void excluirLogicamenteRegistro()
{
    char first_name[TAM_FIRST_NAME];
    printf("First name do registro a ser excluído: ");
    getchar();
    scanf("%[^\n]s", first_name);
    getchar();

    //procura indice daquele registro no arquivo de indices
    indice *p_indice = (indice *)malloc(sizeof(indice));
    int posi_indice = buscaBinariaIndice(first_name, p_indice);

    printf("Posi_indice: %d\n", posi_indice);
    //nao achou o nome especificado no arquivo de indices
    if (posi_indice != -1)
    {
        marcaRegistroEIndiceComoExcluido(posi_indice, p_indice);
    }
    else
    {
        printf("Nome não encontrado!\n");
    }
}

int getPosicaoLivre()
{
    FILE *arquivoRemovidos;
    char arquivo_name[14] = "Removidos.bin";
    arquivoRemovidos = fopen(arquivo_name, "rb+");

    if (arquivoRemovidos != NULL)
    {
        int *p_posicao = (int *)malloc(sizeof(int));
        int posicao_livre;

        //se possuir espaço vazio
        if (fread(p_posicao, sizeof(int), 1, arquivoRemovidos))
        {
            posicao_livre = *p_posicao;
            free(p_posicao);

            //cria um novo arquivo de registros vazios sem a primeira posicao
            int qntd_registros_vazios_restantes = (tam_arq(arquivo_name) / sizeof(int)) - 1;
            int *p_posicoes_restantes = (int *)malloc(qntd_registros_vazios_restantes * sizeof(int));
            fread(p_posicoes_restantes, qntd_registros_vazios_restantes * sizeof(int), 1, arquivoRemovidos);
            fclose(arquivoRemovidos);
            arquivoRemovidos = fopen(arquivo_name, "wb+");
            fwrite(p_posicoes_restantes, qntd_registros_vazios_restantes * sizeof(int), 1, arquivoRemovidos);
            free(p_posicoes_restantes);
            fclose(arquivoRemovidos);

            return posicao_livre;
        }
        return -1;
    }
    else
    {
        return -1;
    }
}
