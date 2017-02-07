/*
 * Pokrocile operacni systemy: 1. projekt - signaly
 * Author: Anna Ostroukh <xostro00>
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

/*
 * Shared variables for tracing signals SIGUSR1, SIGUSR2
 */
static volatile int signaled1, signaled2;
char ch = 'A';
/*
 * Function for tracing signals SIGUSR1, SIGUSR2
 */
static void catcher(int sig) {
	if(sig == SIGUSR1) {
		signaled1 = 1;    //if SIGUSR1 is accepted
	} else if (sig == SIGUSR2) {
		signaled2 = 1;	  //if SIGUSR2 is accepted
	}
}

int main(void) {
	pid_t pid;
	struct sigaction sigact;
	sigset_t setusr, setemp;
	int status;
	/*
	 * Set setusr for temporary blocking SIGUSR signal
	 */
	sigemptyset(&setusr);
	sigaddset(&setusr, SIGUSR1); //add SIGUSR1 to the set of signals
	sigaddset(&setusr, SIGUSR2); //add SIGUSR2 to the set of signals
	/*
	 * Change of handling SIGUSR signal on custom function catcher()
	 */
	sigact.sa_handler = catcher;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;

	if (sigaction(SIGUSR1, &sigact, NULL) == -1) {
		perror("sigaction SIGUSR1 error");
		return 1;
	}
	if (sigaction(SIGUSR2, &sigact, NULL) == -1) {
		perror("sigaction SIGUSR2 error");
		return 1;
	}
	sigprocmask(SIG_BLOCK, &setusr, &setemp); //SIGUSR1 && SIGUSR2 blocking 
	
	if ((pid = fork()) == 0) { //child process
		while (1) { 
			sigprocmask(SIG_BLOCK, &setusr, NULL); 	//SIGUSR1 && SIGUSR2 blocking without saving previous states

			while(!signaled1)
				sigsuspend(&setemp); //waiting for signal from set

			if (signaled2) { 	//if SIGUSR2 received 
				signaled2 = 0;
				ch = 'A';
			}	

			if (signaled1) {	//signal from parent received
				printf("Child (%d): '%c'\n", getpid(), ch);
				signaled1 = 0;
				ch++;
				if (ch > 'Z')
					ch = 'A';
			}

			if (kill(getppid(), SIGUSR1) == -1) perror("kill child error"); //send signal to parent

			sigprocmask(SIG_UNBLOCK, &setusr, NULL);
		}

		return 0;
	}
	if (pid > 0) { //parent process
		while (1) {
			sigprocmask(SIG_BLOCK, &setusr, NULL);	//SIGUSR1 && SIGUSR2 blocking without saving previous states

			if (signaled2) { //if SIGUSR2 received
				signaled2 = 0;
				ch = 'A';
			}
				
			printf("Parent (%d): '%c'\n", getpid(), ch);

			if(kill(pid, SIGUSR1) == -1) perror("kill parent error"); //send signal to child

			while(!signaled1)
				sigsuspend(&setemp); //waiting for signal from set

			if (signaled1) {	//signal from child received
				signaled1 = 0;
				ch++;
				if (ch > 'Z')
					ch = 'A';
			}
			printf("Press enter...");
			getchar();		//wait until Enter is pressed

			sigprocmask(SIG_UNBLOCK, &setusr, NULL);
		}
		if(waitpid(pid, &status, 0) == -1) {
			perror("waitpid error");
			return 1;
		}

	} else {
		perror("error in fork");
		return 1;
	}

	return 0;
}


			
