#include <elf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t bswap16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }
static uint32_t bswap32(uint32_t v) {
    return ((v & 0x000000FFu) << 24) | ((v & 0x0000FF00u) << 8) | ((v & 0x00FF0000u) >> 8) | ((v & 0xFF000000u) >> 24);
}
static uint64_t bswap64(uint64_t v) {
    return ((uint64_t)bswap32((uint32_t)(v & 0xFFFFFFFFu)) << 32) |
           (uint64_t)bswap32((uint32_t)((v >> 32) & 0xFFFFFFFFu));
}

static int host_is_little_endian(void) {
    uint16_t x = 0x0102;
    return *((uint8_t *)&x) == 0x02;
}

static void fix_ehdr_endian(const Elf64_Ehdr *src, Elf64_Ehdr *dst, int file_is_le, int host_is_le) {
    *dst = *src;
    if (file_is_le != host_is_le) {
        dst->e_type      = bswap16(src->e_type);
        dst->e_machine   = bswap16(src->e_machine);
        dst->e_version   = bswap32(src->e_version);
        dst->e_entry     = bswap64(src->e_entry);
        dst->e_phoff     = bswap64(src->e_phoff);
        dst->e_shoff     = bswap64(src->e_shoff);
        dst->e_flags     = bswap32(src->e_flags);
        dst->e_ehsize    = bswap16(src->e_ehsize);
        dst->e_phentsize = bswap16(src->e_phentsize);
        dst->e_phnum     = bswap16(src->e_phnum);
        dst->e_shentsize = bswap16(src->e_shentsize);
        dst->e_shnum     = bswap16(src->e_shnum);
        dst->e_shstrndx  = bswap16(src->e_shstrndx);
    }
}

static void fix_phdr_endian(const Elf64_Phdr *src, Elf64_Phdr *dst, int file_is_le, int host_is_le) {
    *dst = *src;
    if (file_is_le != host_is_le) {
        dst->p_type   = bswap32(src->p_type);
        dst->p_flags  = bswap32(src->p_flags);
        dst->p_offset = bswap64(src->p_offset);
        dst->p_vaddr  = bswap64(src->p_vaddr);
        dst->p_paddr  = bswap64(src->p_paddr);
        dst->p_filesz = bswap64(src->p_filesz);
        dst->p_memsz  = bswap64(src->p_memsz);
        dst->p_align  = bswap64(src->p_align);
    }
}

static const char *etype_to_str(uint16_t e_type) {
    switch (e_type) {
        case ET_NONE:
            return "ET_NONE";
        case ET_REL:
            return "ET_REL";
        case ET_EXEC:
            return "ET_EXEC";
        case ET_DYN:
            return "ET_DYN";
        case ET_CORE:
            return "ET_CORE";
        default:
            return "ET_UNKNOWN";
    }
}

int main(int argc, char **argv) {
    const char *path = (argc >= 2) ? argv[1] : "/bin/ls";

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "无法打开文件 %s: %s\n", path, strerror(errno));
        return 1;
    }

    Elf64_Ehdr eh_src;
    if (fread(&eh_src, 1, sizeof(eh_src), fp) != sizeof(eh_src)) {
        fprintf(stderr, "读取 ELF 头失败\n");
        fclose(fp);
        return 1;
    }

    if (memcmp(eh_src.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "不是有效的 ELF 文件\n");
        fclose(fp);
        return 1;
    }
    if (eh_src.e_ident[EI_CLASS] != ELFCLASS64) {
        fprintf(stderr, "当前仅支持 ELF64\n");
        fclose(fp);
        return 1;
    }
    int file_is_le = (eh_src.e_ident[EI_DATA] == ELFDATA2LSB);
    int host_is_le = host_is_little_endian();

    Elf64_Ehdr eh;
    fix_ehdr_endian(&eh_src, &eh, file_is_le, host_is_le);

    if (eh.e_phoff == 0 || eh.e_phnum == 0) {
        fprintf(stderr, "ELF 中不存在程序头表\n");
        fclose(fp);
        return 1;
    }
    if (fseek(fp, (long)eh.e_phoff, SEEK_SET) != 0) {
        fprintf(stderr, "定位到程序头表失败\n");
        fclose(fp);
        return 1;
    }

    const size_t max_phnum = 4096;
    if (eh.e_phnum > max_phnum) {
        fprintf(stderr, "程序头数量过大，可能是畸形文件\n");
        fclose(fp);
        return 1;
    }

    size_t entsz = eh.e_phentsize;
    if (entsz < sizeof(Elf64_Phdr)) {
        fprintf(stderr, "程序头项大小异常：%zu\n", entsz);
        fclose(fp);
        return 1;
    }

    int first = 1;
    printf("Type: %s, Entry: 0x%" PRIx64 ", Load segments: ", etype_to_str(eh.e_type),
           eh.e_entry);
    for (uint16_t i = 0; i < eh.e_phnum; ++i) {
        if (fseek(fp, (long)(eh.e_phoff + (uint64_t)i * entsz), SEEK_SET) != 0) {
            fprintf(stderr, "\n读取程序头项定位失败\n");
            fclose(fp);
            return 1;
        }

        Elf64_Phdr ph_src;
        memset(&ph_src, 0, sizeof(ph_src));
        size_t to_read = sizeof(Elf64_Phdr);
        size_t readn = fread(&ph_src, 1, to_read, fp);
        if (readn < to_read) {
            fprintf(stderr, "\n读取程序头项失败\n");
            fclose(fp);
            return 1;
        }

        Elf64_Phdr ph;
        fix_phdr_endian(&ph_src, &ph, file_is_le, host_is_le);
        if (ph.p_type == PT_LOAD) {
            if (!first) {
                printf(", ");
            }
            first = 0;
            uint64_t start = ph.p_vaddr;
            uint64_t end = ph.p_vaddr + ph.p_memsz;
            printf("0x%" PRIx64 "-0x%" PRIx64, start, end);
        }
    }
    if (first) {
        printf("(none)");
    }
    printf("\n");

    fclose(fp);
    return 0;
}
