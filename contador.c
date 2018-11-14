#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc,char** argv){
	printf("************CONTADOR************\n");
	printf("Soy contador. El proceso %s me ha invocado %s veces\n",argv[0],argv[1]);
	return 0;
	/*int i;
	for(i=0;i<argc;i++){
		printf("argv[%d] = %s\n",i,argv[i]);
	}
	int pidSource;
	int count;
	opterr = 0;
	int q;
	int index;
	
	while((q = getopt(argc,argv, "p:c:")) != -1){
		switch(q){
			case 'p':
				sscanf(optarg,"%d",&pidSource);
				break;
			case 'c':
				sscanf(optarg,"%d", &count);
				break;
			case '?':
				if(optopt == 'c'){
					fprintf(stderr, "Opcion -c requiere un argumento.\n");
				}
				else if(isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida -c.\n");
				}
				else{
					fprintf(stderr,"Opcion con caracter desconocido.\n");
				}
				return -1;
			default:
				abort();
		}
	}
	printf("Valores leidos: pid: %d, count: %d\n",pidSource,count );*/
}