set(WIN_PACKAGES ASPHERE BODY CLASS2 COLLOID COMPRESS CORESHELL DIPOLE
                 GRANULAR KSPACE MANYBODY MC MISC MOLECULE OPT PERI POEMS
                 PYTHON QEQ REAX REPLICA RIGID SHOCK SNAP SRD VORONOI
                 USER-ATC USER-AWPMD USER-BOCS USER-CGDNA USER-CGSDK USER-COLVARS
                 USER-DIFFRACTION USER-DPD USER-DRUDE USER-EFF USER-FEP USER-INTEL
                 USER-MANIFOLD USER-MEAMC USER-MESO USER-MISC USER-MOFFF USER-MOLFILE
                 USER-OMP USER-PHONON USER-QTB USER-REAXC USER-SDPD USER-SMD
                 USER-SMTBQ USER-SPH USER-TALLY USER-UEF)

foreach(PKG ${WIN_PACKAGES})
  set(PKG_${PKG} ON CACHE BOOL "" FORCE)
endforeach()
set(DOWNLOAD_VORO ON CACHE BOOL "" FORCE)
set(DOWNLOAD_EIGEN3 ON CACHE BOOL "" FORCE)
set(LAMMPS_MEMALIGN "0" CACHE STRING "" FORCE)
set(INTEL_LRT_MODE "none" CACHE STRING "" FORCE)
