#!/bin/sh

 procs=1
 if [ $# -gt 0 ]; then procs="$1"; fi

 echo "Using $procs threads."

 export CORIOLIS_TOP=/asim/coriolis2
 export MANGROVE_TOP=/asim/mangrove

 ${HOME}/mangrove/src/bootstrap/buildCaiman.py \
     --root=${HOME}/checkout \
     --project=mangrove      \
     --project=caiman        \
     --make="-j${procs} install"
