DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
DIR_BUILD=$DIR_TOP/build
DIR_INST=$DIR_TOP/inst

#source /project/ptgroup/spinquest/this-e1039.sh
#source /exp/seaquest/app/software/osg/software/e1039/this-e1039.sh
#source /exp/seaquest/app/software/osg/users/$USER/e1039/core/this-e1039.sh
export LD_LIBRARY_PATH=$DIR_INST/lib:$LD_LIBRARY_PATH

module purge
module load miniforge
module load gcc/11.4.0
module load openmpi/4.1.4
module load root/6.32.06
module load intel-compilers/2023.1.0
module load impi/2021.9.0
module load python/3.11.4

function cmake-this {
    if [ -e $DIR_BUILD ] ; then
        echo "Clean up the build directory..."
        \rm -rf $DIR_BUILD
    fi  
    if [ -e $DIR_INST ] ; then
        echo "Clean up the install directory..."
        \rm -rf $DIR_INST
    fi  
    mkdir -p $DIR_BUILD
    echo "Run cmake..."
    ( cd $DIR_BUILD && cmake -DCMAKE_INSTALL_PREFIX=$DIR_INST $DIR_TOP/src )
    local RET=$?
    if [ $RET -eq 0 ] ; then
        echo "OK.  Run 'make-this' at any time/directory."
    else
        echo "NG.  Need a fix."
    fi  
    return $RET
}

function make-this {
    if [ ! -e $DIR_BUILD/Makefile ] ; then
        echo "The build directory is not ready.  Complete 'cmake-this' first."
        return 1
    fi  
    ( cd $DIR_BUILD && make install )
    return $?
}

function cmake-ana-sim-dst {
    echo "!!WARNING!!  This function is obsolete."
    echo "Please execute 'cmake-this' and then 'make-this' instead."
}

##
## Main
##

if [ ${HOSTNAME:0:13} != 'spinquestgpvm' ] ; then
    echo "!!CAUTION!!"
    echo "This AnaSimDst package does not support your computer ($HOSTNAME)."
    echo "It might not run properly even if you follow 'README.md'."
    echo
fi

echo "Two commands for source build:"
echo "  cmake-this"
echo "  make-this"
echo
echo "These commands can be executed in any directory."
echo "All built files are always created under $DIR_BUILD."
echo
echo "You need execute 'make-this'  when you modify source files."
echo "You need execute 'cmake-this' when you add/delete source files."
