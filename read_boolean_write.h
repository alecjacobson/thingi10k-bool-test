#include "MatrixXE.h"
#include "detect.h"
#include <igl/read_triangle_mesh.h>
#include <igl/get_seconds.h>
#include <igl/remove_duplicate_vertices.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/copyleft/cgal/assign.h>
#include <igl/xml/serialize_xml.h>
#include <iostream>
#include <string>

inline void canonicalize(Eigen::MatrixXi & F)
{
  for(int f = 0;f<F.rows();f++)
  {
    if(       F(f,0) < F(f,1) && F(f,0) < F(f,2))
    {
      // no-op
    }else if( F(f,1) < F(f,2) && F(f,1) < F(f,0))
    {
      const Eigen::RowVector3i new_row( F(f,1), F(f,2), F(f,0) );
      F.row(f) << new_row;
    }else if( F(f,2) < F(f,0) && F(f,2) < F(f,1))
    {
      const Eigen::RowVector3i new_row( F(f,2), F(f,0), F(f,1) );
      F.row(f) << new_row;
    }
  }
}


inline void read_boolean_write(
  const std::string & input_filename,
  const std::string & output_filename,
  const bool verify)
{
  Eigen::MatrixXd Vd;
  Eigen::MatrixXi F;
  igl::read_triangle_mesh(input_filename,Vd,F);
  {
    Eigen::VectorXi _1,_2;
    igl::remove_duplicate_vertices( 
      Eigen::MatrixXd(Vd),Eigen::MatrixXi(F),0,Vd,_1,_2,F);
  }

  MatrixXE Ve,Vsu;
  igl::copyleft::cgal::assign(Vd,Ve);
  Eigen::MatrixXi Fsu;
  Eigen::VectorXi Jsu;
  const double t0 = igl::get_seconds();
  const bool mb_success = igl::copyleft::cgal::mesh_boolean(
    Ve,F,MatrixXE(),Eigen::MatrixXi(),igl::MESH_BOOLEAN_TYPE_UNION,
    Vsu,Fsu,Jsu);
  const double t = igl::get_seconds()-t0;

  if(verify)
  {
    igl::copyleft::cgal::RemeshSelfIntersectionsParam params(false,false,true);
    Eigen::MatrixXi IF;
    Eigen::VectorXi IM;
    igl::copyleft::cgal::remesh_self_intersections(
      Ve,F,params,Vsu,Fsu,IF,Jsu,IM);
    printf("#IF: %d\n",detect(Vsu,Fsu));
  }

  igl::xml::serialize_xml(t,"t",output_filename,false,true);
  canonicalize(Fsu);
  igl::xml::append_mesh_to_xml(output_filename,Vsu,Fsu,Jsu);
#ifndef NDEBUG
  {
    MatrixXE rVsu;
    Eigen::MatrixXi rFsu;
    Eigen::VectorXi rJsu;
    igl::xml::read_mesh_from_xml(output_filename,rVsu,rFsu,rJsu);
    std::cout<<"Vsu "<<(Vsu.isApprox(rVsu,0) ? "=" : "≠")<<" rVsu"<<std::endl;
    std::cout<<"Fsu "<<(Fsu.isApprox(rFsu,0) ? "=" : "≠")<<" rFsu"<<std::endl;
    std::cout<<"Jsu "<<(Jsu.isApprox(rJsu,0) ? "=" : "≠")<<" rJsu"<<std::endl;
  }
#endif
}

#ifdef IGL_STATIC_LIBRARY
#undef IGL_STATIC_LIBRARY
#ifndef PARALLEL_CGAL
#include <igl/copyleft/cgal/mesh_boolean.cpp>
template bool igl::copyleft::cgal::mesh_boolean<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, igl::MeshBooleanType const&, Eigen::PlainObjectBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
#include <igl/copyleft/cgal/assign.cpp>
template void igl::copyleft::cgal::assign<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> >&);
#endif
#define IGL_STATIC_LIBRARY
#endif
