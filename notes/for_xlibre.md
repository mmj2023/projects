looking for conflicting packages...
:: xlibre-xserver-25.0.0.16-1 and xorg-server-21.1.21-1 are in conflict. Remove xorg-server? [y/N] y
:: xlibre-xserver-25.0.0.16-1 and xorg-server-common-21.1.21-1 are in conflict. Remove xorg-server-common? [y/N] y
:: xlibre-input-libinput-1.5.1.0-1 and xf86-input-libinput-1.5.0-1 are in conflict. Remove xf86-input-libinput? [y/N] y
:: xlibre-xserver-devel-25.0.0.16-1 and xorg-server-devel-21.1.21-1 are in conflict. Remove xorg-server-devel? [y/N] y
:: xlibre-xserver-xephyr-25.0.0.16-1 and xorg-server-xephyr-21.1.21-1 are in conflict. Remove xorg-server-xephyr? [y/N] y
:: xlibre-xserver-xnest-25.0.0.16-1 and xorg-server-xnest-21.1.21-1 are in conflict. Remove xorg-server-xnest? [y/N] y
:: xlibre-xserver-xvfb-25.0.0.16-1 and xorg-server-xvfb-21.1.21-1 are in conflict. Remove xorg-server-xvfb? [y/N] y
:: xlibre-video-dummy-0.4.1.4-1 and xf86-video-dummy-0.4.1-2 are in conflict. Remove xf86-video-dummy? [y/N] y
:: xlibre-video-vesa-2.6.0.4-1 and xf86-video-vesa-2.6.0-2 are in conflict. Remove xf86-video-vesa? [y/N] y

It is required to replace all installed xf86* xorg driver and
    xorg-server* packages with their xlibre6* counterparts.
    If for example xf86-video-intel is installed this should be
    replaced with xlibre-xf86-video-intel.
