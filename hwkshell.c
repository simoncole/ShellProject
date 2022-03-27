/*Simon Cole
3/20/22
With help from Dr. Bethelmy*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


void processInput(char input[], char * myargv[], char * myenv[], char * paths[], int myArgc);
char * locatePath(char * env[]);
void splitPath(char * path, char * paths[]);
int findPosOfLess(char * argv[]);
int findPosOfMore(char * argv[]);

int main(int argc, char * argv[], char * env[]){
	char * myargv[100];
	char input[100];
	int i = 0;
	int j = 0;
	char * paths[200];
	int myArgc = 0;

	char * path = locatePath(env);
	splitPath(path, paths);

	while(1){
		printf(">> ");
		fgets(input, 100, stdin); 
		input[strlen(input)-1] = '\0';
		if(strlen(input) == 0)
			continue;
		if(strcmp(input, "exit") == 0)
			break;
		processInput(input, myargv, env, paths, myArgc);
	}
	
	//cat a slash on to the paths before attaching name of file trying to execute

	
	return 0;
}

void processInput(char input[], char * myargv[], char * myenv[], char * paths[], int myArgc){
	//get input like normal and print out the env recieved
	int i = 0, status;
	char * path;
	char * str = strtok(input, " ");
	while(str){
		//myargv[i] = (char *) malloc(sizeof(char) * strlen(str) + 1);
		//strcpy(myargv[i], str);
		myargv[i] = str;
		//printf("%s\n", myargv[i]);
		str = strtok(NULL, " ");
		myArgc++;
		i++;

	}

	if (myArgc < 2)
   {
      printf("Usage: %s '<' input_file '>' output_file\n", myargv[0]);
	  return;
   }
   int posOfLess = findPosOfLess(myargv);
   if (posOfLess != -1)
   {
     close(0);  //close stdin from the keyboard
     int fd = open(myargv[posOfLess+1], O_RDONLY);
     if (fd == -1)
     {
        printf("Invalid file name: %s\n", myargv[posOfLess+1]);
        return;
     }
   }
   int posOfMore = findPosOfMore(myargv);
   if (posOfMore != -1)
   {
      close(1);  //close stdout
      int fd = open(myargv[posOfMore+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
     if (fd == -1)
     {
        printf("Invalid file name: %s\n", myargv[posOfMore+1]);
        return;
     }
    }
	char strRedirect[1000];
   while(fgets(strRedirect, 1000, stdin))
   {
      puts(strRedirect);
   }

	
	// myargv[i] = NULL;
	int pid = fork();
	if(pid == 0)
	{
		char cmd[1024];
		while(paths[i]){
			strcpy(cmd, paths[i]); //copy the path to the command
			strcat(cmd, "/");
			strcat(cmd, myargv[0]);
			if(access(cmd, F_OK) != -1){
				//path = paths[i];
				break;
			}
			i++;
		}
		//printf("%s\n", cmd); 
		int r = execve(cmd, myargv, myenv); 
		printf("Cannot execute command. %04X\n", r);
		exit(1);
	}

	wait(&status);
}

char * locatePath(char * env[]){
	
	int i = 0;
	char * leftOfSign = NULL;
	char strInitial[5000];
	char * path = NULL;
	while(env[i]){
		strcpy(strInitial, env[i]);
		leftOfSign = strtok(strInitial, "=");
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
		str = strtok(NULL, ":");
		i++;
	}
	paths[i] = NULL;
	
}
int findPosOfLess(char * argv[])
{
  int pos = 0;
  while(argv[pos])
  {
    if(strcmp(argv[pos], "<") == 0)
      return pos;
    pos++;
  }

  return -1;
}
int findPosOfMore(char * argv[])
{
  int pos = 0;
  while(argv[pos])
  {
    if(strcmp(argv[pos], ">") == 0)
      return pos;
    pos++;
  }

  return -1;
}
