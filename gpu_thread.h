//Below code if without constant memory usage
// __global__ void gpumultiplication(int * input ,int irow,int icol, int * kernel, int krow,int kcol, long long unsigned int * output,int rowlen , int collen)
// { 
//     int ROW = blockIdx.y*blockDim.y+threadIdx.y;
//     int COL = blockIdx.x*blockDim.x+threadIdx.x;
//      if (ROW < rowlen && COL < collen) {
//             int sum = 0;
//             for(int kernel_i = 0; kernel_i< krow; kernel_i++)
//             {
//                 for(int kernel_j = 0; kernel_j< kcol; kernel_j++)
//                 {
//                     int input_i = (ROW + 2*kernel_i) % irow;
//                     int input_j = (COL + 2*kernel_j) % icol;
//                     sum += input[input_i*icol +input_j]* kernel[kernel_i*kcol +kernel_j];
//                 }
//             }
//             output[ROW*collen + COL] = sum;
//     }
// }
// void gpuThread( int input_row,int input_col,int *input,int kernel_row,int kernel_col,int *kernel,int output_row,int output_col,
//                 long long unsigned int *output )
// {
//     // Declare pointers for input, kernel, and output matrice
//     int * d_i,*d_k;
//    long long unsigned int * d_o;
//     // Calculate the total size (in bytes) for input, kernel, and output matrices
//     size_t i = input_col*input_row*sizeof(int);
//     size_t k = kernel_col*kernel_row*sizeof(int);
//     size_t o = output_col*output_row*sizeof(long long unsigned int);
// // Allocate device memory for input, kernel, and output matrices
//     cudaMalloc((void**)&d_i, i);
//     cudaMalloc((void**)&d_o,o);
//     cudaMalloc((void**)&d_k,k);
// // Copy the input matrix from host to device
//     cudaMemcpy(d_i,input,i,cudaMemcpyHostToDevice);
// // Copy the kernel matrix to device constant memory
//     cudaMemcpy(d_k,kernel,k,cudaMemcpyHostToDevice);
//     dim3 dimBlock(32,32,1); //32*32 thread block size
//     dim3 dimGrid(ceil(output_col/32.0),ceil(output_row/32.0)); //Grid dimensions based on output matrix size
//     gpumultiplication<<<dimGrid,dimBlock>>>(d_i,input_row,input_col,d_k,kernel_row,kernel_col,d_o,output_row,output_col);// Launch the GPU kernel for matrix multiplication
//     cudaMemcpy(output,d_o,output_col*output_row*sizeof(long long unsigned int),cudaMemcpyDeviceToHost);// Copy the result matrix from device to host
// // Free allocated memory
// cudaFree(d_i);
// cudaFree(d_o);
// }

// This code is for withconstant memory execution
__constant__ int d_k[16384];

__global__ void gpumultiplication(int * input ,int irow,int icol, int krow,int kcol, long long unsigned int * output,int rowlen , int collen)
{ 
    int ROW = blockIdx.y*blockDim.y+threadIdx.y;
    int COL = blockIdx.x*blockDim.x+threadIdx.x;
     if (ROW < rowlen && COL < collen) {
            int sum = 0;
            for(int kernel_i = 0; kernel_i< krow; kernel_i++)
            {
                for(int kernel_j = 0; kernel_j< kcol; kernel_j++)
                {
                    int input_i = (ROW + 2*kernel_i) % irow;
                    int input_j = (COL + 2*kernel_j) % icol;
                    sum += input[input_i*icol +input_j]* d_k[kernel_i*kcol +kernel_j];
                }
            }
            output[ROW*collen + COL] = sum;
    }
}
void gpuThread( int input_row,int input_col,int *input,int kernel_row,int kernel_col,int *kernel,int output_row,int output_col,
                long long unsigned int *output )
{
    // Declare pointers for input, kernel, and output matrice
    int *d_i,*d_k;
    long long unsigned int * d_o;
    // Calculate the total size (in bytes) for input, kernel, and output matrices
    size_t i = input_col*input_row*sizeof(int);
    size_t k = kernel_col*kernel_row*sizeof(int);
    size_t o = output_col*output_row*sizeof(long long unsigned int);
    // Allocate device memory for input, and output matrices
    cudaMalloc((void**)&d_i, i);
    cudaMalloc((void**)&d_o,o);
    // Copy the input matrix from host to device
    cudaMemcpy(d_i,input,i,cudaMemcpyHostToDevice);
     // Copy the kernel matrix to device constant memory
    cudaMemcpyToSymbol(d_k,kernel,k,0,cudaMemcpyHostToDevice);
    // Define CUDA thread block and grid dimensions
    dim3 dimBlock(32,32,1); // 32x32 thread block
    dim3 dimGrid(ceil(output_col/32.0),ceil(output_row/32.0));// Grid dimensions based on output matrix size
    // Launch the GPU kernel for matrix multiplication
    gpumultiplication<<<dimGrid,dimBlock>>>(d_i,input_row,input_col,kernel_row,kernel_col,d_o,output_row,output_col);
   // Copy the result matrix from device to host
    cudaMemcpy(output,d_o,output_col*output_row*sizeof(long long unsigned int),cudaMemcpyDeviceToHost);
   // Free allocated memory
    cudaFree(d_i);
    cudaFree(d_o);
}