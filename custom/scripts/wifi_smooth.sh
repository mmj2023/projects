#!/bin/bash
IFACE=$1
STATUS=$2

if [ "$IFACE" = "wlan0" ] && [ "$STATUS" = "up" ]; then
    iw dev wlan0 set power_save off
    nmcli connection modify mywifi wifi.band a
fi

# other tweak
sudo tc qdisc add dev wlan0 root fq_codel

git clone https://github.com/oracle/bpftune
cd bpftune
make
sudo make install
sudo systemctl enable --now bpftune

echo "net.core.default_qdisc=fq_codel" | sudo tee /etc/sysctl.d/10-fq_codel.conf
echo "net.ipv4.tcp_congestion_control=bbr" | sudo tee /etc/sysctl.d/10-bbr.conf
sudo sysctl --system

sudo mkdir -p /etc/NetworkManager/conf.d
echo -e "[connection]\nwifi.powersave = 2" | \
  sudo tee /etc/NetworkManager/conf.d/wifi-powersave.conf
sudo systemctl restart NetworkManager

