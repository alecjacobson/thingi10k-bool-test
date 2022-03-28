#include <igl/read_triangle_mesh.h>
#include <igl/get_seconds.h>
#include <igl/remove_duplicate_vertices.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/copyleft/cgal/assign.h>
#include <igl/xml/serialize_xml.h>
#include "MatrixXE.h"
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
  const std::string & output_filename)
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
  igl::copyleft::cgal::mesh_boolean(
    Ve,F,MatrixXE(),Eigen::MatrixXi(),igl::MESH_BOOLEAN_TYPE_UNION,
    Vsu,Fsu,Jsu);
  const double t = igl::get_seconds()-t0;
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
