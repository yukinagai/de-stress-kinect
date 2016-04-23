#ifndef __LINK_H__
#define __LINK_H__

#include <opencv2/core/version.hpp>

//#define OPENCV_VER_STR CVAUX_STR(CV_VERSION_MAJOR) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define OPENCV_EXT_STR "d.lib"
#else
#define OPENCV_EXT_STR ".lib"
#endif

#pragma comment(lib, "opencv_world310" OPENCV_EXT_STR)
#pragma comment(lib, "libcurl" OPENCV_EXT_STR)

#endif /* __LINK_H__ */