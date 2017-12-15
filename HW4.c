

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINE_SIZE 80
#define NUM_PROC 4

// function for taking input
void takeInput(int write_desc){
        // initialize variables used
        char c;

        // take input from stdin
        while(scanf("%c", &c) != EOF){
                // write to pipe
                write(write_desc, &c, sizeof(char));
        }

        // close the pipe
        close(write_desc);
}

// function for processing newline
void processNewline(int read_desc, int write_desc){
        // initialize variables used
        char c;

        // take input and remove newlines
        while(read(read_desc, &c, sizeof(char)) > 0){
                // replace the newline
                if(c == '\n'){
                        c = ' ';
                }

                // write the output
                write(write_desc, &c, sizeof(char));
        }

        // close the pipe
        close(read_desc);
        close(write_desc);
}

// function for removing double star and replacing with ^
void processStar(int read_desc, int write_desc){
        // initialize variables used
        char current;
        char previous = '\0';
        int starCollapse = 0;

        // take input and remove **
        while(read(read_desc, &current, sizeof(char)) > 0){
                // remove the star and write
                if(current == '*' && previous == '*'){
                        current = '^';
                        write(write_desc, &current, sizeof(char));
                        current = '\0';
                        starCollapse = 1;
                }
                else if(previous == '*'){
                        if(!starCollapse){
                                write(write_desc, &previous, sizeof(char));
                        }
                        starCollapse = 0;
                }
                else{
                        if(previous != '\0'){
                                write(write_desc, &previous, sizeof(char));
                        }
                        starCollapse = 0;
                }

                // assign previous char
                previous = current;
        }

        // write last previous
        if(!starCollapse){
                write(write_desc, &previous, sizeof(char));
        }

        // close the pipe
        close(read_desc);
        close(write_desc);
}

// function for printing output
void printOutput(int read_desc){
        char line[LINE_SIZE + 1];
        char c;
        int charPos = 0;

        // loop to print output taken from buffer
        while(read(read_desc, &c, sizeof(char)) > 0){
                // put input in line
                if(charPos < LINE_SIZE){
                        line[charPos] = c;
                }

                // increment position
                charPos++;

                // if end of line has been reached, print
                if(charPos == LINE_SIZE){
                        line[charPos] = 0;
                        printf("%s\n", line);
                        charPos = 0;
                }
        }

        // close the pipe
        close(read_desc);
}

int main(){
        // make three pipes
        int pipe1[2];
        int pipe2[2];
        int pipe3[2];
        
        // if pipe function returns -1 there was an error, exit
        if(pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1){
                perror("error creating pipe");
                exit(EXIT_FAILURE);
        }

        // make child processes
        int i;
        pid_t childpids[NUM_PROC];
        for(i = 0; i < NUM_PROC; i++){
                // if fork() returns less than 0 then there was an error, exit
                childpids[i] = fork();
                if(childpids[i] < 0){
                        perror("error creating child process");
                        exit(EXIT_FAILURE);
                }
                else if(childpids[i] == 0){
                        // decide which child process it is
                        switch(i){
                                case 0:
                                        // take input process
                                        // close uneeded pipes
                                        close(pipe1[0]);

                                        close(pipe2[0]);
                                        close(pipe2[1]);
                                        close(pipe3[0]);
                                        close(pipe3[1]);

                                        // call function
                                        takeInput(pipe1[1]);

                                        // end the process
                                        return 0;
                                case 1:
                                        // replace newline process
                                        // close uneeded pipes
                                        close(pipe1[1]);
                                        close(pipe2[0]);

                                        close(pipe3[0]);
                                        close(pipe3[1]);

                                        // call function
                                        processNewline(pipe1[0], pipe2[1]);

                                        // end the process
                                        return 0;
                                case 2:
                                        // replace ** process
                                        // close uneeded pipes
                                        close(pipe2[1]);
                                        close(pipe3[0]);

                                        close(pipe1[0]);
                                        close(pipe1[1]);

                                        // call function
                                        processStar(pipe2[0], pipe3[1]);

                                        // end the process
                                        return 0;
                                case 3:
                                        // print output process
                                        // close uneeded pipes
                                        close(pipe3[1]);

                                        close(pipe1[0]);
                                        close(pipe1[1]);
                                        close(pipe2[0]);
                                        close(pipe2[1]);

                                        // call function
                                        printOutput(pipe3[0]);

                                        // end the process
                                        return 0;
                                default:
                                        // should never reach here
                                        perror("reached default in switch, thats a problem");
                                        exit(EXIT_FAILURE);
                        }
                }
        }

        // close all pipes in parent process
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);

        // wait for children
        int status;
        i = 0;
        while (i < NUM_PROC){
                wait(&status);
                i++;
        }

        return 0;
}

