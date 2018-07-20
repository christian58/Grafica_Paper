#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat met_Sobel(Mat &img){

  Mat sali(img);
  int rows = img.rows;
  int cols = img.cols;
  int num_pixels = rows*cols;

  //int * G = (int *) malloc(num_pixels*sizeof(int));
  for(int i = 1; i< rows-1; i++){
    for(int j = 1; j<cols-1;j++){
      int hx = -1*img.at<Vec3b>(i-1,j-1)[0] + img.at<Vec3b>(i-1,j+1)[0]
                -2*img.at<Vec3b>(i,j-1)[0] + 2*img.at<Vec3b>(i,j+1)[0]
                -1*img.at<Vec3b>(i+1,j-1)[0] + img.at<Vec3b>(i+1,j+1)[0];

      int vx = img.at<Vec3b>(i-1,j-1)[0] + 2*img.at<Vec3b>(i-1,j+1)[0]
                + img.at<Vec3b>(i-1,j+1)[0] - img.at<Vec3b>(i+1,j-1)[0]
                -2*img.at<Vec3b>(i+1,j)[0] - img.at<Vec3b>(i+1,j+1)[0];                
   
      hx = hx/5;
      vx = vx/5;
      int val = sqrt(hx*hx + vx*vx);
      sali.at<Vec3b>(i-1,j-1)[0] = val;
      sali.at<Vec3b>(i-1,j-1)[1] = val;
      sali.at<Vec3b>(i-1,j-1)[2] = val;
    }
  }
  return sali;

}

int main( int, char** argv )
{
  Mat img_oring, img_gray, img_sal;
  img_oring = imread( argv[1], IMREAD_COLOR ); // Load an image
  GaussianBlur( img_oring, img_oring, Size(3,3), 0, 0, BORDER_DEFAULT );

  
  cvtColor( img_oring, img_gray, COLOR_BGR2GRAY );

  Mat aux(img_gray);

  const char* window_name = "Sobel Demo - Simple Edge Detector";
  imshow( window_name ,img_gray);
  
  //int a = img_gray.at<Vec3b>(2,2)[0] + img_gray.at<Vec3b>(4,4)[0];

  //cout<<a<<endl;
  
//  cout<<img_gray(1,1)<<endl;
  
  img_sal = met_Sobel(img_gray);

  //const char* window_name = "Sobel Demo - Simple Edge Detector";
  imshow( "final" ,img_sal);


  Mat grad;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  

  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  
  Sobel( aux, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( aux, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  
  convertScaleAbs( grad_x, abs_grad_x );
  convertScaleAbs( grad_y, abs_grad_y );
  
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  
  //cout<<grad.at<Vec3b>(2,2)<<endl;
  
  imshow( "Propio", grad );















/*


  Mat src, src_gray;
  Mat grad;
  const char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  src = imread( argv[1], IMREAD_COLOR ); // Load an image
  if( src.empty() )
    { return -1; }
  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
  
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  cout<<src_gray.at<Vec3b>(2,2)<<endl;

  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  
  convertScaleAbs( grad_x, abs_grad_x );
  convertScaleAbs( grad_y, abs_grad_y );
  
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  
  cout<<grad.at<Vec3b>(2,2)<<endl;
  
  imshow( window_name, grad );*/
  waitKey(0);
  return 0;
}