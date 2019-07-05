/*
 *_________________________________________________________________________*
 *      POEMS: PARALLELIZABLE OPEN SOURCE EFFICIENT MULTIBODY SOFTWARE     *
 *      DESCRIPTION: SEE READ-ME                                           *
 *      FILE NAME: rowmatrix.h                                             *
 *      AUTHORS: See Author List                                           * 
 *      GRANTS: See Grants List                                            *
 *      COPYRIGHT: (C) 2005 by Authors as listed in Author's List          *
 *      LICENSE: Please see License Agreement                              *
 *      DOWNLOAD: Free at www.rpi.edu/~anderk5                             *
 *      ADMINISTRATOR: Prof. Kurt Anderson                                 *
 *                     Computational Dynamics Lab                          *
 *                     Rensselaer Polytechnic Institute                    *
 *                     110 8th St. Troy NY 12180                           * 
 *      CONTACT:        anderk5@rpi.edu                                    *
 *_________________________________________________________________________*/


#ifndef ROWMATRIX_H
#define ROWMATRIX_H

#include <iostream>

#include "virtualrowmatrix.h"
#include "virtualmatrix.h"

namespace POEMS {
class RowMatrix : public VirtualRowMatrix  {
  double* elements;
public:
  RowMatrix();
  ~RowMatrix();
  RowMatrix(const RowMatrix& A);  // copy constructor
  RowMatrix(const VirtualRowMatrix& A);  // copy constructor
  RowMatrix(const VirtualMatrix& A);  // copy constructor
  RowMatrix(int n);  // size constructor

  void Const(double value);
  double& operator_1int (int i); // array access
  double Get_1int(int i) const;
  void Set_1int(int i, double value);
  double BasicGet_1int(int i) const;
  void BasicSet_1int(int i, double value);
  void BasicIncrement_1int(int i, double value);
  void Dim(int n);
  MatrixType GetType() const;
  std::istream& ReadData(std::istream& c);
  std::ostream& WriteData(std::ostream& c) const;

  void AssignVM(const VirtualMatrix& A);
  RowMatrix& operator=(const RowMatrix& A); // assignment operator
  RowMatrix& operator=(const VirtualRowMatrix& A); // overloaded =
  RowMatrix& operator=(const VirtualMatrix& A); // overloaded =
  RowMatrix& operator*=(double b);
};
}
#endif
