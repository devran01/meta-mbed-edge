require mbed-edge.inc

PROVIDES += " virtual/mbed-edge virtual/mbed-edge-dbg "
RPROVIDES_${PN} += " virtual/mbed-edge virtual/mbed-edge-dbg "

FILESEXTRAPATHS_prepend := "${THISDIR}/files/mbed-rpi3:"
SRC_URI += "file://target.cmake \
            file://sotp_fs_rpi3_yocto.h"
