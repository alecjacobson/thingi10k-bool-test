#include "../MatrixXE.h"
#include <igl/read_triangle_mesh.h>

template <typename T>
bool compare(
  const std::string & ch,
  const std::string & a1,
  const T & V1,
  const std::string & a2,
  const T & V2)
{
  bool res = true;
  if(V1.rows() != V2.rows() || V1.cols() != V2.cols()) { res = false; }
  for(int i = 0;i<V1.rows() && res;i++)
  {
    for(int j = 0;j<V1.cols();j++)
    {
      res = res && V1(i,j) == V2(i,j);
    }
  }
  std::cout<<"  "<<a1<<"."+ch+" "<<(res ? "=" : "≠")<<" "<<a2<<"."+ch<<std::endl;
  return res;
}

// Assumes V1=V2 and F1=F2
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>
bool compare_J(
  const std::string & ch,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const std::string & a1,
  const Eigen::VectorXi & J1,
  const std::string & a2,
  const Eigen::VectorXi & J2)
{
  typedef CGAL::Epeck Kernel;
  typedef CGAL::Plane_3<Kernel>    Plane_3;
  typedef CGAL::Point_3<Kernel>    Point_3;
  typedef CGAL::Triangle_3<Kernel>    Triangle_3;
  bool res = true;
  if(J1.size() != J2.size()){ res = false; }
  else
  {
    for(int j = 0;j<J1.size();j++)
    {
      const int j1 = J1(j);
      const int j2 = J2(j);
      if(j1 == j2){ continue; }
      // they don't match, are the co-planar?
      Triangle_3 t1(
          Point_3( 
            V(F(j1,0),0), 
            V(F(j1,0),1), 
            V(F(j1,0),2)),
          Point_3( 
            V(F(j1,1),0), 
            V(F(j1,1),1), 
            V(F(j1,1),2)),
          Point_3( 
            V(F(j1,2),0), 
            V(F(j1,2),1), 
            V(F(j1,2),2))
          );
      Triangle_3 t2(
          Point_3( 
            V(F(j2,0),0), 
            V(F(j2,0),1), 
            V(F(j2,0),2)),
          Point_3( 
            V(F(j2,1),0), 
            V(F(j2,1),1), 
            V(F(j2,1),2)),
          Point_3( 
            V(F(j2,2),0), 
            V(F(j2,2),1), 
            V(F(j2,2),2))
          );
      if( 
          t1.supporting_plane()  == t2.supporting_plane() ||
          t1.supporting_plane()  == t2.supporting_plane().opposite()
        )
      {
        continue;
      }
      res = false;
    }
  }

  std::cout<<"  "<<a1<<"."+ch+" "<<(res ? "=" : "≠")<<" "<<a2<<"."+ch<<std::endl;
  return res;
}

int main(int arc, char * argv[])
{
  
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::read_triangle_mesh(argv[1],V,F);
  MatrixXE V1;
  Eigen::MatrixXi F1;
  Eigen::VectorXi J1;
  igl::xml::read_mesh_from_xml(argv[2],V1,F1,J1);
  MatrixXE V2;
  Eigen::MatrixXi F2;
  Eigen::VectorXi J2;
  igl::xml::read_mesh_from_xml(argv[3],V2,F2,J2);
  if(!compare(  "V",    argv[2],V1,argv[3],V2)){ return EXIT_FAILURE; }
  if(!compare(  "F",    argv[2],F1,argv[3],F2)){ return EXIT_FAILURE; }
  if(!compare_J("J",V,F,argv[2],J1,argv[3],J2)){ return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
