/*
 *_________________________________________________________________________*
 *      POEMS: PARALLELIZABLE OPEN SOURCE EFFICIENT MULTIBODY SOFTWARE     *
 *      DESCRIPTION: SEE READ-ME                                           *
 *      FILE NAME: rigidbody.cpp                                           *
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

#include "rigidbody.h"
#include "mat3x3.h"
#include "virtualmatrix.h"

using namespace std;

RigidBody::RigidBody(){
}
RigidBody::~RigidBody(){
}

BodyType RigidBody::GetType(){
  return RIGIDBODY;
}

bool RigidBody::ReadInBodyData(istream& in){
  in >> mass >> inertia;
  return true;
}

void RigidBody::WriteOutBodyData(ostream& out){
  out << mass << ' ' << inertia;
}
