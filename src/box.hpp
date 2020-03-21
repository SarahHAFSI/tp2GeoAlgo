#ifndef __BOX_HPP__
#define __BOX_HPP__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <map>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <cmath>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel>                          Polyhedron;
typedef Polyhedron::Facet_iterator                          Facet_iterator;
typedef Polyhedron::Halfedge_iterator                       Halfedge_iterator;
typedef Polyhedron::Vertex_iterator                         Vertex_iterator;
typedef Polyhedron::Vertex_handle                           Vertex_handle;
typedef Kernel::Point_3                                     Point_3;
typedef std::map<Polyhedron::Facet_handle, float>		    Facet_float_map;
typedef std::map<Polyhedron::Facet_handle, int>			    Facet_int_map;
typedef std::map<Point_3, std::vector<float>>               Point_3_color_map;

Point_3_color_map colorMap;

class BoundingBox{
    private:
        std::vector<float> _min;
        std::vector<float> _max;

    public:
        BoundingBox() : _min{0.0, 0.0, 0.0}, _max{0.0, 0.0, 0.0} {};

        BoundingBox(Polyhedron& mesh){
            float minX = std::min_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().x() < b.point().x() ;}) -> point().x();
            float minY = std::min_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().y() < b.point().y() ;}) -> point().y();
            float minZ = std::min_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().z() < b.point().z() ;}) -> point().z();     
            float maxX = std::max_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().x() < b.point().x() ;}) -> point().x();
            float maxY = std::max_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().y() < b.point().y() ;}) -> point().y();    
            float maxZ = std::max_element(mesh.vertices_begin(), mesh.vertices_end(), [](auto a, auto b){return a.point().z() < b.point().z() ;}) -> point().z();

            _min = {minX, minY, minZ};
            _max = {maxX, maxY, maxZ};
        };

        std::vector<float> getMin() const{return _min;};

        std::vector<float> getMax() const{return _max;};

        void setMax(float x, float y, float z){
            _min.clear();
            _min = {x, y, z};
        };

        void setMin(float x, float y, float z){
            _max.clear();
            _max = {x, y, z};
        };
};
#endif