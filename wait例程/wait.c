#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
	pid_t pid = fork();
	int k;
	int i;
	switch(pid)
	{
		case 0:
				printf("child is running,Parent is running %d\n", getppid());
                       		k = 5;
			break;
		case -1:
			perror("Process creation failed\n");
			break;
		default:
				printf("This is Parent,child PID is %d\n",pid);
	}
	if(pid)
	{
		int status;
		pid_t child_pid;
		child_pid = wait(&status);
		printf("child process has exited,it pid is %d\n",child_pid);
	}
	else
	{
		while(k--)
		{
			sleep(1);
		}	
	}
	exit(1);
}
