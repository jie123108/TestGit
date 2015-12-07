#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc < 2){
		exit(1);
	}
	const char* str= argv[1];
	int len = strlen(str);
	int i;
	for(i=0;i<len;i++){
		if(str[i] == ' ') printf("-");
		else printf("%c", tolower(str[i]));
	}
	printf("\n");
	return 0;
}
