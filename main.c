#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

void controladorSIGTERM(int id);
void controladorSIGUSR1(int id);
void controladorSIGUSR2(int id);
void controladorSIGINT(int id);
int enviarSenal(int target,int senal);
void recibirSenales(pid_t *res,int cantHijos);
int soyHijo(pid_t* hijosProceso,int n);
pid_t* inicializarHijos(int numHijos, int mflag);

int contadorSIGUSR1 = 0;
int contadorSIGINT = 0;
pid_t* hijos;
int hValue;

//Esta funcion es ejecutada cuando se le envia la señal SIGTERM a un proceso
//se encarga de matar al proceso que se le envia la señal
void controladorSIGTERM(int id)
{
    printf("\nSoy el hijo con pid %d y mi papa me quiere matar :(.\n",getpid());
    exit(0);
}


//Esta funcion es ejecutada cuando se le envia la señal SIGUSR1 a un proceso
//se encarga de sumar un 1 al contador
void controladorSIGUSR1(int id)
{
    printf("\nSoy el proceso %d y recibi la senal SIGUSR1: %d\n",getpid(),id);
    printf("Recibida la señal SIGUSR1: %d en el proceso : %d.\n",id,getpid());
    contadorSIGUSR1 ++;
    char  buffer[2][100];
    snprintf(buffer[0],100,"%d",getpid());
    snprintf(buffer[1],100,"%d",contadorSIGUSR1);
    char* argv[]={buffer[0],buffer[1],(char*) NULL};
    execv("./contador",argv);
}

//Esta funcion es ejecutada cuando se le envia la señal SIGUSR2 a un proceso
//se encarga de que el hijo al que se le envia la señal cree un hijo
void controladorSIGUSR2(int id)
{
    printf("\nSoy el proceso %d y recibi la senal SIGUSR2: %d\n",getpid(),id);
    pid_t nuevoHijo;
    nuevoHijo = fork();
    if(nuevoHijo != 0)
    {
        printf("Soy el PID: %d y he creado un hijo de PID: %d.\n",getpid(),nuevoHijo);
    }
}

//Esta funcion es ejecutada cuando se le envia la señal SIGINT a un proceso
//se encarga de enviar un aviso cuando se presiona Ctrl+C y de finalizar el programa se presiona denuevo
void controladorSIGINT(int id)
{   
    int i;
    if(contadorSIGINT == 0)
    {
        if(!soyHijo(hijos, hValue)){
            printf("\n");
            for(i = 0; i< hValue;i++){
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

//Esta funcion se encarga de coordinar que señal envíar a que hijo una vez ingresada la solicitud por parte del usuario
int enviarSenal(int target,int senal)
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
    return 0;
}

//Esta funcion solicita al usuario ingresar el hijo y la señal que desea enviar
void recibirSenales(pid_t *res, int cantHijos){
    int iterar = 1;
    int pidTarget;
    int signal;
    while(iterar != 0){
        printf("\n----------------------------------------------------------------------");
        printf("\nIngrese el número del proceso al que desea enviar una señal: ");
        scanf("%d",&pidTarget);
        printf("Ingrese la señal que se enviara al proceso %d: ",pidTarget);
        scanf("%d",&signal);
        
        if(enviarSenal(res[pidTarget-1], signal)){}
        else{
            printf("No se pudo enviar la señal al proceso indicado\n");
        }
        if(getchar()!=10){
            iterar = 0;
        }
        getchar();
    }
    
}

//Esta funcion verifica si los procesos son hijos
int soyHijo(pid_t* hijosProceso,int n){
    int i;
    for(i = 0; i < n; i++){
        if(hijosProceso[i] == 0){
            return 1;
        }
    }
    return 0;
}

//Esta funcion inicializa los hijos solicitados como parametro de entrada
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

int main(int argc,char** argv)
{
    //señales
    signal(SIGTERM, controladorSIGTERM);
    signal(SIGUSR1, controladorSIGUSR1);
    signal(SIGUSR2, controladorSIGUSR2);
    signal(SIGINT, controladorSIGINT);
    //manejo de getopt
    hValue = 0;
    int mflag = 0;
    opterr = 0;
    int i,c;
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
    for(i=0;i<hValue;i++)
    {
        if(hijos[i] == 0)
        {
            for(;;);
        }
    }
    recibirSenales(hijos, hValue);
    return 0;
}