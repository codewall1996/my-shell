#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

char * getline2();
void shell_loop();
char** splitline(char* a_str, const char a_delim);
int shell_launch(char **args);
int shell_cd(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
//int shell_num_builtins();
int shell_execute(char **args);
char * remove_newline_ch(char *line);
int main()
{
	shell_loop();
}

void shell_loop()
{
	char * line;
	char ** args;
	int a = 1;
	int status;
	printf("Welcome to my shell. Press ctrl c or type exit to exit \n");
	do{
		printf("> ");
	line = getline2();
	//printf("%s\n", line);
	line = remove_newline_ch(line);
	args = splitline(line, ' ');
	//printf("%s\n", args[2]);
	status = shell_execute(args);
	free(line);
	free(args);
	}while(status > 0);
	
}

char * getline2() {
	 char *buffer;
    size_t bufsize = 32;
    size_t characters;
     buffer = (char *)malloc(bufsize * sizeof(char));
     characters = getline(&buffer,&bufsize,stdin);
     return buffer;
}

char** splitline(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
char * remove_newline_ch(char *line)
{
    int new_line = strlen(line) -1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
    return line;
}

int shell_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  //char * app = ""
  if (pid == 0) {
    // Child process
  	//printf("%s hello \n",args[0]);
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int shell_cd(char **args)
{
	//printf("allll\n");
	if (args[1] == NULL) {
    printf("shell: expected argument to \"cd\"\n");
  } 
  else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
    else
    	printf("Directory changed successfully \n");
  }
  return 1;
}

int shell_pwd(char **args)
{
	char cwd[1024];
	//printf("allll\n");
   if (getcwd(cwd, sizeof(cwd)) != NULL)
       printf("Current working dir: %s\n", cwd);
   else
       perror("getcwd() error");
   return 1;
}


int shell_exit(char **args)
{
  return 0;
}

char * builtin_str[] = {
  "cd",
  "pwd",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_pwd,
  &shell_exit
};


int shell_execute(char **args)
{
  
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  	//printf("%s HELLO\n", args[0] );
  char * cd = "cd";
  //cd[3] = '\0';
  //args[strlen(args)] = '\0';
  char * pwd = "pwd";
  char * exit = "exit";
  //printf("%d \n",strlen(cd));
    //printf("%d \n",strlen(args[0]));
    if (strcmp(cd , args[0]) == 0) {
    	//printf("hello\n");
      return shell_cd(args);
    }
    else if(strcmp(pwd , args[0]) == 0) {
    	//printf("hello\n");
      return shell_pwd(args);
    }
    else if(strcmp(exit , args[0]) == 0) {
    	//printf("hello\n");
      return shell_exit(args);
    }
    else
    {
    	printf("Not a built in command\n");
  		return shell_launch(args);
    }
}
