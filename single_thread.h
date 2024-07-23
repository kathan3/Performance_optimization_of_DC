//function call by the main
void singleThread(register int input_row,
                register int input_col,
                  register int *input,
                  register int kernel_row,
                  register int kernel_col,
                  register int *kernel,
                  register int output_row,
                  register int output_col,
                  register long long unsigned int *output)
{
    
    for (int i = 0; i < output_row * output_col; ++i)
        output[i] = 0;

    //declaring all the variables which will be required in the optmisation
    register int a;
    register int input_i;
    register int b;
    register int c;
    register int input_j1;
    register int input_j2;
    register int input_j3;
    register int input_j4;
    register int input_j5;
    register int input_j6;
    register int input_j7;
    register int input_j8;
    register int d;
    register int e;
    register int f;
    register int g;


    // optmising the single threaded code by using techniques such as changing loop order ,
    // loop unrolling , etc mentioned in the report
    for(int output_i = 0; output_i< output_row; output_i++)
    {
        a=output_i * output_col;
        for(int kernel_i = 0; kernel_i< kernel_row; kernel_i++)
        {
            input_i = (output_i + 2*kernel_i) % input_row;
            b=input_i*input_col;
            c=kernel_i*kernel_col;
            for(int kernel_j = 0; kernel_j< kernel_col; kernel_j++)
            {
                d = c + kernel_j;
                e =2*kernel_j;

                for(int output_j = 0; output_j<output_col; output_j+=8)
                {
                    f=output_j + e ;
                    g=output_j;
                    if(output_j + e+9<input_col){
                        input_j1=f;
                        input_j2 = ++f;
                        input_j3 = ++f;
                        input_j4 = ++f;
                        input_j5 = ++f;
                        input_j6 = ++f;
                        input_j7 = ++f;
                        input_j8 = ++f;
                    }
                    else{
                        input_j1 = f % input_col;
                        input_j2 = (++f) % input_col;
                        input_j3 = (++f) % input_col;
                        input_j4 = (++f) % input_col;
                        input_j5 = (++f) % input_col;
                        input_j6 = (++f) % input_col;
                        input_j7 = (++f) % input_col;
                        input_j8 = (++f) % input_col;
                    } 
                    output[a+g] += input[b + input_j1] * kernel[d];
                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j2] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j3] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j4] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j5] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j6] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j7] * kernel[d];

                    if(g+1 >= output_col) continue;
                    output[a + ++g] += input[b + input_j8] * kernel[d];
                }
            }
        }
    }

}