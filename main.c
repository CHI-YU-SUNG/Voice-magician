/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_syscalls.c"	//for dfss_uart
#include "dev_pinmux.h"	//for io_arduino_config_uart //board\iotdk\drivers\mux\mux.c
#include <stdio.h>
#include <stdlib.h>		//for atoi

#define int_array_size 1000	//size of int_array
#define layer_size 26
int last_layer = 0;	//record the last layer in int_array
int last_count;	//record the last element in last_layer th layer in int_array
int int_array[layer_size][int_array_size];	//store received numbers in this array
int input_channel_size = 0;	//number of total input element
int output_channel_size;	//number of total output element
int windowSize = 1;	//different by input voice
int batch_size = 1;
//int input[layer_size+1][int_array_size];
int previous=0;
int pre_sum1=0;
int pre_sum2=0;

void received_function(DEV_UART_PTR dfss_uart_ptr){
	char temp_int[20];	//temporary store received number
	int temp_count = 0;	//length of receivedd number in temp_int
	last_count = 0;
	char receive_buffer[1];
	int receive_length;
	bool first=true;
	bool second=false;
	while (1) {
		receive_length = dfss_uart_ptr -> uart_read(receive_buffer, sizeof(receive_buffer));
		if (receive_length) {
			if ((int)receive_buffer[0] == 'a'){	//have received complete number
				temp_int[temp_count] = '\0';
				if (first == true){
					batch_size = atoi(temp_int);
					printf("input batch size = %d\n", batch_size);
					second = true;
					first = false;
					temp_count = 0;
					continue;
				}
				else if (second == true){
					windowSize = atoi(temp_int);
					printf("windowSize = %d\n", windowSize);
					second = false;
					temp_count = 0;
					continue;
				}
				int_array[last_layer][last_count] = atoi(temp_int);
				EMBARC_PRINTF("%d\n",int_array[last_layer][last_count]);
				input_channel_size ++;
				last_count ++;
				if (last_count >= int_array_size){
					last_layer ++;
					last_count = 0;
				}
				temp_count = 0;
				continue;
			}
			else if ((int)receive_buffer[0] == 'b'){	//stop receive
				EMBARC_PRINTF("quit\n");
				return ;
			}
			else{	//store one char
				temp_int[temp_count] = receive_buffer[0];
				temp_count ++;
			}
		}
	}
}

int array_insert (int insertNumber, int index, int elements){
	int lastNumber, nowNumber;
	int x,y;
	for (int number_count = 0; number_count < elements + 1; number_count++){
		if (number_count < index)
			continue;
		x = number_count / int_array_size;
		y = number_count % int_array_size;
		if (number_count == index){
			lastNumber = int_array[x][y];
			int_array[x][y] = insertNumber;
		}
		else {
			nowNumber = int_array[x][y];
			int_array[x][y] = lastNumber;
			lastNumber = nowNumber;
		}
	}
	return (elements + 1);
}
int runningsum (int Sum, int elements){
	int x,y;
	for (int number_count = 0; number_count < elements; number_count++){
		x = number_count / int_array_size;
		y = number_count % int_array_size;
		int_array[x][y] += Sum;
		Sum = int_array[x][y];
	}
	return Sum;
}
void array_sub_divide (int bias, int Denominator, int elements, int temp_array[], int *last_temp){
	int x1, y1;
	int x2, y2;
	for (int number_count = 0; number_count < elements; number_count++){
		x1 = (number_count + bias) / int_array_size;
		y1 = (number_count + bias) % int_array_size;
		x2 = number_count / int_array_size;
		y2 = number_count % int_array_size;
		int_array[x2][y2] = (int_array[x1][y1] - int_array[x2][y2])/Denominator;
	}
	for (int number_count = elements, last_temp = 0; number_count < elements + bias; number_count++, last_temp++)
		temp_array[last_temp] = int_array[number_count / int_array_size][number_count % int_array_size];
}
/*
void possess(int nowbatch){
	
	double runningsum_one[layer_size+1][int_array_size];
	double runningsum_two[layer_size+1][int_array_size];
	if(nowbatch==0){
		//set first matrix
		input[0][0]=0;
		runningsum_one[0][0]=0;
		runningsum_two[0][0]=0;
	}else{
		input[0][0]=previous;
		runningsum_one[0][0]=pre_sum1;
		runningsum_two[0][0]=pre_sum2;//non define pre_sum1 and pre_sum2
	}
	
	for(int i=0;i<layer_size;i++){
		for(int j=0;j<int_array_size;j++){
			for(int m=0;m<i;m++){
				for(int n=0;n<j;n++){
					if(j+1==int_array_size)
						previous+=int_array[m][n];//bug
					else
						input[i][j+1]+=int_array[m][n];
				}
			}
		}
	}
	//去尾(可省略)
	if(nowbatch == batch_size){
		for(int i=0;i<layer_size+1;i++){	
			for(int j=0;j<int_array_size;j++){	
				if(i==layer_size && j>(int_array_size-1)-windowSize-1)
					runningsum_one[i+1][j]=0;
				else
					runningsum_one[i][j]=input[i][j];						
			}
		}	
	}
	
	//去頭
	if(nowbatch ==0){
		for(int i=0;i<layer_size;i++){	
			for(int j=0;j<int_array_size;j++){	
				if(j+windowSize>int_array_size)
					runningsum_two[i][j]=input[i+1][j+windowSize-int_array_size];
				else
					runningsum_two[i][j]=input[i][j+windowSize];				
			}
		}
	}
	
	
	//相減相除
	if(nowbatch == batch_size){
		for(int i=0;i<layer_size;i++)
			for(int j=0;j<int_array_size;j++)
				if(i==layer_size && j>int_array_size-windowSize)
					int_array[i][j]=0;
				else	
					int_array[i][j]=(int)(runningsum_two[i][j]-runningsum_one[i][j])/windowSize;
	}
	else{
		int_array[i][j]=(int)(runningsum_two[i][j]-runningsum_one[i][j])/windowSize;
	
	}
*/
void transmmit_function(DEV_UART_PTR dfss_uart_ptr){
	char trans_buffer[20];
	int_array[0][0]=123;
	int_array[0][1]=54;
	int_array[0][2]=21654;
	output_channel_size = 2;
	last_layer=0;
	last_count=3;
	/*transmit data*/
	int nowTrans;
	for (int number_count = 0; number_count < output_channel_size; number_count++){
		nowTrans = 54;
		sprintf (trans_buffer, "%d\n",  nowTrans);
		printf("transmit %d th = %s", number_count, trans_buffer);
		dfss_uart_ptr -> uart_close();
		dfss_uart_ptr -> uart_write(trans_buffer, sizeof(trans_buffer));
	}
	/*transmit stop flag */
	/*
	sprintf (trans_buffer, "end\n");
	printf("transmit %s", trans_buffer);
	dfss_uart_ptr -> uart_write(trans_buffer, sizeof(trans_buffer));
	/*
	for (int i = 0; i < last_layer; i++){
		for (int j = 0; j < int_array_size; j++){
			sprintf (trans_buffer, "%d\n", int_array[i][j]);
			EMBARC_PRINTF("transmit %s", trans_buffer);
			dfss_uart_ptr -> uart_write(trans_buffer, sizeof(trans_buffer));
		}
	}
	for (int j = 0; j < last_count ; j++){
		sprintf (trans_buffer, "%d\n", int_array[last_layer][j]);
		EMBARC_PRINTF("transmit %s", trans_buffer);
		dfss_uart_ptr -> uart_write(trans_buffer, sizeof(trans_buffer));
	}
*/
	return;
}

/*void lowPassFrequency(double* output, int points)
{
	int SAMPLE_RATE=115200;
	int CUTOFF = 0.9;
	double *input = int_array;
    /*
	double RC = 1.0/(CUTOFF*2*3.14); 
    double dt = 1.0/SAMPLE_RATE; 
    double alpha = dt/(RC+dt);
    output[0] = input[0];
    for(int i = 1; i < points; ++i)
    { 
        output[i] = output[i-1] + (alpha*(input[i] - output[i-1]));
    } 
	
	output[0] = input[0]+1;
	for(int i = 1; i < points; i++){
		output[i] = output[i-1] *CUTOFF;
	}
	
	
}*/  
   
int main(void)
{
	/*uart initialize*/
	io_arduino_config_uart(1);	//enable arduino uart port
	DEV_UART_PTR dfss_uart_ptr = uart_get_dev(2);	//use dfss_uart_2
	dfss_uart_ptr -> uart_close();
	dfss_uart_ptr -> uart_open(115200);
	printf(" open successful\n");
	//transmmit_function(dfss_uart_ptr);
	int Sum;

	while (true){
		Sum = 0;
		for (int nowbatch = 0; nowbatch < batch_size; nowbatch++){
			/*received data*/
			received_function(dfss_uart_ptr);
			/*
			for (int number_count = 0; number_count < input_channel_size; number_count++){
				printf ("%d th = %d\n", number_count, int_array[number_count / int_array_size][number_count % int_array_size]);
			}
			*/
			printf ("total number = %d\n", input_channel_size);
			//output_channel_size = input_channel_size;

			/*prossess*/
			//possess();

			int temp_array[windowSize];
			int last_temp = 0;
			
			if (nowbatch == 0){
				output_channel_size = array_insert(0,0,input_channel_size);
				Sum = runningsum(int_array[0][0], output_channel_size);
			}
			else {
				Sum = runningsum(Sum, input_channel_size);
				if (last_temp > 0)
					for (int i = 0; i < last_temp; i++)
						output_channel_size = array_insert(temp_array[last_temp - 1 - i], 0, input_channel_size);
				if (nowbatch == batch_size - 1)
					output_channel_size -= windowSize;
			}
			output_channel_size -= windowSize;
			array_sub_divide(windowSize, windowSize, output_channel_size, temp_array, &last_temp);

			//lowPassFrequency(output_double_array, 2);
			/*
			for (int i=0; i < last_count; i++){
				EMBARC_PRINTF("output:%d th = %f\n", i, output_double_array[i]);
				
			}
			*/
			
			//transmmit_function(dfss_uart_ptr);
		}
	}
}