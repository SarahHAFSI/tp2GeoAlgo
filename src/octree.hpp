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
           
            for(Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); it++){
                _root.addNewVertex(it, maxVertex, box, maxDepth);
            }
        };
        
        Cell getRoot() const{
            return _root;
        };

        void coloring(Polyhedron& mesh, std::ostream & flux){
            std::vector<float> color;

            if(flux){
                CGAL::set_ascii_mode(flux);
                flux << "COFF" << std::endl << mesh.size_of_vertices() << ' ' << mesh.size_of_facets() << " 0" << std::endl;

                for(Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); it++){
                    color.clear();
                    color = colorMap[it->point()];
                    flux << it->point().x() << ' ' << it->point().y() << ' ' << it->point().z() << ' ' << color[0] << ' ' << color[1] << ' ' << color[2] << " 0.75" << std::endl;
                }

                for (Facet_iterator i = mesh.facets_begin(); i !=mesh.facets_end(); ++i) {
                    Halfedge_facet_circulator j = i->facet_begin();
                    CGAL_assertion(CGAL::circulator_size(j) >= 3);
                    flux << CGAL::circulator_size(j) << ' ';
                    do {
                        flux << ' ' << std::distance(mesh.vertices_begin(), j->vertex());
                    } while ( ++j != i->facet_begin());
                    flux << std::endl;
                }
            }
        }

};


#endif