/* Matt Martin
Jan 17, 2017
Piece class
*/

#include "piece.h"

Piece::Piece() { }

Piece::Piece(int teamNum,float markerSizeParam) {
	team = teamNum;
	scale = markerSizeParam*0.15;
	setColor();
}

void Piece::setColor() {
	if (team == 1) {
		// draw white piece
		colorA[0] = 0.98; color[1] = 0.98; color[2] = 0.98; color[3] = 1.0;
		colorD[0] = 0.75; color[1] = 0.75; color[2] = 0.75; color[3] = 1.0;
	} else {
		// draw black piece
		colorA[0] = 0.1; color[1] = 0.1; color[2] = 0.1; color[3] = 1.0;
		colord[0] = 0.25; color[1] = 0.25; color[2] = 0.25; color[3] = 1.0;
	}
	grey[0] = 0.5; grey[1] = 0.5; grey[2] = 0.5; grey[3] = 1.0;
    lowAmbient[0] = 0.2; lowAmbient[1] = 0.2; lowAmbient[2] = 0.2; lowAmbient[3] = 1.0;
    white[0] = 1.0; white[1] = 1.0; white[2] = 1.0; white[3] = 1.0;
}

void Piece::draw( std::vector<cv::Point3f> corners ) {
	// if (marker == nullptr) return;

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,colorA);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,colorD);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialf(GL_FRONT,GL_SHININESS,128.0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lowAmbient);

    center = cv::Point3f(((corners[0].x+corners[1].x+corners[2].x+corners[3].x) / 4.0) ,
		    			 ((corners[0].y+corners[1].y+corners[2].y+corners[3].y) / 4.0),
		    			 zTranslate);
    //cout<<center<<endl;
    glPushMatrix();
    glTranslatef(center.x,center.y,center.z);
    glRotatef(90.f,1.f,0.f,0.f);
    //glLoadIdentity();

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


King::King(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/king.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale*3.0;
}

Queen::Queen(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/queen.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale*2.75;
}

Bishop::Bishop(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/bishop.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale*0.0;
}

Knight::Knight(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/knight.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale;
}

Rook::Rook(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/rook.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale*1.8;
}

Pawn::Pawn(int teamNum, float markerSizeParam) : Piece(teamNum,markerSizeParam) {
	objPath = "../data/models/pawn.obj";
	obj = OBJLoader(objPath,&verts,&norms,&faces);
	zTranslate = scale*0.0;
}