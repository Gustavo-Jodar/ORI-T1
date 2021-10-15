/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include "handleDelete.h"

int main()
{
    record registro;
    record *p_registro = &registro;
    int opcao, continuar;
    while (opcao != 7)
    {
        continuar = 1;
        printf("\n\n===========================================\n");
        printf("1-Escrever registro\n");
        printf("2-Ler todos os registros\n");
        printf("3-Buscar registro por numero de registro\n");
        printf("4-Buscar registro por key\n");
        printf("5-Buscar registro por first_name\n");
        printf("6-Apagar registro por nome\n");
        printf("7-Terminar\n");
        printf("===========================================\n");
        printf("8-Ler Arquivo de Indices\n");
        printf("9-Ler Arquivo de Posicoes Livres\n");
        printf("===========================================\n");
        printf("Opção:");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            while (continuar == 1)
            {
                int posicao_arquivo_registrado;
                read_data(p_registro);
                posicao_arquivo_registrado = escreve_arquivo(p_registro);
                escreveIndiceOrdenado(p_registro->first_n, posicao_arquivo_registrado);
                printf("\nAdicionar mais um registro? Digite 1 para continuar.\n");
                printf("Continuar: ");
                scanf("%d", &continuar);
            }
            break;
        case 2:
            ler_arquivo(p_registro);
            break;
        case 3:
            buscar_reg(p_registro, 0, 0);
            break;
        case 4:
            buscar_key(p_registro);
            break;
        case 5:
            //busca_first_name(p_registro);
            busca_bin_first_name(p_registro);
            break;
        case 6:
            excluirLogicamenteRegistro();
            break;
        case 7:
            printf("Terminando...\n");
            break;
        case 8:
            lerIndices();
            break;
        case 9:
            lerExcluidos();
            break;
        default:
            printf("Opção inválida! Digite novamente.\n");
            break;
        }
    }
}
