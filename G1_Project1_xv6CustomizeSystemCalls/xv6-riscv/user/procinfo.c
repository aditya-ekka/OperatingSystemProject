#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  struct pstat st;
  
  if (getpinfo(&st) < 0) {
    printf("procinfo: getpinfo failed\n");
    exit(1);
  }
  
  printf("PID\tPRIO\tSTATE\t\tSIZE\tNAME\n");
  for (int i = 0; i < NPROC; i++) {
    if (st.inuse[i]) {
      char *state_str = "UNKNOWN";
      switch(st.state[i]) {
        case 0: state_str = "UNUSED  "; break;
        case 1: state_str = "USED    "; break;
        case 2: state_str = "SLEEPING"; break;
        case 3: state_str = "RUNNABLE"; break;
        case 4: state_str = "RUNNING "; break;
        case 5: state_str = "ZOMBIE  "; break;
      }
      printf("%d\t%d\t%s\t%d\t%s\n", st.pid[i], st.priority[i], state_str, (int)st.size[i], st.name[i]);
    }
  }
  
  exit(0);
}
