#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>


void controladorSIGTERM(int id);
void controladorKILL(int id);
void controladorSIGUSR1(int id);
void controladorSIGUSR2(int id);
void controladorSIGINT(int id);
int soyHijo(pid_t* hijosProceso,int n);
pid_t* inicializarHijos(int numHijos, int mflag);
void recibirSenales(pid_t *res,int cantHijos);
void enviarSenal(int target,int senal);

int contadorSIGUSR1 = 0;
int contadorSIGINT = 0;
pid_t* hijos;
int hValue;



void controladorSIGTERM(int id)
{
	printf("\nSoy el hijo con pid %d y mi papa me quiere matar :(.\n",getpid());
	exit(0);
}

void controladorKILL(int id)
{
	printf("\nproceso muerto\n");
	exit(0);
}

/*Adem´as, se puede pulsar Ctrl{C, lo que produce que todos los hijos reciban la se˜nal SIGINT, la cual
deber´a imprimir por pantalla <Soy el hijo con pid: X, y estoy vivo a´un. No me mates pap´a>. Luego de
ejecutar esta funci´on, tendr´a que poner la se˜nal a su defecto, tal que si se presiona nuevamente Ctrl-C se
cierran todos los procesos.*/

void controladorSIGUSR1(int id)
{
	printf("Recibida la señal SIGUSR1: %d en el proceso : %d.\n",id,getpid());
	contadorSIGUSR1 ++;
	char  buffer[2][100];
	snprintf(buffer[1],100,"%d",getpid());
	snprintf(buffer[2],100,"%d",contadorSIGUSR1);
	char* argv[]={buffer[1],buffer[2],(char*) NULL};
	execv("./contador",argv);
}
/*• SIGUSR1: Al enviar esta se˜nal, el proceso con el id correspondiente deber´a crear un nuevo proceso,
el cual ejecutar´a un archivo llamado ”contador.c” mediante el uso de alguna funci´on de la familia de
exec. El c´odigo contador.c debe mantener un acumulador que parte desde 0, y cada vez que se ejecuta
esta se˜nal sobre el proceso, sumar´a uno al contador, de manera que cuando el proceso reciba la se˜nal,
imprimir´a por pantalla:
>> pid: X, y he recibido esta llamada i veces.
Cabe destacar que una vez finalizada la ejecuci´on de c´odigo.c, el proceso asociado a ´este debe ser
matado (mediante cualquier m´etodo). Por lo tanto, cada vez que se ejecute SIGUSR1, se deber´a
crear un nuevo proceso. Note que el proceso que crear al encargado de ejecutar el contador, debe
permanecer atento a las se˜nales que el padre env´ıe.
*/

void controladorSIGUSR2(int id)
{
	pid_t nuevoHijo;
	nuevoHijo = fork();
	printf("Soy el PID: %d y he creado un hijo de PID: %d.\n",getpid(),nuevoHijo);

}

//SIGUSR2: que el proceso hijo al cual se le env´ıa la se˜nal (receptor), cree su propio hijo.

void controladorSIGINT(int id)
{	int i;
	if(contadorSIGINT == 0)
	{
		if(!soyHijo(hijos, hValue)){
			printf("\n");
			for(i = 0; i< hValue;i++){
				kill(hijos[i],7);
				printf("Soy el hijo con PID :%d, y estoy vivo aun. No me mates papa :(.\n",hijos[i]);
			}
		}

	}
	if(contadorSIGINT)
	{
		exit(0);
	}
	contadorSIGINT++;
}

/* SIGTERM: Se encarga de matar al proceso con el id correspondiente. No obstante, antes de morir
el proceso deber´a escribir en consola: <Soy el hijo con pid: X y mi pap´a me quiere matar>.*/

int soyHijo(pid_t* hijosProceso,int n){
	int i;
	for(i = 0; i < n; i++){
		if(hijosProceso[i] == 0){
			return 1;
		}
	}
	return 0;
}

pid_t* inicializarHijos(int numHijos, int mflag){
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
				res[i] = fork();
				if(res[i]<0){
					printf("Error al crear proceso hijo\n");
					free(res);
					return NULL;			
				}
				if(soyHijo(res,numHijos)==0 && mflag == 1)
				printf("El hijo %d tiene el pid %d\n",i+1,res[i]);
			}
		}
		return res;
	}
}

void enviarSenal(int target,int senal)
{
	switch(senal)
	{
		case 2:
		{
			kill(target,SIGINT);
		}break;
		case 9:
		{
			kill(target,SIGTERM);
		}break;
		case 10:
		{
			kill(target,SIGUSR1);
		}break;
		case 12:
		{
			kill(target,SIGUSR2);
		}break;
		default:
		{
			printf("Ingrese una señal valida.\n");
		}
	}
}

void recibirSenales(pid_t *res, int cantHijos){
	int iterar = 1;
	int pidTarget;
	int signal;
	while(iterar != 0){
		printf("Ingrese el número del proceso al que desea enviar una señal: ");
		scanf("%d",&pidTarget);
		printf("\n");
		printf("Ingrese la señal que se enviara al proceso %d: ",pidTarget);
		scanf("%d",&signal);
		printf("\n");
		printf("La senal %d sera enviada al hijo %d de pid %d\n",signal,pidTarget, res[pidTarget-1]);

		//printf("Resultado kill :%d \n",kill(res[pidTarget-1],signal));
		if(pidTarget > 0 && pidTarget <= cantHijos)
		{
			enviarSenal(res[pidTarget-1], signal);
		}
		if(getchar()!=10){
			iterar = 0;
		}
	}
	
}

int main(int argc,char** argv){
	//manejo de getopt
	hValue = 0;
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
					fprintf(stderr, "Opcion -h requiere un argumento.\n");
				}
				else if(isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida -h.\n");
				}
				else{
					fprintf(stderr,"Opcion con caracter desconocido.\n");
				}
				return -1;
			default:
				abort();
		}
	}
	hijos = inicializarHijos(hValue, mflag);
	signal(SIGTERM, controladorSIGTERM);
	signal(SIGUSR1, controladorSIGUSR1);
	signal(SIGUSR2, controladorSIGUSR2);
	signal(SIGINT, controladorSIGINT);
	signal(7, controladorKILL);
	if(!soyHijo(hijos,hValue)){
		recibirSenales(hijos, hValue);
	}
	getchar();
	return 0;
}
