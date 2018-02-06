#include <stdlib.h>
#include <string.h>

#include "service.h"

int main(void)
{
      log_init();
      service_main();
      return 0;
}
