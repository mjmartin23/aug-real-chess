/*
Matt Martin
Jan 9, 2017
aruco markers
*/
/*****************************
Copyright 2011 Rafael Muñoz Salinas. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Rafael Muñoz Salinas.
********************************/
#include <iostream>

#include <fstream>
#include <sstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
// #include <GL/glew.h>
// #include <GL/glfw.h>
#endif
#include <opencv2/opencv.hpp>
#include "aruco/aruco.h"
#include "board.h"
//#include "glm/glm.hpp"
//#include "objloader.h"
using namespace cv;
using namespace aruco;

string TheInputVideo;
string TheIntrinsicFile;
bool The3DInfoAvailable=false;
float TheMarkerSize=-1;
MarkerDetector PPDetector;
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;
Mat TheInputImage,TheUndInputImage,TheResizedImage;
CameraParameters TheCameraParams;

Board board;

Size TheGlWindowSize;
bool TheCaptureFlag=true;
bool readIntrinsicFile(string TheIntrinsicFile,Mat & TheIntriscCameraMatrix,Mat &TheDistorsionCameraParams,Size size);

void vDrawScene();
void vIdle();
void vResize( GLsizei iWidth, GLsizei iHeight );
void vMouse(int b,int s,int x,int y);
void initialize();
void drawThing(vector<Marker>);
void drawOBJ(const char *);


/************************************
 *
 *
 *
 *
 ************************************/

bool readArguments ( int argc,char **argv )
{
    if (argc!=4) {
        cerr<<"Invalid number of arguments"<<endl;
        cerr<<"Usage: (in.avi|live)  intrinsics.yml (../data/logitech_calibration.yml)   size (0.0425)"<<endl;
        return false;
    }
    TheInputVideo=argv[1];
    TheIntrinsicFile=argv[2];
    TheMarkerSize=atof(argv[3]);
    return true;
}


/************************************
 *
 *
 *
 *
 ************************************/

int main(int argc,char **argv)
{
    try
    {//parse arguments
        if (readArguments (argc,argv)==false) return 0;
        //read from camera
        if (TheInputVideo=="live") TheVideoCapturer.open(1);
        else TheVideoCapturer.open(TheInputVideo);
        if (!TheVideoCapturer.isOpened())
        {
            cerr<<"Could not open video"<<endl;
            return -1;

        }

        //read first image
        TheVideoCapturer>>TheInputImage;
        //read camera paramters if passed
        TheCameraParams.readFromXMLFile(TheIntrinsicFile);
        TheCameraParams.resize(TheInputImage.size());

        glutInit(&argc, argv);
        glutInitWindowPosition( 0, 0);
        glutInitWindowSize(TheInputImage.size().width,TheInputImage.size().height);
        glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
        glutCreateWindow( "AruCo" );
        initialize();
        glutDisplayFunc( vDrawScene );
        glutIdleFunc( vIdle );
        glutReshapeFunc( vResize );
        glutMouseFunc(vMouse);
        glClearColor( 0.0, 0.0, 0.0, 1.0 );
        glClearDepth( 1.0 );
        TheGlWindowSize=TheInputImage.size();
        vResize(TheGlWindowSize.width,TheGlWindowSize.height);
        glutMainLoop();

    } catch (std::exception &ex)

    {
        cout<<"Exception :"<<ex.what()<<endl;
    }

}


void initialize() {
    board = Board(TheCameraParams,TheMarkerSize);
    //lighting
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //light levels
    GLfloat qaAmbientLight[] = {0.2,0.2,0.2,1.0};
    GLfloat qaDiffuseLight[] = {0.8,0.8,0.8,1.0};
    GLfloat qaSpecularLight[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR, qaSpecularLight);

    //light position
    GLfloat qaLightPosition[] = {0.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}


/************************************
 *
 *
 *
 *
 ************************************/

void vMouse(int b,int s,int x,int y)
{
    if (b==GLUT_LEFT_BUTTON && s==GLUT_DOWN) {
        TheCaptureFlag=!TheCaptureFlag;
    }

}

/************************************
 *
 *
 *
 *
 ************************************/
void axis(float size)
{
    glColor3f (1,0,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(size,0.0f, 0.0f); // ending point of the line
    glEnd( );

    glColor3f ( 0,1,0 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f( 0.0f,size, 0.0f); // ending point of the line
    glEnd( );


    glColor3f (0,0,1 );
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
    glVertex3f(0.0f, 0.0f, size); // ending point of the line
    glEnd( );


}
/************************************
 *
 *
 *
 *
 ************************************/
void vDrawScene()
{
    if (TheResizedImage.rows==0) //prevent from going on until the image is initialized
        return;
    ///clear
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glDisable(GL_DEPTH_TEST);
    ///draw image in the buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, TheGlWindowSize.width, 0.0, TheGlWindowSize.height, -1.0, 1.0);

    //colors
    // GLfloat black[] = {0.0,0.0,0.0,1.0};
    // GLfloat red[] = {1.0,0.0,0.0,1.0};
    // GLfloat green[] = {0.0,1.0,0.0,1.0};
    // GLfloat blue[] = {0.0,0.0,1.0,1.0};
    // GLfloat white[] = {1.0,1.0,1.0,1.0};
    // GLfloat grey[] = {0.5,0.5,0.5,1.0};
    // GLfloat lowAmbient[] = {0.2,0.2,0.2,1.0};

    glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
    glDisable(GL_TEXTURE_2D);
    glPixelZoom( 1, -1);
    glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0 );
    glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB , GL_UNSIGNED_BYTE , TheResizedImage.ptr(0) );


    ///Set the appropriate projection matrix so that rendering is done in a enrvironment
    //like the real camera (without distorsion)
    glMatrixMode(GL_PROJECTION);
    double proj_matrix[16];
    TheCameraParams.glGetProjectionMatrix(TheInputImage.size(),TheGlWindowSize,proj_matrix,0.05,10);
    glLoadIdentity();
    glLoadMatrixd(proj_matrix);



    board.update(TheMarkers);
    //drawThing(TheMarkers);
    //drawOBJ("../data/models/bishop.obj");

    glutSwapBuffers();

}


void drawOBJ(const char* filename) {
    // Read our .obj file
    // OBJLoader obj = OBJLoader(filename);
    // std::vector<Vertex> verts;
    // std::vector<NormalVector> norms;
    // std::vector<Face> faces;
    // obj.load(verts,norms,faces);

    // // Load it into a VBO
    // glBegin(GL_TRIANGLES);
    // for (int i = 0; i < faces.size(); ++i) {
    //     glNormal3f(norms[(faces[i].vn1-1)].x, norms[(faces[i].vn1-1)].y, norms[(faces[i].vn1-1)].z);
    //     glVertex3f(verts[(faces[i].v1-1)].x, verts[(faces[i].v1-1)].y, verts[(faces[i].v1-1)].z);

    //     glNormal3f(norms[(faces[i].vn2-1)].x, norms[(faces[i].vn2-1)].y, norms[(faces[i].vn2-1)].z);
    //     glVertex3f(verts[(faces[i].v2-1)].x, verts[(faces[i].v2-1)].y, verts[(faces[i].v2-1)].z);

    //     glNormal3f(norms[(faces[i].vn3-1)].x, norms[(faces[i].vn3-1)].y, norms[(faces[i].vn3-1)].z);
    //     glVertex3f(verts[(faces[i].v3-1)].x, verts[(faces[i].v3-1)].y, verts[(faces[i].v3-1)].z);
    // }
    // glEnd();

    // GLuint vertexbuffer;
    // glGenBuffers(1, &vertexbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(cv::Point3f), &vertices[0], GL_STATIC_DRAW);

    // GLuint uvbuffer;
    // glGenBuffers(1, &uvbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(cv::Point2f), &uvs[0], GL_STATIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glVertexAttribPointer(
    //     0,                  // attribute
    //     3,                  // size
    //     GL_FLOAT,           // type
    //     GL_FALSE,           // normalized?
    //     0,                  // stride
    //     (void*)0            // array buffer offset
    // );

    // // 2nd attribute buffer : UVs
    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // glVertexAttribPointer(
    //     1,                                // attribute
    //     2,                                // size
    //     GL_FLOAT,                         // type
    //     GL_FALSE,                         // normalized?
    //     0,                                // stride
    //     (void*)0                          // array buffer offset
    // );

    // // Draw the triangle !
    // glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);

}


void drawThing(vector<Marker> markers) {
    GLfloat black[] = {0.0,0.0,0.0,1.0};
    GLfloat red[] = {1.0,0.0,0.0,1.0};
    GLfloat green[] = {0.0,1.0,0.0,1.0};
    GLfloat blue[] = {0.0,0.0,1.0,1.0};
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat grey[] = {0.5,0.5,0.5,1.0};
    GLfloat lowAmbient[] = {0.2,0.2,0.2,1.0};


    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //now, for each marker,
    double modelview_matrix[16];
    for (unsigned int m=0;m<markers.size();m++)
    {
        if (m == 0) {
            glMaterialfv(GL_FRONT,GL_AMBIENT,black);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,grey);
            glMaterialfv(GL_FRONT,GL_SPECULAR,white);
            glMaterialf(GL_FRONT,GL_SHININESS,128.0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);


            markers[m].glGetModelViewMatrix(modelview_matrix);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLoadMatrixd(modelview_matrix);

            //glColor3f(0.4,0.4,0.4);
            glTranslatef(0, 0, TheMarkerSize/2);
            glRotatef(90.f,1.f,0.f,0.f);
            glPushMatrix();
            //glutWireCube( markerSize );
            glutSolidTeapot(TheMarkerSize/2 );  
            glPopMatrix();
        }
    }
}


/************************************
 *
 *
 *
 *
 ************************************/
void vIdle()
{
    if (TheCaptureFlag) {
        //capture image
        TheVideoCapturer.grab();
        TheVideoCapturer.retrieve( TheInputImage);
        TheUndInputImage.create(TheInputImage.size(),CV_8UC3);
        //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
        cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);
        //remove distorion in image
        cv::undistort(TheInputImage,TheUndInputImage, TheCameraParams.CameraMatrix, TheCameraParams.Distorsion);
        //detect markers
        aruco::Dictionary::DICT_TYPES  TheDictionary = Dictionary::getTypeFromString( "ARUCO_MIP_16h3" );
        PPDetector.setDictionary(TheDictionary);
        PPDetector.detect(TheUndInputImage,TheMarkers, TheCameraParams.CameraMatrix,Mat(),TheMarkerSize,false);
        //resize the image to the size of the GL window
        cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
        
    }
    glutPostRedisplay();
}


/************************************
 *
 *
 *
 *
 ************************************/
void vResize( GLsizei iWidth, GLsizei iHeight )
{
    TheGlWindowSize=Size(iWidth,iHeight);
    //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
    if (iWidth*3%4!=0) {
        iWidth+=iWidth*3%4;//resize to avoid padding
        vResize(iWidth,TheGlWindowSize.height);
    }
    else {
        //resize the image to the size of the GL window
        if (TheUndInputImage.rows!=0)
            cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
    }
}

