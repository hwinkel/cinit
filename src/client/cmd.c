/***********************************************************************
 *
 *    2007 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    cmd - the cinit client program - sends commands to cinit
 */

#include <unistd.h>        /* getopt            */
#include <signal.h>        /* signals           */
#include <stdio.h>         /* printf()          */
#include <stdlib.h>        /* free()            */

#include "cmd.h"           /* own header        */
#include "signals.h"       /* which signal      */
#include "svc.h"           /* service related   */
#include "intern.h"        /* print_errno()     */

#include "cinit.h"         /* cinit external    */

#define C_USAGE(error) usage(USAGE_TEXT,error)
#define LOG(a,b)     mini_printf(a,1); minit_printf(b,1);

enum CMD_ARGS {
   CMD_HPR,
   CMD_ENABLE,
   CMD_DISABLE,
   CMD_STATUS
};

enum CMD_OPTS {
   CMD_NO_WANTS = 0x1,
   CMD_NO_NEEDS = 0x2
};

/***********************************************************************
 * cmd: main
 */
int main(int argc, char **argv)
{
   int opt, tmp, what;
   char *svc;

   tmp = 0;

   while((opt = getopt(argc,argv,CMD_OPTIONS)) != -1) {
      switch(opt) {
         case 'h':   /* halt */
               what = CMD_HPR;
               tmp  = SIG_CINIT_HALT;
         break;

         case 'p':   /* power off */
               what = CMD_HPR;
               tmp  = SIG_CINIT_POWEROFF;
         break;
         
         case 'r':   /* reboot */
               what = CMD_HPR;
               tmp  = SIG_CINIT_REBOOT;
         break;

         /********************************************/
         case 'e':   /* enable service */
               what = CMD_ENABLE;
         break;

         case 'd':   /* disable service */
               what = CMD_DISABLE;
         break;

         case 'n':   /* exclude needs */
               tmp  |= CMD_NO_NEEDS;
         break;

         case 'w':   /* exclude wants */
               tmp  |= CMD_NO_WANTS;
         break;


         /********************************************/
         case 's':   /* get status */
            what = CMD_STATUS;
            svc  = optarg;
         break;

         case 'v':   /* get version */
            svc = cinit_get_version();
            if(svc) {
               printf("Version of cinit: %s\n",svc);
               free(svc);
               return 0;
            } else {
               printf("Cannot get version of cinit!\n");
               return 1;
            }
         break;

         default:
            break;
      }
   }
   printf("read args\n");

   switch(what) {
      case CMD_HPR:
         if(kill(1,tmp) == -1) {
            print_errno(MSG_KILL);
            return 1;
         }
      break;
      case CMD_STATUS:
         printf("get status\n");
         tmp = cinit_get_svc_status(svc);
         printf("Status is: %d\n",tmp);
         /*switch(tmp) {
            case ST_SH_ONCE:
            break;
            case ST_NEED_FAILD:
            break;
         }*/
      break;
   }

   return 0;
}
