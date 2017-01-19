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
#include <vector>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <exception>

#include <GL/gl.h>
#include <GL/glut.h>

#include <opencv2/opencv.hpp>
#include "aruco/aruco.h"
#include "aruco/posetracker.h"
#include "aruco/cvdrawingutils.h"

#include "expodetector.h"
#include "board.h"
//#include "square.h"

using namespace std;
using namespace cv;
using namespace aruco;

string TheInputVideo;
string TheIntrinsicFile;
string MapConfigFile;
bool The3DInfoAvailable=false;
float TheMarkerSize=-1;
MarkerDetector PPDetector;
VideoCapture TheVideoCapturer;
vector<Marker> TheMarkers;
Mat TheInputImage,TheUndInputImage,TheResizedImage,ExpoThresh;
CameraParameters TheCameraParams;

MarkerMap TheMarkerMap;//configuration of the map
MarkerMapPoseTracker MSPoseTracker;
string InputString;

Board board;
ExpoDetector TheExpoDetector;

Size TheGlWindowSize;
bool TheCaptureFlag=true;
bool readIntrinsicFile(string TheIntrinsicFile,Mat & TheIntriscCameraMatrix,Mat &TheDistorsionCameraParams,Size size);

void vDrawScene();
void vIdle();
void vResize( GLsizei iWidth, GLsizei iHeight );
void vMouse(int b,int s,int x,int y);
void vKey(unsigned char key, int x, int y);
void initialize();
void drawThing(vector<Marker>);
void modelView(double[],cv::Mat,cv::Mat);


/************************************
 *
 *
 *
 *
 ************************************/

bool readArguments ( int argc,char **argv )
{
    if (argc!=5) {
        cerr<<"Invalid number of arguments"<<endl;
        cerr<<"Usage: (in.avi|live)  marker_map_config.yml intrinsics.yml (../data/logitech_calibration.yml)   size (0.0425)"<<endl;
        return false;
    }
    TheInputVideo=argv[1];
    MapConfigFile=argv[2];
    TheIntrinsicFile=argv[3];
    TheMarkerSize=atof(argv[4]);
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
        if (TheInputVideo=="live") TheVideoCapturer.open(0);
        else TheVideoCapturer.open(TheInputVideo);
        if (!TheVideoCapturer.isOpened())
        {
            cerr<<"Could not open video"<<endl;
            return -1;

        }

        InputString = "";
        TheExpoDetector = ExpoDetector(44,100,75,255,75,255);

        TheMarkerMap.readFromFile(MapConfigFile);
        if ( TheMarkerMap.isExpressedInPixels() && TheMarkerSize>0) {
            TheMarkerMap=TheMarkerMap.convertToMeters(TheMarkerSize);
        }
        PPDetector.setDictionary(TheMarkerMap.getDictionary());

        //read first image
        TheVideoCapturer>>TheInputImage;
        //read camera paramters if passed
        TheCameraParams.readFromXMLFile(TheIntrinsicFile);
        TheCameraParams.resize(TheInputImage.size());

        if ( TheMarkerMap.isExpressedInMeters() && TheCameraParams.isValid()) {
            MSPoseTracker.setParams(TheCameraParams,TheMarkerMap);
        }

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
        glutKeyboardFunc( vKey );
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
    board = Board(TheMarkerSize*1.25);
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
    GLfloat qaLightPosition[] = {0.5,0.5,0.2,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    cout<<"initialized"<<endl;
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
    
    if ( !MSPoseTracker.estimatePose(TheMarkers)) {
        //cerr<<"marker map pose not correctly computed"<<endl;
    } else {
        double modelview_matrix[16];
        modelView(modelview_matrix,MSPoseTracker.getRvec().t(),MSPoseTracker.getTvec().t());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixd(modelview_matrix);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);

        glPushMatrix();
        board.updateGraphics();
        glPopMatrix();
    }

    // expo
    TheExpoDetector.detect(&TheUndInputImage,&ExpoThresh);
    axis(TheMarkerSize);
    // glBegin(GL_POLYGON);
    // glColor3f()


    glutSwapBuffers();

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
            glTranslatef(0, 0, 0);
            //glRotatef(90.f,1.f,0.f,0.f);
            glPushMatrix();
            glBegin(GL_POLYGON);
            glVertex3f(TheMarkerSize/2.0*0,TheMarkerSize/2.0*0,TheMarkerSize/2.0*0);
            glVertex3f(TheMarkerSize/2.0*1,TheMarkerSize/2.0*0,TheMarkerSize/2.0*0);
            glVertex3f(TheMarkerSize/2.0*1,TheMarkerSize/2.0*1,TheMarkerSize/2.0*0);
            glVertex3f(TheMarkerSize/2.0*0,TheMarkerSize/2.0*1,TheMarkerSize/2.0*0);
            glEnd();
            glPopMatrix();
        }
    }
}

void modelView(double modelview_matrix[],cv::Mat Rvec,cv::Mat Tvec) {
    bool invalid = false;
    for (int i = 0; i < 3 && !invalid; i++) {
        if (Tvec.at< float >(i, 0) != -999999)
            invalid |= false;
        if (Rvec.at< float >(i, 0) != -999999)
            invalid |= false;
    }
    if (invalid)
        throw cv::Exception(9002, "extrinsic parameters are not set", "modelView()", __FILE__, __LINE__);

    cv::Mat Rot(3, 3, CV_32FC1), Jacob;
    cv::Rodrigues(Rvec, Rot, Jacob);

    double para[3][4];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            para[i][j] = Rot.at< float >(i, j);
    // now, add the translation
    para[0][3] = Tvec.at< float >(0, 0);
    para[1][3] = Tvec.at< float >(1, 0);
    para[2][3] = Tvec.at< float >(2, 0);
    double scale = 1;

    modelview_matrix[0 + 0 * 4] = para[0][0];
    // R1C2
    modelview_matrix[0 + 1 * 4] = para[0][1];
    modelview_matrix[0 + 2 * 4] = para[0][2];
    modelview_matrix[0 + 3 * 4] = para[0][3];
    // R2
    modelview_matrix[1 + 0 * 4] = para[1][0];
    modelview_matrix[1 + 1 * 4] = para[1][1];
    modelview_matrix[1 + 2 * 4] = para[1][2];
    modelview_matrix[1 + 3 * 4] = para[1][3];
    // R3
    modelview_matrix[2 + 0 * 4] = -para[2][0];
    modelview_matrix[2 + 1 * 4] = -para[2][1];
    modelview_matrix[2 + 2 * 4] = -para[2][2];
    modelview_matrix[2 + 3 * 4] = -para[2][3];

    modelview_matrix[3 + 0 * 4] = 0.0;
    modelview_matrix[3 + 1 * 4] = 0.0;
    modelview_matrix[3 + 2 * 4] = 0.0;
    modelview_matrix[3 + 3 * 4] = 1.0;
    if (scale != 0.0) {
        modelview_matrix[12] *= scale;
        modelview_matrix[13] *= scale;
        modelview_matrix[14] *= scale;
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
    //board.updateGame();
    if (TheCaptureFlag) {
        //capture image
        TheVideoCapturer.grab();
        TheVideoCapturer.retrieve( TheInputImage);
        TheUndInputImage.create(TheInputImage.size(),CV_8UC3);
        //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
        cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);
        //remove distorion in image
        cv::undistort(TheInputImage,TheUndInputImage,TheCameraParams.CameraMatrix, TheCameraParams.Distorsion);
        //detect markers
        PPDetector.detect(TheUndInputImage,TheMarkers,TheCameraParams.CameraMatrix,Mat(),TheMarkerSize,false);
        //detect the 3d camera location wrt the markerset (if possible)
        //resize the image to the size of the GL window
        cv::resize(TheUndInputImage,TheResizedImage,TheGlWindowSize);
        
    }
    glutPostRedisplay();
}


void vKey(unsigned char key, int x, int y) {
    if (key == 13) {
        cout<<"executing command: "<<InputString<<endl;
        board.executeCommand(InputString);
        InputString = "";
    } else if (key == 8) {
        try {
            InputString.erase(InputString.size() - 1);
        } catch (exception& e) {
            cout << "Input string is empty" << endl;
        }
        cout<<InputString<<endl;
    } else {
        InputString += key;
        cout<<InputString<<endl;
    }
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

