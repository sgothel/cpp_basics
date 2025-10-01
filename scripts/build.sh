#! /bin/sh

sdir=`dirname $(readlink -f $0)`
rootdir=`dirname $sdir`
bname=`basename $0 .sh`

. $rootdir/scripts/setup-machine-arch.sh

logfile=$rootdir/$bname-$os_name-$archabi.log
rm -f $logfile

CPU_COUNT=`getconf _NPROCESSORS_ONLN`

# run as root 'dpkg-reconfigure locales' enable 'en_US.UTF-8'
# perhaps run as root 'update-locale LC_MEASUREMENT=en_US.UTF-8 LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8'
export LC_MEASUREMENT=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

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

    cd ${rootdir}
    rm -rf $dist_dir
    mkdir -p $dist_dir
    rm -rf $build_dir
    mkdir -p $build_dir

    # CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++"
    CLANG_ARGS="-DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_CLANG_TIDY=/usr/bin/clang-tidy;-p;$rootdir/$build_dir"

    ${time_cmd} cmake -DCMAKE_INSTALL_PREFIX=$rootdir/$dist_dir $CLANG_ARGS -B ${build_dir}
    if [ $? -ne 0 ] ; then
        echo "CONFIG FAILURE $bname $os_name $archabi"
        return 1
    fi

    ${time_cmd} cmake --build ${build_dir} --parallel
    if [ $? -ne 0 ] ; then
        echo "BUILD FAILURE $bname $os_name $archabi"
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

    ${time_cmd} cmake --install ${build_dir}
    if [ $? -ne 0 ] ; then
        echo "INSTALL FAILURE $bname $os_name $archabi"
        return 1
    fi
}

buildit 2>&1 | tee $logfile
