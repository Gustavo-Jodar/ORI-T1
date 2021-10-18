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
        printf("3-Buscar registro por numero de registro (posicao no arquivo)\n");
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
            //opcao de adicionar um registro
            while (continuar == 1)
            {
                int posicao_arquivo_registrado;
                //le os dados do novo registro
                read_data(p_registro);
                //adiciona os dados ao arquivo, retornando a posicao em que ele foi adicionado
                posicao_arquivo_registrado = escreve_arquivo(p_registro);
                //adiciona o indice do registro adicionado no arquivo de indices
                escreveIndiceOrdenado(p_registro->first_n, posicao_arquivo_registrado);
                printf("\nAdicionar mais um registro? Digite 1 para continuar.\n");
                printf("Continuar: ");
                scanf("%d", &continuar); //condicao de parada
            }
            break;
        case 2:
            //lê todos os registros nao apagados
            ler_arquivo(p_registro);
            break;
        case 3:
            //busca registro por sua posicao no arquivo
            buscar_reg(p_registro, 0, 0);
            break;
        case 4:
            //busca sequencialmente através do campo key do registro
            buscar_key(p_registro);
            break;
        case 5:
            //busca o registro por seu primeiro nome através de busca binária no arquivo de indices;
            busca_bin_first_name(p_registro);
            break;
        case 6:
            //marca a flag deletado no registro e remove ele doarquivo de indices
            //adiciona sua posicao vaga no arquivo de espaços vazios
            excluirLogicamenteRegistro();
            break;
        case 7:
            //termina o programa
            printf("Terminando...\n");
            break;
        case 8:
            //le como está organizado o arquivo de indices
            lerIndices();
            break;
        case 9:
            //le como esta organizado o arquivo de espaços vazios
            lerExcluidos();
            break;
        default:
            printf("Opção inválida! Digite novamente.\n");
            break;
        }
    }
}
