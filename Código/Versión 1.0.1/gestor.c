#include <linux/gestor.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define PAGO "pagos"
#define ANULACION "anulaciones"
#define PRERRESERVA "pre-reservas"
#define EVENTO "eventos"
#define GRADA "gradas"

#define X 2
#define K 5
#define N 7

#define MAX_TIME 10

#define LOW_LOAD 1
#define MED_LOAD 2
#define HIGH_LOAD 3
#define LIMIT_LOAD 4

int contPagos = 0, contAnulaciones = 0, contPrerreservas = 0, contEventos = 0, contGradas = 0;

void admin(struct task_struct *tsk) {
// Comprobamos que los procesos sean uno de los 5 tipos que estamos tratando. En caso afirmativo, se continua su procesamiento con el metodo admin.	
	printk(KERN_ERR "%s\n", tsk->comm);
	if((strcmp(tsk->comm,PAGO)==0)||(strcmp(tsk->comm,ANULACION)==0)||(strcmp(tsk->comm,PRERRESERVA)==0)||(strcmp(tsk->comm,GRADA)==0)||(strcmp(tsk->comm,EVENTO)==0)){
		
		switch(get_sys_load(contPagos, contAnulaciones, contPrerreservas, contEventos, contGradas)) {
			case LOW_LOAD:
			printk(KERN_ERR "Carga del sistema baja. Encolado en FIFO\n"); 
			add_to_queue(tsk, 0);
			break;

			case MED_LOAD:
			printk(KERN_ERR "Carga del sistema media. Encolado según las prioridades\n");
			add_to_queue(tsk, 1);
			break;

			case HIGH_LOAD:
			printk(KERN_ERR "Carga del sistema alta. Encolando solo pagos\n");
			if (strcmp(tsk->comm, PAGO) == 0) {
				printk(KERN_ERR "Pago encolado");
				add_to_queue(tsk, 1);
			} else {
				printk(KERN_ERR "Proceso eliminado, no es de pagos\n");
				increase_counter(tsk);
				kill_process(tsk);
			}
			break;

			case LIMIT_LOAD:
			printk(KERN_ERR "Carga límite. Reemplazando procesos de menor prioridad para encolar pagos\n");
			if (strcmp(tsk->comm, PAGO) == 0) {	
				replace_process(tsk);
				 printk(KERN_ERR "Proceso reemplazado\n");
			} else {
				increase_counter(tsk);
				kill_process(tsk);
				 printk(KERN_ERR "No se puede encolar, todos los procesos son de pagos\n");
			}
			break;
		} //Cierre switch
	} //Cierre if
} //Cierre admin

void check_time(){
	struct task_struct *tsk;

	for_each_process(tsk){
		if(cputime_to_secs(tsk->utime)>=MAX_TIME){
			if((strcmp(tsk->comm,PAGO)==0)||(strcmp(tsk->comm,ANULACION)==0)||(strcmp(tsk->comm,PRERRESERVA)==0)||(strcmp(tsk->comm,GRADA)==0)||(strcmp(tsk->comm,EVENTO)==0)){
				// Comprobamos que los procesos sean uno de los 5 tipos que estamos tratando. En caso afirmativo, se mata por superar el tiempo maximo en procesador permitido.
				printk(KERN_ERR "Tiempo del proceso en CPU excedido. Se eliminara el proceso con PID %d\n",tsk->pid);
				kill_process(tsk);
			}
		}
	}
} //Cierre check_time

int get_sys_load (int contPagos, int contAnulaciones, int contPrerreservas, int contEventos, int contGradas) {

	int num_procesos = contPagos + contAnulaciones + contPrerreservas + contEventos + contGradas;

	printk(KERN_ERR "Número de procesos (ejecución + cola): %d", num_procesos);

	if ( num_procesos <= X) {
		return LOW_LOAD;
	}
	else if (num_procesos <= K) {
		return MED_LOAD;
	} else if (num_procesos <= N) {
		return HIGH_LOAD;
	} else {
		return LIMIT_LOAD;
	}
} //Cierre get_sys_load

void add_to_queue(struct task_struct *tsk, int aux){

	char tipo = tsk->comm;
	struct sched_param prioridad;
	if (aux == 1) {
	        if (strcmp(tipo, PAGO) == 0) {
       		        prioridad.sched_priority = 85;
       		        sched_setscheduler(tsk, SCHED_FIFO, &prioridad);
        	}
        	else if (strcmp(tipo, ANULACION) == 0) {
                	prioridad.sched_priority = 80;
                	sched_setscheduler(tsk, SCHED_FIFO, &prioridad);
        	}
        	else if (strcmp(tipo, PRERRESERVA) == 0) {
                	prioridad.sched_priority = 75;
                	sched_setscheduler(tsk, SCHED_FIFO, &prioridad);
        	}
        	else if (strcmp(tipo, EVENTO) == 0) {
                	prioridad.sched_priority = 70;
                	sched_setscheduler(tsk, SCHED_FIFO, &prioridad);
        	}
        	else if (strcmp(tipo, GRADA) == 0) {
                	prioridad.sched_priority = 65;
                	sched_setscheduler(tsk, SCHED_FIFO, &prioridad);
        	}
		increase_counter(tsk);
	
	} else {
		printk(KERN_ERR "Se añade a la cola FIFO\n");
                prioridad.sched_priority = 85;
                sched_setscheduler(tsk, SCHED_FIFO, &prioridad);

		increase_counter(tsk);
	}

} //Cierre add_to_queue

void kill_process(struct task_struct *tsk){
	// Así es como lo queríamos hacer nosotros, no recuerdo exactamente la solución que nos habían dado
	//ellos.
	kill_pid(find_vpid(tsk->pid), SIGKILL, 1);
} //Cierre kill_process

int replace_process(struct task_struct *task_param){

	struct task_struct *tsk; // Estructura para el bucle
	int prio_param = 65; // Valor inicial de la prioridad a buscar
	int prio_pro = 0; // Prioridad del proceso a consultar

	if (prio_param < 85) {
	  for_each_process(tsk){

	    prio_pro = tsk->rt_priority;

			if (prio_pro == prio_param) {
				kill_process(tsk);
				add_to_queue(task_param, 1);
				return 0;
			}

		} //Cierre for_each_process

		prio_param += 5;
	}

	return -1; //Si no reemplazamos nada devolvemos un -1
} //Cierre replace_process

void decrease_counter(struct task_struct *tsk) {
	char tipo = tsk->comm;
                if (strcmp(tipo, PAGO) == 0) {
                        contPagos--;
                }
                if (strcmp(tipo, ANULACION) == 0) {
                        contAnulaciones--;
                }
                if (strcmp(tipo, PRERRESERVA) == 0) {
                        contPrerreservas--;
                }
                if (strcmp(tipo, EVENTO) == 0) {
                        contEventos--;
                }
                if (strcmp(tipo, GRADA) == 0) {
                        contGradas--;
                }
} //Cierre decrease_counter

void increase_counter(struct task_struct *tsk) {
        char tipo = tsk->comm;
                if (strcmp(tipo, PAGO) == 0) {
                        contPagos++;
                }
                if (strcmp(tipo, ANULACION) == 0) {
                        contAnulaciones++;
                }
                if (strcmp(tipo, PRERRESERVA) == 0) {
                        contPrerreservas++;
                }
                if (strcmp(tipo, EVENTO) == 0) {
                        contEventos++;
                }
                if (strcmp(tipo, GRADA) == 0) {
                        contGradas++;
                }
} //Cierre increase_counter

