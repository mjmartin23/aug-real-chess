/*
Matt Martin
Jan 9, 2017
aruco markers
*/

#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include <iostream>
#include <string>
#include <cstdio>

// int main(int argc, char const *argv[])
// {
// 	cerr<<"\tDictionaries: "; for(auto dict:aruco::Dictionary::getDicTypes())    cerr<<dict<<" ";cerr<<endl;
// 	return 0;
// }

class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};

using namespace std;
using namespace cv;
int main(int argc, char **argv) {
    try {
        CmdLineParser cml(argc,argv);
        if (argc < 4 || cml["-h"]) {
            cerr << "Usage: X:Y Image.png Configuration.yml [-d dictionary_name (ARUCO_MIP_36h12 default)] [-s <pixSize>] [-t <Type>(0: panel,1: chessboard )] [-r rand_seed] [-i interMarkerDistance(0,1)]" << endl;
            cerr<<"\tDictionaries: "; for(auto dict:aruco::Dictionary::getDicTypes())    cerr<<dict<<" ";cerr<<endl;
            return -1;
        }
        int XSize, YSize;
        if (sscanf(argv[1], "%d:%d", &XSize, &YSize) != 2) {
            cerr << "Incorrect X:Y specification" << endl;
            return -1;
        }

        auto Dict=aruco::Dictionary::loadPredefined(cml("-d","ARUCO_MIP_36h12"));
        int pixSize = stoi(cml("-s","500"));
        int typeMarkerMap = stoi(cml("-t","0"));
        int rand_seed = stoi(cml("-r","0"));
        float interMarkerDistance = stof(cml("-i","0.2"));

        if ((interMarkerDistance > 1.f) || (interMarkerDistance < 0.f)) {
            cerr << "Incorrect interMarkerDistance '" << interMarkerDistance << "' -- needs to be [0,1]" << endl;
            return -1;
        }

        //get the list of ids
        std::vector<int> ids;
        for(auto d:Dict.getMapCode()) ids.push_back(d.second);
        //random shuffle
        srand(rand_seed);
        std::random_shuffle(ids.begin(),ids.end());
        //take the required ones
        ids.resize(XSize*YSize);
        aruco::MarkerMap BInfo=Dict.createMarkerMap(Size(XSize, YSize), pixSize, pixSize * interMarkerDistance,ids,typeMarkerMap==1);
        //create a printable image to save
        cv::Mat MarkerMapImage=BInfo.getImage();

        //save
        BInfo.saveToFile(argv[3]);
       imwrite(argv[2], MarkerMapImage);

    } catch (std::exception &ex) {
        cout << ex.what() << endl;
    }
}
