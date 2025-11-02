#!/usr/bin/env bash
cc main.c -o main -g -Werror -DWLR_USE_UNSTABLE \
  -I$HOME/projects/c_Projects/wayland/wlroots/include \
  -I/usr/include/pixman-1 \
  -L$HOME/projects/c_Projects/wayland/wlroots/build \
  -lwlroots-0.20 -lwayland-server -lxkbcommon -lpixman-1 -ldrm -linput -lseat -ldisplay-info -lm
# Run the program
# LD_LIBRARY_PATH=~/projects/c_Projects/wayland/wlroots/build/:$LD_LIBRARY_PATH ./main
