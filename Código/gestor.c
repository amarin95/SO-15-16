#define PAGO "pago"
#define ANULACION "anulacion"
#define PRERRESERVA "pre_reserva"
#define EVENTO "evento"
#define GRADA "grada"

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

	//Comprobar si los contadores son accesibles desde el otro metodo al estar definidos fuera de admin()
	switch(get_sys_load(contPagos, contAnulaciones, contPrerreservas, contEventos, contGradas)) {
		case LOW_LOAD: 
		add_to_queue(tsk);
		break;

		case MED_LOAD:
		add_to_queue(tsk);
		break;

		case HIGH_LOAD:
		if (strcmp(tsk->comm, PAGO) == 0) {
			add_to_queue(tsk);
		} else {
			kill_process(tsk);
		}
		break;

		case LIMIT_LOAD:
		if (strcmp(tsk->comm, PAGO) == 0) {
			replace_process(tsk);
		} else {
			kill_process(tsk);
		}
		break;
	}
} //Cierre admin

int get_sys_load (int contPagos, int contAnulaciones, int contPrerreservas, int contEventos, int contGradas) {

	int num_procesos = contPagos + contAnulaciones + contPrerreservas + contEventos + contGradas;

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

void add_to_queue(struct task_struct *tsk){

	char tipo = tsk->comm;
	struct sched_param prioridad;

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

	} else {
		// ¿Darle una prioridad más baja si es otro tipo?
	}

	// Si le vamos a cambiar la prioridad a todos los procesos (incluso los que no son del proyecto)
	//se le asigna en el último else y podemos poner aquí el sched_setscheduler
    //sched_setscheduler(tsk, SCHED_FIFO, &prioridad);

} //Cierre add_to_queue

void kill_process(struct task_struct *tsk) {
	// Así es como lo queríamos hacer nosotros, no recuerdo exactamente la solución que nos habían dado
	//ellos.
	kill_pid(find_vpid(proceso->pid), SIGKILL, 1);
} //Cierre kill_process

int replace_process(struct task_struct *tsk) {

	struct task_struct *task_buc; // Estructura para el bucle
	int prio_param = 65; // Valor inicial de la prioridad a buscar
	int prio_pro = 0; // Prioridad del proceso a consultar

	if (prio_param < 85) {
		for_each_process(task_buc) {

			prio_pro = task_buc->sched_priority;

			if (prio_pro == prio_param) {
				kill_process(task_buc);
				add_to_queue(tsk);
				return 0;
			}

		} //Cierre for_each_process

		prio_param += 5;
	}

	return -1; //Si no reemplazamos nada devolvemos un -1
} //Cierre replace_process