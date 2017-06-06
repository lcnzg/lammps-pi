/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
   -----------------------------------------------------------------------

   This file is a part of the USER-MANIFOLD package.

   This package allows LAMMPS to perform MD simulations of particles
   constrained on a manifold (i.e., a 2D subspace of the 3D simulation
   box). It achieves this using the RATTLE constraint algorithm applied
   to single-particle constraint functions g(xi,yi,zi) = 0 and their
   derivative (i.e. the normal of the manifold) n = grad(g).

   It is very easy to add your own manifolds to the current zoo
   (we now have sphere, a dendritic spine approximation, a 2D plane (for
   testing purposes) and a wave-y plane.
   See the README file for more info.

   Stefan Paquay, s.paquay@tue.nl
   Applied Physics/Theory of Polymers and Soft Matter,
   Eindhoven University of Technology (TU/e), The Netherlands

   Thanks to Remy Kusters at TU/e for testing.

   This software is distributed under the GNU General Public License.

------------------------------------------------------------------------- */

#ifndef LMP_MANIFOLD_GAUSSIAN_BUMP_H
#define LMP_MANIFOLD_GAUSSIAN_BUMP_H

#include "manifold.h"

namespace LAMMPS_NS {

namespace user_manifold {

  // A Gaussian bump with a smoothed decay to flat 2D.
  class manifold_gaussian_bump : public manifold {
   public:
    enum { NPARAMS = 4 };
    manifold_gaussian_bump(class LAMMPS*, int, char **) : manifold(lmp) {}
    virtual ~manifold_gaussian_bump(){}

    virtual double g( const double * );
    virtual void   n( const double *, double * );

    // Variant of g that computes n at the same time.
    virtual double g_and_n( const double *x, double *nn );

    static const char* type(){ return "gaussian_bump"; }
    virtual const char *id() { return type(); }

    virtual int nparams(){ return NPARAMS; }
    virtual void post_param_init();
   private:
    // Some private constants:
    double aa, bb, cc, dd, AA, ll, ll2, f_at_rc, fp_at_rc;
    double rc1, rc2, rc12, rc22, dr, inv_dr;

    double gaussian_bump    ( double );
    double gaussian_bump_x2 ( double );
    double gaussian_bump_der( double );

  };
}

}


#endif // LMP_MANIFOLD_GAUSSIAN_BUMP_H
