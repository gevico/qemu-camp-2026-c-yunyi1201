#include "myfile.h"

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
  case ET_NONE:
    type_str = "Unknown";
    break;
  case ET_REL:
    type_str = "Relocatable";
    break;
  case ET_EXEC:
    type_str = "Executable";
    break;
  case ET_DYN:
    type_str = "Shared Object/PIE";
    break;
  default:
    type_str = "Unknown";
    break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char *filename) {
  char filepath[256];
  int fd;
  Elf64_Ehdr ehdr;

  strcpy(filepath, filename);
  fflush(stdout);

  // 打开文件并读取 ELF header
  fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }
  if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
    perror("read");
    close(fd);
    return -1;
  }

  print_elf_type(ehdr.e_type);
  close(fd);
  return 0;
}