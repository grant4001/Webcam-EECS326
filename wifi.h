/*
 * wifi.h
 *
 * Created: 2/7/2019 4:27:42 PM
 *  Author: mhjas
 */ 


#ifndef WIFI_H_
#define WIFI_H_
#include <asf.h>
#include <string.h>
#include "timer_interface.h"

#define WIFI_USART					USART0
#define WIFI_USART_ID				ID_USART0
#define WIFI_USART_BAUDRATE			921600
#define WIFI_USART_HANDLER			USART0_Handler
#define WIFI_USART_IRQn				USART0_IRQn
#define WIFI_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define WIFI_USART_PARITY			US_MR_PAR_NO
#define WIFI_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define WIFI_USART_MODE				US_MR_CHMODE_NORMAL

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

#define ALL_INTERRUPT_MASK  0xffffffff

//Our comm pin is on PB0. This is subject to possible changes in future
#define WIFI_COMM_PIN_NUM			PIO_PB0
#define WIFI_COMM_PIO				PIOB
#define WIFI_COMM_ID				ID_PIOB
#define WIFI_COMM_MASK				PIO_PB0_IDX
#define WIFI_COMM_ATTR				PIO_IT_RISE_EDGE

//Our wifi setup button pin is on pa14.
#define WIFI_SETUP_PIN_NUM			PIO_PA14
#define WIFI_SETUP_PIO				PIOA
#define WIFI_SETUP_ID				ID_PIOA
#define WIFI_SETUP_MASK				PIO_PA14_IDX
#define WIFI_SETUP_ATTR				PIO_PULLUP|PIO_DEBOUNCE|PIO_IT_FALL_EDGE

#define MAX_INPUT_WIFI 1000
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile uint32_t input_pos_wifi;
volatile uint32_t received_byte_wifi;
//flags indicating the main function should do this
volatile uint32_t wifi_comm_success_flag;
volatile uint32_t wait_one_second_flag;
volatile uint32_t start_image_transfer_flag;
volatile uint32_t wifi_setup_flag;
volatile uint32_t image_is_transferring;

char transfer_comm[30];

void process_incoming_byte_wifi(uint8_t in_byte);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void process_data_wifi(void);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_setup_pin(void);
void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);

#endif /* WIFI_H_ */