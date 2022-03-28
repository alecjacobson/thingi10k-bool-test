
# Compile 

    cd main
    mkdir -p build
    cd build
    rcmake ../ -DLIBIGL_USE_STATIC_LIBRARY=ON
    make -j64
    # assumes "home" libigl is compiled on the parallel-cgal branch
    cd ../../parallel-cgal
    mkdir -p build
    cd build
    rcmake ../
    make -j64

# Run
