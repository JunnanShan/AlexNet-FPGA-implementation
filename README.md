# AlexNet-FPGA-implementation

This repository contains all the parameters you need to synthesize the AlexNet by using Vivado High Level Synthesis.

# Intro to AlexNet

There are the basic layers of AlexNet in the following:

 1. Conv1 + Relu  
  Input dimention: 3 * 227 * 227   
  Output dimention: 96 * 55 * 55  
  Filter dimention: 96 * 3 * 11 * 11  
  group: 1
  
 2. Pool1   
  Input dimention: 96 * 55 * 55   
  Output dimention: 96 * 27 * 27  
  Window dimention: 3 * 3   
  
 3. Norm1  
  Input dimention: 96 * 27 * 27   
  Output dimention: 96 * 27 * 27  
  local size: 5  
  alpha: 0.0001  
  Beta: 0.75
  
 4. Pad1  
  Input dimention: 96 * 27 * 27   
  Output dimention: 96 * 31 * 31  
  
 5. Conv2 + Relu  
  Input dimention: 96 * 31 * 31    
  Output dimention: 256 * 27 * 27  
  Filter dimention: 256 * 38 * 5 * 5  
  group: 2  
  
 6. Pool2  
  Input dimention: 256 * 27 * 27   
  Output dimention: 256 * 13 * 13  
  Window dimention: 3 * 3   
  
 7. Norm2  
  Input dimention: 256 * 13 * 13   
  Output dimention: 256 * 13 * 13  
  local size: 5  
  alpha: 0.0001  
  Beta: 0.75
  
 8. Pad2  
  Input dimention: 256 * 13 * 13   
  Output dimention: 256 * 15 * 15  
  
 9. Conv3 + Relu  
  Input dimention: 256 * 15 * 15   
  Output dimention: 384 * 13 * 13  
  Filter dimention: 384 * 256 * 3 * 3  
  group: 1  
  
 10. Pad3  
  Input dimention: 384 * 13 * 13   
  Output dimention: 384 * 15 * 15  

 11. Conv4 + Relu  
  Input dimention: 384 * 15 * 15    
  Output dimention: 384 * 13 * 13  
  Filter dimention: 384 * 192 * 3 * 3  
  group: 2  
  
 12. Pad4  
  Input dimention: 384 * 13 * 13    
  Output dimention: 384 * 15 * 15  
  
 13. Conv5 + Relu  
  Input dimention: 384 * 15 * 15   
  Output dimention: 256 * 13 * 13  
  Filter dimention: 256 * 192 * 3 * 3  
  group: 2  
  
 14. Pool5  
  Input dimention: 256 * 13 * 13   
  Output dimention: 256 * 6 * 6  
  Window dimention: 3 * 3    

All the parameters(Weights and Bias for convolution layers) are geneerated using "Caffe - Deep learning framwork". Each layer mentioned before is an independed Vivado HLS project. Each project contains the required files(testbench, header, top level function, and the Weights and Bias). All the computations are performed using single floating point data type. 

# Work environment

Tool: Vivado HLS 2017.3.1  
Target FPGA: xcvu9p-flgb2104-2-i


| BRAM| DSP| FF| LUT|URAM|  
| ----|----|---|----|--- |  
| 4320|6840|2364480|1182240|960|  







