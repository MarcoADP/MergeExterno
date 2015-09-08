#include <stdio.h>
#include <stdlib.h>

int tamBufferEntrada;
int tamBufferSaida;
int qtdRegistro;
char lixo[1];

FILE *arq;

//struct {
	char chave[5];
	char desc[56];
//}

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
	
	
	//while( (fscanf(arq,"%s %f %f %f\n", nome, &nota1, &nota2, &nota3))!=EOF )
	
	/*while(fscanf(arq, "%4[^\n] %c %55[^\n]", chave, lixo, desc) != EOF){
		printf("%s -- %s\n", chave, desc);
	}*/
	
	int i;
	
	for(i = 0; i<qtdRegistro; i++){
		fgets(chave, sizeof(chave), arq);
		printf("%s\n", chave);
		fgets(lixo, 2, arq);
		fgets(desc, sizeof(desc), arq);
		printf("%s\n", desc);	
		fgets(lixo, 3, arq);
	}
}



int main(int argc, char const *argv[]) {
	recebeParametros();
    //abreArquivo();
	return 0;
}