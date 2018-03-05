 //in this c++ file, we define the 1st convolution layer of Alexnet. Actually it's not just convolution, it's convolution together with
//Relu, we just compare the output of the convolution, if it's greater than 0, then we store it as output, if it's negative, we store it as 0.
//Also in caffe the Relu layer is inlined with the other layer. 
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcnt1.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include "pad1.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;

typedef float DataType;

using namespace std;


//The function need to be accelerated in FPGA.
void pad1(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
	     DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL], int pad)
	   
{
	for (int c = 0; c < OUT_IMAGE_CHANNEL; c++){
	  for (int i = 0; i < OUT_IMAGE_SIZE; i++){
            for (int j = 0; j < OUT_IMAGE_SIZE; j++){
#pragma HLS PIPELINE

	      if ((i<pad) || (i>=OUT_IMAGE_SIZE-pad) || (j<pad) || (j>=OUT_IMAGE_SIZE-pad))
		{
		  out_img[c*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i*OUT_IMAGE_SIZE + j] = 0;	
		}
	      else
 		{
		  out_img[c*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i*OUT_IMAGE_SIZE + j] = inp_img[c*INP_IMAGE_SIZE*INP_IMAGE_SIZE + (i-2)*INP_IMAGE_SIZE + (j-pad)];
		}
       }//end of j colomn index
	  }//end of i row index
	}//end of c channel index
}//end of function padding.

