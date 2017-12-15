// process 3
#include <stdlib.h>
#include <stdio.h>

int main(){
        // initialize variables used
        char current;
        char previous = '\0';
        int starCollapse = 0;

        // take input and remove **
        while(1){
                // read input
		scanf("%c", &current);

                // remove the star and write
                if(current == '*' && previous == '*'){
                        current = '^';
			printf("%c", current);
                        current = '\0';
                        starCollapse = 1;
                }
                else if(previous == '*'){
                        if(!starCollapse){
				printf("%c", previous);
                        }
                        starCollapse = 0;
                }
                else{
                        if(previous != '\0'){
				printf("%c", previous);
                        }
                        starCollapse = 0;
                }

                // stop on EOF
                if(current == EOF){
			printf("%c", current);
                        break;
                }

                // assign previous char
                previous = current;
        }
}
