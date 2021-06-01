#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1
#define SIZ 5

void pipeClose(int (*pipeDis)[2], int myNum); // close pipe except pipe that will use process
void freeAll(int (*pipeDis)[2], int myPipeReadNum, pid_t pid); // before exit, close all pipe
int readFile(int fd, char *rbuf); // read file one line, line's length will be smaller than BUFSIZ

void pipeClose(int (*pipeDis)[2], int myNum){
	for(int i=0; i<SIZ; i++){
		if(i == myNum){
			close(pipeDis[i][WRITE]);
		}
		else if(i == (myNum + 1) % SIZ){
			close(pipeDis[i][READ]);
		}
		else if(i == (myNum - 1)){
			close(pipeDis[i][READ]);
		}
		else{
			close(pipeDis[i][READ]);
			close(pipeDis[i][WRITE]);
		}
	}
}
void freeAll(int (*pipeDis)[2], int readNum, pid_t pid){
	int writeNum;
	if(pid == 0) writeNum = SIZ - 1;
	else writeNum = readNum - 1;
	close(pipeDis[readNum][READ]);
	close(pipeDis[writeNum][WRITE]);
}

int readFile(int fd, char *rbuf){
	int readByte = 1;
	int check;
	for(int i=0; i<BUFSIZ; i++){
		check = read(fd, &(rbuf[i]), readByte);
		if(rbuf[i] == '\n') {
			char ch;
			rbuf[i+1] = '\0';
			check = read(fd, &ch, readByte);
			if(check == 0) return -1;
			else{
				lseek(fd, -1, SEEK_CUR);
				return 1;
			}
		}
	}
	// make warning, not required return  
	return -1;
}
