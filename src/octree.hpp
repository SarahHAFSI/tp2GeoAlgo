#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

#include "cell.hpp"
#include <typeinfo>

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

        void coloring(Polyhedron& mesh, std::ostream& flux){
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

        //FUSION DES POINTS D UNE MEME FEUILLE EN FAISANT LEUR MOYENNE
        void fusionCellAVG(Cell& cell, Vertex_Point_3_map& simplificationMap, std::vector<Point_3>& newVertex){
            if(cell.getVertexList().size()!=0){
                float x = 0, y=0, z=0;
                for(auto pt: cell.getVertexList()){
                    x += pt->point().x();
                    y += pt->point().y();
                    z += pt->point().z();
                }
                x /= cell.getVertexList().size();
                y /= cell.getVertexList().size();
                z /= cell.getVertexList().size();
                Point_3 avgPoint(x, y, z);
                newVertex.push_back(avgPoint); //on ajoute les nouveaux sommets dans une liste afin de connaitre le nombre de nvx sommets differents
                for(auto vertex : cell.getVertexList()){
                    simplificationMap.insert({vertex, avgPoint});
                }
            }
            if(cell.getSonCell().size()!=0){
                for(auto son : cell.getSonCell()){
                    fusionCellAVG(*son, simplificationMap, newVertex);
                }
            }
        }

        void createNewFaces(Polyhedron& mesh, Vertex_Point_3_map& simplificationMap, std::vector<Facet_handle>& newFaces){
            Facet_int_map tagedMesh;
            for(Facet_iterator i = mesh.facets_begin(); i != mesh.facets_end(); ++i){
                tagedMesh.insert({i, 1});
            }
            
            for(Facet_iterator i = mesh.facets_begin(); i != mesh.facets_end(); ++i){
                Halfedge_facet_circulator j = i->facet_begin();
                CGAL_assertion(CGAL::circulator_size(j) >= 3);
                do{
                    Halfedge_facet_circulator k = j++;
                    do{
                        if(simplificationMap[j->vertex()] == simplificationMap[k->vertex()]){
                            tagedMesh[i] = 0;
                        }
                    }while(++k != j);
                }while(++j != i->facet_begin());

                if(tagedMesh[i]==1) newFaces.push_back(i);
            }
        }

        void createSimplifiedMesh(Polyhedron& mesh, std::ostream& flux){
            Vertex_Point_3_map simplificationMap;
            std::vector<Point_3> newVertex;
            std::vector<Facet_handle> newFaces;
            std::map<Point_3, int> vertexIndex;
            int ind = 0;

            fusionCellAVG(_root, simplificationMap, newVertex);
            createNewFaces(mesh, simplificationMap, newFaces);

            if(flux){
                CGAL::set_ascii_mode(flux);
                flux << "OFF" << std::endl << newVertex.size() << ' ' << newFaces.size() << " 0" << std::endl;

                for(auto it : newVertex){
                    flux << it.x() << ' ' << it.y() << ' ' << it.z() << std::endl;
                    vertexIndex.insert({it, ind});
                    ind++;
                }

                for(auto it : newFaces){
                    Halfedge_facet_circulator j = it->facet_begin();
                    CGAL_assertion(CGAL::circulator_size(j) >= 3);
                    flux << CGAL::circulator_size(j) << ' ';
                    do {
                        Point_3 currentVertex = simplificationMap[j->vertex()];
                        flux << ' ' << vertexIndex[currentVertex];
                    } while (++j!=it->facet_begin());
                    flux << std::endl;
                }
            }

        }


};


#endif