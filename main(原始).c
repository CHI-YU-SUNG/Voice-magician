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
#include "dev_pinmux.h"	//for io_arduino_config_uart
#include <stdlib.h>		//for atoi

#define input_array_size 100

int main(void)
{
	char temp_int[20];
	int temp_count = 0;
	int input_int_array[input_array_size];
	int input_count = 0;
	char receive_buffer[1];
	int receive_length;
	io_arduino_config_uart(1);
	DEV_UART_PTR dfss_uart_ptr = uart_get_dev(2);
	dfss_uart_ptr -> uart_close();
	dfss_uart_ptr -> uart_open(115200);
	EMBARC_PRINTF(" open successfal\n");

	while (1) {
		receive_length = dfss_uart_ptr -> uart_read(receive_buffer, sizeof(receive_buffer));
		if (receive_length) {
			int compare = (int)receive_buffer[0];
			if ((int)receive_buffer[0] == 'a'){
				temp_int[temp_count] = '\0';
				input_int_array[input_count] = atoi(temp_int);
				input_count ++;
				temp_count = 0;
				EMBARC_PRINTF("%d\n", input_int_array[input_count-1]);
				continue;
			}
			else{
				temp_int[temp_count] = receive_buffer[0];
				temp_count ++;
			}
		}
	}
}