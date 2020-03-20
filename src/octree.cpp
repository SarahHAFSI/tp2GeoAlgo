#include "octree.hpp"

//typedef std::map<std::vector<std::vector<float>>, std::vector<float>> Vertex_color_map;
typedef std::map<Point_3, std::vector<float>> Vertex_color_map;

Vertex_color_map colorMap;


unsigned int computeNbVert(Polyhedron& mesh){
	unsigned int nbVerts = 0;
	for (Vertex_iterator i = mesh.vertices_begin(); i != mesh.vertices_end(); ++i) {
		++nbVerts;
	}
	return nbVerts;
}


// BOUNDING BOX


BoundingBox::BoundingBox(Polyhedron& mesh){
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
    
}

std::vector<float> BoundingBox::getMin() const{
    return _min;
}

std::vector<float> BoundingBox::getMax() const{
    return _max;
}

void BoundingBox::setMin(float x, float y, float z){
    _min.clear();
    _min.push_back(x);
    _min.push_back(y);
    _min.push_back(z);
}

void BoundingBox::setMax(float x, float y, float z){
    _max.clear();
    _max.push_back(x);
    _max.push_back(y);
    _max.push_back(z);
}


// CELL

int Cell::sizeCell() const{
    return _vertexList.size();
}

bool Cell::Isfeuille() const {
     return feuille;
}

void Cell::setfeuille(bool f) {
    feuille = f;
}

int Cell::getDepth() const{
    return _depth;
}

void Cell::setDepth(int depth){
    _depth = depth;
}


void Cell::setColor(){
    color.clear();

    float r = (double)rand()/RAND_MAX;
    float g = (double)rand()/RAND_MAX;
    float b = (double)rand()/RAND_MAX;

    color.push_back(r);
    color.push_back(g);
    color.push_back(b);
}

std::vector<float> Cell::getColor() const {
    return color;
}

std::vector<Cell*> Cell::getSonCell() const{
    return _sonCell;
}

std::vector<Vertex_handle> Cell::getVertexList() const{
    return _vertexList;
}



void Cell::addNewVertex(Vertex_handle newVertex, int maxVertex, BoundingBox & box){
    if (this->Isfeuille()){ 
        //std::cout << "iffeuille" << std::endl;
        if(this->getDepth() >= MaxDepth){   //Si la profondeur max est atteinte
            //std::cout << "prof max atteinte" << std::endl;
            _vertexList.push_back(newVertex);
        }
        else {
            //std::cout << "prof max non atteinte" << std::endl;
            if(this->sizeCell() < maxVertex){                   //Si il y a de la place dans la cellule
                //std::cout << "place dans la cellule" << std::endl;
                _vertexList.push_back(newVertex);
            }
            else {
                //std::cout << "pas de place" << std::endl;
                //divison de cellules, on commence par créer les fils et on ajoute ensuite les sommets dans la liste des fils
                //Cell * dad = this;  
                for (int i = 0; i < 8; ++i){
                    Cell * x = new Cell();  //On crée un fils
                    x->_dadCell = this;
                    //x.setDepth(this->_depth + 1);
                    x->_depth = (this->_depth) + 1; //La profondeur du fils est celle du père +1
                    _sonCell.push_back(x);
                    //_sonCell[i]->_dadCell = dad;
                }
                this->setfeuille(0);
                _vertexList.push_back(newVertex);
                this->divisionCell(box, maxVertex);
            }
        }
    }
    else {
        //std::cout << "else isfeuille" << std::endl;
        float minX = box.getMin()[0];
        float minY = box.getMin()[1];
        float minZ = box.getMin()[2];
        float maxX = box.getMax()[0];
        float maxY = box.getMax()[1];
        float maxZ = box.getMax()[2];

        //découpage en 8 cubes
        float middleX = (minX + maxX)/2;
        float middleY = (minY + maxY)/2;
        float middleZ = (minZ + maxZ)/2;
        //on relance la fn sur la cellule fils correspondante
        //on change la box a chaque fois
        BoundingBox B;
        B.setMin(0,0,0);
        B.setMax(0,0,0);
        if(newVertex->point().x() > middleX){
            if(newVertex->point().y() > middleY){
                if(newVertex->point().z() > middleZ){
                    
                    //box.setMin(middleX, middleY, middleZ);
                    B.setMin(middleX, middleY, middleZ);
                    _sonCell[7]->addNewVertex(newVertex, maxVertex, B); //111
                    std::cout << "add 7" << std::endl;
                }
                else {
                    //box.setMin(middleX, middleY, box.getMin()[2]);
                    //box.setMax(box.getMin()[0], box.getMin()[1], middleZ);
                    B.setMin(middleX, middleY, box.getMin()[2]);
                    B.setMax(box.getMin()[0], box.getMin()[1], middleZ);
                    _sonCell[6]->addNewVertex(newVertex, maxVertex, B); //110
                    std::cout << "add 6" << std::endl;
                }
            }
            else {
                if(newVertex->point().z() > middleZ){
                    B.setMin(middleX, box.getMin()[1], middleZ);
                    B.setMax(box.getMin()[0], middleY, box.getMin()[2]);
                    _sonCell[5]->addNewVertex(newVertex, maxVertex, B); //101
                    std::cout << "add 5" << std::endl;
                }
                else {
                    B.setMin(middleX, box.getMin()[1], box.getMin()[2]);
                    B.setMax(box.getMin()[0], middleY, middleZ);
                    _sonCell[4]->addNewVertex(newVertex, maxVertex, B); //100
                    std::cout << "add 4" << std::endl;
                }
            }
        }
        else {
            if(newVertex->point().y() > middleY){
                if(newVertex->point().z() > middleZ){
                    B.setMin(box.getMin()[0], middleY, middleZ);
                    B.setMax(middleX, box.getMin()[1], box.getMin()[2]);
                    _sonCell[3]->addNewVertex(newVertex, maxVertex, B); //011
                    std::cout << "add 3" << std::endl;
                }
                else {
                    B.setMin(box.getMin()[0], middleY, box.getMin()[2]);
                    B.setMax(middleX, box.getMin()[1], middleZ);
                    _sonCell[2]->addNewVertex(newVertex, maxVertex, B); //010
                    std::cout << "add 2" << std::endl;
                }
            }
            else {
                if(newVertex->point().z() > middleZ){
                    B.setMin(box.getMin()[0], box.getMin()[1], middleZ);
                    B.setMax(middleX, middleY, box.getMin()[2]);
                    _sonCell[1]->addNewVertex(newVertex, maxVertex, B); //001
                    std::cout << "add 1" << std::endl;
                }
                else {
                    B.setMax(middleX, middleY, middleZ);
                    _sonCell[0]->addNewVertex(newVertex, maxVertex, B); //000
                    std::cout << "add 0" << std::endl;
                }
            }
        }

        
        auto it = colorMap.find(newVertex->point());
        if (it == colorMap.end()){
            colorMap.insert({newVertex->point(), this->getColor()});
        }
        
        std::cout << _sonCell.size() << std::endl;
    }
}


void Cell::divisionCell(BoundingBox & box, int maxVertex) {
    float minX = box.getMin()[0];
    float minY = box.getMin()[1];
    float minZ = box.getMin()[2];
    float maxX = box.getMax()[0];
    float maxY = box.getMax()[1];
    float maxZ = box.getMax()[2];

    //découpage en 8 cubes
    float middleX = (minX + maxX)/2;
    float middleY = (minY + maxY)/2;
    float middleZ = (minZ + maxZ)/2;

    for (int k = 0; k < 8; k++){
        this->_sonCell[k]->setColor();
    }

    //choix du fils dans lequel on met le sommet
    for (int i = 0; i <= maxVertex; i++){
        if(this->_vertexList[i]->point().x() > middleX){
            if(this->_vertexList[i]->point().y() > middleY){
                if(this->_vertexList[i]->point().z() > middleZ){            
                    this->_sonCell[7]->_vertexList.push_back(_vertexList[i]);//111
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[7]->getColor()});
                    std::cout << "div 7" << std::endl;
                }
                else {
                    this->_sonCell[6]->_vertexList.push_back(this->_vertexList[i]);//110
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[6]->getColor()});
                    std::cout << "div 6" << std::endl;
                }
            }
            else {
                if(this->_vertexList[i]->point().z() > middleZ){
                    this->_sonCell[5]->_vertexList.push_back(this->_vertexList[i]);//101
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[5]->getColor()});
                    std::cout << "div 5" << std::endl;
                }
                else {
                    this->_sonCell[4]->_vertexList.push_back(this->_vertexList[i]);//100
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[4]->getColor()});
                    std::cout << "div 4" << std::endl;
                }
            }
        }
        else {
            if(this->_vertexList[i]->point().y() > middleY){
                if(this->_vertexList[i]->point().z() > middleZ){
                    this->_sonCell[3]->_vertexList.push_back(this->_vertexList[i]);//011
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[3]->getColor()});
                    std::cout << "div 3" << std::endl;
                }
                else {
                    this->_sonCell[2]->_vertexList.push_back(this->_vertexList[i]);//010
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[2]->getColor()});
                    std::cout << "div 2" << std::endl;
                }
            }
            else {
                if(this->_vertexList[i]->point().z() > middleZ){
                    this->_sonCell[1]->_vertexList.push_back(this->_vertexList[i]);//001
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[1]->getColor()});
                    std::cout << "div 1" << std::endl;
                }
                else {
                    this->_sonCell[0]->_vertexList.push_back(this->_vertexList[i]);//000
                    colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[0]->getColor()});
                    std::cout << "div 0" << std::endl;
                }
            }
        }
    }
    _vertexList.clear(); 
}


// OCTREE

Octree::Octree(Polyhedron& mesh, int maxVertex, BoundingBox & box){
    BoundingBox bb(mesh);
    _boundingbox = bb;
    _maxVertex = maxVertex;
    _root.setDepth(0);

    std::cout << "avant octree" << std::endl;
    
    for(Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); it++){
        _root.addNewVertex(it, maxVertex, box);
    }
    std::cout << "apres octree" << std::endl;
}

void Octree::setMaxVertex(int maxVertex){
    _maxVertex = maxVertex;
}

int Octree::getMaxVertex() const{
    return _maxVertex;
}

Cell Octree::getRoot() const{
    return _root;
}



//Je sais pas si les sommets dans le map seront dans le bon ordre 
void createOff(Polyhedron& mesh){

	std::ofstream flux("../src/meshDegrade.off");
    int count = 0;

	if(flux){
		CGAL::set_ascii_mode(flux);
		flux << "COFF" << std::endl << mesh.size_of_vertices() << ' ' << mesh.size_of_facets() << " 0" << std::endl;
		//std::copy(mesh.points_begin(), mesh.points_end(), std::ostream_iterator<Point_3>( flux, "\n"));

        for (auto i = colorMap.begin(); i != colorMap.end(); ++i) {   //on visite tout le map
            flux << i->first.x() << ' ' << i->first.y() << ' ' << i->first.z() << ' ' << i->second[0] << ' ' << i->second[1] << ' ' << i->second[2] << " 1" << std::endl; 
            std::cout << count << std::endl;
            count++;
        }

        std::cout << "ntm" << std::endl;

        for (Facet_iterator i = mesh.facets_begin(); i !=mesh.facets_end(); ++i) {
            Halfedge_facet_circulator j = i->facet_begin();
                CGAL_assertion(CGAL::circulator_size(j) >= 3);
                flux << CGAL::circulator_size(j) << ' ';
                do {
                    flux << ' ' << std::distance(mesh.vertices_begin(), j->vertex());
                } while ( ++j != i->facet_begin());
                flux << std::endl;
		}

	}else{
		std::cout << "ERROR : impossible to open or create the file !" << std::endl;
	}
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

    BoundingBox bb(mesh);

    //std::cout << "min : " << bb.getMin()[0] << " " << bb.getMin()[1] << " " << bb.getMin()[2] << std::endl;
    //std::cout << "max : " << bb.getMax()[0] << " " << bb.getMax()[1] << " " << bb.getMax()[2] << std::endl;


    Octree octo(mesh, 2, bb);

    //createOff(mesh);
    for (int i=0; i<8; i++){
        std::cout << " cel " << i << " " << octo.getRoot().getSonCell()[i]->getVertexList().size() << std::endl;
        if (octo.getRoot().getSonCell()[i]-> getVertexList().size() == 0){
            for (int j=0;j<8;j++){
                std::cout << "     fils " << i << " " << octo.getRoot().getSonCell()[i]->getSonCell()[j]->getVertexList().size()  << std::endl;
                //std::cout << "     fils " << i << " " << octo.getRoot().getSonCell()[i]->getSonCell()[j]->getVertexList().size() << " c : " << colorMap[octo.getRoot().getSonCell()[i]->getSonCell()[j]->getVertexList()[i][0]] << std::endl; 
            }
        }
    }

    std::cout << "taille : " << colorMap.size() << std::endl;
    std::cout << "nb sommets : ";
    unsigned int nb = computeNbVert(mesh);
    std::cout << nb << std::endl;

    //createOff(mesh);


    return 0;
}