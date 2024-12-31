#--------------------------------------
#
# TI-84+ CE Gamepad - makefile
# By TIny_Hacker
# Copyright 2024
# License: GPL-3.0
#
#--------------------------------------

NAME = GAMEPAD
ICON = icon.png
DESCRIPTION = "TI-84+ CE as a USB gamepad"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

final:
	make
	convbin -r -e zx0 -k 8xp-compressed -u -n $(NAME) -i bin/$(NAME).bin -o bin/$(NAME).8xp
