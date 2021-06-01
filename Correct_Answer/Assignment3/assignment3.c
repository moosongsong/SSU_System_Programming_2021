#include "assignment3.h"

int main(int argc, char **argv){
	int pipeDis[SIZ][2];
	int myPipeNum = 0;

	const char *done = "done";
	const char *pass = "pass";
	const char *end = "end";

	char pipeReadBuf[BUFSIZ];
	char fileReadBuf[BUFSIZ];

	int fd;
	pid_t pid;

	// open File
	fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		printf("file open error\n");
		exit(-1);
	}

	// open Pipe
	for(int i=0; i<SIZ; i++){
		if(pipe(pipeDis[i]) == -1){
			printf("%d pipe open error\n", i);
			exit(-1);
		}
	}

	// Fork
	for(int i=1; i<SIZ; i++){
		pid = fork();
		if(pid == 0){ // child
			pipeClose(pipeDis, i);
			myPipeNum = i;
			break;
		}
	}

	if(pid != 0){ // parents
		pipeClose(pipeDis, myPipeNum);
		int myPipeWriteNum = (myPipeNum + 1) % SIZ;
		int myPipeReadNum = myPipeNum;

		while(1){	
			memset(fileReadBuf, 0, sizeof(fileReadBuf));
			if(readFile(fd, fileReadBuf) == -1){
				printf("%d %s", getpid(), fileReadBuf);
				printf("%d Read all data\n", getpid());
				write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
				break;
			}

			else{
				printf("%d %s", getpid(), fileReadBuf);
				write(pipeDis[myPipeWriteNum][WRITE], pass, strlen(pass));
			}

			read(pipeDis[myPipeReadNum][READ], pipeReadBuf, BUFSIZ); 
			if(strcmp(pipeReadBuf, pass) == 0) continue;

			else if(strcmp(pipeReadBuf, done) == 0){
				write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
				break;
			}
		}

		while(1){
			memset(pipeReadBuf, 0, sizeof(pipeReadBuf));
			read(pipeDis[myPipeReadNum][READ], pipeReadBuf, BUFSIZ);
			if(strcmp(pipeReadBuf, done) == 0){
				write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
			}
			if(strcmp(pipeReadBuf, end) == 0){
				printf("%d I'm exiting...\n", getpid());
				freeAll(pipeDis, myPipeReadNum, pid);
				close(fd);
				return 0;
			}
		}
	}

	else{ // child	
		int myPipeWriteNum = (myPipeNum + 1) % SIZ;
		int myPipeReadNum = myPipeNum;
		int myPipeWriteEndNum = myPipeNum - 1; // end, childs call (myPipeNum - 1)th process

		while(1){
			read(pipeDis[myPipeReadNum][READ], pipeReadBuf, BUFSIZ);

			if(strcmp(pipeReadBuf, pass) == 0){ // remains text data
				memset(fileReadBuf, 0, sizeof(fileReadBuf));
				if(readFile(fd, fileReadBuf) == -1){
					printf("%d %s", getpid(), fileReadBuf);
					printf("%d Read all data\n", getpid());
					write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
					break;
				}
				else{
					printf("%d %s", getpid(), fileReadBuf);
					write(pipeDis[myPipeWriteNum][WRITE], pass, strlen(pass));
				}
			}

			else if(strcmp(pipeReadBuf, done) == 0){ // no text data
				write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
				break;
			}
			else{ // error 
				printf("error! pipeReadBuf is %s\n", pipeReadBuf);
			}

		}

		while(1){
			memset(pipeReadBuf, 0, sizeof(pipeReadBuf));
			read(pipeDis[myPipeReadNum][READ], pipeReadBuf, BUFSIZ);

			if(strcmp(pipeReadBuf, done) == 0 && myPipeNum == SIZ - 1){ // first exit process
				printf("%d I'm exiting...\n", getpid());
				write(pipeDis[myPipeWriteEndNum][WRITE], end, strlen(end));
				freeAll(pipeDis, myPipeReadNum, pid);
				return 0;
			}

			else if(strcmp(pipeReadBuf, done) == 0){ // ready to exit
				write(pipeDis[myPipeWriteNum][WRITE], done, strlen(done));
			}

			else if(strcmp(pipeReadBuf, end) == 0){
				printf("%d I'm exiting...\n", getpid());
				write(pipeDis[myPipeWriteEndNum][WRITE], end, strlen(end));
				freeAll(pipeDis, myPipeReadNum, pid);
				return 0;
			}
		}
	}
}
