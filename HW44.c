// process 4
#include <stdlib.h>
#include <stdio.h>

#define LINE_SIZE 80

int main(){
	char line[LINE_SIZE + 1];
        char c;
        int charPos = 0;

        // loop to print output taken from buffer
        while(1){
                // take input from buffer
		scanf("%c", &c);

                // exit if EOF is found
                if(c == EOF){
                        break;
                }

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
}
