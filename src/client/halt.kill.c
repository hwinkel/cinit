/***********************************************************************
 *
 *    2007 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Halt the system through a kill call
 */

#include <sys/types.h>     /* kill()            */
#include <signal.h>        /* kill()            */

#include "messages.h"      /* MSG_*             */
#include "cinit.h"         /* print_errno()     */
#include "signals.h"       /* signals mapping   */

int main()
{
   if(kill(1,SIG_CINIT_HALT) == -1) {
      print_errno(MSG_HALT_KILL);
      return 1;
   }

   return 0;
}