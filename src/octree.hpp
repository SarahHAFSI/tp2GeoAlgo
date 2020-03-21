#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

#include "cell.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Facet_handle Facet_handle;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Kernel::Point_3 Point_3;


class Octree{

    private:
        Cell _root;

    public:
        Octree() {};

        Octree(Polyhedron& mesh, int maxVertex, int maxDepth){
            BoundingBox box(mesh);
            _root.setDepth(0);

            std::cout << "avant octree" << std::endl;
            
            for(Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); it++){
                _root.addNewVertex(it, maxVertex, box, maxDepth);
            }
            std::cout << "apres octree" << std::endl;
        };
        
        Cell getRoot() const{
            return _root;
        };

};


#endif