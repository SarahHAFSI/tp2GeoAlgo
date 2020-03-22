#include "octree.hpp"


unsigned int computeNbVert(Polyhedron& mesh){
	unsigned int nbVerts = 0;
	for (Vertex_iterator i = mesh.vertices_begin(); i != mesh.vertices_end(); ++i) {
		++nbVerts;
	}
	return nbVerts;
}

int main(int argc, char* argv[]){

	if (argc < 2) {
		std::cerr << "Il manque un paramètre au programme. Veuillez lui donner en entrée un nom de fichier au format off." << std::endl;
		return 1;
	}
	
	Polyhedron mesh;
	std::ifstream input(argv[1]);
	if (!input || !(input >> mesh) || mesh.empty()) {
		std::cerr << "Le fichier donné n'est pas un fichier off valide." << std::endl;
		return 1;
  	}
    
    Octree octo(mesh, 2, 4);
	std::ofstream flux("colorationSommets.off");
    octo.coloring(mesh, flux);
    std::ofstream flux2("fusionAvg.off");
    octo.createSimplifiedMesh(mesh, flux2);
    
    for (int i=0; i<8; i++){
        std::cout << " cel " << i << " " << octo.getRoot().getSonCell()[i]->getVertexList().size() << std::endl;
        if (octo.getRoot().getSonCell()[i]-> getVertexList().size() == 0){
            for (int j=0;j<8;j++){
                std::cout << "     fils " << i << " " << octo.getRoot().getSonCell()[i]->getSonCell()[j]->getVertexList().size()  << std::endl;
            }
        }
    }
    
    //std::cout << "taille : " << colorMap.size() << std::endl;
    //std::cout << "nb sommets : ";
    //unsigned int nb = computeNbVert(mesh);
    //std::cout << nb << std::endl;

    return 0;
}