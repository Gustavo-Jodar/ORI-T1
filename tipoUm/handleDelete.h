/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include "handleIndice.h"

//função que lê o conteudo do arquivo de posicoes vazias
void lerExcluidos()
{
    FILE *arquivoRemovidos;
    char arquivo_name[14] = "Removidos.bin";
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
//remove o índice que apontava para aquele registro no arquivo de indice
void adicionaRegRemovido(int posicao, char nome[])
{
    FILE *arquivoRemovidos;
    char arquivo_name[14] = "Removidos.bin";
    int *p_posicao = &posicao;
    int *p_aux = (int *)malloc(sizeof(int));

    arquivoRemovidos = fopen(arquivo_name, "rb+");

    // se o arquivo ainda nao existe, abre apenas para escrita no fim e criando o arquivo
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

//função que marca um registro como excluído, adiciona sua posicao (offset) no arquivo de posicoes vazias
// e remove seu indice do arquivo de indices
void marcaRegistroEIndiceComoExcluido(int posicao_indice, indice *p_indice)
{
    FILE *arquivoIndice;
    FILE *arquivoDados;
    char arquivo_indice[12] = "Indices.bin";
    char arquivo_name[10] = "Dados.bin";

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

//funçãoq que le o nome a ser excluído logicamente e chama a funação marcaRegistroEIndiceComoExcluido()
void excluirLogicamenteRegistro()
{
    //lendo o nome a ser excluído
    char first_name[TAM_FIRST_NAME];
    printf("First name do registro a ser excluído: ");
    getchar();
    scanf("%[^\n]s", first_name);
    getchar();

    //procura indice daquele registro no arquivo de indices
    indice *p_indice = (indice *)malloc(sizeof(indice));
    int posi_indice = buscaBinariaIndice(first_name, p_indice);

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

//função que devolve uma posicao livre do arquivo de posicoes livres e remove a posicao dadas do mesmo (ela será reutilizada)
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

            //adiciona as posicoes restantes em um vetor
            int qntd_registros_vazios_restantes = (tam_arq(arquivo_name) / sizeof(int)) - 1;
            int *p_posicoes_restantes = (int *)malloc(qntd_registros_vazios_restantes * sizeof(int));
            fread(p_posicoes_restantes, qntd_registros_vazios_restantes * sizeof(int), 1, arquivoRemovidos);
            fclose(arquivoRemovidos);

            //cria um novo arquivo de registros vazios sem a primeira posicao (a que será reutilizada)
            arquivoRemovidos = fopen(arquivo_name, "wb+");
            fwrite(p_posicoes_restantes, qntd_registros_vazios_restantes * sizeof(int), 1, arquivoRemovidos);
            free(p_posicoes_restantes);
            fclose(arquivoRemovidos);

            return posicao_livre;
        }
        //se o arquivo existir mas nao possui posicoes livres
        return -1;
    }
    //se o arquivo ainda nao existe
    else
    {
        return -1;
    }
}
