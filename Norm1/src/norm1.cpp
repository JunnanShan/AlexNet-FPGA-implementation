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
#include "norm1.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;

typedef float DataType;

using namespace std;


//The function need to be accelerated in FPGA.
void norm1(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
	  DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL])
{
#pragma HLS INTERFACE m_axi port=inp_img offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=out_img offset=slave bundle=gmem0

#pragma HLS INTERFACE s_axilite port=inp_img bundle=control
#pragma HLS INTERFACE s_axilite port=out_img bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

	DataType inp_image[INP_IMAGE_CHANNEL][INP_IMAGE_SIZE][INP_IMAGE_SIZE];
#pragma HLS ARRAY_PARTITION variable=inp_image complete dim=1

	DataType sum[INP_IMAGE_SIZE][INP_IMAGE_SIZE];
#pragma HLS ARRAY_PARTITION variable=sum complete dim=0


L1:	for (int i=0; i<INP_IMAGE_CHANNEL; i++){
		for(int j=0; j<INP_IMAGE_SIZE; j++){
			for(int m=0; m<INP_IMAGE_SIZE; m++){
#pragma HLS PIPELINE
				inp_image[i][j][m] = inp_img[i * INP_IMAGE_SIZE * INP_IMAGE_SIZE + j*INP_IMAGE_SIZE + m];
			}
		}
	}

  //////////////////////////////////////////////////////////this is for chan = 0;
  L2:	  for (int x=0; x<INP_IMAGE_SIZE; x++){


  L3:	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
  	sum[x][y] = 0;
  L4:		for (int m = 0; m<=2; m++)
  	#pragma HLS PIPELINE
  				sum[x][y] += powf(inp_image[m][x][y], 2);
  	

  		    out_img[x*INP_IMAGE_SIZE + y] = inp_image[0][x][y] / powf((1+0.00002*sum[x][y]), 0.75);
     }
  }
  //////////////////////////////////////////////////////////this is for chan = 1;
  L5:	  for (int x=0; x<INP_IMAGE_SIZE; x++){


  L6:	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
  	sum[x][y] = 0;
  L7:		for (int m = 0; m<=3; m++)
  	#pragma HLS PIPELINE
  				sum[x][y] += powf(inp_image[m][x][y], 2);
  		    out_img[INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] = inp_image[1][x][y] / powf((1+0.00002*sum[x][y]), 0.75);
     }
  }
  //////////////////////////////////////////////////////////


L8:	    	for (int i=2; i<INP_IMAGE_CHANNEL-2; i++){
	
//#pragma HLS PIPELINE
L9:	  for (int x=0; x<INP_IMAGE_SIZE; x++){


L10:	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
	sum[x][y] = 0;
L11:		for (int m = 0; m<=4; m++)
#pragma HLS PIPELINE
			sum[x][y] += powf(inp_image[i-2+m][x][y], 2);
	    out_img[i*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] = inp_image[i][x][y] / powf((1+0.00002*sum[x][y]), 0.75);
	    }
	  }
	}

//////////////////////////////////////////////////////////this is for chan = INP_IMAGE_CHANNEL-2;
L12:	  for (int x=0; x<INP_IMAGE_SIZE; x++){


L13:	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
	sum[x][y] = 0;
L14:		for (int m = INP_IMAGE_CHANNEL-4; m<=INP_IMAGE_CHANNEL-1; m++)
	#pragma HLS PIPELINE
				sum[x][y] += powf(inp_image[m][x][y], 2);
		    out_img[(INP_IMAGE_CHANNEL-2)*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] = inp_image[INP_IMAGE_CHANNEL-2][x][y] / powf((1+0.00002*sum[x][y]), 0.75);
   }
}
//////////////////////////////////////////////////////////this is for chan = 2;
L15:	  for (int x=0; x<INP_IMAGE_SIZE; x++){


L16:	    for (int y=0; y<INP_IMAGE_SIZE; y++){
#pragma HLS PIPELINE
	sum[x][y] = 0;
L17:		for (int m = INP_IMAGE_CHANNEL-3; m<=INP_IMAGE_CHANNEL-1; m++)
	#pragma HLS PIPELINE
				sum[x][y] += powf(inp_image[m][x][y], 2);

		    out_img[(INP_IMAGE_CHANNEL-1)*INP_IMAGE_SIZE*INP_IMAGE_SIZE + x*INP_IMAGE_SIZE + y] = inp_image[INP_IMAGE_CHANNEL-1][x][y] / powf((1+0.00002*sum[x][y]), 0.75);

    }
  }
}
