Meshing Point Clouds using spherical parameterization
=====================================================
3D scanners produce sets of 3D data points, sampled  from  the  surface  of  a  3D  object.  These  points are  frequently  unorganized,  and  to  use  them  in  3D  applications  requires  computing  a  polygon  (usually  triangular) mesh  which  best  approximates  the  sampled  surface.  This means associating a connectivity structure with the point set.

This project addresses the problem of meshing point clouds using spherical parameterization method presented in Gotsman's paper here http://www.cs.technion.ac.il/~gotsman/AmendedPubl/Matthias/meshing.pdf.

Our approach relies on the automatic finding of the correspondences between the landmarks on the input laser scan and a template face mesh. Once we find the set of correspondences, we define a morphing function and apply it to the template mesh. As a result, the template mesh is morphed and represents the same facial features captured from the laser scan. Our method is completely automatic and does not rely neither on:

1) any prior anatomical knowledge about the position of the landmarks
or on
2) the morphology of the input face.


Usage
=====
library used in the porject:
- Boost, Qt
- OpenMesh
- CGAL
- Profiler 
- Qhull
- Eigen
- visual studio 

![Screenshot](https://github.com/NadineAB/MPC-Meshing_Point_Clouds/blob/master/Screen%20Shot%202018-08-07%20at%2000.22.54.png)
