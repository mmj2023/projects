#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main() {
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);

    drmModeRes *res = drmModeGetResources(fd);
    drmModeConnector *conn = NULL;
    for (int i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(fd, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED) break;
        drmModeFreeConnector(conn);
        conn = NULL;
    }

    drmModeModeInfo mode = conn->modes[0]; // pick first mode
    uint32_t crtc_id = res->crtcs[0];

    struct drm_mode_create_dumb creq = {0};
    creq.width  = mode.hdisplay;
    creq.height = mode.vdisplay;
    creq.bpp    = 32;
    drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq);

    uint32_t handle = creq.handle;
    uint32_t pitch  = creq.pitch;
    uint32_t size   = creq.size;

    uint32_t fb;
    drmModeAddFB(fd, creq.width, creq.height, 24, 32, pitch, handle, &fb);

    struct drm_mode_map_dumb mreq = {0};
    mreq.handle = handle;
    drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);

    uint32_t *map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);

    // Fill with solid red (XRGB8888: 0x00RRGGBB)
    for (uint32_t y = 0; y < creq.height; y++) {
        for (uint32_t x = 0; x < creq.width; x++) {
            map[y * (pitch / 4) + x] = 0x00FF0000; // red
        }
    }

    drmModeSetCrtc(fd, crtc_id, fb, 0, 0, &conn->connector_id, 1, &mode);

    sleep(5); // keep it visible for 5 seconds

    return 0;
}
