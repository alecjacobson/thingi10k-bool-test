
#include <igl/copyleft/cgal/RemeshSelfIntersectionsParam.h>
#include <igl/copyleft/cgal/remesh_self_intersections.h>
template <typename T>
int detect(
  const T & V,
  const Eigen::MatrixXi & F)
{
  using namespace igl::copyleft::cgal;
  RemeshSelfIntersectionsParam params(true,false,false);
  T _VV;
  Eigen::MatrixXi _FF, IF;
  Eigen::VectorXi _J,_IM;
  remesh_self_intersections(V,F,params,_VV,_FF,IF,_J,_IM);
  return IF.rows();
}


