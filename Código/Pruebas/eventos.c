#include <stdio.h>
#include <unistd.h>   
#include <signal.h>
#include <stdlib.h> /* exit() */



/* Función principal */
int main( int argc, char *argv[] ){
	
	int i, j, opcion;
	long duracion1=0, duracion2=600000000;
	
	opcion = atoi( argv[0] );
	
	switch( opcion ){
		
		case 0: duracion1 = 4; break; 
		case 1: duracion1 = 12; break; /* Con este tiempo el 
proceso dura más que el tiempo máximo de nuestro sistema, MAX_T*/
	
	}	
	
	printf( "\nEl proceso %s, con pid %ld, ha comenzado.\n", "EVENTO", (long)getpid());
	
	for( i=0; i<duracion1; i++ ){ 
		for( j=0; j<duracion2; j++ );
		printf( "\n\tEl proceso %s, con pid %ld, está ejecutándose.\n",  "EVENTO", (long)getpid());
	}
 
	exit( EXIT_SUCCESS ); 
	return 0;
}
