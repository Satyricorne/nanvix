/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <nanvix/klib.h>
#include <signal.h>
int back_counter;

/**
 * @brief Schedules a process to execution.
 * 
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 * 
 * @param proc Process to be resumed.
 * 
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{	
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	struct process *p;    /* Working process.     */
	struct process *np; /* Next process to run. */

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;

	/* Check alarm. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;
		
		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	/* Choose a process to run next. */
	np = IDLE;
	
		/* Skip non-ready process. */
		
		
	struct process *tmp; /* Next process to run. */
		
	np = foreground;
	while (np->state != PROC_READY && np != NULL){	// On cherche le premier process ready dans foreground
		tmp = np;
		np = np->next;
	}
	if (np == NULL)	// Il n'y a aucun process ready dans foreground
	{
		np = background;
		while(np->state != PROC_READY && np != NULL) // On cherche un process ready dans background
			np = np->next;
		tmp->next = np->next;
	} else {					// On a trouvé un process ready dans foreground
		tmp->next = np->next;
		back_counter++;
	}	
	if(back_counter>42){		// Le conteur d'anti famine se reveil
		back_counter = 0;
		while(np->next != NULL)	// On va chercher le dernier process de foreground
			np=np->next;
		np->next = background;	// On met la chaine background au bout du foreground
		background = NULL;		// On vide la chaine background
	}

	
	/* Switch to next process. */
	np->priority = PRIO_USER;
	np->state = PROC_RUNNING;
	np->counter = PROC_QUANTUM;
	switch_to(np);
}
