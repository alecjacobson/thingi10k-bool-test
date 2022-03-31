#include "MatrixXE.h"
#include "detect.h"
#include <igl/read_triangle_mesh.h>
#include <igl/copyleft/cgal/assign.h>
void read_snap(const std::string & filename)
{
  MatrixXE Ve;
  Eigen::MatrixXi F;
  {
    Eigen::VectorXi J;
    igl::xml::read_mesh_from_xml(filename,Ve,F,J);
  }
  printf("%d\n",detect(Ve,F));

  Eigen::MatrixXd Vd;
  igl::copyleft::cgal::assign(Ve,Vd);
  printf("%d\n",detect(Vd,F));
}

