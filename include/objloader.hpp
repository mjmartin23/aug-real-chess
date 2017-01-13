#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJ(
	const char * path, 
	std::vector<cv::Point3f> & out_vertices, 
	std::vector<cv::Point2f> & out_uvs, 
	std::vector<cv::Point3f> & out_normals
);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<cv::Point3f> & vertices,
	std::vector<cv::Point2f> & uvs,
	std::vector<cv::Point3f> & normals
);

#endif