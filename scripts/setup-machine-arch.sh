#!/bin/sh

if [ "$1" = "-quiet" ] ; then
    quiet=1
    shift 1
else
    quiet=0
fi

kernel=`uname -s`
os=`uname -o`
machine=`uname -m`

case "$os" in
    "FreeBSD") 
        os_name="freebsd"
    ;;
    "GNU/Linux") 
        os_name="linux"
    ;;
    "Linux") 
        os_name="linux"
    ;;
    *) 
        echo "Unsupported os $os"
        exit 1
    ;;
esac

case "$machine" in
    "arm") 
        cpu="arm"
        cpufamily="arm"
        archabi="armhf"
        case "$os_name" in
            "linux")
                syslibdir="arm-${os_name}-gnueabihf"
            ;;
            *) 
                syslibdir=""
            ;;
        esac
    ;;
    "armv7l")
        cpu="armv7l"
        cpufamily="arm"
        archabi="armhf"
        case "$os_name" in
            "linux")
                syslibdir="arm-${os_name}-gnueabihf"
            ;;
            *) 
                syslibdir=""
            ;;
        esac
    ;;
    "aarch64")
        cpu="aarch64"
        cpufamily="arm"
        archabi="arm64"
        case "$os_name" in
            "linux")
                syslibdir="aarch64-${os_name}-gnu"
            ;;
            *) 
                syslibdir=""
            ;;
        esac
    ;;
    "amd64")
        cpu="x86_64"
        cpufamily="x86"
        archabi="amd64"
        case "$os_name" in
            "linux")
                syslibdir="x86_64-${os_name}-gnu"
            ;;
            *) 
                syslibdir=""
            ;;
        esac
    ;;
    "x86_64")
        cpu="x86_64"
        cpufamily="x86"
        archabi="amd64"
        case "$os_name" in
            "linux")
                syslibdir="x86_64-${os_name}-gnu"
            ;;
            *) 
                syslibdir=""
            ;;
        esac
    ;;
    *) 
        echo "Unsupported machine $machine"
        exit 1
    ;;
esac

if [ $quiet -eq 0 ] ; then
    echo kernel $kernel
    echo os $os
    echo machine $machine
    echo os_name $os_name
    echo cpu $cpu
    echo cpufamily $cpufamily
    echo archabi $archabi
    echo syslibdir $syslibdir
fi

