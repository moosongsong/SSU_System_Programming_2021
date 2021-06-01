#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFMAX 256 // 힙 최대 사이즈는 256이다

typedef struct __data // 데이터 타입의 이름과 사이즈를 저장하기 위한 구조체
{
    int size;
    char name[33];
} data;

void dump_mem(const void *mem, size_t len) // 메모리 덤프 함수
{
    printf("Here is the mem dump!\n");
    const char *buffer = mem;
    size_t i;
    for (i = 0; i < len; i++)
    {
        if (i > 0 && i % 16 == 0)
        {
            printf("\n");
        }
        printf("%02x ", buffer[i] & 0xff);
    }
    puts("");
}

int space_check(int cur_point, size_t data_size) // 메모리에 여유 공간이 있는지 체크하는 함수
{
    if (cur_point + data_size <= BUFMAX) // 공간이 남아있다면
        return 1; // 1 리턴

    else // 공간이 부족하다면
    {
        printf("There is not enough memory for the data, you can only use %d byte(s)\n", BUFMAX - cur_point); 
        return 0; // 0 리턴
    }
}

char change2oneByte(char *c_buf, int char_tmp_buffer) // Char형 데이터를 받을 때 숫자는 숫자, 문자는 ASCII 값으로 바꾸기 위한 함수
{
    char ctmp;
    if (c_buf[0] == '0') // atoi값이 0인 경우 구분하기 위함. 0이 입력되는 경우
    {
        int zero = 0;
        ctmp = (char)zero;
    }
    else // 입력된 문자가 0이 아닐 때
    {
        if (char_tmp_buffer == 0) // atoi값이 0이면 이는 숫자가 아닌 문자이다
            ctmp = c_buf[0]; // 맨 앞글자만 사용
        else // 다른 숫자가 나오는 경우 숫자가 입력된 것이다
            ctmp = (char)char_tmp_buffer; // down cast
    }
    
    return ctmp;
}

int allocate_mem(void *mem, int cur_point, char *input_type) // char, int, double을 메모리에 할당하는 함수
{
    double d_buf;
    int i_buf;
    char c_buf[10];
    int update_point = 0; // 배열의 위치를 가리키는 포인터

    fflush(stdin);

    if (!strcmp(input_type, "double")) // double이 입력되는 경우
    {

        if (space_check(cur_point, sizeof(double))) // 공간이 남았는지 확인
        { // 공간이 남아있다면
            printf("Please specify a value for the data type\n");
            scanf("%lf", &d_buf);  // double 입력받음
            memcpy(mem + cur_point, &d_buf, sizeof(double)); // 메모리에 쓴다
            update_point = cur_point + sizeof(double); // 포인터 위치 갱신
        }
        else
            update_point = cur_point; // 할당하지 않은 경우 포인터 위치는 유지된다
    }
    else if (!strcmp(input_type, "int")) // int가 입력되는 경우
    {
        if (space_check(cur_point, sizeof(int))) // 공간이 남았는지 확인
        { // 공간이 남아있다면
            printf("Please specify a value for the data type\n");
            scanf("%d", &i_buf); // int 입력받음
            memcpy(mem + cur_point, &i_buf, sizeof(int)); // 메모리에 쓴다
            update_point = cur_point + sizeof(int); // 포인터 위치 갱신
        }
        else
            update_point = cur_point; // 할당하지 않은 경우 포인터 위치는 유지된다
    }
    else if (!strcmp(input_type, "char")) // char가 입력되는 경우
    {
        if (space_check(cur_point, sizeof(char))) // 공간이 남았는지 확인
        { // 공간이 남아있다면
            printf("Please specify a value for the data type\n");
            char ctmp;
            scanf("%s", c_buf);    
            int char_tmp_buffer = atoi(c_buf); // 숫자는 숫자로 입력하기 위함
            ctmp = change2oneByte(c_buf, char_tmp_buffer); // 문자인지 숫자인지 체크
            memcpy(mem + cur_point, &ctmp, sizeof(char)); // 메모리에 쓴다
            update_point = cur_point + sizeof(char); // 포인터 위치 갱신
        }
        else
            update_point = cur_point; // 할당하지 않은 경우 포인터 위치는 유지된다
    }
    return update_point;
}

int struct_allocate_mem(void *mem, int input_cnt) // struct의 경우 입력받는 폼이 다르기에 별도의 함수로 입력받음
{
    double d_buf;
    int i_buf;
    char cs_buf[10];
    int buf_point = 0; // struct 데이터를 임시로 담을 버퍼의 포인터
    char *struct_input_type = malloc(10 * sizeof(char)); // struct의 명령 문자열을 받을 임시 버퍼

    for (int i = 0; i < input_cnt; i++)
    {

        fflush(stdin);
        memset(struct_input_type, 0, 10);
        scanf("%s", struct_input_type); // double, int, char을 받음

        if (!strcmp(struct_input_type, "double")) // double의 경우
        {
            scanf("%lf", &d_buf); // double 입력 받음
            memcpy(mem + buf_point, &d_buf, sizeof(double)); // 메모리에 넣기
            buf_point += sizeof(double); // 포인터 사이즈 갱신
        }
        else if (!strcmp(struct_input_type, "int")) // int의 경우
        {

            scanf("%d", &i_buf); // int 입력 받음
            memcpy(mem + buf_point, &i_buf, sizeof(int)); // 메모리에 넣기
            buf_point += sizeof(int); // 포인터 사이즈 갱신
        }
        else if (!strcmp(struct_input_type, "char")) // char의 경우
        {
            char ctmp;
            scanf("%s", cs_buf);
            int char_stmp_buffer = atoi(cs_buf); // 숫자인지 문자인지 체크하기 위함
            ctmp = change2oneByte(cs_buf, char_stmp_buffer); // char로 변환(1바이트로 변환)
            memcpy(mem + buf_point, &ctmp, sizeof(char)); // 메모리에 넣기
            buf_point += sizeof(char); // 포인터 사이즈 갱신
        }
        else
        {
            char *trash = malloc(100 * sizeof(char)); // 뒤에 들어오는 쓰레기 값을 처리하기 위한 임시 버퍼
            i--;
            scanf("%s", trash); // 쓰레기 값을 입력 받음
            printf("Please input \"char\" or \"int\" or \"double\"\n");
            free(trash); // 버림
        }
    }
    return buf_point; // struct의 크기를 반환
}

int search_name(data *info_table, char *input_type_name) // 위치와 이름을 관리하는 테이블에서 이름을 찾아주는 함수
{
    int name = -1; // -1이 반환되는 경우 찾는 이름이 없다는 뜻이다
    for (int i = 0; i < 256; i++) // 테이블 순회
    {
        if ((!strcmp(info_table[i].name, input_type_name)) && (info_table[i].size != 0)) // 이름의 초기화 값이 0이기에 이름이 0인 경우도 대비하기 위해서 사이즈도 체크.
        {
            name = i;
            break; // 이름 찾음
        }
    }
    return name; // 이름 위치 반환
}

void deallocate_table(data *info_table, int delete_index, int data_index) // 할당을 해제할 때 테이블 갱신하는 함수
{
    if (data_index == 0) // 맨 처음의 데이터를 해제하는 경우
    {
        info_table[data_index].size = 0; // 데이터 관리 테이블 사이즈 초기화
        memset(info_table[data_index].name, 0, 33); // 이름 초기화
    }
    else if (delete_index == data_index) // 맨 마지막 데이터를 해제하는 경우
    {
        info_table[delete_index].size = 0; 
        memset(info_table[delete_index].name, 0, 33);
    }
    else // 중간에 위치한 데이터를 해제하는 경우
    {
        for (int i = delete_index; i < data_index; i++) // 배열을 한칸 앞으로 당겨온다
        {
            info_table[i].size = info_table[i + 1].size;
            memcpy(info_table[i].name, info_table[i + 1].name, 33);
        }
        info_table[data_index].size = 0; // 배열 마지막 부분을 초기화한다.
        memset(info_table[data_index].name, 0, 33);
    }
}

int calculate_position(data *info_table, int index) // 테이블을 통해 데이터가 힙에서 위치한 부분을 계산하는 함수
{
    int position = 0;
    for (int i = 0; i < index; i++) // 주어진 이름에 도달하기 까지 모든 size를 더함
        position += info_table[i].size;
    return position;
}

int deallocate_mem(void *mem, int delete_index, data *info_table, int data_index) // 메모리를 할당 해제하는 함수
{
    int delete_size = info_table[delete_index].size; // 삭제하려는 데이터의 크기
    int last_size = info_table[data_index].size;  // 마지막 데이터의 크기
    int delete_position = calculate_position(info_table, delete_index); // 삭제하려는 데이터가 시작하는 위치 계산
    int last_position = calculate_position(info_table, data_index); // 마지막 데이터가 시작하는 위치 계산
    int change_size = last_position + last_size - delete_position - delete_size; // 움직여야 하는 배열의 크기 계산

    void *change_buffer = malloc(change_size); // 옮기기 위한 임시 버퍼
    memset(change_buffer, 0, change_size); // 초기화
    memcpy(change_buffer, mem + delete_position + delete_size, change_size); // 당길 크기 만큼 임시 버퍼에 저장
    memcpy(mem + delete_position, change_buffer, change_size); // 앞으로 당김
    memset(mem + last_position + last_size - delete_size, 0, delete_size); // 뒤에 남은 잉여값은 제거함. 지운 데이터 만큼 남아있을 것임

    deallocate_table(info_table, delete_index, data_index); // 테이블 갱신
    free(change_buffer); // 임시 버퍼는 다 쓰고 해제
    return last_position + last_size - delete_size; // 포인터의 위치 반환
}


