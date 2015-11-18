//
//  main.cpp
//  hw5_0
//
//  Created by 徐海翔 on 15/11/17.
//  Copyright © 2015年 徐海翔. All rights reserved.
//
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


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "structs.h"
#include "halfedge.h"

using namespace std;

// function definition
void init(void);
void init(void);
void reshape(int width, int height);
void display(void);
void init_lights();
void set_lights();
void draw_objects();

void parse(char* filename);

//parameter definitions
float cam_position[3];
float cam_orientation_axis[3];
float cam_orientation_angle = 0; // in deg
float near_param = 1, far_param = 20,
left_param = -0.5, right_param = 0.5,
top_param = 0.5, bottom_param = -0.5;

int mouse_x, mouse_y;
float mouse_scale_x, mouse_scale_y;
const float step_size = 0.2;
const float x_view_step = 90.0, y_view_step = 90.0;
float x_view_angle = 0, y_view_angle = 0;

bool is_pressed = false;
bool wireframe_mode = false;

int xres, yres;

vector<Point_Light> lights;
vector<GraphObj> graph;
// function implementation
void init(void)
{
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); //backface culling
    glEnable(GL_DEPTH_TEST); //depth buffering
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glMatrixMode(GL_PROJECTION); // projection mode
    glLoadIdentity(); //projection matrix=I
    glFrustum(left_param, right_param,
              bottom_param, top_param,
              near_param, far_param); // camera space F
    //P = P * F, P=I=>F
    glMatrixMode(GL_MODELVIEW); //modelview mode
    init_lights();
}


void reshape(int width, int height)
{
    height = (height == 0) ? 1 : height;
    width = (width == 0) ? 1 : width;
    glViewport(0, 0, width, height);
    mouse_scale_x = (float) (right_param - left_param) / (float) width;
    mouse_scale_y = (float) (top_param - bottom_param) / (float) height;
    glutPostRedisplay();//re-render
}


void display(void) //world & camera space
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // window black, depth buffer value high
    glLoadIdentity();
    glRotatef(y_view_angle, 1, 0, 0); //camera rotate first, ini 0
    glRotatef(x_view_angle, 0, 1, 0);
    glRotatef(-cam_orientation_angle,
              cam_orientation_axis[0], cam_orientation_axis[1], cam_orientation_axis[2]);
    // inv rotation matrix
    glTranslatef(-cam_position[0], -cam_position[1], -cam_position[2]);
    // inv trans
    set_lights(); // set lights in world space
    draw_objects();// draw objs
    //double buffering
    glutSwapBuffers(); //swap double buffers, active/off-screen
}


void init_lights()
{
    glEnable(GL_LIGHTING); //Phong to every vertex
    int num_lights = lights.size();
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i; //GL_LIGHT0 - 7
        glEnable(light_id);
        glLightfv(light_id, GL_AMBIENT, lights[i].color);
        glLightfv(light_id, GL_DIFFUSE, lights[i].color);
        glLightfv(light_id, GL_SPECULAR, lights[i].color);
        glLightf(light_id, GL_QUADRATIC_ATTENUATION, lights[i].attenuation_k);
    }
}


void set_lights() //position lights in camera space
{
    int num_lights = lights.size();
    
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i;
        glLightfv(light_id, GL_POSITION, lights[i].position);
    }
}


void draw_objects(){
    
}

void parse(char* filename){
    ifstream graphfile(filename);
    string line;
    string objname, objfilename;
    unsigned int flag = 0;
    unsigned int number = 0;
    string stringtemp, command;
    int numtemp;
    
    if (graphfile.is_open()){
        while (getline(graphfile, line)){
            // read in camera parameters
            if (line.size() >0 && flag ==0){
                getline(graphfile, line);
                stringstream ss(line);
                getline(ss, command, ' ');
                for (int i=0;i<3;i++){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>cam_position[i];
                }
                
                getline(graphfile, line);
                stringstream ss1(line);
                getline(ss1, command, ' ');
                for (int i=0;i<3;i++){
                    getline(ss1, stringtemp, ' ');
                    stringstream(stringtemp)>>cam_orientation_axis[i];
                }
                getline(ss1, stringtemp, ' ');
                stringstream(stringtemp)>>cam_orientation_angle;
                
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>near_param;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>far_param;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>left_param;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>right_param;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>top_param;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>bottom_param;
            }else if (line.size()==0 && flag==0){
                flag=1;
                getline(graphfile,line);
            }
            // end of read in camera parameters
            
            // read in lights
            if(line.size()!=0 && flag==1){
                
                Point_Light light1;
                
                string lighttemp;
                stringstream ss_l(line);
                getline(ss_l, stringtemp, ' ');
                getline(ss_l, stringtemp, ',');// , as separation. xyz
                stringstream ss_l1(stringtemp);
                for (int i=0;i<3;i++){
                    getline(ss_l1, lighttemp, ' ');
                    stringstream(lighttemp)>>light1.position[i];
                }
                light1.position[3]=1;
                getline(ss_l, stringtemp, ',');// , as separation. rgb
                stringstream ss_l2(stringtemp);
                getline(ss_l2, lighttemp, ' ');// avoid one more ' '
                for (int i=0;i<3;i++){
                    getline(ss_l2, lighttemp, ' ');
                    stringstream(lighttemp)>>light1.color[i];
                }
                getline(ss_l, stringtemp);// , as separation. k
                stringstream ss_l3(stringtemp);
                ss_l3>>light1.attenuation_k;
                
                lights.push_back(light1);
                
            }else if(line.size()==0 && flag==1){
                flag=2;
                getline(graphfile,line);
            }
            // end of read in lights
            
            // read in objects
            if(flag==2&&line.size()>0&&line!="objects:"){
                stringstream ss(line);
                if (getline(ss, objname,' ')){
                    getline(ss, objfilename);
                    
                    char* cstr = new char[objfilename.length()+1];
                    strcpy(cstr, objfilename.c_str());
                    
                    GraphObj obj(cstr);//define a object named 'obj'
                    delete cstr;
                    stringstream ss2;
                    ss2<<objname<<"_copy_1";
                    obj.setPrintName(ss2.str());
                    obj.setName(objname);
                    obj.setNumber(number);//total number of objs
                    number++;
                    graph.push_back(obj);
                }
            }else if (flag==2&&line.size()==0){
                flag=3;
            }
            // end of read in objs
            
            // read in transforms
            if(flag==3&&line.size()==0){
                getline(graphfile,line);
                stringtemp=line;
                
                for (numtemp=0;numtemp<graph.size();numtemp++){
                    if (stringtemp==graph[numtemp].getName()){
                        graph[numtemp].dupMore();
                        if (graph[numtemp].getDupNumber()>1){
                            GraphObj newobj;
                            newobj=graph[numtemp];
                            stringstream ss1;
                            ss1<<graph[numtemp].getName()<<"_copy_"<<graph[numtemp].getDupNumber();
                            newobj.setPrintName(ss1.str());
                            newobj.setName(ss1.str());
                            newobj.setNumber(number);
                            numtemp=number;//IMPORTANT!! record the index of obj in vector<obj>
                            number++;
                            graph.push_back(newobj);
                        }
                        break;
                    }
                }//for
            }else if(flag==3&&line.size()>0){
                stringstream ss(line);
                getline(ss, stringtemp, ' ');
                float r,g,b,p;
                float a[3];
                if (stringtemp=="ambient"){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>r;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>g;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>b;
                    graph[numtemp].setAmbient(r, g, b);
                }else if(stringtemp=="diffuse"){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>r;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>g;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>b;
                    graph[numtemp].setDiffuse(r, g, b);
                }else if (stringtemp=="specular"){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>r;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>g;
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>b;
                    graph[numtemp].setSpecular(r, g, b);
                }else if(stringtemp=="shininess"){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>p;
                    graph[numtemp].setShininess(p);
                }else if (stringtemp=="t"){
                    Transforms trans_temp;
                    for (int i=0;i<3;i++){
                        getline(ss, stringtemp,' ');
                        stringstream(stringtemp)>>trans_temp.translation[i];
                    }
                    graph[numtemp].transform_sets.push_back(trans_temp);
                    
                }else if (stringtemp=="r"){
                    Transforms trans_temp;
                    for (int i=0;i<3;i++){
                        getline(ss, stringtemp,' ');
                        stringstream(stringtemp)>>trans_temp.rotation[i];
                    }
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>trans_temp.rotation_angle;
                    graph[numtemp].transform_sets.push_back(trans_temp);
                    
                }else if (stringtemp=="s"){
                    Transforms trans_temp;
                    for (int i=0;i<3;i++){
                        getline(ss, stringtemp, ' ');
                        stringstream(stringtemp)>>trans_temp.scaling[i];
                    }
                    graph[numtemp].transform_sets.push_back(trans_temp);
                    
                }else{
                    cout << "No such transform!\n";
                }//if flag=2 && not empty line
            }
        }
    }
}


int main(int argc, char* argv[]){
    xres = atoi(argv[2]);
    yres = atoi(argv[3]);
    
    parse(argv[1]);
    
    for (int i = 0; i < graph.size(); i++){
        vector<Vertex*> *v1=graph[i].getVertex();
        for (int j=0;j<v1->size();j++){
            cout<<"v "<<(*v1)[j]->x<<" "<<(*v1)[j]->y<<" "<<(*v1)[j]->z<<endl;
        }
    }

    return 0;
}










