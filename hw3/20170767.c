#include "20170767.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: 20170767 <file>\n");
        return 0;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Error %d Unable to open %s\n", fd, argv[1]);
        return 0;
    }

    // pipe
    int pipeToRight[NUMOFPID + 1][2]; // read = 0, write = 1
    int pipeToLeft[NUMOFPID + 1][2]; // read = 0, write = 1

    // pipe 명영어용 버퍼
    char bufferForCommand[BUFSIZE];

    // 파일 내용 읽어올 버퍼
    char bufferForFile[BUFSIZE];

    // 프로세스 ID를 담을 배열
    pid_t pid[NUMOFPID];
    // 프로세스 구분용
    int i = 0;
    int exit_status;

    // 파이프 만들기
    for (int j = 0; j <= NUMOFPID; ++j) {
        if ((pipe(pipeToRight[j]) < 0) || (pipe(pipeToLeft[j]))) { // read = 0, write = 1
            printf("pipe error\n");
            return 0;
        }
    }

    // 자식 프로세스 만들기
    for (i = 0; i < NUMOFPID; ++i) {
        pid[i] = fork();
        if (pid[i] == 0) {
            break;
        }
    }
    //여기까지 부모랑 자식의 흐름 완성

    // 자신 파이프 찾기
    int rightPipe = i + 1;
    int leftPipe = i;

    if (i == NUMOFPID) { // 부모부터 읽기 시작하게 신호주기
        rightPipe = 0;
        write(pipeToRight[leftPipe][WRITE_NUM], COMMAND_READ, SIZEOFCOMMAND);
    }

    // 순행
    while (1) {
        // 신호 읽어오기
        read(pipeToRight[leftPipe][READ_NUM], bufferForCommand, SIZEOFCOMMAND);

        // READ 신호를 받은 경우
        if (strncmp(bufferForCommand, COMMAND_READ, SIZEOFCOMMAND) == 0) {
            //read
            printf("%d ", getpid());
            int result = -1;

            while (1){
                result = read(fd, bufferForFile, 1);

                if(result != 0){
                    printf("%c", bufferForFile[0]);
                } else{
                    break;
                }
                if(bufferForFile[0]=='\n'){
                    break;
                }
            }

            if (result == 0) { // 파일의 마지막인 경우
                printf("\n%d Read all data\n", getpid()); // FIN 시그널 뒤에 누가 FIN 신호를 보냈는지 기재
                char command[5] = {'F', 'I', 'N', 0, 0};
                command[3] = ('0' + i);
                write(pipeToRight[rightPipe][WRITE_NUM], command, SIZEOFCOMMAND);
            } else {
                write(pipeToRight[rightPipe][WRITE_NUM], COMMAND_READ, SIZEOFCOMMAND);
            }

        } // 다읽었다는 FIN 신호를 받는 경우
        else if (strncmp(bufferForCommand, COMMAND_FINISH, SIZEOFCOMMAND - 1) == 0) {
            // 누가 FIN 신호를 보냈는지 확인
            int index = bufferForCommand[3] - '0';

            if (index == i) { // 본인이 FIN신호를 먼저 보낸 사람인지 확인
                if (i == 3){ // 마지막 프로세스인 경우 그냥 바로 종료 신호 보내고 정상종료
                    printf("%d I'm exiting...\n", getpid());
                    write(pipeToLeft[leftPipe][WRITE_NUM], COMMAND_BYE, SIZEOFCOMMAND);
                    return 0;
                }else{ // 아닌 경우 마지막 프로세스까지 EXIT 신호 보내기
                    write(pipeToRight[rightPipe][WRITE_NUM], COMMAND_EXIT, SIZEOFCOMMAND);
                }
                break;
            }

            if (index == NUMOFPID){ // 부모프로세스에서 FIN을 보낸 경우 그냥 끝까지 FIN 신호 보내기
                write(pipeToRight[rightPipe][WRITE_NUM], bufferForCommand, SIZEOFCOMMAND);
            } else if (index == NUMOFPID-1){ // 마지막 프로세스인경우 FIN신호를 보내고 순방향 빠져나오기
                write(pipeToRight[rightPipe][WRITE_NUM], bufferForCommand, SIZEOFCOMMAND);
                break;
            } else{
                if (i > index && i < NUMOFPID) { // FIN을 보낸 프로세스와 마지막 프로세스의 사이의 프로세스는 그냥 FIN 신호 보내기
                    write(pipeToRight[rightPipe][WRITE_NUM], bufferForCommand, SIZEOFCOMMAND);
                } else { // FIN 신호를 보낸 프로세스보다 전의 프로세스일때 FIN신호 보내고 순방향 빠져나오기
                    write(pipeToRight[rightPipe][WRITE_NUM], bufferForCommand, SIZEOFCOMMAND);
                    break;
                }
            }
        } // 종료하자는 EXIT 신호를 받았을때
        else if (strncmp(bufferForCommand, COMMAND_EXIT, SIZEOFCOMMAND) == 0) {
            if (i == (NUMOFPID - 1)) { // 마지막 프로세스이면 종료되었다는 BYE 신호를 보내고 정상종료
                write(pipeToLeft[leftPipe][WRITE_NUM], COMMAND_BYE, SIZEOFCOMMAND);
                printf("%d I'm exiting...\n", getpid());
                return 0;
            } else { // 마지막 프로세스로 계속 EXIT 신호 보내고 순방향 종료하기
                write(pipeToRight[rightPipe][WRITE_NUM], COMMAND_EXIT, SIZEOFCOMMAND);
                break;
            }
        } else {
            break;
        }
    }

    // 역행
    while (1) {
        // 종료되었다는 신호를 받기위한 작업
        read(pipeToLeft[rightPipe][READ_NUM], bufferForCommand, SIZEOFCOMMAND);

        if (strncmp(bufferForCommand, COMMAND_BYE, SIZEOFCOMMAND) == 0) {
            printf("%d I'm exiting...\n", getpid());
            // 이전 프로세스가 종료되었으면 BYE 신호를 보내고 종료하기
            write(pipeToLeft[leftPipe][WRITE_NUM], COMMAND_BYE, SIZEOFCOMMAND);
            return 0;
        }
    }

    if (i == 4) { // 좀비프로세스 방지 목적의 wait
        for (int i = 0; i < NUMOFPID; i++) {
            pid_t child = wait(&exit_status);

            if (WIFEXITED(exit_status)) {
                printf("%d I'm exiting... %d\n", child, WEXITSTATUS(exit_status));
            }
        }
    }

    return 0;
}

