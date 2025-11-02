/*
  Bare-bones Wayland To-Do List (C, raw Wayland + shm)
  - Type in the top box, Enter to add task, Backspace to edit.
  - Tasks are listed below.
  - No toolkit—just wl_shm and pixel blitting with an 8×8 bitmap font.
*/

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/memfd.h> // for MFD_CLOEXEC
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h> // for syscall(SYS_memfd_create)
#include <unistd.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#define WIN_W 400
#define WIN_H 300

#define INPUT_H 24
#define MARGIN 4
#define LINE_SP 16
#define MAX_INPUT 128
#define MAX_TASKS 100

/* 8×8 basic ASCII bitmap font */
static const uint8_t font8x8_basic[128][8] = {
    /* ASCII 0–31: empty */
    [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    [33] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00},
    [34] = {0x6C, 0x6C, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* … fill in 35–126 or include full array … */
    /* For brevity, you must paste a complete 8×8 ASCII font here. */
};

/* Global state */
static struct wl_display *display;
static struct wl_registry *registry;
static struct wl_compositor *compositor;
static struct wl_shm *shm;
static struct wl_shell *shell;
static struct wl_seat *seat;
static struct wl_surface *surface;
static struct wl_shell_surface *shell_surface;
static struct wl_buffer *buffer;
static void *shm_data;

static int shm_fd;
static size_t shm_size;

static struct xkb_context *xkb_ctx;
static struct xkb_keymap *xkb_keymap;
static struct xkb_state *xkb_state;
static struct wl_keyboard *keyboard;

static char input_buf[MAX_INPUT];
static int input_len = 0;

static char tasks[MAX_TASKS][MAX_INPUT];
static int task_count = 0;

/* Utility: create anonymous shm file descriptor */
/* static int */
/* create_shm_fd(size_t size) { */
/*     char template[] = "/tmp/todo-shm-XXXXXX"; */
/*     int fd = mkstemp(template); */
/*     if (fd >= 0) { */
/*         unlink(template); */
/*         if (ftruncate(fd, size) < 0) { */
/*             close(fd); */
/*             return -1; */
/*         } */
/*     } */
/*     return fd; */
/* } */
static int create_shm_fd(size_t size) {
  int fd = syscall(SYS_memfd_create, "todo_shm", MFD_CLOEXEC);
  if (fd < 0) {
    perror("memfd_create");
    return -1;
  }
  if (ftruncate(fd, size) < 0) {
    perror("ftruncate");
    close(fd);
    return -1;
  }
  return fd;
}

/* Draw helpers */
static inline void put_pixel(int x, int y, uint32_t col) {
  if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
    return;
  uint32_t *px = (uint32_t *)shm_data + y * WIN_W + x;
  *px = col;
}
static void fill_rect(int x, int y, int w, int h, uint32_t col) {
  for (int j = y; j < y + h; j++)
    for (int i = x; i < x + w; i++)
      put_pixel(i, j, col);
}
static void draw_char(int cx, int cy, char c, uint32_t fg) {
  if ((unsigned char)c >= 128)
    return;
  for (int by = 0; by < 8; by++) {
    uint8_t row = font8x8_basic[(int)c][by];
    for (int bx = 0; bx < 8; bx++) {
      if (row & (1 << bx))
        put_pixel(cx + bx, cy + by, fg);
    }
  }
}
static void draw_text(int x, int y, const char *s, uint32_t fg) {
  for (; *s; s++) {
    draw_char(x, y, *s, fg);
    x += 8;
  }
}

/* Redraw entire window */
static void redraw() {
  /* background */
  fill_rect(0, 0, WIN_W, WIN_H, 0xfff0f0f0);

  /* input box */
  fill_rect(MARGIN, MARGIN, WIN_W - 2 * MARGIN, INPUT_H, 0xffffffff);
  fill_rect(MARGIN, MARGIN, WIN_W - 2 * MARGIN, 1, 0xff000000);
  fill_rect(MARGIN, MARGIN + INPUT_H - 1, WIN_W - 2 * MARGIN, 1, 0xff000000);
  draw_text(MARGIN + 4, MARGIN + 4, input_buf, 0xff000000);

  /* tasks */
  for (int i = 0; i < task_count; i++) {
    int y = MARGIN * 2 + INPUT_H + i * LINE_SP;
    draw_text(MARGIN + 4, y, tasks[i], 0xff202020);
  }
}

/* Commit buffer to Wayland surface */
static void commit() {
  wl_surface_attach(surface, buffer, 0, 0);
  wl_surface_damage(surface, 0, 0, WIN_W, WIN_H);
  wl_surface_commit(surface);
}

/* wl_shm format callback */
static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
  /* ignore */
}
static const struct wl_shm_listener shm_listener = {.format = shm_format};

/* wl_buffer release callback */
static void buffer_release(void *data, struct wl_buffer *buf) {
  /* We reuse single buffer, nothing to do */
}
static const struct wl_buffer_listener buffer_listener = {.release =
                                                              buffer_release};

/* xkb common helper: process Enter/Backspace/text */
static void handle_key(uint32_t key, uint32_t state) {
  if (state != WL_KEYBOARD_KEY_STATE_PRESSED)
    return;

  xkb_state_update_key(xkb_state, key + 8, XKB_KEY_DOWN);
  char buf[64];
  int len = xkb_state_key_get_utf8(xkb_state, key + 8, buf, sizeof(buf));
  if (len > 0) {
    if (buf[0] == '\r' || buf[0] == '\n') {
      if (input_len > 0 && task_count < MAX_TASKS) {
        memcpy(tasks[task_count++], input_buf, input_len);
        tasks[task_count - 1][input_len] = '\0';
        input_len = 0;
        input_buf[0] = '\0';
      }
    } else {
      /* append normal character */
      if (input_len + len < MAX_INPUT - 1) {
        memcpy(input_buf + input_len, buf, len);
        input_len += len;
        input_buf[input_len] = '\0';
      }
    }
  } else {
    /* maybe Backspace */
    xkb_keysym_t sym = xkb_state_key_get_one_sym(xkb_state, key + 8);
    if (sym == XKB_KEY_BackSpace && input_len > 0) {
      /* delete one UTF-8 codepoint */
      while (input_len > 0 && (input_buf[--input_len] & 0xC0) == 0x80)
        ;
      input_buf[input_len] = '\0';
    }
  }
  redraw();
  commit();
  xkb_state_update_key(xkb_state, key + 8, XKB_KEY_UP);
}

/* wl_keyboard listener */
static void keyboard_keymap(void *data, struct wl_keyboard *kb, uint32_t format,
                            int fd, uint32_t size) {
  char *map_str = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  xkb_keymap = xkb_keymap_new_from_string(
      xkb_ctx, map_str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
  munmap(map_str, size);
  close(fd);

  xkb_state = xkb_state_new(xkb_keymap);
}
static void keyboard_enter(void *data, struct wl_keyboard *kb, uint32_t serial,
                           struct wl_surface *surf, struct wl_array *keys) {}
static void keyboard_leave(void *data, struct wl_keyboard *kb, uint32_t serial,
                           struct wl_surface *surf) {}
static void keyboard_key(void *data, struct wl_keyboard *kb, uint32_t serial,
                         uint32_t time, uint32_t key, uint32_t state) {
  handle_key(key, state);
}
static void keyboard_modifiers(void *data, struct wl_keyboard *kb,
                               uint32_t serial, uint32_t mods_depressed,
                               uint32_t mods_latched, uint32_t mods_locked,
                               uint32_t group) {}
static const struct wl_keyboard_listener keyboard_listener = {
    keyboard_keymap, keyboard_enter, keyboard_leave, keyboard_key,
    keyboard_modifiers};

/* wl_seat listener */
static void seat_handle_cap(void *data, struct wl_seat *seat, uint32_t caps) {
  if (caps & WL_SEAT_CAPABILITY_KEYBOARD) {
    keyboard = wl_seat_get_keyboard(seat);
    wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
  }
}
static const struct wl_seat_listener seat_listener = {seat_handle_cap};

/* registry binding */
static void registry_global(void *data, struct wl_registry *reg, uint32_t id,
                            const char *ifc, uint32_t ver) {
  if (strcmp(ifc, "wl_compositor") == 0) {
    compositor = wl_registry_bind(reg, id, &wl_compositor_interface, 4);
  } else if (strcmp(ifc, "wl_shm") == 0) {
    shm = wl_registry_bind(reg, id, &wl_shm_interface, 1);
    wl_shm_add_listener(shm, &shm_listener, NULL);
  } else if (strcmp(ifc, "wl_shell") == 0) {
    shell = wl_registry_bind(reg, id, &wl_shell_interface, 1);
  } else if (strcmp(ifc, "wl_seat") == 0) {
    seat = wl_registry_bind(reg, id, &wl_seat_interface, 5);
    wl_seat_add_listener(seat, &seat_listener, NULL);
  }
}
static void registry_global_remove(void *data, struct wl_registry *reg,
                                   uint32_t id) {}
static const struct wl_registry_listener registry_listener = {
    registry_global, registry_global_remove};

int main() {
  display = wl_display_connect(NULL);
  if (!display) {
    fprintf(stderr, "Cannot connect to Wayland display\n");
    return 1;
  }
  registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, NULL);
  wl_display_roundtrip(display);

  /* create surface & shell surface */
  surface = wl_compositor_create_surface(compositor);
  shell_surface = wl_shell_get_shell_surface(shell, surface);
  wl_shell_surface_set_toplevel(shell_surface);

  /* create SHM buffer */
  shm_fd = create_shm_fd(WIN_W * WIN_H * 4);
  if (shm_fd < 0) {
    /* fprintf(stderr,"Cannot create SHM fd\n"); */
    fprintf(stderr, "Failed to create SHM fd, aborting\n");
    return 1;
  }
  shm_size = WIN_W * WIN_H * 4;
  shm_data =
      mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_data == MAP_FAILED) {
    perror("mmap");
    close(shm_fd);
    return 1;
  }
  struct wl_shm_pool *pool = wl_shm_create_pool(shm, shm_fd, shm_size);
  buffer = wl_shm_pool_create_buffer(pool, 0, WIN_W, WIN_H, WIN_W * 4,
                                     WL_SHM_FORMAT_XRGB8888);
  wl_buffer_add_listener(buffer, &buffer_listener, NULL);
  wl_shm_pool_destroy(pool);

  /* init xkb */
  xkb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

  /* initial draw & commit */
  redraw();
  commit();

  /* event loop */
  while (wl_display_dispatch(display) != -1) {
    /* all redraws happen in handle_key */
  }

  munmap(shm_data, shm_size);
  close(shm_fd);
  wl_display_disconnect(display);
  return 0;
}
