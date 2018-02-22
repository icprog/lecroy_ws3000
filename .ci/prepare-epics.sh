#!/bin/sh
set -e -x

# Build Base for use with https://travis-ci.org
#
# Set environment variables
# BASE= 3.15 or 3.16  (VCS branch)
# STATIC=  static or shared

die() {
  echo "$1" >&2
  exit 1
}

[ "$BASE" ] || exit 0

CDIR="$HOME/.cache/base-$BASE-$STATIC"
EPICS_BASE="$CDIR/base"

if [ ! -e "$CDIR/built" ]
then
  install -d "$CDIR"
  ( cd "$CDIR" && git clone --depth 50 --branch $BASE https://github.com/epics-base/epics-base.git base )

  EPICS_HOST_ARCH=`sh $EPICS_BASE/startup/EpicsHostArch`

  case "$STATIC" in
  static)
    cat << EOF >> "$EPICS_BASE/configure/CONFIG_SITE"
SHARED_LIBRARIES=NO
STATIC_BUILD=YES
EOF
    ;;
  *) ;;
  esac

  case "$CMPLR" in
  clang)
    echo "Host compiler is clang"
    cat << EOF >> "$EPICS_BASE/configure/os/CONFIG_SITE.Common.$EPICS_HOST_ARCH"
GNU         = NO
CMPLR_CLASS = clang
CC          = clang
CCC         = clang++
EOF

    # hack
    sed -i -e 's/CMPLR_CLASS = gcc/CMPLR_CLASS = clang/' "$EPICS_BASE/configure/CONFIG.gnuCommon"

    clang --version
    ;;
  *)
    echo "Host compiler is default"
    gcc --version
    ;;
  esac

  make -C "$EPICS_BASE" -j2

  touch "$CDIR/built"
fi


EPICS_HOST_ARCH=`sh $EPICS_BASE/startup/EpicsHostArch`

CDIR="$HOME/.cache/asyn"

if [ ! -e "$CDIR/built" ]
then
  install -d "$CDIR"

  ( cd "$CDIR" && git clone --depth 50 --branch master https://github.com/epics-modules/asyn.git asyn )

  ASYN="$CDIR/asyn"

  echo "EPICS_BASE=$EPICS_BASE" >  ${ASYN}/configure/RELEASE
  echo "CHECK_RELEASE = YES"    >  ${ASYN}/configure/CONFIG_SITE
  echo "LINUX_GPIB=NO"          >> ${ASYN}/configure/CONFIG_SITE
  echo "DRV_USBTMC=YES"         >> ${ASYN}/configure/CONFIG_SITE

  make -C "$ASYN" -j2

  touch "$CDIR/built"
fi



EPICS_HOST_ARCH=`sh $EPICS_BASE/startup/EpicsHostArch`

CDIR="$HOME/.cache/stream"

if [ ! -e "$CDIR/built" ]
then
  install -d "$CDIR"

  ( cd "$CDIR" && git clone --depth 50 --branch master https://github.com/epics-modules/stream.git stream )

  STREAM="$CDIR/stream"

  echo "ASYN=$ASYN"             >  ${STREAM}/configure/RELEASE
  echo "EPICS_BASE=$EPICS_BASE" >  ${STREAM}/configure/RELEASE
 
  make -C "$STREAM"

  touch "$CDIR/built"
fi




EPICS_HOST_ARCH=`sh $EPICS_BASE/startup/EpicsHostArch`

cat << EOF > configure/RELEASE.local
EPICS_BASE=$EPICS_BASE
ASYN=${ASYN}
STREAM=${STREAM}
EOF
