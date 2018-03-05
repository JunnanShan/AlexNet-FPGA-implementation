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
#include "norm.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;

typedef float DataType;

using namespace std;


//The function need to be accelerated in FPGA.
void norm(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
	  DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL])
{

  int n=5;
  int mu;
  int ml;
	for (int i=0; i<INP_IMAGE_CHANNEL; i++){

	  for (int x=0; x<INP_IMAGE_SIZE; x++){

	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
		mu = i+n/2;
		if (INP_IMAGE_CHANNEL-1 < i+n/2)
		  mu = INP_IMAGE_CHANNEL-1;
		ml = 0;
		if (i-n/2 > 0)
		  ml = i-n/2;
		float sum=0;
		for (int m = ml; m<=mu; m++)


#pragma HLS LOOP_TRIPCOUNT max=5
sum += powf(inp_img[m*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y], 2);
	    out_img[i*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] = inp_img[i*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] / powf((1+0.00002*sum), 0.75);
	    }
	  }
	}
}
