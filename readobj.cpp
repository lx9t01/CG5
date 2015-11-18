
#include "structs.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include </usr/local/include/Eigen/Dense>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <Eigen/Dense>
#endif

using namespace std;
using Eigen::MatrixXd;

void split(string s, int &s1, int &s2);

//GraphObj::GraphObj(char* s){
//  ifstream objfile(s);
//  string ch;
//  string x, y, z;
//  string str;
//    
//  if (objfile.is_open()) {
//    while (!objfile.eof()) {
//      objfile >> ch >> x >> y >> z;
//      if (ch == "v") {
//        Vertex v1 = Vertex(stof(x), stof(y), stof(z));
//        v->push_back(&v1);
//      } else if (ch == "f") {
//        Face f1 = Face(stoi(x), stoi(y), stoi(z));
//        f->push_back(&f1);
//      }
//        else
//        cout << "Fail to read obj line";
//      }
//    
//    f->pop_back(); // one more last line
//    objfile.close();
//  } else {
//    cout << "File not open.\n";
//  }
//}




string GraphObj::getName(){
  return objname;

}

unsigned int GraphObj::getNumber(){
  return objnumber;

}

bool GraphObj::setName(string s){
  objname=s;
  return true;
}

bool GraphObj::setNumber(unsigned int n){
  objnumber=n;
  return true;
}

bool GraphObj::setPrintName(string s){
    printname=s;
    return true;
}

string GraphObj::getPrintName(){
    return printname;
}

int GraphObj::dupMore(){
    dupnumber++;
    return dupnumber;
}

int GraphObj::getDupNumber(){
    return dupnumber;
}

void split(string s, int &s1, int &s2){// to split vertex and norm vectors
    int l;
    l=s.length();
    size_t pos=s.find("//");
    string sub1=s.substr(0,pos);
    string sub2=s.substr(pos+2,l);
    s1=atoi(sub1.c_str());
    s2=atoi(sub2.c_str());
    
}

bool GraphObj::setAmbient(float r, float g,float b){
    ambient[0]=r;
    ambient[1]=g;
    ambient[2]=b;
    return true;
}

bool GraphObj::setDiffuse(float r, float g, float b){
    diffuse[0]=r;
    diffuse[1]=g;
    diffuse[2]=b;
    return true;
}

bool GraphObj::setSpecular(float r, float g, float b){
    specular[0]=r;
    specular[1]=g;
    specular[2]=b;
    return true;
}

bool GraphObj::setShininess(float p){
    shininess=p;
    return true;
}

vector<Vertex*> *GraphObj::getVertex(){
    
    return v;
}

vector<Face*> *GraphObj::getFace(){
    return f;
}

bool GraphObj::setVertex(vector<Vertex*> v1){
    v=&v1;
    
    return true;
};

bool GraphObj::setFace(vector<Face*> f1){
    f=&f1;
    return true;
}

