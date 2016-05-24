#include <cctag/Canny.hpp>

#include <boost/gil/image_view.hpp>

#include "utils/Defines.hpp"

//#define USE_CANNY_OCV3
#ifdef USE_CANNY_OCV3
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/timer.hpp>
#endif

namespace cctag
{

void edgesPointsFromCanny(
        EdgePointCollection& edgeCollection,
        const cv::Mat & edges,
        const cv::Mat & dx,
        const cv::Mat & dy )
{
  std::size_t width = edges.cols;
  std::size_t height = edges.rows;
  
  auto& edgePointsMap = edgeCollection.map();
  auto& points = edgeCollection.list();

  
  edgePointsMap.resize( boost::extents[width][height] );
  memset(edgePointsMap.origin(), -1, width*height*sizeof(int));
  
  points.reserve( width * height / 2 );

  for( int y = 0 ; y < height ; ++y )
  {
    for( int x = 0 ; x < width ; ++x )
    {
      if ( edges.at<uchar>(y,x) == 255 )
      {
        edgePointsMap[x][y] = (int)points.size();
        points.emplace_back( EdgePoint( x, y, (float) dx.at<short>(y,x), (float) dy.at<short>(y,x) ) );
      }
    }
  }
  
  // Make place for the links.
  edgeCollection.links().resize(points.size(), std::make_tuple(-1, -1));

}

} // namespace cctag


