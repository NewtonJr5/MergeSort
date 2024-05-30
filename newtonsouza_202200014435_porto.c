#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct {
	char codigo[12];
	char CNPJ[19];
	char CNPJ_sel[19];
	int32_t peso;
	int32_t posicao;
	int32_t dif_CNPJ;
	int32_t dif_peso_int32_t;
	float dif_peso;
	
}CONTAINER;

void print_res1(int *arr1, int32_t tamanho, FILE *arq, CONTAINER *registro_c) {

	for(int i = 0; i < tamanho; i++) {
		int pos = arr1[i];
		fprintf(arq, "%s: %s<->%s\n", registro_c[pos].codigo, registro_c[pos].CNPJ, registro_c[pos].CNPJ_sel);
	}
	
	
}

void print_res2( int32_t tamanho, FILE *arq, CONTAINER* reg) {

	for(int i = 0; i < tamanho; i++) {
		fprintf(arq, "%s: %dkg (%.0f%%)\n", reg[i].codigo, reg[i].dif_peso_int32_t, reg[i].dif_peso);
	}
	
}

// Calcula a diferença de CNPJ

int32_t comparaCNPJ (char cnpj1[], char cnpj2[]) {
	
	if(strcmp(cnpj1, cnpj2) == 0) return 0;
	else return 1;
		
	
}

// Calcula a diferença de peso percentual

float comparaP (int32_t peso1, int32_t peso2) {
	
	if(peso1 == peso2) return 0;
	else {
		float porc;;
		if (peso1 < peso2) 
			porc = (float) 100*(peso2 - peso1)/peso2;
		else 
			porc = (float) 100*(peso1 - peso2)/peso2;

		return porc;
		
	}
}

// Calcula a diferença de peso absoluta

int32_t diferencaPeso (int32_t peso1, int32_t peso2) {

	if(peso1 == peso2){
			
			return 0;
			
	} else {
		
		int32_t abs;
		if (peso1 < peso2) {
			abs = peso2 - peso1;
		} else {
			abs = peso1 - peso2;
		}

		return abs;
		
	}
}

// Busca binaria
int buscaBinariaString(CONTAINER cont[], int inicio, int fim, const char *alvo) {
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        // Compara as strings usando strcmp
        int comparacao = strcmp(cont[meio].codigo, alvo);

        // Verifica se a string está no meio
        if (comparacao == 0)
            return meio;

        // Se a string é menor, ignora a metade direita
        if (comparacao > 0)
            fim = meio - 1;
        // Se a string é maior, ignora a metade esquerda
        else
            inicio = meio + 1;
    }

    // Retorna -1 se a string não for encontrada
    return -1;
}

// Função que uni as duas partes do mergesort
void juntar_peso(int32_t inicio, int32_t medio, int32_t final, CONTAINER arr1[], CONTAINER aux[]) {
    int32_t i = inicio, j = medio + 1, k = inicio;

    while (i <= medio && j <= final) {
        if (arr1[i].dif_peso >= arr1[j].dif_peso) {
            aux[k] = arr1[i];
            i++;
        } else {
            aux[k] = arr1[j];
            j++;
        }
        k++;
    }

    while (i <= medio) {
        aux[k++] = arr1[i++];
    }

    while (j <= final) {
        aux[k++] = arr1[j++];
    }

    for (k = inicio; k <= final; k++) {
        arr1[k] = aux[k];
    }
}

// Função que uni as duas partes do mergesort
void juntar(int32_t inicio, int32_t medio, int32_t final, int arr1[], int aux[]) {
    int32_t i = inicio, j = medio + 1, k = inicio;

    while (i <= medio && j <= final) {
        if (arr1[i] <= arr1[j]) {
            aux[k] = arr1[i];
            i++;
        } else {
            aux[k] = arr1[j];
            j++;
        }
        k++;
    }

    while (i <= medio) {
        aux[k++] = arr1[i++];
    }

    while (j <= final) {
        aux[k++] = arr1[j++];
    }

    for (k = inicio; k <= final; k++) {
        arr1[k] = aux[k];
    }
}


void intercalar_alfa(int32_t inicio, int32_t medio, int32_t final, CONTAINER arr1[], CONTAINER aux[]) {
    int32_t i = inicio, j = medio + 1, k = inicio;

    while (i <= medio && j <= final) {
        int32_t comparacao = strcmp(arr1[i].codigo, arr1[j].codigo);
        if (comparacao <= 0) {
            aux[k++] = arr1[i++];
            if (comparacao == 0) j++;
        } else {
            aux[k++] = arr1[j++];
        }
    }

    while (i <= medio) {
        aux[k++] = arr1[i++];
    }

    while (j <= final) {
        aux[k++] = arr1[j++];
    }
	
    for (k = inicio; k <= final; k++) {
        arr1[k] = aux[k];
    }
}

// // Mergesort peso
void mergeSort_peso (int32_t inicio, int32_t final, CONTAINER arr1[], CONTAINER aux[]) {
		
	if(inicio < final) {

		int32_t medio = (inicio+final)/2;
		
		mergeSort_peso(inicio, medio, arr1, aux);
		mergeSort_peso(medio+1, final, arr1, aux);
		
		juntar_peso(inicio, medio, final, arr1, aux);
		
	}
}

// // Mergesort inteiro (posicao)
void mergeSort (int32_t inicio, int32_t final, int arr1[], int aux[]) {
		
	if(inicio < final) {

		int32_t medio = (inicio+final)/2;
		
		mergeSort(inicio, medio, arr1, aux);
		mergeSort(medio+1, final, arr1, aux);
		
		juntar(inicio, medio, final, arr1, aux);
		
	}
}
// // Mergesort alfabetico
void mergeSort_e (int32_t inicio, int32_t final, CONTAINER arr1[], CONTAINER aux[]) {

	if(inicio < final) {
		
		int32_t medio =  (inicio + final) / 2;
		
		mergeSort_e(inicio, medio, arr1, aux);
		mergeSort_e(medio+1, final, arr1, aux);
		
		intercalar_alfa(inicio, medio, final, arr1, aux);
		
	}

}


int main (int argc, char* argv[]) {

	// Escaneia o arquivo com a entrada
    FILE *arq;
    arq = fopen(argv[1], "r");

	if(argc != 1){
		arq = fopen(argv[1], "r");
	} else {
		arq = fopen("input.txt", "r");
	}

	FILE *output;
	if(argc != 1){
		output = fopen(argv[2], "w");
	} else {
		output = fopen("output.txt", "w");
	}
	
	//Cria variáres
	CONTAINER cont;
    int32_t quant_reg, aux, tam_r = 0;
    
    fscanf(arq, "%d", &quant_reg);
    
	CONTAINER* registros = (CONTAINER*)malloc(sizeof(CONTAINER)*quant_reg);
	CONTAINER* registros_c = (CONTAINER*)malloc(sizeof(CONTAINER)*quant_reg);
    
	// Pega os valores dos registros e armazena no array
    for(int32_t i = 0; i < quant_reg; i++){
		
		fscanf(arq, "%s", cont.codigo);
		fscanf(arq, "%s", cont.CNPJ);
		fscanf(arq, "%d", &aux);
		
		cont.peso = aux;
		cont.posicao = i;
		registros[tam_r] = cont;
		registros_c[tam_r++] = cont;

	}

	CONTAINER* auxiliar = (CONTAINER*)malloc(sizeof(CONTAINER)*tam_r);
	mergeSort_e(0, tam_r - 1, registros, auxiliar);

	int quant_sel;
    fscanf(arq, "%d", &quant_sel);

	int *CNPJ_dif = (int*)malloc(sizeof(int)*quant_sel);
	int tam_CNPJ = 0;
	int *peso_dif = (int*)malloc(sizeof(int)*quant_sel);
	int tam_peso = 0;
	
	int n_e = 0;
	int e = 0;
    // Pega os valores dos selecionados e armazena no array
    for(int i = 0; i < quant_sel; i++){
		
		fscanf(arq, "%s", cont.codigo);
		fscanf(arq, "%s", cont.CNPJ);
		fscanf(arq, "%d", &aux);
		cont.peso = aux;

		int indice = buscaBinariaString(registros, 0, quant_reg - 1, cont.codigo);
		int pos = registros[indice].posicao;

		if (indice == -1) {

			n_e++;
			continue;

		} else {
			
			e++;
			float difpeso = comparaP(cont.peso, registros_c[pos].peso);
			registros_c[pos].dif_peso = round(difpeso);

			int peso_abs = diferencaPeso(cont.peso, registros_c[pos].peso);
			registros_c[pos].dif_peso_int32_t = peso_abs;
			
			int dif_CNPJ =  comparaCNPJ(cont.CNPJ, registros_c[pos].CNPJ);
			registros_c[pos].dif_CNPJ = dif_CNPJ;

			strcpy(registros_c[pos].CNPJ_sel, cont.CNPJ);

			if ( registros_c[pos].dif_CNPJ == 1 ) {

				CNPJ_dif[tam_CNPJ++] = pos;
				
			} else if (registros_c[pos].dif_peso > 10 ) {

				peso_dif[tam_peso++] = pos;

			}	
		}
		
	}

	printf("n_e: %d\n", n_e);
	printf("e: %d\n", e);

	printf("tamanho cnpj: %d\n", tam_CNPJ);
	printf("tamanho peso: %d\n", tam_peso);

	int *aux1 = (int*)malloc(sizeof(int)*tam_CNPJ);
	int *aux2 = (int*)malloc(sizeof(int)*tam_peso);

	// Chama a função mergesort

	mergeSort(0, tam_CNPJ - 1, CNPJ_dif, aux1);
	print_res1(CNPJ_dif, tam_CNPJ, output, registros_c);
	mergeSort(0, tam_peso - 1, peso_dif, aux2);

	CONTAINER* pesos = (CONTAINER*)malloc(sizeof(CONTAINER)*tam_peso);
	CONTAINER* aux3 = (CONTAINER*)malloc(sizeof(CONTAINER)*tam_peso);

	for (int i = 0; i < tam_peso; i++) {
		pesos[i] = registros_c[peso_dif[i]];
	}
	
	mergeSort_peso(0, tam_peso - 1, pesos, aux3);
	print_res2(tam_peso, output, pesos);
	
	// Fecha o arquivo de entrada
    fclose(arq);
	fclose(output);

    return 0;
}
