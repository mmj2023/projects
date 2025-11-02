#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
  int fb = open("/dev/fb0", O_RDWR);
  if (fb < 0) {
    perror("open");
    return 1;
  }

  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

  if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
    perror("FBIOGET_VSCREENINFO");
    close(fb);
    return 1;
  }
  if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo)) {
    perror("FBIOGET_FSCREENINFO");
    close(fb);
    return 1;
  }

  size_t screensize = finfo.smem_len;
  uint8_t *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
  if (fbp == MAP_FAILED) {
    perror("mmap");
    close(fb);
    return 1;
  }

  // Fill with solid blue (assuming 32‑bit ARGB)
  for (size_t y = 0; y < vinfo.yres_virtual; y++) {
    for (size_t x = 0; x < vinfo.xres_virtual; x++) {
      size_t location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                        (y + vinfo.yoffset) * finfo.line_length;
      *(uint32_t *)(fbp + location) = 0xFF0000FF; // ARGB: opaque blue
    }
  }

  munmap(fbp, screensize);
  close(fb);
  return 0;
}
