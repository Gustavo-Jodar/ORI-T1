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

//função para buscar um registro pela sua posicao no arquivo
//tambem utilizada em outras partes do programa para adquirir um registro por um offset
//se a flag "use" estiver setada, ele usa o offset dado nos parâmetros, caso contrário o usuário que fornece esse offset
void buscar_reg(int offSet, int use)
{

    if (!use)
    {
        printf("Qual registro deseja buscar: ");
        scanf("%d", &offSet);
    }

    FILE *arquivo;
    char arquivo_name[10] = "Dados.bin";
    arquivo = fopen(arquivo_name, "rb+");

    record *registro = malloc(sizeof(record));

    fseek(arquivo, offSet, SEEK_SET);
    if (recuperar_registro(arquivo, registro))
    {
        mostra_registro(registro);
        free(registro);
        fclose(arquivo);
        return;
    }
    fclose(arquivo);
    printf("Registro inexistente.\n");
}

//funcao que calcula tamanho do arquivo
int tam_arq(char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

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
    printf("Deletado: %d\n", i->deletado);
}

//função que lê todo conteúdo do arquivo de índices
void lerIndices()
{
    FILE *arquivoIndice;
    char arquivo_name[12] = "Indices.bin";
    arquivoIndice = fopen(arquivo_name, "rb");
    indice *p_indice = (indice *)malloc(sizeof(indice));

    if (arquivoIndice != NULL)
    {
        while (fread(p_indice, TAM_INDICE, 1, arquivoIndice))
        {
            printIndice(p_indice);
        }
        fclose(arquivoIndice);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
}

//função que acha a posicao que deve ser inserido o novo indice ou -1 se o arquivo de indices ainda nao existir
int findPosition(char nome[])
{
    FILE *arquivoIndice;
    char arquivo_name[12] = "Indices.bin";
    arquivoIndice = fopen(arquivo_name, "rb");

    //indice auxiliar
    indice *p_aux = (indice *)malloc(sizeof(indice));

    int posicao = 0;

    //se aquivo existir
    if (arquivoIndice != NULL)
    {
        //determina a posição a ser inserido buscando sequencialmente no arquivo de indices
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
//utilizada na inserção ordenada para poder adicionar um indice sem que a ordem dos outros seja perdida
void avancaRegistrosFrente(int qntd_avancar)
{
    //abrindo arquivo de indices e ajustando ponteiro para o final
    char arquivo_name[12] = "Indices.bin";
    FILE *arquivoIndice;
    arquivoIndice = fopen(arquivo_name, "rb+");
    fseek(arquivoIndice, 0, SEEK_END);

    //indice auxiliar
    indice *p_aux = (indice *)malloc(sizeof(indice));

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

    char arquivo_name[12] = "Indices.bin";

    //novo indice
    indice *p_novo = (indice *)malloc(sizeof(indice));
    copy_name(p_novo->first_n, nome);
    p_novo->posicao = posicao;
    p_novo->deletado = 0;

    //posicao a ser inserido o novo indice
    int i = findPosition(nome);

    //arquivo nao existe OU está no último a ser inserido
    if (i == -1 || tam_arq(arquivo_name) < i * sizeof(indice))
    {
        //criando arquivo para escrita ou abrindo um existente já com o ponteiro no final
        FILE *arquivoIndice;
        arquivoIndice = fopen(arquivo_name, "ab");
        //inserindo na última posição
        fwrite(p_novo, TAM_INDICE, 1, arquivoIndice);
        fclose(arquivoIndice);
        free(p_novo);
        return;
    }

    //arquivo existe e registro não está no final
    //determina a quantidade de registros que devem ser deslocados para adiconar o novo indice
    int qntd_avancar = (tam_arq(arquivo_name) - (sizeof(indice) * i)) / sizeof(indice);
    //avançando registros em uma posicao para adicionar ordenadamente
    avancaRegistrosFrente(qntd_avancar);

    //adicionando registro na posicao correta p/ ordenação
    FILE *arquivoIndice;
    arquivoIndice = fopen(arquivo_name, "rb+");
    fseek(arquivoIndice, i * sizeof(indice), SEEK_SET);
    fwrite(p_novo, TAM_INDICE, 1, arquivoIndice);

    fclose(arquivoIndice);
    free(p_novo);
}

//colocando por refrência o conteudo do indice encontrado em p_aux
//função utilizada na busca binária por nomes no arquivo de indice
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

    //se os nomes forem iguais
    if (nomes_iguais(p_aux->first_n, nome))
    {
        return 2;
    }
    //se o nome procurado é menor do que o atual
    else if (compara_nomes(nome, p_aux->first_n) == 1)
    {
        return 0;
    }
    //se o nome procurado é maior do que o atual
    else
    {
        return 1;
    }
}

//função que implemnta a busca binária no arquivo de indices, utilizando da verificaIndice()
//colocando por refrência o conteudo do indice encontrado em p_indice
int buscaBinariaIndice(char nome[], indice *p_aux)
{
    int begin = 0;
    int end = (tam_arq("Indices.bin") / sizeof(indice)) - 1;
    int i_passado = -1;
    while (begin <= end)
    {
        int i = (begin + end) / 2; /* Calcula o meio do sub-vetor */
        //garantia de nao ficar em loop infinito
        if (i_passado == i)
            break;
        else
            i_passado = i;

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
    //nome nao encontrado
    return -1;
}

//função que utiliza a busca binária para recuperar um registro cujo first_name foi dado pelo usuario
void busca_bin_first_name(record *registro)
{
    //adquire o first_name
    char first_name[TAM_MAX_CAMPO];
    printf("Digite o nome: ");
    getchar();
    scanf("%[^\n]s", first_name);
    getchar();

    indice *p_indice = (indice *)malloc(sizeof(indice));

    FILE *arquivoIndice;
    char arquivo_indice[12] = "Indices.bin";
    char arquivo_name[10] = "Dados.bin";

    arquivoIndice = fopen(arquivo_indice, "rb+");

    if (arquivoIndice != NULL)
    {
        //acha a posicao em que se encontra o indice com o first_name
        //o conteudo do indice encontrado é armazenado em p_indice
        int i = buscaBinariaIndice(first_name, p_indice);
        if (i == -1)
            printf("Registro não encontrado!\n");
        else
        {
            printf("Registro encontrado!\n\n");
            //utiliza da função de busca por offset utilizando o offset contido no indice encontrado
            buscar_reg(p_indice->posicao, 1);
        }
        fclose(arquivoIndice);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
    free(p_indice);
}

//função retira uma posicao deletada do arquivo de indices
void refatoraArquivoIndice(int posicao_deletado)
{
    FILE *arquivoIndice;
    arquivoIndice = fopen("Indices.bin", "rb+");

    //aloca espaco para ler todos os indices contidos no arquivo de indices
    int numero_indices = tam_arq("Indices.bin") / sizeof(indice);
    indice *p_indice = (indice *)malloc(numero_indices * sizeof(indice));

    //lê todos os indices do arquivo de indices
    fread(p_indice, sizeof(indice), numero_indices, arquivoIndice);

    //retira o indices deletado, escrevendo os seus subsequentes uma posicao a menos
    for (int i = posicao_deletado; i < (numero_indices - 1); i++)
    {
        p_indice[i] = p_indice[i + 1];
    }
    fclose(arquivoIndice);

    //reescreve o arquivo com um indice a menos
    arquivoIndice = fopen("Indices.bin", "wb+");
    fwrite(p_indice, (numero_indices - 1) * sizeof(indice), 1, arquivoIndice);
    fclose(arquivoIndice);
    free(p_indice);
}

int rec_reg(FILE *arquivo, record *registro, int *tamanho, int *passar)
{
    int tam_reg = 0, tam_reg_excluido = 0;
    int tam_campo = 0;
    int i = 0, j = 0;
    if (fread(&tam_reg, sizeof(int), 1, arquivo))
    {
        *tamanho = tam_reg;
        //le o tamanho total do registro
        fread(&registro->key, sizeof(int), 1, arquivo);
        *passar = 1;
        if (registro->key == -1)
        {
            fseek(arquivo, tam_reg - (2 * sizeof(int)), SEEK_CUR);
            *passar = 0;
            *tamanho = tam_reg;
            return 1;
        }

        while (i < tam_reg)
        {
            fread(&tam_campo, sizeof(int), 1, arquivo); //le o tamanho do campo
            switch (j)
            { //salva o campo do arquivo no record registro para podermos imprimir eles.
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
            j++;               //passa para o proximo campo do registro.
            i = i + tam_campo; //Soma o i para verificar se ainda nao terminamos de ler todo o registro
        }
        return tam_reg;
    }
    else
        return 0;
}

//função que lê todos os registros e monta um arquivo de indices
int criaIndices()
{
    FILE *arquivoDados, *arquivoIndice;

    //reescreve o arquivo de indices
    arquivoDados = fopen("Dados.bin", "rb+");
    remove("Indices.bin");

    record *reg = malloc(sizeof(record));

    int tamanho, contador = 0;
    int x = 0;
    int passar=0;
    //escreve todos os indices
    while (rec_reg(arquivoDados, reg, &tamanho, &passar))
    {
        if (passar){
            escreveIndiceOrdenado(reg->first_n, contador);
            contador = contador + tamanho + 9 * sizeof(int);
        }
        else contador += tamanho;
        
    }
    fclose(arquivoDados);
    free(reg);
    return 0;
}