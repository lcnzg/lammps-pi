mkdir build
cd build
cmake ../cmake/ -DBUILD_MPI=on -DMPI_C_COMPILER=mpicc -DMPI_CXX_COMPILER=mpic++
make -j2
cp ../bench_lj.tar.gz .
tar -xf bench_lj.tar.gz
