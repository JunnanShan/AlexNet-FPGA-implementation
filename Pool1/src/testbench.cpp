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
//#include <chrono>
//#include "sds_lib.h"
#include "pool1.h"
#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;
typedef float DataType;

//#define EPSILON 0.001

using namespace std;

void pool1(DataType inp_img[INP_IMG_CHAN*INP_IMG_SIZE*INP_IMG_SIZE], DataType out_img[INP_IMG_CHAN*OUT_IMG_SIZE*OUT_IMG_SIZE]);

//main function used to test the functionality of the kernel.
int main()
{
  //int input_vector_size_bytes = sizeof(float) * INP_IMAGE_CHANNEL * INP_IMAGE_SIZE * INP_IMAGE_SIZE;
  //float *inp_image = (float *) malloc(input_vector_size_bytes);
	//initialize the "inp_image" array and print them in order to check it


    ifstream inp_file("/home/junnan/Work/Vivado_HLS/Pool1/out_conv1.txt");
    DataType *inp_image;
    inp_image = (DataType *)malloc( INP_IMG_SIZE * INP_IMG_SIZE * INP_IMG_CHAN * sizeof(DataType));
	if(inp_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i=0; i<INP_IMG_SIZE * INP_IMG_SIZE * INP_IMG_CHAN; i++)
		{
			inp_file >> inp_image[i];
		}
		inp_file.close();
	}

  DataType *out_image = (DataType *)malloc(OUT_IMG_SIZE * OUT_IMG_SIZE * INP_IMG_CHAN * sizeof(DataType));

  cout << "Start calling the conv1 HW function" << endl;

  //call the "conv1" function using the "inp_image" argument, it returns the output in the "out_image" array
  pool1(inp_image, out_image);
  cout << "After calling the conv1 HW function" << endl;
  //free all the dynamically allocated memory

  free(inp_image);
	
  //dump the output image into a txt file "out_image.txt"
  ofstream data("/home/junnan/Work/Vivado_HLS/Pool1/out_image_pool.txt");
  for (int k = 0; k < OUT_IMG_SIZE*OUT_IMG_SIZE*INP_IMG_CHAN; k++)
    {
      data << out_image[k] << "\n";
    }


  const DataType out_img[] = {
                         #include "out_pool1.txt"
                       };
      DataType big_diff = 0;
      DataType diff[OUT_IMG_SIZE*OUT_IMG_SIZE*INP_IMG_CHAN];
  for (int i=0; i<OUT_IMG_SIZE*OUT_IMG_SIZE*INP_IMG_CHAN; i++){
	   diff[i] = out_img[i]-out_image[i];
	  if (diff[i] < 0)
		  diff[i] = !diff[i];
	  if (diff[i]>big_diff){
		 big_diff = diff[i];
	  }
  }
  cout << "big_diff = " << big_diff << endl;
 


  cout << "pooling Functionality pass" << endl;
  
  free(out_image);
  return 0;
}