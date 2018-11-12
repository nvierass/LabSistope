#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc,char** argv){
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
	printf("Valores leidos: pid: %d, count: %d\n",pidSource,count );
}