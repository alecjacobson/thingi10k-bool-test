
# Compile 

    cd main
    mkdir -p build
    cd build
    rcmake ../ -DLIBIGL_USE_STATIC_LIBRARY=ON
    make -j56
    cd ../../parallel-cgal
    mkdir -p build
    cd build
    rcmake ../
    make -j56

# Run

## Self-unions

Computes self-union of all pwn meshes, stores the result in .xml files and compares them, writing results into .result.txt files

    # Using 1 subprocess
    ./test_all.sh -j1 /path/to/Thingi10k/raw_meshes
    
## Float rounding

Read in the .xml exact results, check there are no self-intersections, round to float, check how many new intersectionts, writes to .verify.txt
    
    # Using 1 subprocess
    ./verify_all.sh -j1 /path/to/Thingi10k/raw_meshes
