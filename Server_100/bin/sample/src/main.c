#include "main.h"
#include "service.h"
#include "basic_define.h"

int main()
{
      //set_level(LOG_ALL);
      log_set_level(~LOG_T_DEBUG);

      if( !service_init())
      {
            log_p(LOG_T_ERROR, "server init failed\n");
            return(0);
      }

      service_run();

      return 0;
}
