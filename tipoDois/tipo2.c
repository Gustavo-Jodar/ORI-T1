/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include "handleBinFile2.h"

int main()
{
    record registro;
    record *p_registro = &registro;
    int opcao, continuar;
    while (opcao != 5)
    {
        continuar = 1;
        printf("\n\n===========================================\n");
        printf("1-Escrever registro\n");
        printf("2-Ler todos os registros\n");
        // printf("3-Buscar registro por key\n");
        // printf("4-Buscar registro por first_name\n");
        printf("5-Terminar\n");
        printf("===========================================\n");
        printf("Opção:");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                while (continuar == 1)
                {
                    escreve_arquivo();
                    printf("\nAdicionar mais um registro? Digite 1 para continuar.\n");
                    printf("Continuar: ");
                    scanf("%d", &continuar);
                }
                break;
            case 2:
                ler_arquivo();
                break;
            case 3:
                // buscar_key(p_registro);
                break;
            case 4:
                //busca_first_name(p_registro);
                // busca_bin_first_name(p_registro);
                break;
            case 5:
                printf("Terminando...\n");
                break;
            case 6:
                // lerIndices();
                //escreveIndiceOrdenado("az", 2);
                break;
            default:
                printf("Opção inválida! Digite novamente.\n");
                break;
        }
    }
}
