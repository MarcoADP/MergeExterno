#include <stdio.h>
#include <stdlib.h>

int tamBufferEntrada;
int tamBufferSaida;
int qtdRegistro;
char lixo[1];

FILE *arq;

char chave[100][5];
char desc[100][56];



void abreArquivo(){
	char nomeArq[] = "arq.txt";
	//printf("Digite o nome do arquivo: ");
	//scanf("%s", nomeArq);
	arq = fopen(nomeArq, "r");
	
	if (arq == NULL) {
       printf("Erro na abertura do arquivo\n");
	   printf("Programa encerrado!\n");
       exit(1);
    } else{
		printf("abriu!!!\n");
	}	
}

void recebeParametros(){
	//printf("Digite o tamanho do Buffer de Entrada: ");
	//scanf("%d", &tamBufferEntrada);
	
	//printf("Digite o tamanho do Buffer de Saida: ");
	//scanf("%d", &tamBufferSaida);
	
	abreArquivo();
	
	fscanf(arq, "%d\n", &qtdRegistro);
	printf("Registros: %d\n", qtdRegistro);
	
	int i;
	
	for(i = 0; i<qtdRegistro; i++){
		fgets(chave[i], sizeof(chave[i]), arq);
		printf("%s\n", chave[i]);
		fgets(lixo, 2, arq);
		fgets(desc[i], sizeof(desc[i]), arq);
		printf("%s\n", desc[i]);	
		fgets(lixo, 3, arq);
	}
}



int main(int argc, char const *argv[]) {
	recebeParametros();
    //abreArquivo();
	return 0;
}