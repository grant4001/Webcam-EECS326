/*
 * camera.c
 *
 * Created: 2/9/2019 9:11:29 AM
 *  Author: mhjas
 */ 
#include "camera.h"

volatile uint32_t vsync_flag = 0;

void vsync_handler(uint32_t ul_id,uint32_t ul_mask){
	unused(ul_id);
	unused(ul_mask);
	
	vsync_flag = true;
}

void init_vsync_interrupts(void){
	
	//pmc_enable_periph_clk(VSYNC_PIN_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(VSYNC_PIN_PIO, VSYNC_PIN_ID, VSYNC_PIN_NUM, VSYNC_PIN_ATTR, vsync_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)VSYNC_PIN_ID);
	
	pio_enable_interrupt(VSYNC_PIN_PIO, VSYNC_PIN_NUM);
	
	
}

void configure_twi(void){
		
	twi_options_t opt;
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

static void pio_capture_init(Pio *p_pio, uint32_t ul_id){
	/* Enable periphral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);

	/* Samples only data with even index */
	//p_pio->PIO_PCMR |= PIO_PCMR_HALFS;
	//p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_FRSTS);

}

static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf,uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		} else {
		return 0;
	}
}

void init_camera(void){
	
		
		
	//gpio_configure_pin(OV_SW_OVT_GPIO, OV_SW_OVT_FLAGS);
	gpio_configure_pin(OV_RST_GPIO, OV_RST_FLAGS);
	//gpio_configure_pin(OV_FSIN_GPIO, OV_FSIN_FLAGS);
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(VSYNC_PIN_MASK, VSYNC_PIN_ATTR);
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	pmc_enable_pllbck(7, 0x1, 1);

	/* Init Vsync handler*/
	init_vsync_interrupts();
	pio_disable_interrupt(VSYNC_PIN_PIO, VSYNC_PIN_NUM);
	vsync_flag = 0;

	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);

	/* Turn on ov2640 image sensor using power pin */
	//ov_power(true, OV_POWER_PIO, OV_POWER_MASK);
	//ov_power(false, OV_POWER_PIO, OV_POWER_MASK);
	//ioport_set_pin_level(OV_RST_PIO,true);

	/* Init PCK1 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_2 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}

	configure_twi();

	/* ov2640 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* ov2640 configuration */
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);
	// ov_configure(BOARD_TWI, JPEG_640x480);
	/*
	ov_configure(BOARD_TWI, JPEG_INIT);
	//ov_configure(BOARD_TWI, YUV422);
	//ov_configure(BOARD_TWI, QVGA_YUV422_10FPS);
	//ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);
	ov_configure_finish(BOARD_TWI);*/

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}

void configure_camera(void){
}

uint8_t start_capture(void){	
	pio_enable_interrupt(VSYNC_PIN_PIO,VSYNC_PIN_NUM);
	
	while(!vsync_flag){}
	pio_disable_interrupt(VSYNC_PIN_PIO, VSYNC_PIN_NUM);
	vsync_flag = 0;
	
	
	pio_capture_enable(OV_DATA_BUS_PIO);
	
	pio_capture_to_buffer(OV_DATA_BUS_PIO, image_buffer,MAX_IMAGE_SIZE >> 2);
	
	while (!((OV_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
	PIO_PCIMR_RXBUFF)) {
	}
	
	vsync_flag = false;
	
	//return 0;
	uint8_t val = find_image_len();
	return val;
}

uint8_t find_image_len(void){
	uint8_t start_flag = 0;
	uint32_t idx;
	image_start_idx = 0;
	image_end_idx = 0;
	for(idx = 0; idx+1 < MAX_IMAGE_SIZE;idx++){
		if(image_buffer[idx] == 0xff && image_buffer[idx+1] == 0xd8){
			image_start_idx = idx;
			start_flag = 1;
		}
		if(image_buffer[idx] == 0xff && image_buffer[idx+1] == 0xd9 && start_flag == 1){
			image_end_idx = idx+2;
			return 1;
		}
	}
	image_start_idx = 0;
	image_end_idx = 0;
	
	return 0;
}
/*
uint8_t find_image_len(void){
	uint32_t idx = 0;
	uint32_t start_idx;
	
	while(!(image_buffer[idx] == 0xff && image_buffer[idx+1] == 0xd8)){
		idx++;
		if(idx+1 >= MAX_IMAGE_SIZE){
			return 0;
		}
	}
	start_idx = idx;
	image_start_idx = idx;
	while(!(image_buffer[idx] == 0xff && image_buffer[idx+1] == 0xd9)){
		idx++;
		if(idx+1 >= MAX_IMAGE_SIZE){
			return 0;
		}
	}
	uint32_t ptr = (uint32_t) image_buffer;
	uint16_t temp1 = image_buffer[start_idx];
	uint8_t temp2 = image_buffer[start_idx+1];
	uint32_t end_idx = idx+2;
	uint16_t temp3 = image_buffer[idx];
	uint16_t temp4 = image_buffer[idx+1];
	image_end_idx = idx+2;
	return 1;
}*/
