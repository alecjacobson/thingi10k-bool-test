#include "../read_boolean_write.h"

#ifdef IGL_STATIC_LIBRARY
#undef IGL_STATIC_LIBRARY
#include <igl/copyleft/cgal/remesh_self_intersections.cpp>
template void igl::copyleft::cgal::remesh_self_intersections<Eigen::Matrix<CGAL::Epeck::FT, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<CGAL::Epeck::FT, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<CGAL::Epeck::FT, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, igl::copyleft::cgal::RemeshSelfIntersectionsParam const&, Eigen::PlainObjectBase<Eigen::Matrix<CGAL::Epeck::FT, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
#define IGL_STATIC_LIBRARY
#endif

int main(int argc, char *argv[])
{
  read_boolean_write(argv[1],std::string(argv[1])+".main.xml",argc>=3 && (std::string(argv[2]) == "verify"));
}
