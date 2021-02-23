/***************************************************************************
                                   zbl.cpp
                             -------------------
                               Trung Dac Nguyen

  Class for acceleration of the zbl pair style.

 __________________________________________________________________________
    This file is part of the LAMMPS Accelerator Library (LAMMPS_AL)
 __________________________________________________________________________

    begin                :
    email                : ndactrung@gmail.com
 ***************************************************************************/

#ifdef USE_OPENCL
#include "zbl_cl.h"
#elif defined(USE_CUDART)
const char *zbl=0;
#else
#include "zbl_cubin.h"
#endif

#include "lal_zbl.h"
#include <cassert>
namespace LAMMPS_AL {
#define ZBLT ZBL<numtyp, acctyp>

extern Device<PRECISION,ACC_PRECISION> device;

template <class numtyp, class acctyp>
ZBLT::ZBL() : BaseAtomic<numtyp,acctyp>(), _allocated(false) {
}

template <class numtyp, class acctyp>
ZBLT::~ZBL() {
  clear();
}

template <class numtyp, class acctyp>
int ZBLT::bytes_per_atom(const int max_nbors) const {
  return this->bytes_per_atom_atomic(max_nbors);
}

template <class numtyp, class acctyp>
int ZBLT::init(const int ntypes, double **host_cutsq,
               double **host_sw1, double **host_sw2,
               double **host_sw3, double **host_sw4,
               double **host_sw5,
               double **host_d1a, double **host_d2a,
               double **host_d3a, double **host_d4a,
               double **host_zze, double cut_globalsq,
               double cut_innersq, double cut_inner,
               const int nlocal, const int nall, const int max_nbors,
               const int maxspecial, const double cell_size,
               const double gpu_split, FILE *_screen) {
  int success;
  success=this->init_atomic(nlocal,nall,max_nbors,maxspecial,cell_size,gpu_split,
                            _screen,zbl,"k_zbl");
  if (success!=0)
    return success;

  // If atom type constants fit in shared memory use fast kernel
  int lj_types=ntypes;
  shared_types=false;
  int max_shared_types=this->device->max_shared_types();
  if (lj_types<=max_shared_types && this->_block_size>=max_shared_types) {
    lj_types=max_shared_types;
    shared_types=true;
  }
  _lj_types=lj_types;

  // Allocate a host write buffer for data initialization
  UCL_H_Vec<numtyp> host_write(lj_types*lj_types*32,*(this->ucl_device),
                               UCL_WRITE_ONLY);

  for (int i=0; i<lj_types*lj_types; i++)
    host_write[i]=0.0;

  coeff1.alloc(lj_types*lj_types,*(this->ucl_device),UCL_READ_ONLY);
  this->atom->type_pack4(ntypes,lj_types,coeff1,host_write,host_sw1,host_sw2,
                                           host_zze, host_cutsq);

  coeff2.alloc(lj_types*lj_types,*(this->ucl_device),UCL_READ_ONLY);
  this->atom->type_pack4(ntypes,lj_types,coeff2,host_write,host_d1a,host_d2a,
                                           host_d3a,host_d4a);

  coeff3.alloc(lj_types*lj_types,*(this->ucl_device),UCL_READ_ONLY);
  this->atom->type_pack4(ntypes,lj_types,coeff3,host_write,host_sw3,host_sw4,host_sw5);

  _cut_globalsq = cut_globalsq;
  _cut_innersq = cut_innersq;
  _cut_inner = cut_inner;

  _allocated=true;
  this->_max_bytes=coeff1.row_bytes()+coeff2.row_bytes()+coeff3.row_bytes();
  return 0;
}

template <class numtyp, class acctyp>
void ZBLT::clear() {
  if (!_allocated)
    return;
  _allocated=false;

  coeff1.clear();
  coeff2.clear();
  coeff3.clear();
  this->clear_atomic();
}

template <class numtyp, class acctyp>
double ZBLT::host_memory_usage() const {
  return this->host_memory_usage_atomic()+sizeof(ZBL<numtyp,acctyp>);
}

// ---------------------------------------------------------------------------
// Calculate energies, forces, and torques
// ---------------------------------------------------------------------------
template <class numtyp, class acctyp>
int ZBLT::loop(const int eflag, const int vflag) {
  // Compute the block size and grid size to keep all cores busy
  const int BX=this->block_size();
  int GX=static_cast<int>(ceil(static_cast<double>(this->ans->inum())/
                               (BX/this->_threads_per_atom)));

  int ainum=this->ans->inum();
  int nbor_pitch=this->nbor->nbor_pitch();
  this->time_pair.start();
  if (shared_types) {
    this->k_pair_sel->set_size(GX,BX);
    this->k_pair_sel->run(&this->atom->x, &coeff1, &coeff2, &coeff3,
                          &_cut_globalsq, &_cut_innersq, &_cut_inner,
                          &this->nbor->dev_nbor, &this->_nbor_data->begin(),
                          &this->ans->force, &this->ans->engv, &eflag, &vflag,
                          &ainum, &nbor_pitch, &this->_threads_per_atom);
  } else {
    this->k_pair.set_size(GX,BX);
    this->k_pair.run(&this->atom->x, &coeff1, &coeff2, &coeff3,
                     &_cut_globalsq, &_cut_innersq, &_cut_inner, &_lj_types,
                     &this->nbor->dev_nbor, &this->_nbor_data->begin(),
                     &this->ans->force, &this->ans->engv, &eflag, &vflag,
                     &ainum, &nbor_pitch, &this->_threads_per_atom);
  }
  this->time_pair.stop();
  return GX;
}

template class ZBL<PRECISION,ACC_PRECISION>;
}
