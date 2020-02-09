#include "header.h"

char* cwd;
char* user;
char* path;

int main()
{
    //Initialise shell
    initShell();
    //Loop until the shell is terminated
    do
    {
        printf("%s", path);
        //Print user, current directory and prompt
        printf("%s: %s", user, cwd);
        //Variable to store users input. Set to hold a max of 512 characters
        char str[STR_LIMIT];

        //Get user input
        getInput(str);
        char **args = tokenise(str);
        commandHandler(args);
    } while (1);
}

//Initialise shell
void initShell(){
    //Set PATH variable
    path = getenv("PATH");
    //Set working directory to users home directory
    cwd = getenv("HOME");
    //Change current directory to home path
    chdir(cwd);
    //Set user to the username of the user
    user = getenv("USER");

}
//Print out the current path 
void getPath(){
    printf("%s\n", getenv("PATH"));
}

//Set path using user input 
void setPath(char* str){
    setenv("PATH", str, 1);
}
//Set directory to user input
void changeDir(char* path){
    //If no arguments set directory to HOME
    if(strcmp(path, "cd") == 0){
        chdir(getenv("HOME"));
        cwd = getenv("HOME");
    }else{
    //Set directory to user input 
    chdir(path);
    cwd = getcwd(NULL, 0);
    }
}

void getInput(char *str) {

    //Print out "> " and wait for user input
    printf("> ");
    char *input = fgets(str, STR_LIMIT, stdin);

    //Check for ctrl + D input
    if (input == NULL)
    {
        printf("\n");
        exit(0);
    }
}

char **tokenise(char *str)
{
    //Tokenise user input using delimiters
    int bufsize = 50, position = 0;
    //Delimiter variable to tokenise string
    char* delimiter = " \t|><&;\n";
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token = strtok(str, delimiter);
    while (token != NULL)
    {
        //Check if user inputs exit command
        if (strcmp("exit", token) == 0)
        {
            exit(0);
        }
        tokens[position] = token;
        position++;
        //Check that tokens are correct
        #ifdef DEBUG
        printf("\"");
        printf("%s", token);
        printf("\"\n");
        #endif
        token = strtok(NULL, delimiter);
    }
    tokens[position] = NULL;
    return tokens;
}

void commandHandler(char **args) {
    //Check for built in commands
    if (strcmp(args[0], "setpath") == 0) {
        //If no path provided print error 
        if(args[1] == NULL){
            printf("Error : Please enter a path\n");
        }else{
        //Set path to user input 
        setPath(args[1]);
        }
    } else if (strcmp(args[0], "cd") == 0) {
        //Check for NULL argument
        if(args[1] == NULL){
        changeDir(args[0]);
        }
        else{
            changeDir(args[1]);
        }
        printf("cd works\n");
    } else if (strcmp(args[0], "getpath") == 0){
        getPath();
        }
        else {
        execArgs(args);
    }
}   

//Function where the system command is executed
void execArgs(char **args)
{ 
    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 
        if (execvp(args[0], args) < 0) {
            printf("\nCould not execute command..\n"); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
}