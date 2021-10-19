/*
Integrantes:

Gustavo Vieira Jodar - 769678
Nayra Kaline dos Santos Vidal - 769847
Sophia Schuster - 760936

*/

void ExcluiRegistro(int key)
{
    record registro;
    char arquivo_name[10] = "Dados.bin";
    FILE *arquivo = fopen(arquivo_name, "rb+");
    FILE *excluidos = fopen("excluidos.bin", "ab"); //Arquivo que guarda os registros excluidos.
    int achou = 0;
    int contador = 1;
    while (recuperar_registro(arquivo, &registro))
    {
        if (key == registro.key){
            //procura pelo registro excluido para adiciona-lo ao arquivo de excluidos. 
            int tam_campos[QCAMPOS] = {strlen(registro.last_n), strlen(registro.first_n), strlen(registro.address), strlen(registro.city), strlen(registro.state), strlen(registro.zip),strlen(registro.phone)};
            int tam_reg = strlen(registro.last_n) + strlen(registro.first_n) + strlen(registro.address) + strlen(registro.city) + strlen(registro.state) + strlen(registro.zip) + strlen(registro.phone);
            //gravando no arquivo de excluidos tamanhos e os campos do registro
            escreve_dados(excluidos, &registro, tam_reg, tam_campos);
            //Optei por salvar os registros inteiros no arquivo de excluídos, assim eu poderia aproveitar funções, lógica e structs.
            printf("%dº pessoa apagada\n", contador);
            achou =1;
        } //esse modo printa apenas o registro da key buscada.
        contador++;
    }
    if (achou == 0) printf("pessoa nao encontrada\n");
    fclose(arquivo);
    fclose(excluidos);
}

int buscaNome(char *name)
{
    record registro; //armazena cada registro lido

    FILE *arquivo;

    int key=-1;

    char arquivo_name[10] = "Dados.bin";

    arquivo = fopen(arquivo_name, "rb");

    int contador = 1;
    if (arquivo != NULL)
    {
        while (recuperar_registro(arquivo, &registro))
        {
            if (!strcmp(name,registro.first_n)){
                key = registro.key;
            } //ao encontrar o nome que desejo apagar eu salvo a key.
            contador++;
        }
        fclose(arquivo);
    }
    else
    {
        printf("Arquivo inexistente!\n");
    }
    return key;
}


void excluirLogicamenteRegistro()
{
    char first_name[64];
    printf("First name do registro a ser excluído: ");
    getchar();
    scanf("%[^\n]s", first_name);
    getchar();

    //procura nome daquele registro. 
    int key = buscaNome(first_name);
    //como no tipo 2 nao é necessario usar indice eu utilizei a key para marcar como excluido. Supondo que o usuario nao adicionaria dois registros com a mesma key. 

    if (key != -1)
    {
        ExcluiRegistro(key);
    }
    else
    {
        printf("Nome não encontrado!\n");
    }
}
