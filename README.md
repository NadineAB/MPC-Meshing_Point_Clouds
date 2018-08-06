Meshing Point Clouds using spherical parameterization
=====================================================
3D scanners produce sets of 3D data points, sampled  from  the  surface  of  a  3D  object.  These  points are  frequently  unorganized,  and  to  use  them  in  3D  applications  requires  computing  a  polygon  (usually  triangular) mesh  which  best  approximates  the  sampled  surface.  This means associating a connectivity structure with the point set

This project addresses the problem of meshing point clouds using spherical parameterization presented in Gotsman's paper here http://www.cs.technion.ac.il/~gotsman/AmendedPubl/Matthias/meshing.pdf

library used in the porject:
- Boost, Qt
- OpenMesh
- CGAL
