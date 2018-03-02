# AlexNet-
This repository contains all the parameters you need to synthesize the AlexNet by using Vivado High Level Synthesis.

This work includes the basic layers of AlexNet:

1. Conv1 + Relu
  Input dimention: 3 * 227 * 227 
  Output dimention: 96 * 55 * 55
  Filter dimention: 96 * 3 * 11 * 11
  
2. Pool1 
  Input dimention: 96 * 55 * 55 
  Output dimention: 96 * 27 * 27
  Window dimention:  
3. Norm1
4. Pad1
5. Conv2 + Relu
6. Pool2
7. Norm2
8. Pad2
9. Conv3 + Relu
10. Pad3
11. Conv4 + Relu
12. Pad4
13. Conv5 + Relu
14. Pool5

All the parameters(Weights and Bias for convolution layers) are geneerated using "Caffe - Deep learning framwork". Each layer mentioned before is an independed Vivado HLS project. Each project contains the required files(testbench, header, top level function, and the Weights and Bias). All the computations are performed using floating point data type. 

