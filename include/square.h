/* Matt Martin
Jan 9,2017
Square class for chessboard
*/

#include "aruco/aruco.h"

class Square {
public:
	Square(int,int,aruco::Marker);

private:
	int row,col;
	std::tuple<int,int> positionOnMarker;
}