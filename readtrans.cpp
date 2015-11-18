
#include "structs.h"


using namespace std;



vector<Vertex> GraphObj::getVertex(){
    
    return v;
}

vector<Face> GraphObj::getFace(){
    return f;
}

bool GraphObj::setVertex(vector<Vertex> v1){
    v=v1;
    
    return true;
};

bool GraphObj::setFace(vector<Face> f1){
    f=f1;
    return true;
}
