#ifndef _GESTOR_H
#define _GESTOR_H
 
#include <linux/kernel.h>
#include <linux/sched.h>

void admin(struct task_struct *tsk);
int process_counter(void);
void check_time(void);
int get_sys_load (int contPagos, int contAnulaciones, int contPrerreservas, int contEventos, int contGradas);
void add_to_queue(struct task_struct *tsk, int aux);
void kill_process(struct task_struct *tsk);
int replace_process(struct task_struct *task_param);
void decrementar_contador(struct task_struct *tsk);
void aumentar_contador(struct task_struct *tsk);
#endif