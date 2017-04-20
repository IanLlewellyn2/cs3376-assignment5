// file: DynPipe.cpp
// author: Ian Llewellyn
// based on OnePipe.cpp from Assignment 5 Handout by M. Amine Belkoura
// date: 04/7/17
// purpose: CS3376
// description:
// this program executes shell scripts input by the user as arguments by dividing the commands among children and the parent process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <cstring>

using namespace std;
int main(int argc, char **argv){
 int status;
 int childpid;
 int childpid2;
 int childpid3;
 int childpid4; // have enough childpids to handle 5 arguments
 
 if(argc < 5)
 	perror("error");//2 or more arguments
 if(argc > 11)
 	perror("error");//5 or less arguments
 
 char *cat_args[] = {argv[1], argv[2], NULL};
 char *grep_args[] = {argv[3], argv[4], NULL};
 char *wc_args[] = {argv[5], argv[6], NULL};
 char *wa_args[] = {argv[7], argv[8], NULL};
 char *wb_args[] = {argv[9], argv[10], NULL};
// printf("%u\n", argc);
 // create one pipe to send the output of "ls" process to "grep" process
 int pipes[2];
 pipe(pipes);

 // fork the first child (to execute cat)
 if((childpid = fork()) == -1){
	perror("Error creating a child process");
	exit(1);
}

 if (childpid == 0) { //child 1
 // replace cat's stdout with write part of 1st pipe
        dup2(pipes[1], 1);
	 // close all pipes (very important!); end we're using was safely copied
	 close(pipes[0]);
	 close(pipes[1]);

	 execvp(*cat_args, cat_args);
	 exit(0);
 }

 else {
 	//create next child
	//create second pipe here to not have child 1 deal with it
	int pipes2[2];
 	pipe(pipes2);
	int pipes3[2];
	pipe(pipes3);
 if (argc > 5 && (childpid2 = fork()) == -1){
 	perror("Error creating a child process");
	exit(1);
}
 if (childpid2 == 0) { //child 2
 	//replace wc's stdin with read end of 2nd pipe
 	dup2(pipes2[0], 0);
	if(argc > 7)
		dup2(pipes3[1], 1);
	close(pipes[0]);
	close(pipes[1]);
	close(pipes2[0]);
	close(pipes2[1]); //close unneeded pipes
	close(pipes3[0]);
	close(pipes3[1]);


	execvp(*wc_args, wc_args); //child 2 executes "wc -l"
	exit(0);
 }
 else{
 if((childpid3 = fork()) == -1){ // create child 3
 	perror("Error creating a child process");
	exit(1);
 }
 if(childpid3 == 0) {
 // child 3
 // replace grep's stdin with read end of 1st pipe, stdout with write end of 2nd
	 dup2(pipes[0], 0);
	 if(argc > 7) //only need this to pipe again if more than 2 args
	 	dup2(pipes2[1], 1);
	 close(pipes[0]);
	 close(pipes[1]);
	 close(pipes2[0]);
	 close(pipes2[1]); //close unneeded pipes
	 close(pipes3[0]);
	 close(pipes3[1]);

	 execvp(*grep_args, grep_args); //execute grep arguments
	 exit(0);
 }
 else{
	int pipes4[2];
	pipe(pipes4);
 if(argc > 7 && (childpid4 = fork()) == -1){
 	perror("Error creating a child process");
	exit(1);
}
if(childpid4 == 0){ //child4
	dup2(pipes3[0], 0);
	if(argc > 9) // only need to pipe if 5 args, otherwise write to stdout
		dup2(pipes4[1], 1);
	
	close(pipes[0]);
	close(pipes[1]);
	close(pipes2[0]);
	close(pipes2[1]);
	close(pipes3[0]);
	close(pipes3[1]);
	close(pipes4[0]);
	close(pipes4[1]);
	execvp(*wa_args, wa_args);
	exit(0);	
}
else{
	close(pipes[0]);
	close(pipes[1]);
	close(pipes2[0]);
	close(pipes2[1]);

	if(argc > 8){
	close(pipes3[0]);
	close(pipes3[1]);
	}
	if(argc > 10){
	dup2(pipes4[0], 0); 
	 
	  //close pipes
	 close(pipes4[0]);
	 close(pipes4[1]);
		 //parent
	execvp(*wb_args, wb_args);
	}
	 exit(0);
 }
 return(0);
}
}
}
}
