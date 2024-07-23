#include <pthread.h>
//ThreadData structure is storing all the details that will be passed to each thread
struct ThreadData {
    int thread_id;
    int output_row_size;
    int output_col_size;
    int kernel_row_size;
    int input_row_size;
    int input_col_size;
    int kernel_col_size;
    int *input;
    int *kernel;    
    long long unsigned int *output;
};
    
//initializing number of threads we want to create
    const int NUM_THREADS = 12;
    pthread_t threads[NUM_THREADS];

// threadFunction is the subroutine that is being executed by each thread
void *threadFunction(void *arg) {
    ThreadData *data = static_cast<ThreadData *>(arg);

    // Assigning the number of output rows which will be assigned to each thread
    int rows_per_thread = data->output_row_size / NUM_THREADS;
    int start_row = data->thread_id * rows_per_thread;
    int end_row = (data->thread_id == NUM_THREADS - 1) ? data->output_row_size : start_row + rows_per_thread;
    int kernel_row=data->kernel_row_size;
    int kernel_col = data->kernel_col_size;
    int input_col= data->input_col_size;
    int input_row= data->input_row_size;
    long long unsigned int *output= data->output;
    int *input= data->input;
    int *kernel=data->kernel;
    int output_col=data->output_col_size;

    //declaring all the intermediate varibles required for effective calculations
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

    // code after applying optimisation techniques such as code motion , loop unrolling ,etc mentioned in the report 
    for(int output_i = start_row; output_i< end_row; output_i++)
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

                for(int output_j = 0; output_j< output_col; output_j+=8)
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




    pthread_exit(NULL);
}


// multiThread function which is called by the main function 
void multiThread( int input_row, 
                int input_col,
                int *input1, 
                int kernel_row, 
                int kernel_col, 
                int *kernel1,
                int output_row_size, 
                int output_col_size, 
                long long unsigned int *output1 ) 
{


    ThreadData thread_data[NUM_THREADS];


    //initializing all the threads with the values given from the main
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].output_row_size = output_row_size;
        thread_data[i].output_col_size = output_col_size;
        thread_data[i].kernel_row_size=kernel_row;
        thread_data[i].input_row_size=input_row;
        thread_data[i].input_col_size=input_col;
        thread_data[i].kernel_col_size=kernel_col;
        thread_data[i].input=input1;
        thread_data[i].kernel=kernel1;
        thread_data[i].output=output1;
    }

    // Creating threads and calling the function "threadFunction"
    for (int i = 0; i < NUM_THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, threadFunction, static_cast<void *>(&thread_data[i]));
        if (rc) {
            cerr << "Error creating thread: " << rc << endl;
            exit(-1);
        }
    }

    // Joining threads after completion of their execution 
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }


}