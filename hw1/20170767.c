#include "20170767.h"

int main() {
    FILE *fp = fopen("input.txt", "r");

    // 모든 출력값은 \n과 \t으로 구분하였음.
    stringToSignedChar(fp);
    stringToAscii(fp);
    stringToUnsignedChar(fp);
    stringToSignedInt(fp);
    stringToUnsignedInt(fp);
    stringToFloat(fp);
    stringToDouble(fp);

    fclose(fp);
    return 0;
}

/// 이진수의 문자열을 부호있는 문자형으로 전환하는 함수
int stringToSignedChar(FILE *fp) {
    signed char result = 0; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(CHAR_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, CHAR_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result에 shift 연산자를 통해 저장
        for (int i = 0; i < CHAR_SIZE * sizeof(char); ++i) {
            result = result << 1;
            if (buffer[i] == '1') {
                result = result | 1;
            }
        }

        // 결과값을 출력
        printf("%d\t", result);

        // 결과값 초기화
        result = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 아스키코드로 전환하는 함수
int stringToAscii(FILE *fp) {
    char result = 0; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(CHAR_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, CHAR_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result에 shift 연산자를 통해 저장
        for (int i = 0; i < CHAR_SIZE * sizeof(char); ++i) {
            result = result << 1;
            if (buffer[i] == '1') {
                result = result | 1;
            } else { ;
            }
        }

        // 아스키 코드에 없는 문자열일 경우, .으로 출력
        if (!(result >= 0 && result <= 127)) {
            result = '.';
        }

        // 결과값을 출력
        printf("%c\t", result);

        // 결과값 초기화
        result = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 부호없는 문자형으로 전환하는 함수
int stringToUnsignedChar(FILE *fp) {
    unsigned char result = 0; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(CHAR_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, CHAR_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result에 shift 연산자를 통해 저장
        for (int i = 0; i < CHAR_SIZE * sizeof(char); ++i) {
            result = result << 1;

            if (buffer[i] == '1') {
                result = result | 1;
            } else { ;
            }
        }

        // 결과값을 출력
        printf("%u\t", result);

        // 결과값 초기화
        result = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 부호있는 정수형으로 전환하는 함수
int stringToSignedInt(FILE *fp) {
    int result = 0; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(INT_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, INT_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result에 shift 연산자를 통해 저장
        // 그냥 입력 받아도 little endian 방식으로 저장됨.
        for (int i = 0; i < INT_SIZE; ++i) {
            result = result << 1;
            if (buffer[i] == '1') {
                result = result | 1;
            }
        }

        // 결과값을 출력
        printf("%d\t", result);

        // 결과값 초기화
        result = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 부호없는 정수형으로 전환하는 함수
int stringToUnsignedInt(FILE *fp) {
    unsigned int result = 0; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(INT_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, INT_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result에 shift 연산자를 통해 저장
        // 그냥 입력 받아도 little endian 방식으로 저장됨.
        for (int i = 0; i < INT_SIZE; ++i) {
            result = result << 1;
            if (buffer[i] == '1') {
                result = result | 1;
            }
        }

        // 결과값을 출력
        printf("%u\t", result);

        // 결과값 초기화
        result = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 4바이트 실수형으로 전환하는 함수
int stringToFloat(FILE *fp) {
    union unionForFloat result; // 최종 결과값을 저장할 곳

    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(FLOAT_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, FLOAT_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result공용체에 shift 연산자를 통해 저장
        // 그냥 입력 받아도 little endian 방식으로 저장됨.
        for (int i = 0; i < FLOAT_SIZE; ++i) {
            result.sample2 = result.sample2 << 1;
            if (buffer[i] == '1') {
                result.sample2 = result.sample2 | 1;
            }
        }

        // 결과값을 출력
        printf("%.4f\t", result.sample1);

        // 결과값 초기화
        result.sample2 = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}

/// 이진수의 문자열을 8바이트 실수형으로 전환하는 함수
int stringToDouble(FILE *fp) {
    union unionForDouble result; // 최종 결과값을 저장할 곳


    while (TRUE) {
        // 필요한 크기의 버퍼 할당하기
        char *buffer = calloc(DOUBLE_SIZE, sizeof(char));

        // 문자형 크기만큼 파일 읽기
        fread(buffer, DOUBLE_SIZE * sizeof(char), 1, fp);

        // 문서의 끝을 만났을 때 해당 반복문을 종료하기
        if (feof(fp)) {
            free(buffer);
            break;
        }

        // 결과를 result공용체에 shift 연산자를 통해 저장
        // 그냥 입력 받아도 little endian 방식으로 저장됨.
        for (int i = 0; i < DOUBLE_SIZE; ++i) {
            result.sample2 = result.sample2 << 1;
            if (buffer[i] == '1') {
                result.sample2 = result.sample2 | 1;
            }
        }

        // 결과값을 출력
        printf("%.4lf\t", result.sample1);
//        dump_mem(&result.sample1, sizeof(result.sample1));

        // 결과값 초기화
        result.sample2 = 0;

        // 버퍼 할당 해제
        free(buffer);
    }

    // 파일 커서 상단으로 올리기
    rewind(fp);
    printf("\n");
    return 0;
}
