DESCRIPTION="mbed-edge"

LICENSE="Apache-2.0"
LIC_FILES_CHKSUM = "file://${WORKDIR}/git/LICENSE;md5=1dece7821bf3fd70fe1309eaa37d52a2"
SCRIPT_DIR = "${WORKDIR}/git/lib/mbed-cloud-client/update-client-hub/modules/pal-linux/scripts"

# Patches for quilt goes to files directory
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "git://git@github.com/ARMmbed/mbed-edge.git;protocol=ssh; \
           file://edge-core \
           file://0001-Set-variable-EVENT__HAVE_WAITPID_WITH_WNOWAIT_EXITCO.patch \
           file://mbed_cloud_dev_credentials.c \
           file://update_default_resources.c \
           file://edge-core.service \
           file://toolchain.cmake "

SRCREV = "0.8.0"

RDEPENDS_${PN} = " procps start-stop-daemon bash tar bzip2 xz util-linux"

# Installed packages
PACKAGES = "${PN} ${PN}-dbg"
FILES_${PN} += "/opt \
                /opt/arm \
                /opt/arm/edge-core"

FILES_${PN}-dbg += "/opt/arm/.debug \
                    /usr/src/debug/mbed-edge"

inherit systemd

SYSTEMD_SERVICE_${PN} = "edge-core.service"

S = "${WORKDIR}/git"

EXTRA_OECMAKE += "-DTARGET_CONFIG_ROOT=${WORKDIR} ${MBED_EDGE_CUSTOM_CMAKE_ARGUMENTS} -DDEVELOPER_MODE=ON -DTRACE_LEVEL=DEBUG"
inherit cmake

do_configure_prepend() {
    cd ${S}
    git submodule init
    git submodule update
    cp ${WORKDIR}/mbed_cloud_dev_credentials.c ${WORKDIR}/git/config/mbed_cloud_dev_credentials.c
    cp ${WORKDIR}/update_default_resources.c ${WORKDIR}/git/config/update_default_resources.c
    cd ${WORKDIR}/build
}

do_install() {
    install -d "${D}/opt/arm"
    install "${WORKDIR}/build/bin/edge-core" "${D}/opt/arm"

    install -d "${D}${systemd_unitdir}/system/"
    install -m 0644 "${WORKDIR}/edge-core.service" "${D}${systemd_unitdir}/system/"
}

# Add a logrotate config files
MBL_LOGROTATE_CONFIG_LOG_NAMES = "edge-core"

MBL_LOGROTATE_CONFIG_LOG_PATH[edge-core] = "/var/log/edge-core.log"
MBL_LOGROTATE_CONFIG_SIZE[edge-core] ?= "1M"
MBL_LOGROTATE_CONFIG_ROTATE[edge-core] ?= "4"
MBL_LOGROTATE_CONFIG_POSTROTATE[edge-core] = "/usr/bin/killall -HUP edge-core"
