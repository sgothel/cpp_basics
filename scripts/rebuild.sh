#! /bin/sh

sdir=`dirname $(readlink -f $0)`
rootdir=`dirname $sdir`
bname=`basename $0 .sh`

. $rootdir/scripts/setup-machine-arch.sh

logfile=$rootdir/$bname-$os_name-$archabi.log
rm -f $logfile

CPU_COUNT=`getconf _NPROCESSORS_ONLN`

# run 'dpkg-reconfigure locales' enable 'en_US.UTF-8'
export LANG=en_US.UTF-8
export LC_MEASUREMENT=en_US.UTF-8

buildit() {
    echo rootdir $rootdir
    echo logfile $logfile
    echo CPU_COUNT $CPU_COUNT

    dist_dir="dist-$os_name-$archabi"
    build_dir="build-$os_name-$archabi"
    echo dist_dir $dist_dir
    echo build_dir $build_dir

    if [ -x /usr/bin/time ] ; then
        time_cmd="time"
        echo "time command available: ${time_cmd}"
    else 
        time_cmd=""
        echo "time command not available"
    fi

    cd $rootdir

    ${time_cmd} cmake --build ${build_dir} --parallel
    if [ $? -ne 0 ] ; then
        echo "REBUILD FAILURE $bname $os_name $archabi"
        return 1
    fi

    cd ${build_dir}
    make test
    if [ $? -ne 0 ] ; then
        echo "TEST FAILURE $bname $os_name $archabi"
        cd ${rootdir}
        return 1
    fi
    cd ${rootdir}
}

buildit 2>&1 | tee $logfile

