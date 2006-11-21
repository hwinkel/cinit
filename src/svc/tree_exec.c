/***********************************************************************
 *
 *    2006 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Start the service tree we created
 */

#include <stdio.h>              /* DEBUG */

#include <unistd.h>              /* _exit, fork */

#include "cinit.h"            
#include "messages.h"            /* D_PRINTF */
#include "svc.h"                 /* svc_init    */

/* some thoughts...
 *
 * - we already generated the tree, we now need to start it from
 *   the ends
 *
 * - after starting the first service we have to care about SIG_CHILD
 *   to record changes
 *
 * - we execute all services in parallel without problems, because of
 *   SIG_CHILD notification
 * 
 * - After successfully starting the service we start the service that
 *   need or want that service
 *
 *
 *
 */

int tree_exec(struct dep *start)
{
   struct dep *tmp;
   
   tmp = start;

   mini_printf("========> tree_exec() \n",1);
   /* the main starting loop: All services in this list should be
    * started, but it is possible that dependent services are in the
    * list. In this case simply skip the current service 
    */
   do {
      mini_printf(tmp->svc->abs_path,1);
      mini_printf(":::",1);

      switch(svc_needs_status(tmp->svc)) {
         case SNS_NEEDS_STARTED:
            mini_printf("abhaengigkeiten gestartet, exec; add wants, needs",1);
            /* FIXME: execute service */

            /* update status */
            /* FIXME: check return code of execute_sth */
            svc_success(tmp->svc);

            /* execute service, remowe from list */
            /* FIXME check return code? */
            dep_needs_wants_add(&tmp,tmp->svc);

            /* delete service from list */
            tmp = dep_entry_del(tmp);
            break;
         case SNS_NEEDS_FAILED:
            mini_printf("wer fehlgeschlagen",1);
            /* mark service as NEED_FAILD and delete from list */
            svc_set_status(tmp->svc,ST_NEED_FAILD);
            tmp = dep_entry_del(tmp);
            break;
         case SNS_NEEDS_UNFINISHED:
            mini_printf("noch warten",1);
            {
               struct dep *tmp2;
               tmp2=tmp->svc->needs;
               do {
                  mini_printf("::",1);
                  mini_printf(tmp2->svc->abs_path,1);
                  fprintf(stderr,"%s: <<%d>>\n",tmp2->svc->abs_path,tmp2->svc->status);
                  tmp2 = tmp2->next;
               } while (tmp2 != tmp->svc->needs);
            }
            /* continue with the next item */
            tmp = tmp->next;
            break;
      }
      mini_printf("\n",1);

//      tmp->svc->pid = fork();


//      if(tmp->svc->pid == -1) return 0;

 //     if(tmp->svc->pid == 0) { /* child code */
  //       execute_sth(tmp->svc->abs_path);
//         _exit(1);
   //   }

      /* add the services that want or need this service to the list
       * of services to be executed the next time
       *
       * We should add the new services at the beginning, so we
       * clean the original onse first
       *
       * And we need to check, whether the dependencies are already
       * solved.
       *
       * A service may be in status:
       *
       * - Unset (never touched)
       * - Being started (passed our loop, but did not yet finish)
       * - Respawning (means was started and we take care
       *   about it
       * - failed (with reason copied, so one can reprint it later?)
       * - once - started once successfully
       * - need failed - not started, because need failed. perhaps
       *   registert which dependenc(y|ies) failed?
       */
      /* FIXME: debug delay */
      sleep(1);
   } while(tmp != NULL);

   return 1;
}
