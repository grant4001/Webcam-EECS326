/*
 * wifi.c
 *
 * Created: 2/7/2019 4:27:15 PM
 *  Author: mhjas
 */ 
#include "wifi.h"
#include "camera.h"

volatile uint32_t received_byte_wifi = 0;
volatile uint32_t input_pos_wifi = 0;
volatile uint32_t wifi_setup_flag = 0;
volatile uint32_t wait_one_second_flag = 0;
volatile uint32_t start_image_transfer_flag = 0;
volatile uint32_t wifi_comm_success_flag = 0;
volatile uint32_t image_is_transferring = 0;

void WIFI_USART_HANDLER(void){
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

void process_incoming_byte_wifi(uint8_t in_byte){
	
	input_line_wifi[input_pos_wifi] = in_byte;
	input_pos_wifi++;
}

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask){
	
	unused(ul_id);
	unused(ul_mask);
	process_data_wifi();
	wifi_comm_success_flag = 1;
	for(int i = 0; i < MAX_INPUT_WIFI;i++){
		input_line_wifi[i] = 0;
	}
	input_pos_wifi = 0;
}

void process_data_wifi(void){
	//TODO: add more recognized commands
	if(strstr(input_line_wifi,"None")){
		wait_one_second_flag = 1;
	}
	if(strstr(input_line_wifi,"Image")){
		start_image_transfer_flag = 1;
	}
	
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask){
	unused(ul_id);
	unused(ul_mask);
	
	wifi_setup_flag = true;	
}

void configure_usart_wifi(void){
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}
void configure_wifi_setup_pin(void){
	pmc_enable_periph_clk(WIFI_SETUP_ID);
	
	pio_set_debounce_filter(WIFI_SETUP_PIO, WIFI_SETUP_MASK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_SETUP_PIO, WIFI_SETUP_ID, WIFI_SETUP_PIN_NUM, WIFI_SETUP_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_SETUP_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_SETUP_PIO, WIFI_SETUP_PIN_NUM);
}

void write_wifi_command(char* comm, uint8_t cnt){
	usart_write_line(WIFI_USART,comm);
	counts = 0;
	
	while(counts < cnt){
		if(wifi_comm_success_flag){
			wifi_comm_success_flag = 0;
			break;
		}
	}
}

void write_image_to_file(void){
	uint32_t idx = image_start_idx;
	uint32_t x;
	sprintf(transfer_comm,"image_transfer %lu\r\n\0",(image_end_idx - image_start_idx));
	write_wifi_command(transfer_comm,10);
	//delay_ms(10);
	if(start_image_transfer_flag){

	
		//uint8_t* pbuffer = &image_buffer[idx];
		//write_wifi_command(pbuffer,10);
		image_is_transferring = 1;
		while(idx < image_end_idx){
			usart_putchar(WIFI_USART,image_buffer[idx]);
			idx++;
		}
		/*
		for(x = 0; x < MAX_IMAGE_SIZE;x++){
			image_buffer[x] = 0;
		}*/
		image_is_transferring = 0;
		start_image_transfer_flag = 0;
	}
	delay_ms(10);
}
