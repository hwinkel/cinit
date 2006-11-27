/***********************************************************************
 *
 *    2006 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Build **argv for execution in cinit way:
 *    1. check whether cmd is a valid executable
 *    2. try to read cmd.params and write parameters
 *    3. try to read cmd.env and setup environment
 *
 *    Returns:
 *       BA_OK:         Everything went well, you can use the struct
 *       BA_NOTFOUND:   Binary executable is not available
 *       BA_PARAMS:     Reading the parameters failed (ignoring non-existence!)
 *       BA_ENV:        Reading the environment failed (same as above)
 *       BA_OTHER:      Other error
 */

#include <fcntl.h>                  /* open()            */
#include <stdlib.h>                 /* *alloc()          */
#include <unistd.h>                 /* readlink          */
#include <errno.h>                  /* errno             */
#include <limits.h>                 /* PATH_MAX          */
#include <stdio.h>                  /* NULL              */
#include <string.h>                 /* strchr            */
#include <sys/stat.h>               /* stat()            */

#include "cinit.h"
#include "build_argv.h"

 /*
 * char *basename: something we should execute
 * (*basename) + ".params" will be added as parameters
 * (*basename) + ".env" will be added as environment
 */

int cinit_build_argv(char *basename, struct ba_argv *bav)
{
   int         tmp;
   int         fd;
   int         argc;
   char        pathtmp[PATH_MAX+1];
   char        *sbuf = NULL;
   char        *p;
   struct stat buf;

   /* sane values */
   bav->argv = NULL;
   bav->envp = NULL;

   mini_printf("CBA::",1);
   mini_printf(basename,1);
   mini_printf("\n",1);

   /***********************************************************************
    * Try to get realname (for links)
    */
   if((tmp = readlink(basename,pathtmp,PATH_MAX)) == -1) {
      /* nothing there? */
      if(errno == ENOENT) {
         return BA_E_NOTFOUND;
      } 
      if (errno != EINVAL) {
         return BA_E_OTHER;
      }
      tmp=strlen(basename);
      strncpy(pathtmp,basename,tmp);
   }
   pathtmp[tmp] = '\0';
   ++tmp; /* the byte to add to memory for \0;
             neither readlink nor strlen count the \0 */
   
   mini_printf("CBA::NOCH DA1",1);
   mini_printf("\n",1);
   /***********************************************************************
    * prepare argv0
    */
   bav->argv = malloc(sizeof(char *));
   if(bav->argv == NULL) return BA_E_MEM;

   *bav->argv = malloc(tmp);
   if(*(bav->argv) == NULL) return BA_E_MEM;

   strncpy(*(bav->argv),pathtmp,tmp);

   /********************** read params *********************/
   /* FIXME check bounds! */
   strcpy(pathtmp,basename);
   strcat(pathtmp,C_PARAMS);
   /* ORC_ERR_NONEXISTENT: Ok, have sbuf set to NULL
    * ORC_OK: Ok, have a filled buffer (perhaps NULL, too)
    * other: Error, print errno
    */
   mini_printf("CBA::NOCH DA2",1);
   mini_printf("\n",1);
   tmp = openreadclose(pathtmp,&sbuf);

   printf("tmp:: %d\n",tmp);

   mini_printf("CBA:",1);
   mini_printf(sbuf,1);
   mini_printf("\n",1);
   
   mini_printf(sbuf,1);

   if(tmp != ORC_ERR_NONEXISTENT && tmp != ORC_OK) {
      print_errno(pathtmp);
      return BA_E_PARAMS;
   }
   mini_printf("CBA::NOCH DA4",1);
   mini_printf("\n",1);
   
   sbuf = strip_final_newline(sbuf);

   /***********************************************************************
    * Now split the string, converting \n to \0
    */
   argc = 1; /* argv0 */
   while(sbuf != NULL) {
      p = strchr(sbuf,'\n');
      bav->argv = realloc(bav->argv, sizeof(char *) * (argc + 1));

      if(bav->argv == NULL) return BA_E_MEM;
      bav->argv[argc] = sbuf;     /* here begins the current argument */
      
      if(p != NULL) {   /* found another \n */
         *p = '\0';
         sbuf = p+1;
      } else {          /* end of string */
         sbuf = NULL;
      }
      mini_printf("argc: ",1);
      mini_printf(sbuf,1);
      mini_printf("\n",1);
      
      ++argc;
   }
   mini_printf("PAST ARGV\n",1);

   /************ close argv list **************/
   bav->argv = realloc(bav->argv, sizeof(char *) * (argc + 1)); /* 1: NULL-pointer */
   if(bav->argv == NULL) return BA_E_MEM;
   bav->argv[argc] = NULL;  /* terminate argv list */

   /********************** read environment *********************/
   strcpy(pathtmp,bav->argv[0]);
   strcat(pathtmp,C_ENV);

   argc = 0;
   sbuf = NULL;
   if( !stat(pathtmp,&buf) ) {
      fd = open(pathtmp,O_RDONLY);

      /* file exists, failing to open it is an error */
      if(fd == -1) {
         return BA_E_ENV;
      }

      while ( (tmp = read(fd,pathtmp,PATH_MAX) ) != 0 ) {
         if(tmp == -1) { 
            return BA_E_ENV;
         }

         sbuf = realloc(sbuf,argc + tmp + 1);
         strncpy(&sbuf[argc],pathtmp,tmp);
         argc += tmp;
      }
      close(fd);
      if(argc)
         sbuf[argc] = '\0'; /* terminate string */
   }

   /************** build environment string **************/
   argc = 0;
   while( sbuf != NULL ) {
      p = strchr(sbuf,'\n');
         
      bav->envp = realloc(bav->envp, sizeof(char *) * (argc + 1));
      if(bav->envp == NULL) {
         return BA_E_MEM;
      }
      bav->envp[argc] = sbuf;

      /* if we found \n */
      if(p != NULL) {
         *p = '\0';
      } else {
         /* set to the end of sbuf, not to the \0, but one before */
         p = sbuf + (strlen(sbuf)-1); 
      }

      if( *(p+1) == '\0') {
         sbuf = NULL;
      } else {
         sbuf = p+1;
      }
      ++argc;
   }

   /************ close env list **************/
   bav->envp = realloc(bav->envp, sizeof(char *) * (argc + 1));
   if(bav->envp == NULL) {
      return BA_E_MEM;
   }
   bav->envp[argc] = NULL;

   return BA_OK;
}
