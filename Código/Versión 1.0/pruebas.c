#include <signal.h>
#include <unistd.h>   /* _getch */
#include <stdlib.h> /* exit() */
#include <stdio.h>
#include <errno.h> /* Códigos de errores. */
#include <termios.h>  /* _getch */
#include <sys/types.h>
#include <sys/wait.h> /* wait() */
#include <string.h> /* memset() */
#include <sched.h> /* struct sched_param */



/* Prototipos de funciones */
void establecerPrioridadPruebas(void);
void mostrarOpciones(void);
char* buscarNombre(pid_t);
void prueba1(void);
void prueba2(void);
void prueba3(void);
void prueba4(void);
void prueba5(void);
void prueba6(void);
void prueba7(void);
void prueba8(void);
void prueba9(void);
void prueba10(void);
/* VARIABLES GLOBALES */
int pids[8];
char *tipos[8];


/* Función principal */
int main( int argc, char *argv[] ){
	
	int opcion;

	establecerPrioridadPruebas();
/* RESETEO DE VARIABLES */
		memset( pids, 0, sizeof( pids ) ); /* Se borra el array pids. */
		memset( tipos, 0, sizeof( tipos ) ); /* Se borra el array tipos.*/
	mostrarOpciones();
	printf("\n\n\tIntroduzca opcion: ");
	scanf("%d",&opcion);
	switch ( opcion ){
		case 1: prueba1(); break;
		case 2: prueba2(); break;
		case 3: prueba3(); break;
		case 4: prueba4(); break;
		case 5: prueba5(); break;
		case 6: prueba6(); break;
		case 7: prueba7(); break;
		case 8: prueba8(); break;
		case 9: prueba9(); break;
		case 10: prueba10();break;
		case 11: break;
	}
	return 0;
}

void establecerPrioridadPruebas(void){
	
	struct sched_param nodo;
	
	nodo.sched_priority = 99;
	
	if( sched_setscheduler( 0, SCHED_FIFO, &nodo ) != 0 ){
		perror( "ERROR: no se ha podido cambiar prioridad del proceso de pruebas" );
		exit(EXIT_FAILURE);
	}
	
	return;
}

void mostrarOpciones(void){

	printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n" );
	printf( "\t     \n" );
	printf( "\t                                       \n" );
	printf( "\t Seleccionar una prueba: (Pulsar la tecla asignada) \n" );
	printf( "\t                                       \n" );
	printf( "\t     \n" );

	printf("\t 1)Prueba 1: Llega un nuevo proceso\n");
	printf("\t 2)Prueba 2: Llegada de dos procesos,anulaciones y luego pagos\n");
	printf("\t 3)Prueba 3: Hay un proceso en ejecución, dos en cola y llega un nuevo proceso\n");
	printf("\t 4)Prueba 4: Hay un proceso en ejecución, cuatro en cola y llega un nuevo proceso\n");
	printf("\t 5)Prueba 5: Hay un proceso en ejecución, cinco en cola y llega un nuevo proceso (Se descarta)\n");
	printf("\t 6)Prueba 6: Hay un proceso en ejecución, seis en cola y llega un nuevo proceso\n");
	printf("\t 7)Prueba 7: Entra a cola proceso de tipo pagos en HIGH_LOAD\n");
	printf("\t 8)Prueba 8: Sustitución de proceso en HIGH_LOAD\n");
	printf("\t 9)Prueba 9: Superacion de tiempo de un proceso\n");
	printf("\t 10)Prueba 10: Cola llena, llega pago\n");
	
	printf("\t 11)SALIR");

	//INCLUIR MAS 


}

char * buscarNombre( pid_t pid ){

	int i;
	
	for( i=0; i<8; i++ ){
		if( (long)pids[i] == (long)pid ) return tipos[i];
	} 

	return "ps"; 
}



void prueba1(void){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("Prueba 1: Incluir un proceso en cola. Ningun proceso en ejecución, entra a ser atendido");
	
	pid_t pid;
	int i,cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n" );

	for(i=0; i<1;i++){
		switch(i){
			case 0:
			tipob = "anulaciones";
			bloqueo = "0";
			break;
		}	
		
		pid = fork();
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ 


			printf( "\nLlega un proceso de tipo anulaciones");

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}

	}
	while( ( pid = wait(&cod_finalizacion) ) > 0 ){
		nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) 
			printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) 
			printf( "\n%sEl proceso %s, con pid %ld, ha sido matado (utime>MAX_TIME).\n", nompre_proceso, (long)pid);
	}

	return;
}

void prueba2(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 2: Hay un proceso ejecutándose. Llegan dos procesos\n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=0; i < 3; i++ ){ 

		switch(i){
			case 0:
				tipob="anulaciones";
				bloqueo="0";
				break;
			case 1:
				tipob="anulaciones";
				bloqueo="0";
				break;
			case 2:
				tipob="pagos";
				bloqueo="0";
				break;
		}
		
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ 

			sleep(i*i/2);

			printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}	

	} // Cierre for
	
	while( ( pid = wait(&cod_finalizacion) ) > 0 ){
		nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
			printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
			printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
	}


	return;
}


void prueba3(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 3: Llegada de dos procesos\n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i = 0; i < 4; i++ ){ 

		switch(i){
			case 0:
			tipob="pagos";
			bloqueo="0";
			break;
			case 1:
			tipob="gradas";
			bloqueo="0";
			break;
			case 2:
			tipob="anulaciones";
			bloqueo="0";
			break;
			case 3:
			tipob="pagos";
			bloqueo="0";
			break;
		}
		
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ 

			sleep(i*i/2);

			printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}	

	}
	
	while( ( pid = wait(&cod_finalizacion) ) > 0 ){
		nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
		printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
		printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
	}
	return;
}






void prueba4(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 4:  Proceso con dos en cola, cambio de contexto\n");


	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=0; i<6; i++ ){ 

		switch(i){
			case 0:
			tipob="pagos";
			bloqueo="0";
			break;
			case 1:
			tipob="gradas";
			bloqueo="0";
			break;
			case 2:
			tipob="anulaciones";
			bloqueo="0";
			break;
                        case 3:
                        tipob="anulaciones";
                        bloqueo="0";
                        break;
                        case 4:
                        tipob="pre-reservas";
                        bloqueo="0";
                        break;
			case 5:
			tipob="pagos";
			bloqueo="0";
			break;
		}
		
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ 

			sleep(i/2);

			printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}	

	}/* fin for */

		while( ( pid = wait(&cod_finalizacion) ) > 0 ){
			nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
			printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
			printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
		}


		return;
	}



	void prueba5(void){
		printf("\n\n\n\n\n\n\n\n\n");
		printf("Prueba 5: \n");


		pid_t pid;
		int i, cod_finalizacion;
		char *tipob, *bloqueo, *nompre_proceso;


		for( i=0; i<7; i++ ){ 
			
			switch(i){
                        case 0:
                        tipob="pagos";
                        bloqueo="0";
                        break;
                        case 1:
                        tipob="gradas";
                        bloqueo="0";
                        break;
                        case 2:
                        tipob="anulaciones";
                        bloqueo="0";
                        break;
                        case 3:
                        tipob="anulaciones";
                        bloqueo="0";
                        break;
                        case 4:
                        tipob="anulaciones";
                        bloqueo="0";
                        break;
                        case 5:
                        tipob="pre-reservas";
                        bloqueo="0";
                        break;
                        case 6:
                        tipob="gradas";
                        bloqueo="0";
                        break;

			}

			pid = fork();

			pids[i] = pid;
			tipos[i] = tipob;

			if( pid == -1 ){
				perror( "Error al tratar de crear un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */

			sleep(i/2);

			printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}	

	}
	
	while( ( pid = wait(&cod_finalizacion) ) > 0 ){
		nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
		printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
		printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
	}


	return;
}

void prueba6(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 6:  Gradas descartado \n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=0; i<7; i++ ){ 

		switch(i){
			case 0:
			tipob="pagos";	
			bloqueo="0";
			break;
			case 1:
			tipob="gradas";
			bloqueo="0";
			break;
			case 2:
			tipob="anulaciones";
			bloqueo="0";
			break;
			case 3:
			tipob="anulaciones";
			bloqueo="0";
			break;
			case 4:
			tipob="anulaciones";
			bloqueo="0";
			break;
			case 5:
			tipob="pre-reservas";
			bloqueo="0";
			break;
			case 6: 
			tipob="gradas";
			bloqueo="0";
			break;

		}
		
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */

		sleep(i/3);

		printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );
		
		if( execl( tipob, bloqueo, NULL ) == -1 ){ 
			perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
			exit( EXIT_FAILURE );
		}
	}	

}

while( ( pid = wait(&cod_finalizacion) ) > 0 ){
	nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
	printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
	printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
}


return;
}

void prueba7(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 7:  HIGH_LOAD \n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=1; i<7; i++ ){ 

		switch(i){

			case 1:
			tipob="pagos";
			bloqueo="0";
			break;
			case 2:
			tipob="pagos";	
			bloqueo="0";
			break;
			case 3:
			tipob="pagos";
			bloqueo="0";
			break;

			case 4:
			tipob="pagos";
			bloqueo="0";
			break;
			case 5:
			tipob="pagos";
			bloqueo="0";
			break;
			case 6:
			tipob="pagos";
			bloqueo="0";
			break;
			case 7:
			tipob="pagos";
			bloqueo="0";
			break;

		}
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */

		sleep(i/4);

		printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );
		
		if( execl( tipob, bloqueo, NULL ) == -1 ){ 
			perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
			exit( EXIT_FAILURE );
		}
	}	

}

while( ( pid = wait(&cod_finalizacion) ) > 0 ){
	nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
	printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
	printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
}


return;
}

void prueba8(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 8:  HIGH_LOAD \n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=1; i<7; i++ ){ 

		switch(i){

			case 1:
			tipob="anulaciones";
			bloqueo="0";
			break;
			case 2:
			tipob="anulaciones";	
			bloqueo="0";
			break;
			case 3:
			tipob="anulaciones";
			bloqueo="0";
			break;

			case 4:
			tipob="pagos";
			bloqueo="0";
			break;
			case 5:
			tipob="pagos";
			bloqueo="0";
			break;
			case 6:
			tipob="gradas";
			bloqueo="0";
			break;
			case 7:
			tipob="pagos";
			bloqueo="0";
			break;

		}
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */

		sleep(i/4);

		printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );
		
		if( execl( tipob, bloqueo, NULL ) == -1 ){ 
			perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
			exit( EXIT_FAILURE );
		}
	}	

}

while( ( pid = wait(&cod_finalizacion) ) > 0 ){
	nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
	printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
	printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
}


return;
}
void prueba9(void){
	printf("\n\n\n\n\n\n\n\n\n");
	printf("Prueba 9:  MAX_T \n");

	pid_t pid;
	int i, cod_finalizacion;
	char *tipob, *bloqueo, *nompre_proceso;
	

	for( i=0; i<1; i++ ){ 

		switch(i){
			case 0:
			tipob="pagos";

			bloqueo="1";


		}
		
		pid = fork();
		
		pids[i] = pid;
		tipos[i] = tipob;
		
		if( pid == -1 ){
			perror( "Error al tratar de crear un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */
		
		sleep(i*i/2);

		printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );
		
		if( execl( tipob, bloqueo, NULL ) == -1 ){ 
			perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
			exit( EXIT_FAILURE );
		}
		}/* fin else if */	

	}/* fin for */

		while( ( pid = wait(&cod_finalizacion) ) > 0 ){
			nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
			printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
			printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
		}


		return;
	}



	void prueba10(void){
		printf("\n\n\n\n\n\n\n\n\n");
		printf("Prueba 10:  Cola_llena \n");

		pid_t pid;
		int i, cod_finalizacion;
		char *tipob, *bloqueo, *nompre_proceso;


		for( i=0; i<9; i++ ){ 
			
			switch(i){
				case 0:
				tipob="pagos";
				bloqueo="0";
				break;
				case 1:
				tipob="pagos";
				bloqueo="0";
				break;
				case 2:
				tipob="pagos";
				bloqueo="0";
				break;
				case 3:
				tipob="pagos";
				bloqueo="0";
				break;

				case 4:
				tipob="pagos";
				bloqueo="0";
				break;
				case 5:
				tipob="pagos";
				bloqueo="0";
				break;
				case 6:
				tipob="pagos";
				bloqueo="0";
				break;
				case 7:
				tipob="pagos";
				bloqueo="0";
				break;
				case 8:
				tipob="pagos";
				bloqueo="0";
				break;
                                case 9:
                                tipob="pagos";
                                bloqueo="0";
                                break;

			}
			pid = fork();

			pids[i] = pid;
			tipos[i] = tipob;

			if( pid == -1 ){
				perror( "Error al tratar de crear un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		else if( pid == 0 ){ /* Código que va a ejecutar el proceso hijo. */

			sleep(i/8);

			printf( "\nLlega %s%d: %ld\n", tipob, i+1, (long)getpid() );

			if( execl( tipob, bloqueo, NULL ) == -1 ){ 
				perror( "Error al llamar a la función execl() para ejecutar un proceso hijo." );
				exit( EXIT_FAILURE );
			}
		}	

	}
	
	while( ( pid = wait(&cod_finalizacion) ) > 0 ){
		nompre_proceso = buscarNombre( pid );
		if( cod_finalizacion == 0 ) /* Exit */
		printf( "\nEl proceso %s, con pid %ld, ha finalizado.\n",  nompre_proceso, (long)pid);
		if( cod_finalizacion == 9 ) /* kill */
		printf( "\nEl proceso %s, con pid %ld, ha sido matado.\n", nompre_proceso, (long)pid);
	}


	return;
}

