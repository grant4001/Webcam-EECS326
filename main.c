/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "camera.h"
#include "wifi.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	
	//configure timer
	configure_tc();
	
	//configure wifi
	
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_setup_pin();
	wifi_setup_flag = 0;
	
	
	//configure camera
	//gpio_configure_pin(VSYNC_PIN_MASK, VSYNC_PIN_ATTR);
	//init_vsync_interrupts();
	
	init_camera();
	
	int ret = start_capture();
	
	/*
	pio_enable_interrupt(VSYNC_PIN_PIO,VSYNC_PIN_NUM);
	while(vsync_flag==0){};
	pio_disable_interrupt(VSYNC_PIN_PIO, VSYNC_PIN_NUM);
	vsync_flag = 0;
	
		
	pio_enable_interrupt(VSYNC_PIN_PIO,VSYNC_PIN_NUM);
	while(vsync_flag==0){};
	pio_disable_interrupt(VSYNC_PIN_PIO, VSYNC_PIN_NUM);
	vsync_flag = 0;*/
	
	//int ret = start_capture();
	//while(ret == 0);
	
	//reset chip
	ioport_set_pin_level(WIFI_CHIP_RESET_PIN,false);
	delay_ms(100);
	ioport_set_pin_level(WIFI_CHIP_RESET_PIN,true);
	
	write_wifi_command("set uart.flow 0 on\r\n",2);
	
	write_wifi_command("set bus.command.rx_bufsize 5000\r\n",2);
	write_wifi_command("set sy i g wlan 20\r\n",2);
	write_wifi_command("set sy i g network 18\r\n",2);
	write_wifi_command("set sy i g softap 21\r\n",2);
	write_wifi_command("set system.cmd.gpio 13\r\n",2);
	write_wifi_command("set wl n o 16\r\n",2);
	write_wifi_command("set sy c p off\r\n",2);
	write_wifi_command("set sy c e off\r\n",2);
	
	write_wifi_command("set uart.baud 0 921600\r\n",2);
	
	write_wifi_command("save\r\n",2);
	write_wifi_command("reboot\r\n",2);
	
	//write_wifi_command("setup web\r\n",10);
	
	while(ioport_get_pin_level(WIFI_CONN_STATUS_PIN) == 0){
		if(wifi_setup_flag){
			write_wifi_command("setup web\r\n",10);
			delay_ms(100);
			wifi_setup_flag = 0;
		}
	}
	//write_wifi_command("poll all\r\n",10);
	//delay_ms(5000);
	//wait_one_second_flag = 0;

	/* Insert application code here, after the board has been initialized. */
	while(1){
		if(wifi_setup_flag){
				write_wifi_command("setup web\r\n",10);
				delay_ms(100);
				wifi_setup_flag = 0;
				while(ioport_get_pin_level(WIFI_CONN_STATUS_PIN) == 0){
					if(wifi_setup_flag){
						write_wifi_command("setup web\r\n",10);
						wifi_setup_flag = 0;
					}
				}
		}
		while(ioport_get_pin_level(WIFI_CONN_STATUS_PIN) == 0){
			ioport_set_pin_level(WIFI_CHIP_RESET_PIN,false);
			delay_ms(100);
			ioport_set_pin_level(WIFI_CHIP_RESET_PIN,true);
			delay_ms(10000);
		}
		write_wifi_command("poll all\r\n",10);
		delay_ms(10);
		if(wait_one_second_flag == 1){
			delay_ms(1000);
			wait_one_second_flag = 0;
		}
		else{
			//TODO: capture image
			//delay_ms(2000);
			
			uint8_t image_is_good = 0;
			image_is_good = start_capture();
			if(image_is_good){
				write_image_to_file(); //write image and delay by 50ms
			}
		}
	}
}
