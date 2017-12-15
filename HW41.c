// process 1
#include <stdlib.h>
#include <stdio.h>

int main(){
        // initialize variables used
        char c;

        // take input from stdin
        while(scanf("%c", &c) != EOF){
                // write to pipe
                printf("%c", c);
        }

        // deposit EOF so next process knows its done
        c = EOF;
        printf("%c", c);
}
