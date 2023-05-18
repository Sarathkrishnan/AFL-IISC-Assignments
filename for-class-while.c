#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int arc, char **argv){

  int i,ca=0,cb=0;
char inp[500];
char *ptr =fgets(inp, sizeof inp, stdin);
i=0;
while(inp[i] !='\0'){
	if(inp[i] == 'a')
		ca++;
	if(inp[i] == 'b')
		cb++;
	i++;
}
return ca+cb;
}
