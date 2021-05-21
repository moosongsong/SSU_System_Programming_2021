#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <elf.h>

#define STR4CMP "software"
#define SIZEOFSTR4CMP 8
#define STR4CHANGE "hackers!"

bool is_elf(int32_t fd);

void change_sw_to_hacker_in_rodata(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]);

void read_elf_header(int32_t fd, Elf64_Ehdr *elf_header);

void print_elf_header(Elf64_Ehdr elf_header);

char *read_section(int32_t fd, Elf64_Shdr sh);

void print_section_headers(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]);

//Implement this function to check whether an input file is an elf file or not.
/// 해당 파일이 ELF 파일인지를 확인하는 함수
bool is_elf(int32_t fd) {
    char test[4]; /// magic number를 담을 버퍼
    read(fd, test, sizeof(char) * 4); /// 파일의 앞의 4바이트 읽어오기
    /// 파일의 매직넘버를 비교하기
    if (test[0] == 0x7f && test[1] == 0x45 && test[2] == 0x4c && test[3] == 0x46) {
        return true;
    } else {
        return false;
    }
}

/// 실행파일 .rodata 영역에 있는 'software' 문자열을 'hackers!'로 바꾸는 함수
void change_sw_to_hacker_in_rodata(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
    uint32_t i;
    char *sh_str;

    assert(lseek(fd, (off_t) eh.e_shoff, SEEK_SET) == (off_t) eh.e_shoff);

    for (i = 0; i < eh.e_shnum; i++) {
        assert(read(fd, (void *) &sh_table[i], eh.e_shentsize) == eh.e_shentsize);
    }

    /* section-header string-table */
    sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

    off_t rodataOffset = 0; /// rodata의 위치
    off_t rodataSize = 0; /// rodata의 크기
    char buf[SIZEOFSTR4CMP]; /// 비교할 문자열 저장할 배열

    /// .rodata영역의 정보 확인하기
    for (i = 0; i < eh.e_shnum; i++) {
        if (!strncmp((sh_str + sh_table[i].sh_name), ".rodata", 7))
        {
            rodataOffset = sh_table[i].sh_offset;
            rodataSize = sh_table[i].sh_size;
        }
    }

    /// .rodata 영역에 위치한 문자열 찾아서 변경하기
    for (int j = 0; j < rodataSize - SIZEOFSTR4CMP; ++j) {
        lseek(fd, (off_t) rodataOffset + j, SEEK_SET);
        read(fd, buf, sizeof(char) * SIZEOFSTR4CMP);

        if (!strncmp(buf, STR4CMP, SIZEOFSTR4CMP)) {
            lseek(fd, (off_t) rodataOffset + j, SEEK_SET);
            write(fd, STR4CHANGE, sizeof(char) * SIZEOFSTR4CMP);
        }
    }
}

void read_elf_header(int32_t fd, Elf64_Ehdr *elf_header) {
    assert(elf_header != NULL);
    assert(lseek(fd, (off_t) 0, SEEK_SET) == (off_t) 0);
    assert(read(fd, (void *) elf_header, sizeof(Elf64_Ehdr)) == sizeof(Elf64_Ehdr));
}

void print_elf_header(Elf64_Ehdr elf_header) {
    printf("Storage class\t= ");
    switch (elf_header.e_ident[EI_CLASS]) {
        case ELFCLASS32:
            printf("32-bit objects\n");
            break;

        case ELFCLASS64:
            printf("64-bit objects\n");
            break;

        default:
            printf("Unknwon CLASS\n");
            break;
    }

    printf("Data format\t= ");
    switch (elf_header.e_ident[EI_DATA]) {
        case ELFDATA2LSB:
            printf("2's complement, little endian\n");
            break;

        case ELFDATA2MSB:
            printf("2's complement, big endian\n");
            break;

        default:
            printf("Unknwon Format\n");
            break;
    }

    printf("OS ABI\t\t= ");
    switch (elf_header.e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            printf("UNIX System V ABI\n");
            break;

        case ELFOSABI_HPUX:
            printf("HP-UX\n");
            break;

        case ELFOSABI_NETBSD:
            printf("NetBSD\n");
            break;

        case ELFOSABI_LINUX:
            printf("Linux\n");
            break;

        case ELFOSABI_SOLARIS:
            printf("Sun Solaris\n");
            break;

        case ELFOSABI_AIX:
            printf("IBM AIX\n");
            break;

        case ELFOSABI_IRIX:
            printf("SGI Irix\n");
            break;

        case ELFOSABI_FREEBSD:
            printf("FreeBSD\n");
            break;

        case ELFOSABI_TRU64:
            printf("Compaq TRU64 UNIX\n");
            break;

        case ELFOSABI_MODESTO:
            printf("Novell Modesto\n");
            break;

        case ELFOSABI_OPENBSD:
            printf("OpenBSD\n");
            break;

        case ELFOSABI_ARM_AEABI:
            printf("ARM EABI\n");
            break;

        case ELFOSABI_ARM:
            printf("ARM\n");
            break;

        case ELFOSABI_STANDALONE:
            printf("Standalone (embedded) app\n");
            break;

        default:
            printf("Unknown (0x%x)\n", elf_header.e_ident[EI_OSABI]);
            break;
    }

    printf("Filetype \t= ");
    switch (elf_header.e_type) {
        case ET_NONE:
            printf("N/A (0x0)\n");
            break;

        case ET_REL:
            printf("Relocatable\n");
            break;

        case ET_EXEC:
            printf("Executable\n");
            break;

        case ET_DYN:
            printf("Shared Object\n");
            break;
        default:
            printf("Unknown (0x%x)\n", elf_header.e_type);
            break;
    }

    printf("Machine\t\t= ");
    switch (elf_header.e_machine) {
        case EM_NONE:
            printf("None (0x0)\n");
            break;

        case EM_386:
            printf("INTEL x86 (0x%x)\n", EM_386);
            break;

        case EM_X86_64:
            printf("AMD x86_64 (0x%x)\n", EM_X86_64);
            break;

        case EM_AARCH64:
            printf("AARCH64 (0x%x)\n", EM_AARCH64);
            break;

        default:
            printf(" 0x%x\n", elf_header.e_machine);
            break;
    }
    printf("\n");
}

char *read_section(int32_t fd, Elf64_Shdr sh) {
    char *buff = malloc(sh.sh_size);
    if (!buff) {
        printf("%s:Failed to allocate %ldbytes\n",
               __func__, sh.sh_size);
    }

    assert(buff != NULL);
    assert(lseek(fd, (off_t) sh.sh_offset, SEEK_SET) == (off_t) sh.sh_offset);
    assert(read(fd, (void *) buff, sh.sh_size) == sh.sh_size);

    return buff;
}

void print_section_headers(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
    uint32_t i;
    char *sh_str;

    assert(lseek(fd, (off_t) eh.e_shoff, SEEK_SET) == (off_t) eh.e_shoff);

    for (i = 0; i < eh.e_shnum; i++) {
        assert(read(fd, (void *) &sh_table[i], eh.e_shentsize) == eh.e_shentsize);
    }

    /* section-header string-table */
    sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

    for (i = 0; i < eh.e_shnum; i++) {
        if (!strncmp((sh_str + sh_table[i].sh_name), ".rodata", 7)) {
            printf("%s section info\n", (sh_str + sh_table[i].sh_name));
            printf("    file offset = 0x%08lx\n", sh_table[i].sh_offset);
            printf("           size = 0x%08lx\n", sh_table[i].sh_size);
        }
    }

}
