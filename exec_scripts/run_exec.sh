#!/bin/bash
NAME=$(date +"%m-%d-%y-%T")
HOSTFILE=$PWD/hostfile
DATASET=$PWD/$1
mkdir $DATASET
cd ..
lammps_a_app="./../lmp -in in.lj"
lammps_b_app="./../lmp -in in.spce"
lammps_c_app="./../lmp -in in.adp"
lammps_e_app="./../lmp -in in.rhodo"
cd build/bench
mpirun -n $2 --hostfile $HOSTFILE $lammps_a_app > $DATASET/$NAME-lammps-A.out 2> $DATASET/$NAME-lammps-A.err
mpirun -n $2 --hostfile $HOSTFILE $lammps_b_app > $DATASET/$NAME-lammps-B.out 2> $DATASET/$NAME-lammps-B.err
mpirun -n $2 --hostfile $HOSTFILE $lammps_c_app > $DATASET/$NAME-lammps-C.out 2> $DATASET/$NAME-lammps-C.err
mpirun -n $2 --hostfile $HOSTFILE $lammps_e_app > $DATASET/$NAME-lammps-e.out 2> $DATASET/$NAME-lammps-e.err
