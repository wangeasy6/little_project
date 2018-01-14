#include "log.h"

int main()
{
      #ifdef USE_LOG_MODULE
	//log_set_level(LOG_M_MAIN, LOG_T_ALL);
      log_init();
	log_p(LOG_M_MAIN, LOG_T_INFO, "test1\n",0);
	log_p(LOG_M_INIT, LOG_T_ERROR, "test2\n",0);
	log_p(LOG_M_MAIN, LOG_T_DEBUG, "test3\n",0);
      log_p(LOG_M_MAX, LOG_T_ALL, "test4\n",0);
      log_fl(LOG_M_MAIN, LOG_T_INFO, "test4\n",0);
      #else

      log_set_level(LOG_T_INFO);
      log_p(LOG_T_INFO, "test1\n",0);
      log_p(LOG_T_INFO, "test2\n",0);
      log_p(LOG_T_DEBUG, "test3\n",0);
      #endif

      log_to_file(LOG_M_MAIN, LOG_T_INFO, "test1\n",0);
      log_to_file(LOG_M_INIT, LOG_T_ERROR, "test2\n",0);
      log_to_file(LOG_M_MAIN, LOG_T_DEBUG, "test3\n",0);
      log_to_file(LOG_M_MAX, LOG_T_ALL, "test4\n",0);

	return 1;
}
