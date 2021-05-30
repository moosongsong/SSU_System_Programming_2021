#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define stdin 1
#define stdout 2
#define stderr 3

// 모드 5개
#define READ 1
#define WRITE 2
#define APPEND 3
#define READWRITE 4
#define READAPPEND 5

// 커서 위치
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define BUFSIZE 1024
#define EOF -1

typedef struct File {
    int mode; //오픈한 파일이 어떤 모드로 오픈되었는지 기재.
    int fd;//파일 디스크립터
    int size;//현재 버퍼에서 읽어야하는 바이트의 크기
    int actual_size;//실제로 저장된 바이트 크기
    char buffer[BUFSIZE];//버퍼
    char *base;//파일의 버퍼의 시작 주소
    char *ptr;//현재 읽고있는 파일 버퍼에서의 커서
    int read_loop;//버퍼의 총 크기만큼 읽어야 하는 횟수
    int read_remain;//추가적으로 읽어야하는 바이트의 수
    int write_loop;//버퍼의 총 크기만큼 저장해야하는 횟수
    int write_remain;//추가적으로 저장해야하는 바이트의 수
    int write_flag;//더 저장할지 말지 체크하는거
    int count;//읽은 루프 수
    int eof;
} FILE;

FILE *fopen(const char *pathname, const char *mode);

int fread(void *ptr, int size, int nmemb, FILE *stream);

int fwrite(const void *ptr, int size, int nmemb, FILE *stream);

int fgetc(FILE *stream);

int fputc(int c, FILE *stream);

int fflush(FILE *stream);

int fseek(FILE *stream, int offset, int whence);

int feof(FILE *stream);

int fclose(FILE *stream);

FILE *fopen(const char *pathname, const char *mode) {
    // calloc으로 받아서 모두 0으로 초기화 간단히 하기
    FILE *stream = calloc(sizeof(FILE), 1);

    // read 모드
    if (!(strcmp(mode, "r"))) {
        stream->fd = open(pathname, O_RDONLY, S_IRUSR);
        stream->mode = READ;
        if (stream->fd == -1) {
            return NULL;
        }
        // read/write 모드
    } else if (!(strcmp(mode, "r+"))) {
        stream->fd = open(pathname, O_RDWR, S_IRUSR | S_IWUSR);
        stream->mode = READWRITE;
        if (stream->fd == -1) {
            return NULL;
        }
        // write 모드
    } else if (!(strcmp(mode, "w"))) {
        stream->fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
        stream->mode = WRITE;
        if (stream->fd == -1) {
            return NULL;
        }
        // read/write 모드
    } else if (!(strcmp(mode, "w+"))) {
        stream->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        stream->mode = READWRITE;
        if (stream->fd == -1) {
            return NULL;
        }
        // append 모드
    } else if (!(strcmp(mode, "a"))) {
        stream->fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR);
        stream->mode = APPEND;
        if (stream->fd == -1) {
            return NULL;
        }
        // read / append 모드
    } else if (!(strcmp(mode, "a+"))) {
        stream->fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        stream->mode = READAPPEND;
        if (stream->fd == -1) {
            return NULL;
        }
    } else {
        return NULL;
    }
    return stream;
}

// 파일 스트림 해제하기
int fclose(FILE *stream) {
    if (stream == NULL){
        return EOF;
    }
    int result = close(stream->fd);
    free(stream);
    return result;
}

// stream의 비어있는 버퍼를 채워주기
int fillBuffer(FILE *stream) {
    // 총 복사해야하는 바이트 수
    int copyBytes = 0;

    // 버퍼의 크기 통채로 읽어와야하는 횟수가 현재 읽는 횟수 보다 큰경우 그냥 다읽어와!
    // 아닐 경우는 버퍼보다 작은 바이트 크기만큼 읽어와!
    if (stream->count <= stream->read_loop) {
        copyBytes = BUFSIZE;
    } else {
        copyBytes = stream->read_remain;
    }

    // 파일의 현재 커서 위치를 buffer 의 시작 위치로 지정
    stream->ptr = stream->buffer;
    // fd에서 buffer로 복사할 바이트 만큼 읽어들이기
    // size는 읽어들이 바이트 개수. 즉, buffer의 남아있는 바이트 수
    stream->size = read(stream->fd, stream->buffer, copyBytes);

    // 만약 읽어들인게 없다면? size가 0일 것임.
    if (stream->size == 0) {
        stream->eof = 1;
        return 1;
    }

    // read가 실패했을 경우 size가 -1.
    if (stream->size < 0) {
        // 읽어들인게 없다고 처리.
        stream->size = 0;
        stream->eof = 1;
        return -1;
    }
    return 0;
}

// ptr이라는 배열에 size 크기의 자료형 nmemb개를 stream으로 부터 읽어오는 함수
int fread(void *ptr, int size, int nmemb, FILE *stream) {
    // 버퍼를 읽어 들이는 횟수
    stream->count = 0;
    // 총 읽어야 하는 바이트 수
    size_t total = size * nmemb;
    // 고정된 크기의 버퍼를 몇번 채워야 하는지
    stream->read_loop = total / BUFSIZE;
    // 버퍼크기 내에서 읽어야하는 바이트 수
    stream->read_remain = total % BUFSIZE;

    // 몇개 읽었는지 세기
    int readByteCount = 0;
    // 읽어서 채워 넣어줘야하는 버퍼 임시 파일로 받기
    char *temp = ptr;

    // 파일의 권한인 읽기일때 못읽었다고 리턴하기
    if (stream->mode == WRITE || stream->mode == APPEND) {
        return 0;
    }

    // 버퍼가 비어 있을때
    if (stream->size == 0) {
        // 우선 한번 읽어들여야해서 증가시키기.
        stream->count++;
        // 버퍼에 값을 채워주기
        fillBuffer(stream);
    }

    // 그래도 버퍼가 비어있다면 읽을게 없다는 거니까 리턴
    if (stream->size == 0) {
        // 결국 0을 리턴하는 셈
        return readByteCount;
    }

    // FILE이 읽어들일 기본 주소
    stream->base = stream->buffer;
    // 파일 버퍼에 남아있는게 있고, 읽어들인 바이트가 총 읽어야할 바이트보다 작을때 계속 읽어야한다!
    while ((stream->size) && (readByteCount < total)) {
        // temp는 입력 받아야할 main의 버퍼, stream의 ptr은 현재 읽고있는 파일 커서의 위치
        // 데이터를 집어 넣어준다.
        *temp = *(stream->ptr);
        // 다음으로 이동하기
        temp++;
        stream->ptr++;
        // 하나 읽었으니까 크기 줄이기
        stream->size--;
        // 읽은 바이트수 하나 늘리기
        readByteCount++;
        // 앞으로 읽어야할 바이트가 버퍼에 없으면?
        if (stream->size == 0) {
            // 다시 버퍼를 채우자
            fillBuffer(stream);
            // 읽은 횟수 하나 늘리기
            stream->count++;
        }
        // 그래도 읽을 게 없다면? 다읽었다하고 리턴
        if (stream->size == 0) {
            return readByteCount;
        }
    }
    // 그리고 꼭 끝에 다 읽었다는 \0문자를 넣어줘야 한다.
    *(temp++) = '\0';
    return readByteCount;
}

int fgetc(FILE *stream) {
    char temp[2] = {0,};
    int result = fread(temp, sizeof(char), 1, stream);
    if (result == 0)
        return EOF;
    return temp[0];
}


void updateBuffer(FILE *stream, char *temp) {
    // stream은 저장할 파일, temp는 소스
    int flag;
    // 써야할 바이트 수
    int wrbytes;
    // 우선 쓴 횟수 증가
    stream->count++;
    // 버퍼 크기만큼 통채로 써야하는 횟수보다 현재 쓴 횟수가 적다면,
    if (stream->count <= stream->write_loop) {
        // 앞으로 써야할 바이트의 수는 버퍼크기만큼.
        wrbytes = BUFSIZE;
    } else {
        // 우선 버퍼의 쓰레기값 모두 널값으로 초기화
        for (int i = 0; i < BUFSIZE; i++) {
            stream->buffer[i] = '\0';
        }
        // 앞으로 써야할 바이트 수..
        wrbytes = stream->write_remain;
        // 더이상 쓸지 말지 표시하기
        stream->write_flag = 1;//더이상 안하겠다는 소리
    }

    // 현재 가리키는 파일 커서가 있다면
    if (stream->ptr != NULL) {
        // 파일에 저장해야하는 값 버퍼에 전부 복사하기
        memcpy(stream->buffer, temp, wrbytes);
        // 그리고 파일에 저장하고, 저장한 크기 flag에 표시
        flag = write(stream->fd, stream->buffer, wrbytes);
        // 실패하면 그냥 리턴
        if (flag == -1) {
            return;
        }
        // 만약에 write를 더 할건지 안할 것지 확인하는데 더 write 안하면 종료
        if (stream->write_flag)
            return;
    } else {//가리키는 파일 커서가 없다면
        //버퍼에 temp 값 복사하기
        memcpy(stream->buffer, temp, wrbytes);
        // 파일에 저장한 크기 flag에 표시
        flag = write(stream->fd, stream->buffer, wrbytes);
    }
    // 현재 커서 위치를 buffer 위치로 조정
    stream->ptr = stream->buffer;
    // 버퍼에 남아있는 값 표시
    stream->size = wrbytes;

    return;
}

// ptr의 배열의 값을 size 자료형의 크기 만큼 nmemb 만큼 stream에 복사한다.
int fwrite(const void *ptr, int size, int nmemb, FILE *stream) {
    // 쓴 횟수 초기화
    stream->count = 0;
    // 입력값 버퍼 temp로 임시파일 만들기
    char *temp = ptr;
    // 총 파일에 입력해야할 갯수
    size_t total = size * nmemb;
    // 버퍼 크기만큼 통째로 읽어야하는 횟수
    stream->write_loop = total / BUFSIZE;
    // 추가적으로 발생하는 바이트의 수
    stream->write_remain = total % BUFSIZE;
    // 실제로 파일에 입력한 값을 0으로 초기화 하기
    size_t bytes_written = 0;

    // 만약에 읽기 모드라면 쓴게 없다고 하고 종료하기
    if (stream->mode == READ) {
        return 0;
    }

    // 파일에 저장할 전체 바이트 수가 버퍼의 크기보다 작으면?
    if (total < BUFSIZE) {
        // 총 입력받은 바이트 수를 받아 파일에 저장하고 그대로 종료.
        bytes_written = write(stream->fd, temp, sizeof(char) * total);
        // 실제로 저장된 크기 입력.
        stream->actual_size += bytes_written;
        return bytes_written;
    }

    // 버퍼에 남아있는 바이트가 없고, 현재 커서가 없다면?
    if ((stream->size == 0) || (stream->ptr == NULL)) {
        // 버퍼를 채워주기
        updateBuffer(stream, temp);
    }
    // 버퍼에 남아있는 데이터 값이 0이라면, 더이상 저장할 값이 없다는 소리
    if (stream->size == 0) {
        // 저장한 크기만큼 리턴
        return bytes_written;
    }

    // 버퍼에 남아있는 값도 있고, 앞으로 읽어야할 값이 모자라다면,
    while ((stream->size) && (bytes_written < total)) {
        // 파일 버퍼에 소스 문자열의 값을 저장
        *(stream->ptr) = *temp;
        // 저장 했으니 한칸씩 이동
        temp++;
        stream->ptr++;
        // 저장했으니 저장해야하는 바이트 크기 줄이기
        stream->size--;
        // 저장한 바이트 수 증가
        bytes_written++;
        // 만약에 버퍼에 들어있는게 없거나, 현재 커서가 가리키는 값이 없다면
        if ((stream->size == 0) || (stream->ptr == NULL)) {
            // 버퍼를 업데이트 하고
            updateBuffer(stream, temp);
            // 쓴 횟수 증가시키기
            stream->count++;
        }
        // 그래도 더이상 저장할 게 없다면
        if (stream->size == 0) {
            // 지금까지 저장한 크기 더하고
            stream->actual_size += bytes_written;
            // 저장한 크기 리턴
            return bytes_written;
        }
    }
    // 저장한 크기 리턴
    return bytes_written;
}

int fputc(int c, FILE *stream) {
    // 그냥 fwrite 이용
    char temp[2] = {0, 0};
    temp[0] = c;
    int result = fwrite(temp, sizeof(char), 1, stream);
    if (result == 0) {
        return EOF;
    }
    return temp[0];
}

// 버퍼 비우고 커서 원래대로...
int fflush(FILE *stream) {
    if (stream == NULL){
        return EOF;
    }

    for (int i = 0; i < BUFSIZE; i++) {
        stream->buffer[i] = '\0';
    }

    stream->ptr = NULL;
    stream->size = 0;
    return 0;
}

// stream 파일에서 whence로부터 offset 만큼 떨어진 포인터 지정하기
int fseek(FILE *stream, int offset, int whence) {
    if (whence == SEEK_SET) {
        if (lseek(stream->fd, offset, SEEK_SET) != -1) {
            stream->ptr = NULL;
            stream->size = 0;
            return 0;
        } else {
            return -1;
        }
    }

    if (whence == SEEK_END) {
        if (lseek(stream->fd, offset, SEEK_END) != -1) {
            stream->ptr = NULL;
            stream->size = 0;
            return 0;
        } else {
            return -1;
        }
    }

    if (whence == SEEK_CUR) {
        if (lseek(stream->fd, offset - stream->size, 1) != -1) {
            stream->ptr = NULL;
            stream->size = 0;
            return 0;
        } else {
            return -1;
        }
    }

    return 0;
}

int feof(FILE *stream) {
    if (stream == NULL)
        return 0;
    if (stream->eof)
        return 1;
    else
        return 0;
}