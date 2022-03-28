#pragma once
#include <igl/xml/serialize_xml.h>
#include <igl/STR.h>
#include <Eigen/Core>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Epeck::FT EScalar;
typedef Eigen::Matrix<EScalar,Eigen::Dynamic,3> MatrixXE;

namespace igl
{
  namespace xml
  {
    namespace serialization_xml
    {
      template <> inline void serialize(
        const MatrixXE & obj,
        tinyxml2::XMLDocument* doc,
        tinyxml2::XMLElement* element,
        const std::string& name)
      {
        const std::function<std::string(const MatrixXE::Scalar &) > to_string = 
          [](const MatrixXE::Scalar & v)->std::string
          {
            return
              STR(CGAL::exact(v));
          };
        serialize(obj,name,to_string,doc,element);
      }
      template <> inline void deserialize(
        MatrixXE & obj,
        const tinyxml2::XMLDocument* doc,
        const tinyxml2::XMLElement* element,
        const std::string& name)
      {
        const std::function<void(const std::string &,MatrixXE::Scalar &)> & 
          from_string = 
          [](const std::string & s, MatrixXE::Scalar & v)
          {
            std::stringstream(s)>>v;
          };
        deserialize(doc,element,name,from_string,obj);
      }
    }
    inline void append_mesh_to_xml(
      const std::string & filename,
      const MatrixXE & V,
      const Eigen::MatrixXi & F,
      const Eigen::VectorXi & J)
    {
      const bool binary = false;
      // Write vertices, overwriting file (true)
      igl::xml::serialize_xml(V,"V",filename,binary,false);
      // Write faces to same file, appending (false)
      igl::xml::serialize_xml(F,"F",filename,binary,false);
      // Write face indices to same file, appending (false)
      igl::xml::serialize_xml(J,"J",filename,binary,false);
    }
    inline void read_mesh_from_xml(
      const std::string & filename,
      MatrixXE & V,
      Eigen::MatrixXi & F,
      Eigen::VectorXi & J)
    {
      igl::xml::deserialize_xml(V,"V",filename);
      igl::xml::deserialize_xml(F,"F",filename);
      igl::xml::deserialize_xml(J,"J",filename);
    }

  }
}

#ifdef IGL_STATIC_LIBRARY
#undef IGL_STATIC_LIBRARY
#include <igl/xml/serialize_xml.cpp>
#ifdef PARALLEL_CGAL
#else
template void igl::xml::serialize_xml<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> >(Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, bool, bool);
template void igl::xml::deserialize_xml<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> >(Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&);
#endif

template void igl::xml::serialize_xml<Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::Matrix<int, -1, 1, 0, -1, 1> const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, bool, bool);
template void igl::xml::serialize_xml<Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::Matrix<int, -1, -1, 0, -1, -1> const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, bool, bool);
template void igl::xml::deserialize_xml<Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::Matrix<int, -1, 1, 0, -1, 1>&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&);
template void igl::xml::deserialize_xml<Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::Matrix<int, -1, -1, 0, -1, -1>&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&);
#define IGL_STATIC_LIBRARY
#endif
