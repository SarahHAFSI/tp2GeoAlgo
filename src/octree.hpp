#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

#include "cell.hpp"

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