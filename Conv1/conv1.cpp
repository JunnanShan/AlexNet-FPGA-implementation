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
#include "conv1.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;

typedef float DataType;

using namespace std;


//The function need to be accelerated in FPGA.
void conv1(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
		DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * FILTER_BATCH])
	   
{
#pragma HLS ALLOCATION instances=fmul limit=130 operation
#pragma HLS INTERFACE m_axi port=inp_img offset=slave bundle=gmem 
#pragma HLS INTERFACE m_axi port=out_img offset=slave bundle=gmem 

#pragma HLS INTERFACE s_axilite port=inp_img bundle=control
#pragma HLS INTERFACE s_axilite port=out_img bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control
	
	
  const DataType filter_local[96][INP_IMAGE_CHANNEL][FILTER_SIZE][FILTER_SIZE] = {
                             #include "conv1Weights.txt"
    	  	  	  	  	  	  	  	  	  	  }

#pragma HLS ARRAY_PARTITION variable=filter_local complete dim=0

  //initialize the "bias" array and print it in order to check it 
  const DataType bias[96] = {
                         #include "conv1Bias.txt"
                       };

  DataType filter_out[FILTER_BATCH];
  DataType window_3D[INP_IMAGE_CHANNEL][FILTER_SIZE][FILTER_SIZE];
#pragma HLS ARRAY_PARTITION variable=window_3D complete dim=0
  DataType right_col_3D[INP_IMAGE_CHANNEL][FILTER_SIZE][STRIDE];
#pragma HLS ARRAY_PARTITION variable=right_col_3D complete dim=0
  DataType right_col_3D_l[INP_IMAGE_CHANNEL][FILTER_SIZE][FILTER_SIZE];
#pragma HLS ARRAY_PARTITION variable=right_col_3D_l complete dim=0
  DataType line_buffer_3D[INP_IMAGE_CHANNEL][FILTER_SIZE][INP_IMAGE_SIZE];
#pragma HLS ARRAY_PARTITION variable=line_buffer_3D complete dim=2
  DataType new_pixels[INP_IMAGE_CHANNEL][STRIDE][STRIDE];
#pragma HLS ARRAY_PARTITION variable=new_pixels complete dim=0
  DataType new_pixels_l[INP_IMAGE_CHANNEL][STRIDE][FILTER_SIZE];
#pragma HLS ARRAY_PARTITION variable=new_pixels_l complete dim=0
  DataType out_img_3D[FILTER_BATCH][OUT_IMAGE_SIZE][OUT_IMAGE_SIZE];
  DataType lm[INP_IMAGE_CHANNEL][FILTER_SIZE*FILTER_SIZE+1];
#pragma HLS ARRAY_PARTITION variable=lm complete dim=0
//			  sum_t = filter_out[batch];
  DataType sum[INP_IMAGE_CHANNEL][62];
  #pragma HLS ARRAY_PARTITION variable=sum complete dim=0
  DataType sum2[INP_IMAGE_CHANNEL][32];
  #pragma HLS ARRAY_PARTITION variable=sum2 complete dim=0
  DataType sum3[INP_IMAGE_CHANNEL][17];
  #pragma HLS ARRAY_PARTITION variable=sum3 complete dim=1
  DataType sum4[INP_IMAGE_CHANNEL][9];
  #pragma HLS ARRAY_PARTITION variable=sum4 complete dim=0
  DataType sum5[INP_IMAGE_CHANNEL][4];
  #pragma HLS ARRAY_PARTITION variable=sum5 complete dim=0
  DataType sum6[INP_IMAGE_CHANNEL][2];
  #pragma HLS ARRAY_PARTITION variable=sum6 complete dim=0
  DataType sum7[INP_IMAGE_CHANNEL];


	  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  // 1st step: This three nested for loops are used to initialize the line-buffer
L2:	  for (int c = 0; c < INP_IMAGE_CHANNEL; c++)
		   {
L3:			 for (int i = STRIDE; i < FILTER_SIZE; i++)
			  	 {
L4:				  	for (int j = 0; j < INP_IMAGE_SIZE; j++)
		    		   {
#pragma HLS PIPELINE
		    			  line_buffer_3D[c][i][j] = inp_img[c*INP_IMAGE_SIZE*INP_IMAGE_SIZE + (i-STRIDE)*INP_IMAGE_SIZE + j];
		    		      //cout << "line_buffer_3D[" << c << "][" << i << "]["<< j <<"] = " << line_buffer_3D[c][i][j] << endl;
		    		   }
			  	 }
		    }
       ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //cause the stride = 4, not 1.
//outer loop for the row index selection
L5:    for (int row = FILTER_SIZE-1; row < INP_IMAGE_SIZE; row+=STRIDE)
      	  {
	//outer loop for the column index selection
L6:        for (int col = FILTER_SIZE-1; col < INP_IMAGE_SIZE; col+=STRIDE)
            {
//2nd step: using the linebuffer to form a 3D sliding window
	////////////////////////////////////////////////////////////////////////////////////////////
L7:             for (int chan = 0; chan < INP_IMAGE_CHANNEL; chan++)
                  {
//#pragma HLS UNROLL factor=3
#pragma HLS PIPELINE
//#pragma HLS DATAFLOW

            	//this if branch is used for the case that the index of the image start a new line
            	//which means the column equals to the filter size
	            if (col == FILTER_SIZE-1){


	            	// to fill the 3D line buffer

L8:	          for (int i = 0; i < STRIDE_NEW; i++)
			{

L9:				for (int ii = 0; ii < FILTER_SIZE-STRIDE; ii++)
	            	  {
	            			 right_col_3D_l[chan][ii][i] = line_buffer_3D[chan][ii][i] = line_buffer_3D[chan][ii+STRIDE][i];
	            	  }//end of fill the colomn of the right_col
	         }


	            	//to fill the new coming 3D pixels
L10:	          for (int n = 0; n < STRIDE_NEW; n++)
					{
L11:
//#pragma HLS PIPELINE
						for (int m = 0; m < STRIDE; m++)
		            	  {
	            			new_pixels_l[chan][m][n] = inp_img[INP_IMAGE_SIZE*INP_IMAGE_SIZE*chan + (row-(STRIDE-1)+m)*INP_IMAGE_SIZE + (col-(STRIDE_NEW-1)+n)];
	            			right_col_3D_l[chan][FILTER_SIZE-STRIDE+m][n] = line_buffer_3D[chan][FILTER_SIZE-STRIDE+m][n] = new_pixels_l[chan][m][n];
	            		  }
					}

	            	//put the right_col_3D at the most right of the sliding window
L12:	            	for (int ii = 0; ii < FILTER_SIZE; ii++)
	            	  {
L13:	            		for (int jj = 0; jj < STRIDE_NEW; jj++)
	            		  {
	            			window_3D[chan][ii][jj] = right_col_3D_l[chan][ii][jj];
	            		  }
	            	  }

	            }//end of the if condition


	            else {


	            	// to fill the 3D line buffer

L14:	            	   for (int ii = 0; ii < FILTER_SIZE-STRIDE; ii++)
	            			{
L15:	            		   for (int i = 0; i < STRIDE; i++)
		            		     {
	            			   right_col_3D[chan][ii][i] = line_buffer_3D[chan][ii][col-(STRIDE-1)+i] = line_buffer_3D[chan][ii+STRIDE][col-(STRIDE-1)+i];
		            		     }//end of fill the colomn of the right_col
	            		     }

	            	   //to fill the new coming 3D pixels
L16:	            	   for (int m = 0; m < STRIDE; m++)
	            	     {

L17:	            		   for (int n = 0; n < STRIDE; n++)
	            		     {

	            			   new_pixels[chan][m][n] = inp_img[INP_IMAGE_SIZE*INP_IMAGE_SIZE*chan + (row-(STRIDE-1)+m)*INP_IMAGE_SIZE + (col-(STRIDE-1)+n)];
	            			   right_col_3D[chan][FILTER_SIZE-STRIDE+m][n] = line_buffer_3D[chan][FILTER_SIZE-STRIDE+m][col-(STRIDE-1)+n] = new_pixels[chan][m][n];
	            		     }
	            	     }

	            	   //shift from left to right the sliding window to make room for the new_pixels.
L18:	            	   for (int ii = 0; ii < FILTER_SIZE; ii++)
	            	     {
L19:	            		   for (int jj = 0; jj < FILTER_SIZE-STRIDE; jj++)
	            		     {
	            			   window_3D[chan][ii][jj] = window_3D[chan][ii][jj+STRIDE];
	            		     }
	            	     }

	            	   //put the right_col_3D at the most right of the sliding window
L20:	            	   for (int ii = 0; ii < FILTER_SIZE; ii++)
	            	     {
L21:	            		   for (int jj = 0; jj < STRIDE; jj++)
	            		     {
	            			   window_3D[chan][ii][FILTER_SIZE-STRIDE+jj] = right_col_3D[chan][ii][jj];
	            		     }
	            	     }

    	            }////END OF ELSE
	}//end of chan

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//3rd step: using the sliding window to calculate the output of the convolution using the 96 batch of the filter

		L1:           for (int batch = 0; batch < FILTER_BATCH; batch++)
		    	       {
#pragma HLS PIPELINE
			            filter_out[batch] = 0;
						for (int chan = 0; chan < INP_IMAGE_CHANNEL; chan++)
				         {

	    L22:	            for (int i = 0; i < FILTER_SIZE; i++)
	    	                 {
	    L23:	              for (int j = 0; j < FILTER_SIZE; j++)
	     	 	 	 	 	   {	    	        
	    						lm [chan][i*FILTER_SIZE +j] =  window_3D[chan][i][j] * filter_local[batch][chan][i][j];
	    					 }
	    	                 }


L24:		    		for (int i = 0; i < 61; i++)
	    	              {
	    					sum[chan][i]=lm[chan][i]+lm[chan][FILTER_SIZE*FILTER_SIZE-i];
	    	              }


L25:	    			for (int j = 0; j < 31; j++)
	     	 	 	 {
	    	            		sum2[chan][j] = sum[chan][j] + sum[chan][61-j];
	    	            	  }

L26:	    		for (int j = 0; j < 16; j++)
	    		 {
 					sum3[chan][j] = sum2[chan][j] + sum2[chan][31-j];
	    		 }


L27:	    				for (int j = 0; j < 8; j++)
	    				 	  {
	    					      sum4[chan][j] = sum3[chan][j] + sum3[chan][15-j];
	    					  }

L28:	    				for (int j = 0; j < 4; j++)
	    					  {

	    					       sum5[chan][j] = sum4[chan][j] + sum4[chan][7-j];
	    					  }

L29:	    				for (int j = 0; j < 2; j++)
	    					 {

	    						sum6[chan][j] = sum5[chan][j] + sum5[chan][3-j];
	    					 }
	    			  sum7[chan]=sum6[chan][0] + sum6[chan][1];
	                          filter_out[batch] += sum7[chan];
			}//end of chan
			
	                  out_img_3D[batch][(row-FILTER_SIZE+1)/STRIDE][(col-FILTER_SIZE+1)/STRIDE] = filter_out[batch];
	                  //add the bias of each output channel
	                  DataType out = out_img_3D [batch][(row-FILTER_SIZE+1)/STRIDE][(col-FILTER_SIZE+1)/STRIDE] + bias[batch];

	                   //start the Relu
	                   if (out > 0){
	                    out_img[batch*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + ((row-FILTER_SIZE+1)/STRIDE)*OUT_IMAGE_SIZE + (col-FILTER_SIZE+1)/STRIDE] = out;
	                                }
	                   else
	                    out_img[batch*OUT_IMAGE_SIZE*OUT_IMAGE_SIZE + ((row-FILTER_SIZE+1)/STRIDE)*OUT_IMAGE_SIZE + (col-FILTER_SIZE+1)/STRIDE] = 0;
		   }//batch

          }//col
      }//row
}//conv


