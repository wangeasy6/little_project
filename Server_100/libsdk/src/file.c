#include "file.h"

int file_write(char* buf, size_t buf_len, char* file_name, char* type)
{
      if ( buf == NULL || buf_len <= 0 ||\
       file_name == NULL || file_name[0] == 0)
      {
            return 0;
      }

      FILE *fd = fopen(file_name, type);
      if (fd == NULL)
      {
            perror("fopen");
            return 0;
      }

      fwrite(buf, 1, buf_len, fd);
      fclose(fd);
      return 1;
}