#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char * myargv[100];

void processInput(char input[], char * myargv[], char * myenv[], char * paths[]);
char * locatePath(char * env[]);
void splitPath(char * path, char * paths[]);

int main(int argc, char * argv[], char * env[]){
	// if(argc != 2){
	// 	printf("Usage %s name_of_file\n", argv[0]);
	// 	return 0;
	// }
	char input[100];
	int i = 0;
	int j = 0;
	char * path = locatePath(env);
	char * paths[200];
	splitPath(path, paths);
	while(paths[j]){
		printf("%s\n", paths[j]);
		j++;
	}


	while(1){
		printf(">> ");
		fgets(input, 100, stdin); 
		input[strlen(input)-1] = '\0';
		if(strlen(input) == 0)
			continue;
		if(strcmp(input, "exit") == 0)
			break;
		processInput(input, argv, env, paths);
	}
	// printf("%s\n", path);
	// printf("%s\n", path);
	// char fullpath[200];
	// int i =0;
	// while(paths[i]){
	// 	strcpy(fullpath, paths[i]);
	// 	strcat(fullpath, argv[1]);
	// 	printf("%s: ", fullpath);
	// 	if(access(fullpath, F_OK) != -1){
	// 		printf("File is in path.\n");
	// 	}
	// 	else{
	// 		printf("File is not in path\n");
	// 	}
	// 	i++;
	// }

	
	//cat a slash on to the paths before attaching name of file trying to execute

	
	return 0;
}

void processInput(char input[], char * myargv[], char * myenv[], char * paths[]){
	//get input like normal and print out the env recieved
	int i = 0, status;
	char * path;
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
		char cmd[1024];
		while(paths[i]){
			if(access(paths[i], R_OK) != -1){
				path = paths[i];
				break;
			}
			i++;
		}
		// char * path = ; //Note: This requires work you need toget the path from the PATH environemt variable.
		strcpy(cmd, path); //copy the path to the command
		strcat(cmd, myargv[0]);
		strcat(cmd, "/");
		printf("%s", cmd); 
		int r = execve(cmd, myargv, myenv); 
		printf("Cannot exectue command. %04X\n", r);
		return;
	}

	wait(&status);
}

char * locatePath(char * env[]){
	
	int i = 0;
	char * leftOfSign = NULL;
	char strInitial[32];
	char * path;
	while(env[i]){
		leftOfSign = strtok(env[i], "=");
		if(strcmp(leftOfSign, "PATH") == 0){
			path = strtok(NULL, "=");
			break;			
		}
		i++;
	}
	return path;
}
void splitPath(char * path, char * paths[]){
	int i = 0;
	char * str;
	str = strtok(path, ":");
	while(str){
		paths[i] = (char *) malloc(sizeof(char) * strlen(str) + 2);
		strcpy(paths[i], str);
		strcat(paths[i], "/");
		str = strtok(NULL, ":");
		i++;
	}
	paths[i] = NULL;
	
}

