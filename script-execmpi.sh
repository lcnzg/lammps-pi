DATASET=exp-results
rm -rf $DATASET
mkdir $DATASET

cd build
sudo perf stat -o ../$DATASET/perf-app-lammps.out ./lmp -var t 300 -echo screen -in lj/in.lj
/usr/bin/time -o ../$DATASET/time-app-lammps.out -v ./lmp -var t 300 -echo screen -in lj/in.lj
ltrace -o ../$DATASET/mpi-app-lammps.out ./lmp -var t 300 -echo screen -in lj/in.lj

cd ../$DATASET
cat mpi-app-lammps.out | grep "MPI" > mpi2-app-lammps.out
rm mpi-app-lammps.out
mv mpi2-app-lammps.out mpi-app-lammps.out

#mpirun -n 2 ./lmp -var t 300 -echo screen -in lj/in.lj
