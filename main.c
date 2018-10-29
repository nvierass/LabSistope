#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int soyHijo(pid_t* hijosProceso,int n){
	int i;
	for(i = 0; i < n; i++){
		if(hijosProceso[i] == 0){
			return 1;
		}
	}
	return 0;
}

void printfArreglo(pid_t* res,int n){
	int i;
	printf("Pid proceso actual %d: [%d,%d,%d,%d]\n",getpid(),res[0],res[1],res[2],res[3]);	
}

pid_t* inicializarHijos(int numHijos){
	pid_t* res = (pid_t*)malloc(numHijos*sizeof(pid_t));
	if(!res){
		return NULL;	
	}
	else{
		int i;
		for(i = 0; i < numHijos; i++){
			res[i]=9999;
		}
		for(i = 0; i < numHijos; i++){
			if(soyHijo(res,numHijos)==0){
				//printfArreglo(res,numHijos);
				res[i] = fork();
				if(res[i]<0){
					printf("Error al crear proceso hijo\n");
					free(res);
					return NULL;			
				}
				if(soyHijo(res,numHijos)==0)
				printf("El hijo %d tiene el pid %d\n",i+1,res[i]);
			}
		}
	}
}

int main(){
	int hValue = 0;
	int mflag = 0;
	inicializarHijos(4);
	return 0;
}
