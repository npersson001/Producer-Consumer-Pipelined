// process 2
#include <stdlib.h>
#include <stdio.h>

int main(){
        // initialize variables used
        char c;

        // take input and remove newlines
        while(1){
                // read the input
                scanf("%c", &c);

                // replace the newline
              if(c == '\n'){
                        c = ' ';
                }

                // write the output
		printf("%c", c);

                // exit if EOF is found
                if(c == EOF){
			printf("%c", c);
                        break;
                }
        }
}
