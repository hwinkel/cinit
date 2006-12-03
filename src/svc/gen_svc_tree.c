/***********************************************************************
 *
 *    2006 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Pre calculate the service tree
 */

#include <stdlib.h>
#include "cinit.h"
#include "svc.h"

struct listitem *gen_svc_tree(char *svc)
{
   struct listitem   *li;
   struct dep        *deps;

   /* only do something if the service is not already known */
   if((li=list_search(svc)))           return li;

   /* create a template, so other instances won't try to recreate us */
   if(!(li=svc_create(svc)))           return NULL;

   if(!check_add_deps(li,DEP_NEEDS))   return NULL;
   if(!check_add_deps(li,DEP_WANTS))   return NULL;

   if(!li->wants && !li->needs) {
      deps = dep_create(li);
      if(!deps) return NULL;
      dep_entry_add(&svc_init,deps);

      /* mark it as being in _THE_ list */
      /* FIXME: is ST_IN_LIST already being checked? */
      li->status |= ST_IN_LIST;
   }

   return li;
}
