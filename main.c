/* ------------------------------------------//
 * Copyright (c) 2019, Synopsys, Inc. All rights reserved.

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

#include "settings.h"
#include "input_buffer.h"

#if defined(BOARD_IOTDK)
#define MPU9250_IIC_ID DFSS_IIC_0_ID
#else
#error Board isnt supported, check BOARD variable
#endif

#define COUNT BOARD_CPU_CLOCK/1000
#define IOTDK_LED_ID		DFSS_GPIO_4B2_ID
#define IOTDK_LED_PIN		0

volatile static int t0 = 0;
volatile static int second = 1;
int state=0;


static int16_t input_buffer[INPUT_FRAME_SIZE * INPUT_CHANNELS_NUM * INPUT_BUFFERING_DEPTH];
static uint32_t frame_position;
extern uint32_t read_done;
int test[]={
    0, 1, 2, 3, 4, 5
};

/** arc timer 0 interrupt routine */
static void timer0_isr(void *ptr)
{
	timer_int_clear(TIMER_0);
	t0++;
}

/** arc timer 0 interrupt delay */
void timer0_delay_ms(int ms)
{
	t0 = 0;
	//led enable

	while(t0 < ms);
	{
		
	
	}
	
}



int main(void)
{
    int16_t *current_frame;
    int16_t count=0;

    pll_fout_config(CORE_FREQ_MHZ);

    input_buffer_init();

    /* Start first frame read */
    input_buffer_next_read(input_buffer);

	//timer enable
	int_disable(INTNO_TIMER0);
	timer_stop(TIMER_0);

	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_pri_set(INTNO_TIMER0, INT_PRI_MIN);

	EMBARC_PRINTF("\r\nThis is a example about timer interrupt.\r\n");

	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);

    do {
        current_frame = &input_buffer[frame_position * INPUT_FRAME_SIZE * INPUT_CHANNELS_NUM];
        /*current_frame=test[count];
        count++;
        if (count==5)
            count =0;
        */ 
        while(read_done == 0) {
            //doing nothing
            EMBARC_PRINTF("\r\nDoes this input exist?  YES\r\n");
        }

        if(++frame_position == INPUT_BUFFERING_DEPTH) {
            frame_position = 0;
        }
        
        /* reading next buffer */
        input_buffer_next_read(&input_buffer[frame_position * INPUT_FRAME_SIZE * INPUT_CHANNELS_NUM]);

        /*
         * TODO: add data frame processing here, use 'current_frame' pointer
         *  
         *    process(current_frame);
         *   
         */
         //EMBARC_PRINTF("\r%d \r\n",*current_frame);
         //timer0_delay_ms(1000);
    } while(1);


    return 0;
}
