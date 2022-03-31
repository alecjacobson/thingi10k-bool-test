#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <igl/read_triangle_mesh.h>
#include <igl/copyleft/cgal/remesh_self_intersections.h>
#include <igl/copyleft/cgal/assign.h>
typedef CGAL::Epeck::FT EScalar;
typedef Eigen::Matrix<EScalar,Eigen::Dynamic,3> MatrixXE;

void foobar(std::string g){}

int main(int argc, char * argv[])
{
  using namespace igl::copyleft::cgal;
  Eigen::MatrixXi F;
  MatrixXE Ve;
  {
    Eigen::MatrixXd Vd;
    igl::read_triangle_mesh(argv[1],Vd,F);
    igl::copyleft::cgal::assign(Vd,Ve);
  }

  MatrixXE Vsu;
  Eigen::MatrixXi Fsu;
  // resolve intersections
  {
    printf(" Ve: %d %d\n",(int)Ve.rows(),(int)Ve.cols());
    printf("  F: %d %d\n",(int)F.rows(),(int)F.cols());
    RemeshSelfIntersectionsParam params(false,false,true);
    Eigen::MatrixXi IF;
    Eigen::VectorXi Jsu,IM;
    remesh_self_intersections(Ve,F,params,Vsu,Fsu,IF,Jsu,IM);
    printf("Vsu: %d %d\n",(int)Vsu.rows(),(int)Vsu.cols());
    printf("Fsu: %d %d\n",(int)Fsu.rows(),(int)Fsu.cols());
  }
  // detect intersections
  {
    RemeshSelfIntersectionsParam params(true,false,false);
    MatrixXE _VV;
    Eigen::MatrixXi _FF, IF;
    Eigen::VectorXi _J,_IM;
    remesh_self_intersections(Vsu,Fsu,params,_VV,_FF,IF,_J,_IM);
    printf(" IF: %d\n",(int)IF.rows());
    std::cout<<IF<<std::endl;
  }
  // detect intersections
  {
    RemeshSelfIntersectionsParam params(false,false,false);
    MatrixXE _VV;
    Eigen::MatrixXi _FF, IF;
    Eigen::VectorXi _J,_IM;
    remesh_self_intersections(Vsu,Fsu,params,_VV,_FF,IF,_J,_IM);
    printf(" IF: %d\n",(int)IF.rows());
    std::cout<<IF<<std::endl;
  }
}
