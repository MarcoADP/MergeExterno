#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int tamBufferEntrada;
int tamBufferSaida;
int qtdRegistro;
int totalCorridas;

FILE *arq_entrada;

typedef struct {
    int chave;
    char desc[58];
} Registro;




void radixsort(Registro* reg, int tamanho){
	int i;
	int b[tamanho];
	int maior = reg[0].chave;
	int exp = 1;
	for(i = 0; i < tamanho; i++){
		if(reg[i].chave > maior){
			maior = reg[i].chave;
		}
	}
	
	while(maior/exp > 0){
		int bucket[10] = {0};
		for(i = 0; i < tamanho; i++){
			bucket[(reg[i].chave/exp) % 10]++;
		}

		for(i = 1; i < 10; i++){
			bucket[i] += bucket[i-1];
		}
		for(i = (tamanho - 1); i >= 0; i--){
			int valor = (reg[i].chave / exp)%10;
			int aux = --bucket[valor];
			//printf("v1: %d -- peso: %d\n", vetor[i].v1, vetor[i].peso);
			b[aux] = reg[i].chave;
		}
		for (i = 0; i < tamanho; i++){
			reg[i].chave = b[i];
		}
		exp *= 10;
	}
	
}


void abreArquivo(){    
    char nomeArq[30];
    printf("\n\nDigite o nome do arquivo: ");
    scanf("%s", nomeArq);
    arq_entrada = fopen(nomeArq, "r");
    
    if (arq_entrada == NULL) {
       printf("Erro na abertura do arquivo\n");
       printf("Programa encerrado!\n");
       exit(1);
    }  
}

void recebeParametros(){
    fscanf(arq_entrada, "%d", &qtdRegistro);                            // Quantidade de registros, 1ª linha do registrador
    printf("\n\nNumero de registros no arquivo: %d\n", qtdRegistro);
    
    int tamBufferMinimo;
    int numMaximoCorrida = (int)(sqrt(qtdRegistro));                    // (1/corrida)*(qtdRegistro/corrida) >= 1    
    tamBufferMinimo = qtdRegistro / numMaximoCorrida;                   // Tamanho Minimo para o Buffer de Entrada
    
    printf("\nInforme o numero de registros do Buffer de Entrada: ");
    scanf("%d", &tamBufferEntrada);                                     // Tamanho do Buffer de Entrada
    
    while(tamBufferEntrada < tamBufferMinimo){
        printf("\nTamanho Invalido!\nNecessario que o buffer de entrada tenha o tamanho minimo de %d registros\n", tamBufferMinimo);
        printf("\nInforme o numero de registros do Buffer de Entrada: ");
        scanf("%d", &tamBufferEntrada);
    }
    
    printf("Informe o numero de registros do Buffer de Saida: ");
    scanf("%d", &tamBufferSaida);                                       // Tamanho do Buffer de Saida
        
    totalCorridas = (int) ceil((double)qtdRegistro / (double)tamBufferEntrada);
}

void divisaoArquivo(){
    printf("\nFASE DE SORTING: ");
    printf("\n---------------------------------------------------\n");
    FILE *corrida[totalCorridas];
    int numRegBuffer;
    int num_read = 0;
    int num_write = 0;
    int numCorrida;
    Registro regLidos[tamBufferEntrada];
    int i;    
    
    for(numCorrida = 0; numCorrida < totalCorridas; numCorrida++){
        numRegBuffer = 0;
        char nomeCorrida[32];
        sprintf(nomeCorrida, "%s%d.txt", "corrida", numCorrida);
        
        printf("Corrida %d:\n", numCorrida);
        
        /* CRIA ARQUIVO DE CORRIDA */
        corrida[numCorrida] = fopen(nomeCorrida, "w");
        if(corrida[numCorrida] == NULL){
            printf("O Arquivo de corrida nao pode ser criado!\n");
            exit(-1);
        }

        printf("Lendo arquivo de entrada e preenchendo o buffer...\n");
        /* LÊ ARQUIVO DE ENTRADA */
        for(i = 0; i < tamBufferEntrada; i++){            
            if (fscanf(arq_entrada, "%d", &regLidos[i].chave) < 1)
                break;

            fgets(regLidos[i].desc, sizeof(regLidos[i].desc), arq_entrada);

            num_read++;
            numRegBuffer++;
        }
        
        printf("Ordenando o buffer de entrada...\n");
        //sortRegistro(regLidos, numRegBuffer);
        radixsort(regLidos, numRegBuffer);
		
        printf("Escrevendo a corrida...\n");
        /* ESCREVE CORRIDA ORDENADA */
        for(i = 0; i < numRegBuffer; i++){
            fwrite(&regLidos[i], sizeof(Registro), 1, corrida[numCorrida]);
            num_write++;
        }

        fclose(corrida[numCorrida]);
        printf("Corrida salva em %s com %d registros\n", nomeCorrida, numRegBuffer);
        printf("---------------------------------------------------\n");
    }
    printf("\nFim da fase de sorting...\n");
    printf("%d corridas de no maximo %d registros foram geradas.\n", totalCorridas, tamBufferEntrada);
    printf("Numero de total de reads: %d\n", num_read);
    printf("Numero de total de writes: %d\n", num_write);
    printf("\n---------------------------------------------------\n");
    fclose(arq_entrada);
}

int input(Registro reg[], int num, FILE* arq_corrida){
    int retorno_read;
    retorno_read = fread(reg, sizeof(Registro), num, arq_corrida);
    int i;
    for (i = retorno_read; i < num; i++)
        reg[i].chave = -1;
    return retorno_read;
}

int menorChave(int totalCorridas, int reg_por_corrida, Registro regLidos[totalCorridas][reg_por_corrida], int *i_menor, int *j_menor){
    int i, j;
    int menor = INT_MAX;
    for (i = 0; i < totalCorridas; i++){
        for (j = 0; j < reg_por_corrida; j++){
            if (regLidos[i][j].chave != -1 && regLidos[i][j].chave < menor){
                *i_menor = i;
                *j_menor = j;
                menor = regLidos[i][j].chave;
            }
        }
    }
    if (menor != INT_MAX)
        return 1;

    return -1;
}

int lerBloco(int totalCorridas, int reg_por_corrida, Registro regLidos[totalCorridas][reg_por_corrida], FILE *corrida[]){
    int i, j, num_read = 0;
    int flag;
    for (i = 0; i < totalCorridas; i++){
        flag = 0;
        for (j = 0; j < reg_por_corrida; j++){
            if (regLidos[i][j].chave != -1){
                flag = 1;
            }
        }
        if (flag == 0){
            if (input(regLidos[i], reg_por_corrida, corrida[i]) != 0)
                num_read++;
        }
    }
    return num_read;
}

void merging(){
    printf("\nFASE DE MERGING: ");
    printf("\n---------------------------------------------------\n");

    int reg_por_corrida = ((1.0/totalCorridas)*tamBufferEntrada);

    int i, j, num_write = 0, num_read = 0;
    Registro regLidos[totalCorridas][reg_por_corrida];
    Registro regSaida[tamBufferSaida];
    FILE *corrida[totalCorridas];

    char nomeArqSaida[] = "saida-ordenado.txt";
    FILE* arq_saida = fopen(nomeArqSaida, "w");
    if(arq_saida == NULL){
        printf("O arquivo de saida nao pode ser criado.");
        exit(-1);
    }

    fprintf(arq_saida, "%d\n", qtdRegistro);

    /* ABRINDO ARQUIVOS */
    for (i = 0; i < totalCorridas; i++){
        char nomeCorrida[32];
        sprintf(nomeCorrida, "%s%d.txt", "corrida", i);
        corrida[i] = fopen(nomeCorrida, "r");
        
        input(regLidos[i], reg_por_corrida, corrida[i]);
        num_read++;
    }

    /* K-WAY MERGE */
    int i_menor, j_menor, num_reg = 0;
    i = menorChave(totalCorridas, reg_por_corrida, regLidos, &i_menor, &j_menor);
    while (i != -1){
        regSaida[num_reg] = regLidos[i_menor][j_menor];
        regLidos[i_menor][j_menor].chave = -1;
        num_reg++;

        /* ESCRITA DO CONTEUDO DO BUFFER NO ARQUIVO DE SAIDA */
        if (num_reg >= tamBufferSaida){
            for (j = 0; j < num_reg; j++){                
                fprintf(arq_saida, "%04d%s\n", regSaida[j].chave, regSaida[j].desc);                
            }
            num_write++;
            num_reg = 0;
        }

        num_read += lerBloco(totalCorridas, reg_por_corrida, regLidos, corrida);
        i = menorChave(totalCorridas, reg_por_corrida, regLidos, &i_menor, &j_menor);        
    }

    /* TERMINA DE ESCREVER NA SAIDA */
    for (j = 0; j < num_reg; j++){
        fprintf(arq_saida, "%04d%s\n", regSaida[j].chave, regSaida[j].desc);                
    }
    if (num_reg > 0)
        num_write++;
    
    printf("Buffer de entrada de %d registros para %d corridas.\n", tamBufferEntrada, totalCorridas);
    printf("Numero de registros lidos por vez de cada corrida: %d\n", reg_por_corrida);
    printf("Numero de total de reads: %d\n", num_read);
    printf("Numero de total de writes: %d\n", num_write);
    printf("\n---------------------------------------------------\n");

    for (i = 0; i < totalCorridas; i++){
        fclose(corrida[i]);
    }
    fclose(arq_saida);    
}

int main(int argc, char const *argv[]) {
    abreArquivo();
    recebeParametros();
    divisaoArquivo();
    merging();
    
    return 0;
}