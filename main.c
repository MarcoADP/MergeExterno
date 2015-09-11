#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int tamBufferEntrada;
int tamBufferSaida;
int tamBufferMinimo;
int qtdRegistro;
int tamRegistro;
int maxRegistro;
int numCorridas;
char lixo[1];


FILE *arq;
char* nomeArq;

typedef struct {
    int chave;
    char desc[56]; //Lembrar do "\0"
} Registro;

Registro* regLidos;

int chave[100][5];
char desc[100][56];

char aux[5];

//Sort e compRegistro sao do Pedro
int compRegistro(const void *a, const void *b){
    Registro ra = *(Registro*)a;
    Registro rb = *(Registro*)b;
    return (ra.chave - rb.chave);
}

void sortRegistro(Registro* reg, int N){
    qsort(reg, N, sizeof(Registro), compRegistro);
}



void abreArquivo(){
	nomeArq = "arq.txt";
	//printf("Digite o nome do arquivo: ");
	//scanf("%s", nomeArq);
	arq = fopen(nomeArq, "rb");
	
	if (arq == NULL) {
       printf("Erro na abertura do arquivo\n");
	   printf("Programa encerrado!\n");
       exit(1);
    } 	
}

void recebeParametros(){
	fscanf(arq, "%d\n", &qtdRegistro);									// Quantidade de registros, 1ª linha do registrador
	
	tamRegistro = sizeof(Registro);										// Tamanho de cada Registro
	
	tamBufferMinimo = (int)ceil(sqrt(qtdRegistro)) * tamRegistro;		// Tamanho Minimo para o Buffer de Entrada
	
	printf("Digite o tamanho do Buffer de Entrada (EM BYTES): ");
	scanf("%d", &tamBufferEntrada);										// Tamanho do Buffer de Entrada
	
	while(tamBufferEntrada < tamBufferMinimo){
		printf("\nTamanho Invalido!\nNecessario que o buffer de entrada tenha o tamanho minimo de %d\n\n", tamBufferMinimo);
		printf("Digite o tamanho do Buffer de Entrada: ");
		scanf("%d", &tamBufferEntrada);
	}
	
	
	printf("Digite o tamanho do Buffer de Saida (EM BYTES): ");			// Tamanho do Buffer de Saida
	scanf("%d", &tamBufferSaida);
	
	if(tamBufferSaida > tamBufferEntrada){
		tamBufferSaida = tamBufferEntrada;
	}
	
	maxRegistro = (int) floor((double)tamBufferEntrada / (double)tamRegistro);	//Quantidade max de Registro no Buffer de Entrada
	numCorridas = (int) ceil((double)qtdRegistro / (double)maxRegistro);
	
	
	//printf("Registros: %d\n", qtdRegistro);
	
	/*int i;
	
	for(i = 0; i<qtdRegistro; i++){
		fread(&chave[i], sizeof(int), 1, arq);
		printf("%s -- ", chave[i]);
		fgets(lixo, 2, arq);
		fread(desc[i], sizeof(desc[i])-1, 1, arq);
		printf("%s\n", desc[i]);	
		fgets(lixo, 4, arq);
	}*/
}

void mostraParametros(){
	printf("\n-------------------------------------\n");
	printf("\nPARAMETROS PARA O MERGE SORT EXTERNO\n");
	printf("\nREGISTROS:\n");
	printf("Tamanho: %d Bytes\n", tamRegistro);
	printf("Quantidade: %d Registros\n", qtdRegistro);
	
	printf("\nBUFFER ENTRADA/SAIDA\n");
	printf("Tamanho Min. Buffer Entrada: %d Bytes\n", tamBufferMinimo);
	printf("Tamanho Buffer Entrada: %d Bytes\n", tamBufferEntrada);
	printf("Tamanho Buffer Saida: %d Bytes\n", tamBufferSaida);
	
	printf("\nCALCULOS\n");
	printf("Maximo de Registros no Buffer de Entrada: %d Registros\n", maxRegistro);
	printf("Numero de Corridas: %d Corridas\n", numCorridas);
	
}

void divisaoArquivo(){
	FILE *corrida[numCorridas];
	int numRegLidos = 0;
	int regBuffer;
	int seeks = 0;
    int numCorrida;
	int i;
	
    regLidos = calloc(maxRegistro, tamRegistro);
    
	for(numCorrida = 0; numCorrida < numCorridas; numCorrida++){
        regBuffer = 0;
        char nomeCorrida[32];
        sprintf(nomeCorrida, "%s%d", "corrida", numCorrida);
        
        printf("\nCorrida numero: %d\n", numCorrida);
        
        corrida[numCorrida] = fopen(nomeCorrida, "wb");
        if(corrida[numCorrida] == NULL){
            printf("O Arquivo nao pode ser aberto!\n");
            exit(-1);
        }
        
       for(i = 0; i < maxRegistro; i++){
            
			/*fread(&chave[i], sizeof(int), 1, arq);
			printf("%s -- ", chave[i]);
			fgets(lixo, 2, arq);
			fread(desc[i], sizeof(desc[i])-1, 1, arq);
			printf("%s\n", desc[i]);	
			fgets(lixo, 4, arq);*/
			
            fread(aux, sizeof(int), 1, arq);
			regLidos[i].chave = atoi(aux);
			//fgets(&regLidos[i].chave, sizeof(regLidos[i].chave), arq);
			fgets(lixo, 2, arq);
            fread(regLidos[i].desc, sizeof(regLidos[i].desc)-1, 1, arq);
			//printf("%d -- %s\n", regLidos[i].chave, regLidos[i].desc);
			fgets(lixo, 4, arq);
			seeks += 1;
            
            if(feof(arq)){
                break;
            }
            
            numRegLidos++;
            
            regBuffer++;
        }
        int j;
		printf("\nSem ordem:\n");
		for(j = 0; j < regBuffer; j++){
			printf("%4.d -- ", regLidos[j].chave);
			printf("%s\n", regLidos[j].desc);
		}
		
        sortRegistro(regLidos, regBuffer);  
		
        printf("\nCom ordem: \n");
		for(j = 0; j < regBuffer; j++){
			printf("%d -- ", regLidos[j].chave);
			printf("%s\n", regLidos[j].desc);
		}
        
        //buffer ordenado, escrever no arquivo de saida
		for(i = 0; i < regBuffer; i++){
            fwrite(&regLidos[i].chave, sizeof(regLidos[i].chave), 1, corrida[numCorrida]);
            fwrite(regLidos[i].desc, sizeof(regLidos[i].desc), 1, corrida[numCorrida]);
        }
        fclose(corrida[numCorrida]);
        printf(" salva em %s com %d registros\n", nomeCorrida, seeks);
        
    }
    free(regLidos);
    fclose(arq);
}

void kwayMerge(){
	/*int numCorrida;
	regLidos = calloc(numCorrida, tamRegistro);

    char saida[] = "saidaOrdenado.txt";
    FILE* arqSaida = fopen(saida, "wb");
    if(arqSaida == NULL){
        printf("Nao foi possivel abrir o arquivo de saida");
        exit(-1);
    }
    
    fwrite(&qtdRegistro, sizeof(qtdRegistro), 1, arqSaida);
    
    //Inicializar a leitura
    int corridaSaida = 0; //marcador de arquivos abertos
	for(numCorrida = 0; numCorrida < numCorridas; numCorrida++ ){
        char nomeCorrida[32];
        sprintf(nomeCorrida, "%s%d", nomeArq, numCorrida);
        
        corrida[numCorrida] = fopen(nomeCorrida, "r");
        if(corrida[numCorrida] == NULL){
            printf("Nao foi possivel abrir arquivo de corrida");
            exit(-1);
        }
        
        corridaSaida |= (1 << c); //arquivo marcado como aberto
        
        //agora ler o primeiro registro
        fread(&reg_lidos[c].key, sizeof(reg_lidos[c].key), 1, corrida[c]);
        fread(reg_lidos[c].description, sizeof(reg_lidos[c].description), 1, corrida[c]);
    }
    
    //alocar o buffer de saida
    size_t num_regs_saida = floor( ((double)bufferarqSaida / (double)sizeReg) );
    Registro *reg_saida = calloc(num_regs_saida, sizeReg);
    int ind_saida = 0;
    
    printf("\nUnindo arquivos... ");
    int num_escritas = 0;
    int num_reg_escritos = 0;
    while(corridaSaida|0) //enquanto houver arquivo aberto
    {
        //Pegar o menor registro
        int min_reg = 0;
        while( !(corridaSaida&(1<<min_reg)) ) min_reg++; //pegar o primeiro registro de um arquivo aberto
        
		register int c;
        for(c = 0; c < num_corridas; c++){
            if(!(corridaSaida & (1<<c))) //arquivo nao aberto, nao pegar registro
                continue;
            
            if(reg_lidos[c].key < reg_lidos[min_reg].key)
                min_reg = c;
        }

        
        //Escrever o menor registro no buffer de saida
        memcpy(&reg_saida[ind_saida], &reg_lidos[min_reg], sizeReg);
        ind_saida++;
        
        //Ler o proximo registro
        fread(&reg_lidos[min_reg].key, sizeof(reg_lidos[min_reg].key), 1, corrida[min_reg]);
        if(feof(corrida[min_reg]))
        {
            //leitura encontrou fim de arquivo, fechar arquivo
            reg_lidos[min_reg].key = __INT_MAX__;
            reg_lidos[min_reg].description[0] = '\0'; //string "vazia"
            corridaSaida &= ~(1 << min_reg); //marca arquivo como fechado
            
            fclose(corrida[min_reg]);
            
            
            if(!(corridaSaida|0))
            {
                //se fechou o ultimo arquivo, escrever o resto do buffer de saida
                goto LABEL_WRITEFILE; //horrivel usar isso, mas tao facil!
            }
        }
        else{
            fread(reg_lidos[min_reg].description, sizeof(reg_lidos[min_reg].description), 1, corrida[min_reg]);
        }
        
        if(ind_saida >= num_regs_saida)
        {
            LABEL_WRITEFILE:
        
			//int i;
            for(i = 0; i < ind_saida; i++)
            {
                fwrite(&reg_saida[i].key, sizeof(reg_saida[i].key), 1, arqSaida);
                fwrite(reg_saida[i].description, sizeof(reg_saida[i].description), 1, arqSaida);
                num_reg_escritos++;
            }
            num_escritas++;
            ind_saida = 0;
        }
    }
    printf(" uniao completa com %d escritas.\n", num_escritas);
    
    fclose(arqSaida);*/
	
}

int main(int argc, char const *argv[]) {
	abreArquivo();
	recebeParametros();
	//mostraParametros();
	divisaoArquivo();
	//kwayMerge();
	
	return 0;
}