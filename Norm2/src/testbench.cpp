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
//#include <chrono>
//#include "sds_lib.h"
#include "norm.h"
//#include "ap_fixed.h"

//typedef ap_fixed<32,16> DataType;
typedef float DataType;


using namespace std;

void norm2(DataType inp_img[INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL],
	  DataType out_img[OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL]);


//main function used to test the functionality of the kernel.
int main()
{

    ifstream inp_file("/home/junnan/Work/Vivado_HLS/Norm2/out_pool2.txt");
    DataType *inp_image;
    inp_image = (DataType *)malloc( INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL * sizeof(DataType));
	if(inp_file.is_open())
	{
		cout << "can open the text file" << endl;


		for (int i=0; i<INP_IMAGE_SIZE * INP_IMAGE_SIZE * INP_IMAGE_CHANNEL; i++)
		{
			inp_file >> inp_image[i];
			//cout<< "inp_image[" << i << "] = " << inp_image[i] << endl;
		}
		inp_file.close();
	}
    cout << "inp_image[0] = " << inp_image[0] << endl;
	
  DataType *out_image = (DataType *)malloc(OUT_IMAGE_SIZE * OUT_IMAGE_SIZE * OUT_IMAGE_CHANNEL * sizeof(DataType));

  cout << "Start calling the conv1 HW function" << endl;


  //call the "conv1" function using the "inp_image" argument, it returns the output in the "out_image" array
  norm2(inp_image, out_image);

  cout << "After calling the conv1 HW function" << endl;

  //free all the dynamically allocated memory

  free(inp_image);

  //dump the output image into a txt file "out_image.txt"
  ofstream data("/home/junnan/Work/Vivado_HLS/Norm2/out_image.txt");
  for (int k = 0; k < OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*OUT_IMAGE_CHANNEL; k++)
    {
      data << out_image[k] << "\n";
    }


  const DataType out_img[] = {
                         #include "out_norm2.txt"
                       };
	
      DataType big_diff = 0;
      DataType diff[OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*OUT_IMAGE_CHANNEL];
  for (int i=0; i<OUT_IMAGE_SIZE*OUT_IMAGE_SIZE*OUT_IMAGE_CHANNEL; i++){
	   diff[i] = out_img[i]-out_image[i];
	  if (diff[i] < 0)
		  diff[i] = !diff[i];
	  if (diff[i]>big_diff){
		 big_diff = diff[i];
	  }
  }
  cout << "big_diff = " << big_diff << endl;


  cout << "Functionality pass" << endl;

  free(out_image);
  return 0;
}
