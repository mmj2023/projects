# getting davinchi resolve to work on properly on linux (primarily arch linux)
make sure you have the opencl nvidia drivers installed!

``` sh
sudo mv /etc/OpenCL/vendors/intel.icd /etc/OpenCL/vendors/intel.icd.disabled # was nessecary to disable intel OpenCL driver for davinchi to work
```

``` sh
CUDA_VISIBLE_DEVICES=0 \
__NV_PRIME_RENDER_OFFLOAD=1 \
__GLX_VENDOR_LIBRARY_NAME=nvidia \
DRA_FORCE_CUDA=1 \
prime-run /opt/resolve/bin/resolve
```

``` sh
QT_AUTO_SCREEN_SCALE_FACTOR=1 QT_QPA_PLATFORM=xcb __NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia CUDA_VISIBLE_DEVICES=0 prime-run /opt/resolve/bin/resolve
```

## make it work on arch linux

``` sh
cd /opt/resolve/libs && sudo mkdir disabled-libraries && sudo mv libglib* libgio* libgmodule* disabled-libraries
```

## for checking the video audio codec
``` sh
ffprobe -v error -select_streams a -show_entries stream=codec_name,codec_long_name -of default=noprint_wrappers=1:nokey=1 yourvideo.extension
```
