# ----------------------------
# Makefile Options
# ----------------------------

NAME = GAY
ICON = icon.png
DESCRIPTION = "Am I gay quiz?"
COMPRESSED = YES
ARCHIVED = YES
COMPRESSED_MODE = zx0
LTO = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
