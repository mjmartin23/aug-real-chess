/* Matt Martin
Jan 17, 2017
Piece class
*/

#include "piece.h"

Piece::Piece() { }

Piece::Piece(int teamNum,float markerSizeParam, const char * objfile) {
	team = teamNum;
	scale = markerSizeParam / 6.0;
	obj = OBJLoader(objfile,&verts,&norms,&faces);
	setColor();
}

void Piece::setColor() {
	if (team == 1) {
		// draw white piece
		color[0] = 0.9; color[1] = 0.9; color[2] = 0.9; color[3] = 1.0;
	} else {
		// draw black piece
		color[0] = 0.1; color[1] = 0.1; color[2] = 0.1; color[3] = 1.0;
	}
	grey[0] = 0.5; grey[1] = 0.5; grey[2] = 0.5; grey[3] = 1.0;
    lowAmbient[0] = 0.2; lowAmbient[1] = 0.2; lowAmbient[2] = 0.2; lowAmbient[3] = 1.0;
    white[0] = 1.0; white[1] = 1.0; white[2] = 1.0; white[3] = 1.0;
}

void Piece::draw(aruco::Marker* marker, std::vector<cv::Point3f> corners ) {
	if (marker == nullptr) return;

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,grey);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialf(GL_FRONT,GL_SHININESS,128.0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lowAmbient);

	double modelview_matrix[16];

    marker->glGetModelViewMatrix(modelview_matrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(modelview_matrix);


    glTranslatef((scale * (corners[0].x+corners[1].x+corners[2].x+corners[3].x) )/2.9,
    			 (scale * (corners[0].y+corners[1].y+corners[2].y+corners[3].y) )/2.9,
    			 (scale));
    //glTranslatef(markerSize/3.0,markerSize/3.0,scale);
    glRotatef(90.f,1.f,0.f,0.f);
    glPushMatrix();

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < faces.size(); ++i) {

        glNormal3f(scale*norms[(faces[i].vn1-1)].x, 
        			scale*norms[(faces[i].vn1-1)].y, 
        			scale*norms[(faces[i].vn1-1)].z);
        glVertex3f(scale*verts[(faces[i].v1-1)].x, 
        			scale*verts[(faces[i].v1-1)].y, 
        			scale*verts[(faces[i].v1-1)].z);

        glNormal3f(scale*norms[(faces[i].vn2-1)].x, 
        			scale*norms[(faces[i].vn2-1)].y, 
        			scale*norms[(faces[i].vn2-1)].z);
        glVertex3f(scale*verts[(faces[i].v2-1)].x, 
        			scale*verts[(faces[i].v2-1)].y, 
        			scale*verts[(faces[i].v2-1)].z);

        glNormal3f(scale*norms[(faces[i].vn3-1)].x, 
        			scale*norms[(faces[i].vn3-1)].y, 
        			scale*norms[(faces[i].vn3-1)].z);
        glVertex3f(scale*verts[(faces[i].v3-1)].x, 
        			scale*verts[(faces[i].v3-1)].y, 
        			scale*verts[(faces[i].v3-1)].z);
    }
    glEnd();
    glPopMatrix();
}