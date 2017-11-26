#include "main.h"
#include "service.h"
#include "basic_define.h"

int main()
{
      //set_level(LOG_ALL);
      set_level(~LOG_DEBUG);

      if( !service_init())
      {
            print(LOG_ERROR, "server init failed\n");
            return(0);
      }

      service_run();

      return 0;
}
