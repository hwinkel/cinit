/***********************************************************************
 *
 *    2005-2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Pre calculate the service tree
 */

#include "cinit.h"
#include <unistd.h>

int gen_svc_tree(char *start)
{
   /* only do something if the service is not already known */
   if(svc_known(start)) return 1;

   /* check for needs */

   /* check for wants */

   return 1;
}