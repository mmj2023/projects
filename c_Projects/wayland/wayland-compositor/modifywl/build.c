#define _GNU_SOURCE
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

Cmd cmd = {0};

int main(int argc, char **argv) {
  char *Home = NULL;
  char *Include = NULL;
  char *Lib = NULL;
  // trick to use wlroots localy and also check if home is set by using the Home variable
  if (!(Home = getenv("HOME"))) {
   goto cleanup;
   return 1;
  }

  asprintf(&Include, "-I%s/projects/c_Projects/wayland/wlroots/include", Home);
  asprintf(&Lib, "-L%s/projects/c_Projects/wayland/wlroots/build", Home);
  NOB_GO_REBUILD_URSELF(argc, argv);
  cmd_append(&cmd, "cc");
  cmd_append(&cmd, "src/main.c");
  /* cmd_append(&cmd, "-static"); */
  cmd_append(&cmd, "-o", "build/modifywl");
  cmd_append(&cmd, "-g");
  cmd_append(&cmd, "-Werror");
  /* cmd_append(&cmd, "-Wall"); */
  /* cmd_append(&cmd, "Wextra"); */
  cmd_append(&cmd, "-DWLR_USE_UNSTABLE");
  /* cmd_append(&cmd, "-I/home/username/projects/c_Projects/wayland/wlroots/include"); */
  /* cmd_append(&cmd, "-I../../wlroots/include"); */
  cmd_append(&cmd, Include);
  cmd_append(&cmd, "-I/usr/include/pixman-1");
  /* cmd_append(&cmd, "-L../../wlroots/build"); */
  cmd_append(&cmd, Lib);
  cmd_append(&cmd, "-lwlroots-0.20", "-lwayland-server", "-lxkbcommon", "-lpixman-1", "-ldrm", "-linput", "-lseat", "-ldisplay-info", "-lm");
  if (!cmd_run(&cmd)) {
   goto cleanup;
   return 1;
  }

cleanup:
  free(Include);
  free(Lib);
  return 0;
}
