//nvcc mat.cu  -o m `pkg-config opencv --cflags --libs`; ./m
#include <opencv2/highgui/highgui.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
#define THREADS_PER_BLOCK 1024//1024


//=======================CUDA================================
__global__ void cal_sobel(int *a,int *sal, int tam) {
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	if( index < tam){
        //int val = a[index] + a[index] ;
		int t = (a[index] + b[index] + c[index])/3;
		a[index] = t; 
		b[index] = t;
		c[index] = t;
        sal[index] = val;
	}
}


void CUDA_process_img(int *A){

	int *d_A;
    int *sal;
	int nElem = rows * cols;
	int size = nElem * sizeof(int);
	//Allocate device memory for matrices
	cudaMalloc((void **) &d_A, size);
    cudaMalloc((void **) &sal, size);
	//cudaMalloc((void **) &d_B, size);
	//cudaMalloc((void **) &d_C, size);
	//Copy B and C to device memory
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	//cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	//cudaMemcpy(d_C, C, size, cudaMemcpyHostToDevice);
	//run
	cal_sobel<<<(nElem+THREADS_PER_BLOCK-1)/THREADS_PER_BLOCK,THREADS_PER_BLOCK>>>(d_A,sal, nElem);//run
	
	cudaMemcpy(A, d_A, size, cudaMemcpyDeviceToHost);
	//cudaMemcpy(B, d_B, size, cudaMemcpyDeviceToHost);
	//cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	//Free device matrices
	//cudaFree(d_B);
	cudaFree(d_A);
	//cudaFree(d_C);
}

//======================OPEN-CV=================================
//brillo en serial
void CPU_add_brillo(int *R,int *G,int *B, int value, int rows, int cols){
    for (int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            R[cols * i + j] += value;
            G[cols * i + j] += value;
            B[cols * i + j] += value;
        }
    }
}

Mat sobel_cuda(Mat &image){
    int rows = image.rows;
    int cols = image.cols;
    int nElem = rows * cols;
    int * B = (int *) malloc(nElem * sizeof(int));
    //int * G = (int *) malloc(nElem * sizeof(int));
    //int * B = (int *) malloc(nElem * sizeof(int));

    //load IMG
    for( int i = 0; i < image.rows; i++ ){
        for( int j = 0; j < image.cols; j++ ){
        	int r = image.at<Vec3b>(i,j)[0];
        	//int g = image.at<Vec3b>(i,j)[1];
        	//int b = image.at<Vec3b>(i,j)[2];
            R[cols*i+j] = r;
            //G[cols*i+j] = g;
            //B[cols*i+j] = b;

        }
    }

    //CUDA_process_img(R,G,B,value,rows,cols);

    CUDA_process_img(R);
    
    //CPU_add_brillo(R,G,B,value,rows,cols);

    Mat new_image = Mat::zeros( image.size(), image.type() );
    for( int i = 0; i < image.rows; i++ ){
        for( int j = 0; j < image.cols; j++ ){
            new_image.at<Vec3b>(i,j)[0] = saturate_cast<uchar>( R[cols*i+j] );
            //new_image.at<Vec3b>(i,j)[1] = saturate_cast<uchar>( G[cols*i+j] );
            //new_image.at<Vec3b>(i,j)[2] = saturate_cast<uchar>( B[cols*i+j] );
        }
    }
    free(R);free(G);free(B);
    return new_image;
}

void metodo_sobel(){


    Mat img = imread("lena.bmp", CV_LOAD_IMAGE_COLOR);
    imshow( "Imagen Original", img ); 
    Mat img_gray;
    GaussianBlur( img, img, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor( img, img_gray, COLOR_BGR2GRAY );

    Mat img2 =sobel_cuda(img_gray);
    imshow( "brillo", img2 ); 
    waitKey(0);

    /********************/
    
    //Mat img_oring, img_gray, img_sal;
    //img_oring = imread( argv[1], IMREAD_COLOR ); // Load an image
    

    /**********************/
    
    double fps = 60;
    // calcular el tiempo de espera entre cada imagen a mostrar
    int delay = 1000 / fps;
    while (true){
        if(waitKey(delay) == 27) break;
    }

}


int main(){
	metodo_sobel();
	//run();
	return 0;
}
/*
nvcc mat.cu -o m -lopencv_core -lopencv_imgproc -lopencv_highgui
*/