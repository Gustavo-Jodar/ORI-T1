/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

#include "handleBinFile2.h"
#include "handleDelete2.h"

int main()
{
    record registro;
    record *p_registro = &registro;
    int opcao, continuar, key, aux;
    char name[64];
    while (opcao != 6)
    {
        continuar = 1;
        printf("\n\n===========================================\n");
        printf("1-Escrever registro\n");
        printf("2-Ler todos os registros\n");
        printf("3-Buscar registro por key\n");
        printf("4-Buscar registro por first_name\n");
        printf("5-Apagar registro por nome\n");
        printf("6-Terminar\n");
        printf("===========================================\n");
        printf("Opção:");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                aux=0;
                while (continuar == 1)
                {
                    escreve_arquivo(aux);
                    printf("\nAdicionar mais um registro? Digite 1 para continuar.\n");
                    printf("Continuar: ");
                    scanf("%d", &continuar);
                    aux++; //somando o aux nao temos risco de substituir uma mesma linha que foi excluida e ja foi substituida.
                }
                break;
            case 2:
                ler_arquivo(0,0,"undef");
                break;
            case 3:
                printf("Key: ");
                scanf("%d", &key);
                ler_arquivo(1,key,"undef");
                break;
            case 4:
                printf("First Name: ");
                getchar();
                scanf("%[^\n]s", name);
                ler_arquivo(2,0,name);
                break;
            case 5:
                excluirLogicamenteRegistro();
                break;
            case 6:
                printf("Terminando...\n");
                break;
            default:
                printf("Opção inválida! Digite novamente.\n");
                break;
        }
    }
}
