

#include <stdio.h>
#include <unistd.h>   
#include <signal.h>
#include <stdlib.h> 



/* Función principal */
int main( int argc, char *argv[] ){
	
	int i, j, bloqueado, duracion=0;

	bloqueado = atoi( argv[0] );
	
	if( bloqueado == 0 ){
		duracion = 4; 
	}
	else{
		
		if( bloqueado == 1 )
			duracion = 20; /* Con este tiempo el proceso 
dura más que el tiempo máximo de nuestro sistema, MAX_T. */
		else
			exit( EXIT_FAILURE );
		
	}
	
	printf( "\nEl proceso %s, con pid %ld, ha comenzado.\n","CANCELACION", (long)getpid());
	
	for( i=0; i<duracion; i++ ){ 
		for( j=0; j<600000000; j++ );
		printf( "\n\tEl proceso %s, con pid %ld, está ejecutándose.\n", "CANCELACION", (long)getpid());
	}
 
	exit( EXIT_SUCCESS ); 
	return 0;
}
