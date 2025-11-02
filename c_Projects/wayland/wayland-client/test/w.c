#include "wayland-client.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

void reg_glob(void *data, struct wl_registry *wl_registry, uint32_t name,
              const char *interface, uint32_t version) { printf("%u\n", name); };

void reg_glob_remove(void *data, struct wl_registry *wl_registry,
                     uint32_t name) {};

struct wl_registry_listener reg_list = {
    .global = reg_glob,
    .global_remove = reg_glob_remove,
};

int8_t main() {
  // struct wl_display* dis = wl_display_connect(NULL);
  struct wl_display *dis = wl_display_connect(0);
  if (!dis) {
    printf("no display\n");
    return 1;
  }
  struct wl_registry *reg = wl_display_get_registry(dis);
  wl_registry_add_listener(reg, &reg_list, 0);
  wl_display_roundtrip(dis);

  wl_display_disconnect(dis);
  return 0;
}
