#ifndef __CELL_HPP__
#define __CELL_HPP__

#include "box.hpp"


class Cell{

    private:
        std::vector<Vertex_handle> _vertexList; // liste des sommets que contient le noeud
        std::vector<Cell*>         _sonCell;    // liste des noeuds/fils que contient le noeud
        Cell *                     _dadCell;    // pointeur sur le noeud pere
        bool                       _feuille;    // booleen disant si le noeud est une feuille ou pas
        int                        _depth;      // profondeur a laquelle se situe le noeud
        std::vector<float>         _color;      // couleur du noeud

    public:
        Cell(): _feuille(1) {};

        void addNewVertex(Vertex_handle newVertex, int maxVertex, BoundingBox & box, int maxDepth){
            if (this->Isfeuille()){ 
                if(this->getDepth() >= maxDepth){//Si la profondeur max est atteinte
                    _vertexList.push_back(newVertex);//on ajoute directement tous a la liste des sommets du noeud courant
                }else{//Si la profondeur max n'est pas atteinte
                    if(_vertexList.size() < maxVertex){//Si il y a de la place dans le noeud
                        _vertexList.push_back(newVertex);
                    }else{//s'il n'y a pas de place dans le noeud
                        //divison des noeuds, on commence par créer les fils et on ajoute ensuite les sommets dans la liste des fils
                        for (int i = 0; i < 8; ++i){//On cree 8 fils
                            Cell * x = new Cell();  //On crée un fils
                            x->_dadCell = this;
                            x->_depth = (this->_depth) + 1; //La profondeur du fils est celle du père +1
                            _sonCell.push_back(x);
                        }
                        this->setfeuille(0);
                        _vertexList.push_back(newVertex);
                        this->divisionCell(box, maxVertex);
                    }
                }
            }else{
                //découpage en 8 cubes
                float middleX = (box.getMin()[0] + box.getMax()[0])/2;
                float middleY = (box.getMin()[1] + box.getMax()[1])/2;
                float middleZ = (box.getMin()[2] + box.getMax()[2])/2;

                BoundingBox newBox; //creation d'une nouvelle boite

                if(newVertex->point().x() > middleX){
                    if(newVertex->point().y() > middleY){
                        if(newVertex->point().z() > middleZ){
                            newBox.setMin(middleX, middleY, middleZ);
                            _sonCell[7]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //111
                            //std::cout << "add 7" << std::endl;
                        }else{
                            newBox.setMin(middleX, middleY, box.getMin()[2]);
                            newBox.setMax(box.getMin()[0], box.getMin()[1], middleZ);
                            _sonCell[6]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //110
                            //std::cout << "add 6" << std::endl;
                        }
                    }else{
                        if(newVertex->point().z() > middleZ){
                            newBox.setMin(middleX, box.getMin()[1], middleZ);
                            newBox.setMax(box.getMin()[0], middleY, box.getMin()[2]);
                            _sonCell[5]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //101
                            //std::cout << "add 5" << std::endl;
                        }else{
                            newBox.setMin(middleX, box.getMin()[1], box.getMin()[2]);
                            newBox.setMax(box.getMin()[0], middleY, middleZ);
                            _sonCell[4]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //100
                            //std::cout << "add 4" << std::endl;
                        }
                    }
                }else{
                    if(newVertex->point().y() > middleY){
                        if(newVertex->point().z() > middleZ){
                            newBox.setMin(box.getMin()[0], middleY, middleZ);
                            newBox.setMax(middleX, box.getMin()[1], box.getMin()[2]);
                            _sonCell[3]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //011
                            //std::cout << "add 3" << std::endl;
                        }else{
                            newBox.setMin(box.getMin()[0], middleY, box.getMin()[2]);
                            newBox.setMax(middleX, box.getMin()[1], middleZ);
                            _sonCell[2]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //010
                            //std::cout << "add 2" << std::endl;
                        }
                    }else{
                        if(newVertex->point().z() > middleZ){
                            newBox.setMin(box.getMin()[0], box.getMin()[1], middleZ);
                            newBox.setMax(middleX, middleY, box.getMin()[2]);
                            _sonCell[1]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //001
                            //std::cout << "add 1" << std::endl;
                        }else{
                            newBox.setMax(middleX, middleY, middleZ);
                            _sonCell[0]->addNewVertex(newVertex, maxVertex, newBox, maxDepth); //000
                            //std::cout << "add 0" << std::endl;
                        }
                    }
                }

                auto it = colorMap.find(newVertex->point());
                if (it == colorMap.end()){
                    colorMap.insert({newVertex->point(), this->getColor()});
                }                
            }
        };

        void divisionCell(BoundingBox& box, int maxVertex) {
            //découpage en 8 cubes
            float middleX = (box.getMin()[0] + box.getMax()[0])/2;
            float middleY = (box.getMin()[1] + box.getMax()[1])/2;
            float middleZ = (box.getMin()[2] + box.getMax()[2])/2;

            for (int k = 0; k < 8; k++){
                this->_sonCell[k]->randomColor(); //on donne une couleur au noeud
            }

            //choix du fils dans lequel on met le sommet
            for (int i = 0; i <= maxVertex; i++){
                if(this->_vertexList[i]->point().x() > middleX){
                    if(this->_vertexList[i]->point().y() > middleY){
                        if(this->_vertexList[i]->point().z() > middleZ){            
                            this->_sonCell[7]->_vertexList.push_back(_vertexList[i]);//111
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[7]->getColor()});
                            //std::cout << "div 7" << std::endl;
                        }else{
                            this->_sonCell[6]->_vertexList.push_back(this->_vertexList[i]);//110
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[6]->getColor()});
                            //std::cout << "div 6" << std::endl;
                        }
                    }else{
                        if(this->_vertexList[i]->point().z() > middleZ){
                            this->_sonCell[5]->_vertexList.push_back(this->_vertexList[i]);//101
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[5]->getColor()});
                            //std::cout << "div 5" << std::endl;
                        }else{
                            this->_sonCell[4]->_vertexList.push_back(this->_vertexList[i]);//100
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[4]->getColor()});
                            //std::cout << "div 4" << std::endl;
                        }
                    }
                }else{
                    if(this->_vertexList[i]->point().y() > middleY){
                        if(this->_vertexList[i]->point().z() > middleZ){
                            this->_sonCell[3]->_vertexList.push_back(this->_vertexList[i]);//011
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[3]->getColor()});
                            //std::cout << "div 3" << std::endl;
                        }else{
                            this->_sonCell[2]->_vertexList.push_back(this->_vertexList[i]);//010
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[2]->getColor()});
                            //std::cout << "div 2" << std::endl;
                        }
                    }else{
                        if(this->_vertexList[i]->point().z() > middleZ){
                            this->_sonCell[1]->_vertexList.push_back(this->_vertexList[i]);//001
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[1]->getColor()});
                            //std::cout << "div 1" << std::endl;
                        }else{
                            this->_sonCell[0]->_vertexList.push_back(this->_vertexList[i]);//000
                            colorMap.insert({this->_vertexList[i]->point(), this->_sonCell[0]->getColor()});
                            //std::cout << "div 0" << std::endl;
                        }
                    }
                }
            }
            _vertexList.clear(); 
        };

        bool Isfeuille() const{
            return _feuille;
        };

        int getDepth() const{
            return _depth;
        };

        std::vector<Cell*> getSonCell() const{
            return _sonCell;
        };

        std::vector<Vertex_handle> getVertexList() const{
            return _vertexList;
        };

        std::vector<float> getColor() const{
             return _color;
        };

        void setDepth(int depth){
            _depth = depth;
        };

        void setfeuille(bool f){
            _feuille = f;
        };

        void randomColor(){
            _color.clear();

            float r = (double)rand()/RAND_MAX;
            float g = (double)rand()/RAND_MAX;
            float b = (double)rand()/RAND_MAX;

            _color.push_back(r);
            _color.push_back(g);
            _color.push_back(b);
        };

};


#endif