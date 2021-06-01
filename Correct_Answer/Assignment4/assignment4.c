
#include "assignment4.h"

int main()
{
    void *mem; // 할당할 메모리

    // 현재 위치를 계산하기 위한 포인터들
    int cur_point = 0;
    int old_point = 0;
    int buf_point = 0;
    
    void *struct_tmp_mem; // struct용 임시 버퍼
    int data_index = 0; // 들어온 데이터 타입의 개수
    int command; // 처음 allocate 또는 deallocate 명령어 받을 변수
    int input_cnt = 0; // struct 데이터 입력 횟수
    char *input_type; // 데이터 타입

    // data type 이름이 들어갈 버퍼
    char input_type_name[33];

    // 메모리 할당 및 초기화
    mem = malloc(BUFMAX); 
    memset(mem, 0, BUFMAX);
    input_type = malloc(10 * sizeof(char));
    
    // 데이터의 정보를 담을 data 타입 배열 선언. 최대 256개의 데이터가 들어올 수 있음
    data info_table[256];
    memset(info_table, 0, 256*sizeof(data));

    while (1)
    {
        memset(input_type, 0, 10);
        memset(input_type_name, 0, 33);

        printf("Do you want to allocate data (1) or deallocate data (2) ?\n");
        scanf("%d", &command);

        if (command == 1) // allocate
        {
            printf("What is the type of data you want to allocate and what will be the name of the data?\n");
            fflush(stdin);
            scanf("%s %[^'\n']s", input_type, input_type_name); // 데이터 타입 입력 받음, 공백 포함해서 엔터 전 까지 받음

            int check_dup = search_name(info_table, input_type_name); // 중복된 이름인지 체크

            if(check_dup != -1) // 이름이 중복됐다면
            {
                printf("Duplicate data name, please use another name\n");
                continue;
            }
                
            if (!strcmp(input_type, "struct")) // struct가 입력된 경우
            {
                printf("How many data should be in the struct\n"); // 데이터 개수
                scanf("%d", &input_cnt);

                if (input_cnt > BUFMAX) // 256개 이상의 데이터가 들어있는 struct는 입력을 받지 않는다
                {
                    printf("Numbers of data in struct type should be less then 256\n");
                    continue;
                }

                // struct의 경우 임시 버퍼에 다 받은 뒤 남은 공간과 대조해서 가능하면 할당하고, 불가능하면 할당하지 않는다.
                struct_tmp_mem = malloc(input_cnt * sizeof(double) + 1);
                // struct 버퍼의 최대 사이즈는 입력받은 개수 * double 타입의 크기이다
                memset(struct_tmp_mem, 0, input_cnt * sizeof(double) + 1);
                
                printf("Please specify each type and its value\n");

                buf_point = struct_allocate_mem(struct_tmp_mem, input_cnt); // 임시 버퍼에 메모리 할당

                if (space_check(cur_point, buf_point)) // 받을 공간이 남아있다면
                {
                    // 테이블 갱신
                    strncpy(info_table[data_index].name, input_type_name, 32 * sizeof(char));
                    info_table[data_index].size = buf_point;
                    data_index++;
                    // 메모리에 할당
                    memcpy(mem + cur_point, struct_tmp_mem, buf_point);
                    cur_point += buf_point;
                    // 임시 버퍼 해제
                    free(struct_tmp_mem);
                    dump_mem(mem, BUFMAX); // 출력
                }
                
            }
            else if ((!strcmp(input_type, "double")) || (!strcmp(input_type, "int")) || (!strcmp(input_type, "char"))) // double, int, char의 경우
            {
                old_point = cur_point;
                cur_point = allocate_mem(mem, cur_point, input_type); // 메모리 할당

                if (cur_point != old_point) // 공간이 남아 있다면
                {
                    // 테이블 갱신
                    strncpy(info_table[data_index].name, input_type_name, 32 * sizeof(char));
                    info_table[data_index].size = cur_point - old_point;
                    data_index++;
                    dump_mem(mem, BUFMAX);
                }
            }
            else // 다른 값이 들어오는 경우
            {
                printf("Please input \"struct\" or \"double\" or \"int\" or \"char\"\n");
                continue;
            } 
                
        }
        else if (command == 2) // deallocate의 경우
        {
            getchar();
            printf("What is the name of data you want to deallocate?\n");
            scanf("%[^'\n']s", input_type_name); // 해제하려는 데이터 이름 입력 받음, 공백 포함해서 엔터 전 까지 받음
            int delete_index = search_name(info_table, input_type_name); // 존재하는지 확인
            if(delete_index == -1) // 존재하지 않는다면
            {
                printf("There is no %s data in memory\n", input_type_name);
                continue;
            }
            else // 존재한다면
            {
                cur_point = deallocate_mem(mem, delete_index, info_table, data_index - 1); // 매모리 해제
                if(data_index > 0)
                    data_index--; // 전제 데이터 개수에서 하나 감소
                   
                printf("%s has been deallocated successfully\n", input_type_name);
                dump_mem(mem, BUFMAX); // 출력
            }

        }
        else // 다른 명령어가 들어오는 경우
        {
            printf("Please input (1) or (2)\n");
            continue; // 무시한다
        }


        printf("\n\n");
    }

    // 프로그램 종료시 사용했던 메모리 해제
    free(input_type);
    free(mem);

    return 0;
}
