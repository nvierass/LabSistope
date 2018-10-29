#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>


void controladorSIGINT(int id);


void controladorSIGINT(int id){
	signal(id,controladorSIGINT);
	printf("Recibida la senal: %d\n",id);
}

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
		return res;
	}
}
void enviarSenal(int target,int signal){


}

void recibirSenales(){
	int kill666 = 0;
	int iterar = 1;
	int pidTarget;
	int signal;
	while(iterar != 0){
		printf("Ingrese el PID del proceso al que desea enviar una señal: \n");
		scanf("%d",&pidTarget);
		printf("Ingrese la señal que se enviara al proceso %d: \n",pidTarget);
		scanf("%d",&signal);
		enviarSenal(pidTarget,signal);
		if(getchar()!=10){
			iterar = 0;
		}
	}
	
}

int main(int argc,char** argv){
	int hValue = 0;
	int mflag = 0;
	opterr = 0;
	int c;
	int index;
	while((c = getopt(argc,argv, "mh:")) != -1){
		switch(c){
			case 'm':
				mflag = 1;
				break;
			case 'h':
				sscanf(optarg,"%d", &hValue);
				break;
			case '?':
				if(optopt == 'h'){
					fprintf(stderr, "Opcion -h requiere un argumento.\n",optopt);
				}
				else if(isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida -h.\n",optopt);
				}
				else{
					fprintf(stderr,"Opcion con caracter desconocido.\n",optopt);
				}
				return -1;
			default:
				abort();
		}
		printf("mflag = %d, hvaule = %d\n",mflag,hValue);
	}
	pid_t* hijos= inicializarHijos(hValue);
	if(!soyHijo(hijos,hValue)){
//		recibirSenales();
	}
	return 0;
}
