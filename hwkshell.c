#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * myargv[100];

void processInput(char input[], char * myargv[], char * myenv[]);


int main(int argc, char * argv[], char * env[]){
	char input[100];
	int i = 0;
	while(1){
		printf(">> ");
		fgets(input, 100, stdin); 
		input[strlen(input)-1] = '\0';
		if(strlen(input) == 0)
			continue;
		if(strcmp(input, "exit") == 0)
			break;
		processInput(input, argv, env);
	}
return 0;
}

void processInput(char input[], char * myargv[], char * myenv[]){
	int i = 0, status;
	char * str = strtok(input, " ");
	while(str){
		myargv[i] = str;
		str = strtok(NULL, " ");
		i++;

	}
	
	myargv[i] = NULL;
	
	int pid = fork();
	if(pid == 0)
	{
		char cmd[32];
		char * path = "/bin/"; //Note: This requires work you need toget the path from the PATH environemt variable.
		strcpy(cmd, path);
		strcat(cmd, myargv[0]);
		int r = execve(cmd, myargv, myenv);
		printf("Cannot execturecommand. %04X\n", r);
		return;
	}

	wait(&status);
}

