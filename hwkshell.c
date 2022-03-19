#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char * myargv[100];

void processInput(char input[], char * myargv[], char * myenv[]);
char * getEnvPath(char * myEnv[]);


int main(int argc, char * argv[], char * env[]){
	// char input[100];
	// int i = 0;
	// while(1){
	// 	printf(">> ");
	// 	fgets(input, 100, stdin); 
	// 	input[strlen(input)-1] = '\0';
	// 	if(strlen(input) == 0)
	// 		continue;
	// 	if(strcmp(input, "exit") == 0)
	// 		break;
	// 	processInput(input, argv, env);
	// }
	char * path;
	path = getEnvPath(env);
	printf("%s", path);
	
	
	return 0;
}

void processInput(char input[], char * myargv[], char * myenv[]){
	//get input like normal and print out the env recieved
	int i = 0, status;
	char path[1000];
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
		char * path = myenv[8]; //Note: This requires work you need toget the path from the PATH environemt variable.
		strcpy(cmd, path);
		strcat(cmd, myargv[0]);
		int r = execve(cmd, myargv, myenv);
		printf("Cannot exectue command. %04X\n", r);
		return;
	}

	wait(&status);
}

char * getEnvPath(char * myEnv[]){
	//env[13]
	char path[1000]; 
	char fullPath[1000];
	char charStr[2] = "\0";
	int size = strlen(myEnv[13]);
	strcpy(fullPath, myEnv[13]);
	for(int i = 5; i < size; i++){
		charStr[0] = fullPath[i];
		strcat(path, charStr);
	}
	return path;
	
}

