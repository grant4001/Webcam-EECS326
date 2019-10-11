/*
 * camera.h
 *
 * Created: 2/9/2019 9:11:40 AM
 *  Author: mhjas
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_
#include <asf.h>
#include "ov2640.h"

//pins for TWI interface
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, \
PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	\
PIO_PERIPH_A, PIO_DEFAULT}

/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     (400000UL)

#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn

/** OV_RST pin definition */
#define OV_RST_GPIO                    PIO_PA20_IDX
#define OV_RST_FLAGS                   (PIO_OUTPUT_1 | PIO_DEFAULT)
#define OV_RST_MASK                    PIO_PA20
#define OV_RST_PIO                     PIOA
#define OV_RST_ID                      ID_PIOA
#define OV_RST_TYPE                    PIO_OUTPUT_1

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP



/** OV Data Bus pins */
#define OV_DATA_BUS_D0                 PIO_PA24_IDX
#define OV_DATA_BUS_D1                 PIO_PA25_IDX
#define OV_DATA_BUS_D2                 PIO_PA26_IDX
#define OV_DATA_BUS_D3                 PIO_PA27_IDX
#define OV_DATA_BUS_D4                 PIO_PA28_IDX
#define OV_DATA_BUS_D5                 PIO_PA29_IDX
#define OV_DATA_BUS_D6                 PIO_PA30_IDX
#define OV_DATA_BUS_D7                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

/* Image sensor board defines. */
// Image sensor Power pin.
#define OV_POWER_PIO                   PIOA
#define OV_POWER_MASK                  PIO_PA20


#define VSYNC_PIN_NUM				PIO_PA15
#define VSYNC_PIN_PIO				PIOA
#define VSYNC_PIN_ID				ID_PIOA
#define VSYNC_PIN_MASK				PIO_PA15_IDX
#define VSYNC_PIN_ATTR				PIO_PULLUP


#define MAX_IMAGE_SIZE 50000
uint8_t image_buffer[MAX_IMAGE_SIZE];
volatile uint32_t image_start_idx;
volatile uint32_t image_end_idx;
volatile uint32_t vsync_flag;

void vsync_handler(uint32_t ul_id, uint32_t ul_mask);
void init_vsync_interrupts(void);
void configure_twi(void);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);
uint8_t find_image_len(void);




#endif /* CAMERA_H_ */