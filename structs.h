#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Vec3f
{
	double x, y, z;
};

struct Vertex
{
    double x, y, z;
    Vertex(float x1, float y1, float z1){
        x = x1;
        y = y1;
        z = z1;
    }
};

struct Face
{
    int idx1, idx2, idx3;
    Face(int a1, int a2, int a3){
        idx1 = a1;
        idx2 = a2;
        idx3 = a3;
    }
};

struct Mesh_Data
{
    std::vector<Vertex*> *vertices;
    std::vector<Face*> *faces;
};

#endif

struct Point_Light
{
    float position[4]; //xyzw
    float color[3]; //rgb range?
    float attenuation_k;
};

struct Transforms
{
    float translation[3]; //xyz
    float rotation[3];
    float scaling[3];
    float rotation_angle; // angle in deg
};

class GraphObj {
public:
    GraphObj(char* s){
        v = new vector<Vertex*>();
        f = new vector<Face*>();
        ifstream objfile(s);
        string ch;
        string x, y, z;
        string str;
        
        if (objfile.is_open()) {
            while (!objfile.eof()) {
                objfile >> ch >> x >> y >> z;
                if (ch == "v") {
                    Vertex v1(stof(x), stof(y), stof(z));
                    v->push_back(&v1);
                } else if (ch == "f") {
                    Face f1(stoi(x), stoi(y), stoi(z));
                    f->push_back(&f1);
                }
                else
                    cout << "Fail to read obj line";
            }
            
            f->pop_back(); // one more last line
            objfile.close();
        } else {
            cout << "File not open.\n";
        }
    }
    GraphObj(){
        v = new vector<Vertex*>();
        f = new vector<Face*>();
    }
    
//    ~GraphObj(){
//        for (int i = 0; i < v->size(); i++){
//            delete (*v)[i];
//        }
//        delete v;
//        
//        for (int j = 0; j < f->size(); j++){
//            delete (*f)[j];
//        }
//        delete f;
//    }

    string getName();
    unsigned int getNumber();
    bool setName(string s);
    bool setNumber(unsigned int n);
    string getPrintName();
    bool setPrintName(string s);
    int dupMore();
    int getDupNumber();
    
    vector<Vertex*> *getVertex();
    vector<Face*> *getFace();
    bool setVertex(vector<Vertex*> v1);
    bool setFace(vector<Face*> f1);
    
    bool setAmbient(float r, float g,float b);
    bool setDiffuse(float r, float g, float b);
    bool setSpecular(float r, float g, float b);
    bool setShininess(float p);
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float shininess;
    vector<Transforms> transform_sets;
    
private:
    string objname;
    string printname;
    unsigned int objnumber;
    int dupnumber=0;
    vector<Vertex*> *v;
    vector<Face*> *f;

};