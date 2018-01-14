#include <stdlib.h>
#include <string.h>

#include "service.h"

int main(void)
{
      //set_level(LOG_ALL);
      log_set_level(~LOG_T_DEBUG);
      service_main();
      return 0;
}
