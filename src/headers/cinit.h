/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 */

#ifndef _CINIT_H
#define _CINIT_H

/* includes */
#include <sys/types.h>  /* pid_t */
#include "config.h"     /* paths, socket options, etc. */

/***********************************************************************
 * Defines
 */
#define SLASH "/"
#define CINIT_CONFDIR   CINIT_DIR SLASH C_CONFDIR
#define CINIT_PANIC  CINIT_CONFDIR SLASH C_PANIC

enum {         /* FIXME: there are some default defines somewhere for that */
   FD_IN=0,
   FD_OUT=1,
   FD_ERR=2
};

/***********************************************************************
 * Paths
 */
#define CINIT_INIT CINIT_DIR SLASH C_INIT
#define CINIT_TMNT CINIT_DIR SLASH C_TMP
#define CINIT_SOCK CINIT_TMNT SLASH C_SOCK
#define CINIT_BIN  PREFIX SLASH "sbin" SLASH "cinit"

#define CINIT_CONFDIR   CINIT_DIR SLASH C_CONFDIR
#define CINIT_PANIC     CINIT_CONFDIR SLASH C_PANIC
#define CINIT_UMOUNT    CINIT_CONFDIR SLASH C_UMOUNT

/* actions for i/o handlers (see comm/do_*) */
#define ACT_SERV     0
#define ACT_CLIENT   1

struct listitem {
   char     *abs_path;           /* name of service                           */
   int      status;              /* current status                            */
   pid_t    pid;                 /* pid of service / respawn watcher          */
   struct   listitem *before;    /* previous item                             */
   struct   listitem *after;     /* next item                                 */
   struct   listitem *wanted;    /* list of services that want this service   */
   struct   listitem *needed;    /* list of services that need this service   */
};

/* variables */
extern struct  listitem *list;   /* the process linked chain                  */
extern int     sock;             /* current writing/reading socket            */
extern pid_t   cpid;             /* used by main() and reused be respaw_svc() */

/* functions (used by server and client) */
int   run_svc(char *rpath);
void  mini_printf(char *str, int fd);
void  set_signals(int action);
void  usage(char *banner, char *stext);

int   do_svc_name(int sock2, char *svc, int action);
int   do_change_status(char *svc, char *status, pid_t *pid, int sock2, int action);
char  do_result(int sock2, char *value);

/* server functions */
int      list_insert(char *path, int status);
struct   listitem *list_search(char *path);
int      list_delete(char *path);
int      run_init_svc(char *cinit_svc);
void     sigio(int socket);
void     do_reboot(int signal);
void     panic(void);
void     sig_child(int signal);

/* client / message functions */
int      msg_svc_on_off(char *svc, char action);
int      msg_change_status(char *svc, char status, pid_t pid);
int      begin_msg(char cmd);

/* client functions */
int      run_run_svcs(char *abspath);
pid_t    exec_svc(char *abspath, int on);
pid_t    respawn_svc(char *abspath);
int      connect_sock(int socke);
char     **read_file(char *file);
void     sig_terminate(int signal);

/* listing functions (server only) */
int      list_insert(char *path, int status);
int      list_delete(char *path);
int      list_modify(char *path, int new_status, pid_t new_pid);
struct   listitem *list_search(char *path);
inline struct   listitem *list_search_pid(pid_t pid);

/* generic */
void execute_sth(char *basename);
int path_append(char *path, char *append);

/* util */
int      msg_reboot(char cmd);
void     print_errno(char *text);

/* service */
int svc_known(char *svc);

/* os-functions */
void cinit_reboot(void);
void cinit_halt(void);
void cinit_poweroff(void);

#endif   /* _CINIT_H */
