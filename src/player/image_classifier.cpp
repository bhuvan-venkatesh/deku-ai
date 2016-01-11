#include "image_classifier.hpp"
#include <sys/types.h>
#include <dirent.h>
#define key_threshold 3

Image_Classifier::Image_Classifier(int blocks_):
  blocks(blocks_),
  detector(FeatureDetector::create("SIFT")),
  harris(FeatureDetector::create("HARRIS")),
  side((int)sqrt(blocks_)){
  load_templates();
}

bool has_ending (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(),
          ending.length(), ending));
    } else {
        return false;
    }
}

void Image_Classifier::load_templates(){
  const static vector<string> endings = {"jpg", "png", "jpeg", "jpg", "jpe", "bmp", "tiff", "tif"};

  DIR *d=opendir("."); //Should not be error unless dire
  while(de = readdir(d)){
    string file(de->d_name);
    for(auto i = endings.begin(); i != endings.end(); ++i){
      if(has_ending(file, *i)){
        templates.push_back(imread(de->d_name), CV_LOAD_IMAGE_GRAYSCALE);
      }
    }
  }
  closedir(d);
}

vector<int32_t> Image_Classifier::block_classify(const Mat& image){
  vector<int32_t> ret(blocks, 0);
  vector<cv::KeyPoint> keypoints;

  harris->detect(image, keypoints);

  int height = image.rows/side;
  int width = image.cols/side;

  for(auto i = keypoints.begin(); i != keypoints.end(); ++i){
    cv::Point2f color = i->pt;
    ret[(int)(color.x/width*side+color.y/height)] += 1;
  }

  for(size_t i = 0; i < ret.size(); ++i){
    if(ret[i] > key_threshold)
      ret[i] = -1;
  }
  detect_mario(image, ret, width, height);

  return ret;
}

void Image_Classifier::detect_mario(const Mat& img_scene, vector<int32_t>& ret, const int& block_width, const int& block_height){

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector->detect( img_object, keypoints_object );
  detector->detect( img_scene, keypoints_scene );
  Mat img_keypoints_1, s;

  //-- Step 2: Calculate descriptors (feature vectors)

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
  }

  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 3*min_dist )
     { good_matches.push_back( matches[i]); }
  }

  //-- Localize the object
  std::vector<Point2f> obj;
  std::vector<Point2f> scene;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
  }

  Mat H = findHomography( obj, scene, CV_RANSAC );

  //-- Get the corners from the image_1 ( the object to be "detected" )
  std::vector<Point2f> obj_corners(4);
  obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
  obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
  std::vector<Point2f> scene_corners(4);

  perspectiveTransform( obj_corners, scene_corners, H);

  //Mario Rectangle Identified!
  const auto width = abs(scene_corners[0].x - scene_corners[1].x);
  auto height = abs(scene_corners[0].y - scene_corners[3].y);
  scene_corners[0] += Point2f( img_object.cols, 0);
  rightmost = scene_corners[1].x;

  //TODO: Check if it is within reason, if it is then colors those blocks good

  int32_t cell = (int)(scene_corners[0].x/block_width*side+scene_corners[0].y/block_height);
  for(; height > 0; height -= block_height, cell += side){
    int32_t prev = cell;
    for(auto temp_width = width; temp_width > 0; temp_width -= block_width, cell++){
      ret[cell] = 1;
    }
    cell = prev;
  }


}
