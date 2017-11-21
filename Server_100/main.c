#include "main.h"

int main()
{
      set_level(LOG_ALL);
      if( !service_init())
      {
            print(LOG_ERROR, "server init failed\n");
            return(0);
      }

      service_run();

      return 0;
}