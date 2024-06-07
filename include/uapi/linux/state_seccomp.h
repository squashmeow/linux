#ifndef _UAPI_LINUX_STATE_SECCOMP_H
#define _UAPI_LINUX_STATE_SECCOMP_H
#include <asm/unistd.h>
#include <linux/types.h>

enum arg_type {
  ARG_CONST,
  ARG_REG,
  ARG_REG_DEREF,
};

#define PROBE_MAX_ARG_CNT 12
/* The argument spefication struct for each probe, a probe can take up to PROBE_MAX_ARG_CNT arguments */
struct arg_spec {
  unsigned long long val_off;
  enum arg_type arg_type;
  short reg_off;
  _Bool arg_signed;
  char arg_bitshift;
};

/* One of the command option for state_seccomp_ctx, specify which process to enable the feature */
struct action_init {
  int pid;
};

/* One of the command option for state_seccomp_ctx, mainly used to pass probe specification */
struct action_set_probe {
  int pid;
  int perf_fd;
  char ev_type;
  short arg_cnt;
  struct arg_spec args[PROBE_MAX_ARG_CNT];
};

/* One of the command option for state_seccomp_ctx, mainly used to add system call specification profile */
struct action_add_profile {
  int pid;
  const char __user *key;
  char __user *syscall_flag; // a pointer to an array of length NR_syscalls. Each cell take 1 (approve) or 0 (reject)
};

/* The context struct the userspace program use to specify command argument to control the kernel module */
struct state_seccomp_ctx{
  union {
    struct action_init ai;
    struct action_set_probe ap;
    struct action_add_profile aa;
  };
};

/* ---------------------------------------------------------------------- */ 
/* different event that can be triggered through the probe, the blew event strcut tells how to unpack the opaque values */
enum event_type {
  UF_START,
  UF_STOP,
  IF_START,
  IF_STOP,
  REQ_START,
  REQ_STOP,
  REQ_START2,
  REG_STOP2,
  SYS,
};

/* record runtime context for user-defined function execution */
struct user_func_event {
  char class_name;
  char func_name;
  char file_name;
  int lineno;
};

/* record runtime context for interal API function execution */
struct internal_func_event {
  char func_name;
  char file_name;
  int lineno;
};

/* record runtime context for an user request */
struct req_event {
  char method;
  char uri;
};

/* record context for a system call event, mainly used for profiling purpose */
struct sys_event {
  unsigned int sys_num;
};

/* the wrapper structrue for the above event */
struct state_seccomp_event {
  enum event_type ev_type;
  union {
    struct user_func_event uf_ev;
    struct internal_func_event if_ev;
    struct req_event r_ev;
    struct sys_event s_ev;
  };
};

#endif
