#ifndef __CELL_HPP__
#define __CELL_HPP__

#include "box.hpp"


class Cell{

    private:
        std::vector<Vertex_handle> _vertexList;
        std::vector<Cell*>         _sonCell;
        Cell *                     _dadCell;
        bool                       _feuille;
        int                        _depth;
        std::vector<float>         _color;

    public:
        Cell(): _feuille(1) {};

        void addNewVertex(Vertex_handle newVertex, int maxVertex, BoundingBox & box, int maxDepth){
            if (this->Isfeuille()){ 
                if(this->getDepth() >= maxDepth){   //Si la profondeur max est atteinte
                    _vertexList.push_back(newVertex);
                }
                else {
                    if(_vertexList.size() < maxVertex){                   //Si il y a de la place dans la cellule
                        _vertexList.push_back(newVertex);
                    }
                    else {
                        //divison de cellules, on commence par créer les fils et on ajoute ensuite les sommets dans la liste des fils
                        for (int i = 0; i < 8; ++i){
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
            }
            else {
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
                            _sonCell[7]->addNewVertex(newVertex, maxVertex, B, maxDepth); //111
                            std::cout << "add 7" << std::endl;
                        }
                        else {
                            //box.setMin(middleX, middleY, box.getMin()[2]);
                            //box.setMax(box.getMin()[0], box.getMin()[1], middleZ);
                            B.setMin(middleX, middleY, box.getMin()[2]);
                            B.setMax(box.getMin()[0], box.getMin()[1], middleZ);
                            _sonCell[6]->addNewVertex(newVertex, maxVertex, B, maxDepth); //110
                            std::cout << "add 6" << std::endl;
                        }
                    }
                    else {
                        if(newVertex->point().z() > middleZ){
                            B.setMin(middleX, box.getMin()[1], middleZ);
                            B.setMax(box.getMin()[0], middleY, box.getMin()[2]);
                            _sonCell[5]->addNewVertex(newVertex, maxVertex, B, maxDepth); //101
                            std::cout << "add 5" << std::endl;
                        }
                        else {
                            B.setMin(middleX, box.getMin()[1], box.getMin()[2]);
                            B.setMax(box.getMin()[0], middleY, middleZ);
                            _sonCell[4]->addNewVertex(newVertex, maxVertex, B, maxDepth); //100
                            std::cout << "add 4" << std::endl;
                        }
                    }
                }
                else {
                    if(newVertex->point().y() > middleY){
                        if(newVertex->point().z() > middleZ){
                            B.setMin(box.getMin()[0], middleY, middleZ);
                            B.setMax(middleX, box.getMin()[1], box.getMin()[2]);
                            _sonCell[3]->addNewVertex(newVertex, maxVertex, B, maxDepth); //011
                            std::cout << "add 3" << std::endl;
                        }
                        else {
                            B.setMin(box.getMin()[0], middleY, box.getMin()[2]);
                            B.setMax(middleX, box.getMin()[1], middleZ);
                            _sonCell[2]->addNewVertex(newVertex, maxVertex, B, maxDepth); //010
                            std::cout << "add 2" << std::endl;
                        }
                    }
                    else {
                        if(newVertex->point().z() > middleZ){
                            B.setMin(box.getMin()[0], box.getMin()[1], middleZ);
                            B.setMax(middleX, middleY, box.getMin()[2]);
                            _sonCell[1]->addNewVertex(newVertex, maxVertex, B, maxDepth); //001
                            std::cout << "add 1" << std::endl;
                        }
                        else {
                            B.setMax(middleX, middleY, middleZ);
                            _sonCell[0]->addNewVertex(newVertex, maxVertex, B, maxDepth); //000
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
        };

        void divisionCell(BoundingBox & box, int maxVertex) {
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

        void setColor(){
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