/* 
======================================================
*
* Author:   Junnan Shan (junnan.shan@polito.it)
*
======================================================
*/
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
#include "pad3.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;

typedef float DataType;

using namespace std;


//The function need to be accelerated in FPGA.
void pad3(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
	     DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL])
	   
{
	for (int c = 0; c < OUT_IMAGE_CHANNEL; c++){
	  for (int i = 0; i < OUT_IMAGE_SIZE; i++){
            for (int j = 0; j < OUT_IMAGE_SIZE; j++){
#pragma HLS PIPELINE

	      if ((i<PAD) || (i>=OUT_IMAGE_SIZE-PAD) || (j<PAD) || (j>=OUT_IMAGE_SIZE-PAD))
		{
		  out_img[c*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i*OUT_IMAGE_SIZE + j] = 0;	
		}
	      else
 		{
		  out_img[c*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + i*OUT_IMAGE_SIZE + j] = inp_img[c*INP_IMAGE_SIZE*INP_IMAGE_SIZE + (i-PAD)*INP_IMAGE_SIZE + (j-PAD)];
		}
       }//end of j colomn index
	  }//end of i row index
	}//end of c channel index
}//end of function padding.

