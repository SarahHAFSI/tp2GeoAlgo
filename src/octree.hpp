#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

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

int MaxDepth = 2;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Facet_handle Facet_handle;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Kernel::Point_3 Point_3;



class Cell;

class BoundingBox{
    private:
        std::vector<float> _min;
        std::vector<float> _max;

    public:
        BoundingBox(){};
        BoundingBox(Polyhedron& mesh);
        std::vector<float> getMin() const;
        std::vector<float> getMax() const;
        void setMax(float, float, float);
        void setMin(float, float, float);
};


class Cell{

    private:
        std::vector<Vertex_handle> _vertexList;
        std::vector<Cell*> _sonCell;
        Cell * _dadCell;
        bool feuille;
        int Depth;
        std::vector<float> color;

    public:
        Cell(): feuille(1) {};
        void addNewVertex(Vertex_handle, int, BoundingBox &);
        void divisionCell(BoundingBox &, int);
        int  sizeCell() const;
        bool Isfeuille() const;
        void setfeuille(bool);
        int  getDepth() const;
        std::vector<Cell*> getSonCell() const;
        std::vector<Vertex_handle> getVertexList() const;
        void setColor();
        std::vector<float> getColor() const;
};


class Octree{

    private:
        int _maxVertex;
        BoundingBox _boundingbox;
        Cell _root;

    public:
        Octree() {};
        Octree(Polyhedron &, int, BoundingBox &);
        void setMaxVertex(int);
        int getMaxVertex() const;
        Cell getRoot() const;

};

unsigned int computeNbVert(Polyhedron&);

#endif