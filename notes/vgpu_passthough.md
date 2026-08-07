# VGPU Passthrough
use the dkms module for intel gpu passthrough
## check active VGPU
lspci | grep -iE "VGA|Display|3D"

## get rid of any active VGPU
echo 0 | sudo tee /sys/bus/pci/devices/0000:00:02.0/sriov_numvfs

## make half of the VGPUs passthrough
echo 1 | sudo tee /sys/bus/pci/devices/0000:00:02.0/sriov_numvfs

## make all of the VGPUs passthrough
echo 7 | sudo tee /sys/bus/pci/devices/0000:00:02.0/sriov_numvfs

# check if any rules are active for reboots/reoccuring
grep -rnw "sriov_numvfs" /etc/udev/rules.d/ /etc/tmpfiles.d/

# launch looking glass and be sure to use looking glass or sunshine+moonlight for the best experience and for it to work
``` sh
~/Projects/LookingGlass/client/build/looking-glass-client -m KEY_RIGHTCTRL -f /dev/shm/looking-glass & disown
```
