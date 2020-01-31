//myshell.c by Hemal Salvi and 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (void)
{
	int counter = 0; //count instructions
	bool exit = false; //boolean tracking when to exit loop
	char input[50]; //input string
	pid_t pid; //holds pid
	while (exit = false)
	{
		printf("/n{shell:~}"); //print that the shell is ready for a command
		scanf("%s", input); //get input string
		char lastCheck[50];
		lastCheck = strrev(input);
		if (lastcheck[0] == '&')
		{
			if (pid == fork) < 0)
			{
				err_sys("fork error");
			}
			else if (pid == 0) //child process
			{
				//put exec in here
			}
			else
			{
				
			}
		else //string doesn't end with &
		{
			if ((pid == fork) < 0)
			{
				err_sys("fork error");
			}
			else if (pid == 0) //child process
			{
				//put exec in here
				exit();
			}
			else
			{
				wait();
				counter++;
			}
		}
	}
	printf("Terminating shell, %i commands inputted", counter);
	exit(0);
}



