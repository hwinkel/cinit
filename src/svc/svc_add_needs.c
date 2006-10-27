/***********************************************************************
 * 
 *    2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 * 
 *    part of cLinux/cinit
 * 
 *    List handling
 */

//#include <stdlib.h>
//#include <string.h>
#include "cinit.h"
#include "messages.h"

int svc_add_needs(char *needs, char *is_needed)
{
   struct listitem *svc_needs, *svc_is_needed;

   svc_needs = svc_is_needed = NULL;

   svc_needs = list_search(needs);
   svc_is_needed = list_search(is_needed);

   if( !svc_needs || !svc_is_needed ) {
      return 0;
   }

   /* first add needs */

   /* now add needed_by */

   malloc... /* for adding */

   if(svc->needs == NULL) {
      svc->needs = neu;
   } else {
      svc->next = neu;
   }

   /* write generic function: */
   list_entry_add(list_pointer,new_entry);

   if( list == NULL ) { /* list is empty, we have to init it */
      list = tmp;
      list->after    = list;
      list->before   = list;
   } else {                                  /* list has members,add this one */
      tmp->after           = list;           /* begin after the new element   */
      tmp->before          = list->before;   /* change to the ex-last         */
      list->before->after  = tmp;            /* change last element           */
      list->before         = tmp;            /* first refers to previous now  */
   }

   tmp->abs_path = malloc( strlen(path) + 1);
   if( tmp->abs_path == NULL ) {
      LOG(MSG_ERR_ALLOC);
      return 0;
   }

   strcpy(tmp->abs_path,path);
   tmp->status = status;
   tmp->pid    = 0;
   
   return 1;
}
