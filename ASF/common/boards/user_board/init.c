/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "camera.h"

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	ioport_init();
	
	
	//pin to read connection status of wifi chip
	ioport_set_pin_dir(WIFI_CONN_STATUS_PIN,IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(WIFI_CHIP_RESET_PIN,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CHIP_RESET_PIN,true);
	
	//set up button to reset wifi chip
	ioport_set_pin_level(WIFI_SETUP_BUTTON, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(WIFI_SETUP_BUTTON, IOPORT_MODE_PULLDOWN);
	
	// Pull CTS pin of WiFi low to enable communication
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
	
	//configure camera pins
	//camera twi pins
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	//camera clock
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
	

	
	/*
	ioport_set_pin_dir(OV_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OV_RST_GPIO,true);
	ioport_set_pin_dir(OV_HSYNC_GPIO,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_VSYNC_GPIO,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D2,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D3,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D4,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D5,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D6,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D7,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D8,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(OV_DATA_BUS_D9,IOPORT_DIR_INPUT);
	*/
}
