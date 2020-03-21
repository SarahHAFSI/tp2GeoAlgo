#ifndef __BOX_HPP__
#define __BOX_HPP__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Facet_handle Facet_handle;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Kernel::Point_3 Point_3;

typedef std::map<Point_3, std::vector<float>> Vertex_color_map;

Vertex_color_map colorMap;

class BoundingBox{

    private:

        std::vector<float> _min;
        std::vector<float> _max;


    public:

        BoundingBox(){};

        BoundingBox(Polyhedron& mesh){
            Vertex_iterator i = mesh.vertices_begin();
            float minX = i->point().x();
            float minY = i->point().y();
            float minZ = i->point().z();
            float maxX = i->point().x();
            float maxY = i->point().y();
            float maxZ = i->point().z();

            for (Vertex_iterator i = mesh.vertices_begin(); i != mesh.vertices_end(); ++i){
                if(i->point().x() < minX) minX = i->point().x();
                if(i->point().y() < minY) minY = i->point().y();
                if(i->point().z() < minZ) minZ = i->point().z();

                if(i->point().x() > maxX) maxX = i->point().x();
                if(i->point().x() > maxY) maxY = i->point().y();
                if(i->point().x() > maxZ) maxZ = i->point().z();
            }

            _min.push_back(minX);
            _min.push_back(minY);
            _min.push_back(minZ);

            _max.push_back(maxX);
            _max.push_back(maxY);
            _max.push_back(maxZ);
        };

        std::vector<float> getMin() const{return _min;};

        std::vector<float> getMax() const{return _max;};

        void setMax(float x, float y, float z){
            _min.clear();
            _min.push_back(x);
            _min.push_back(y);
            _min.push_back(z);
        };

        void setMin(float x, float y, float z){
            _max.clear();
            _max.push_back(x);
            _max.push_back(y);
            _max.push_back(z);
        };
};


#endif