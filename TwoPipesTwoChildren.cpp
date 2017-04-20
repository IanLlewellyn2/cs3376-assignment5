// file: TwoPipesTwoChildren.cpp
// author: Ian Llewellyn
// based on OnePipe.cpp from Assignment 5 Handout by M. Amine Belkoura
// date: 04/5/17
// purpose: CS3376
// description:
// this program executes "ls -ltr | grep 3376 | wc -l", by dividing the two commands among the children and parent process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#pragma GCC diagnostic pop
using namespace std;
int main(int argc, char **argv){
 int status;
 int childpid;
 int childpid2;

 char *cat_args[] = {"ls", "-ltr", NULL};
 char *grep_args[] = {"grep", "3376", NULL};
 char *wc_args[] = {"wc", "-l", NULL};

 // create one pipe to send the output of "ls" process to "grep" process
 int pipes[2];
 pipe(pipes);

 // fork the first child (to execute cat)
 if((childpid = fork()) == -1){
	perror("Error creating a child process");
	exit(1);
}

 if (childpid == 0) {
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
	int pipes2[2];
 	pipe(pipes2);
 if ((childpid2 = fork()) == -1){
 	perror("Error creating a child process");
	exit(1);
}
 if (childpid2 == 0) {
 	//replace wc's stdin with read end of 2nd pipe
 	dup2(pipes2[0], 0);
	close(pipes[0]);
	close(pipes[1]);
	close(pipes2[0]);
	close(pipes2[1]);

	execvp(*wc_args, wc_args);
	
 }
 else{

 // replace grep's stdin with read end of 1st pipe, stdout with write end of 2nd
	 dup2(pipes[0], 0);
	 dup2(pipes2[1], 1);
	 close(pipes[0]);
	 close(pipes[1]);
	 close(pipes2[0]);
	 close(pipes2[1]);

	 execvp(*grep_args, grep_args);
 }
 return(0);
}
}
