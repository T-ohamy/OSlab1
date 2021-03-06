
void getJobDone(){
    char *line;
    char **args;
    int status;

    do {
        printf(">>>");
        line = readLine();
        args = readArgs(line);
        status = executeShell(args);

        free(line);
        free(args);
    } while(status);
}

char* readLine(){
    char *line = NULL;
    size_t buffersize = 0;

    if(getline(&line, &buffersize, stdin) == -1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS);
        }else{
            perror("Error in Reading Line...\n");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char** readArgs(char* line){
    int buffersize = TOKEN_BUFFER_SIZE;
    int position = 0;
    char **tokens = malloc(buffersize*sizeof(char*));
    char *token;

    if(!token){
        fprintf(stderr, "Allocate Error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMTER);
    while(token != NULL){
        tokens[position] = token;
        ++position;

        if(position >= buffersize){
        buffersize += TOKEN_BUFFER_SIZE;
        tokens = realloc(tokens, buffersize*sizeof(char*));
            if(!tokens){
                fprintf(stderr, "Allocate Error...\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIMTER);
    }
    tokens[position] = NULL;
    return tokens;
}

int executeShell(char** args){
    if(args[0] == NULL){
        return 1;
    }else if(strcmp(args[1], "&") == 0){

        int XD = fork();
        if(XD == 0){
            char* WINNER ={NULL};
            execve(args[0], WINNER,WINNER );
            return 1;
        }
    }
    for(int i=0; i<getBuiltInNums(); ++i){
        if(strcmp(args[0], builtInChar[i]) == 0)
            return (*builtInFunctions[i])(args);
    }

    return launchShell(args);
}
int launchShell(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("Error executing...\n");
            exit(EXIT_FAILURE);
            }
    }else if(pid < 0){
        perror("Error in processes...\n");
    }else{
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
