// Davey Taylor, Arduino Verkstad AB
// OLED Display Driver

#include "driver_display.h"
#include <stm32f2xx_rcc.h>
#include <stm32f2xx_dma.h>
#include <stm32f2xx_spi.h>
#include <string.h>

// Waits for DMA transfer to complete
static void oled_wait_dma(void) {
  while(!DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)) {
    if(!DMA2_Stream3->NDTR) break;
  }
  DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);
  DMA_Cmd(DMA2_Stream3, DISABLE);
  SPI_I2S_DMACmd(OLED_SPI, SPI_I2S_DMAReq_Tx, DISABLE);
}

// Waits for SPI transfer to complete
static void oled_wait_spi(void) {
  while(SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_TXE) == 0);
  while(SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_BSY) == 1);
}

// Starts a DMA transfer to OLED
static void oled_dma(uint8_t *data, uint32_t count) {
  DMA2_Stream3->NDTR = count;
  DMA2_Stream3->M0AR = (uint32_t)data;
  DMA_Cmd(DMA2_Stream3, ENABLE);
  SPI_I2S_DMACmd(OLED_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
}

// Sends a command to OLED
static void oled_cmd(uint8_t cmd, uint32_t count, uint8_t *data) {
  digitalWrite(OLED_NCS, LOW);
  digitalWrite(OLED_A0, LOW);
  SPI_I2S_SendData(OLED_SPI, cmd);
  oled_wait_spi();
  digitalWrite(OLED_A0, HIGH);
  if(count) {
    oled_dma(data, count);
    oled_wait_dma();
    oled_wait_spi();
  }
  digitalWrite(OLED_NCS, HIGH);
}

// Ramps up the 1.8V (directly on = unstable behaviour)
static void oled_vramp(void) {
  const uint32_t count = 2000; // Sony says minimum 500
  for(uint32_t outer = 0; outer < count; outer++) {
    digitalWrite(OLED_VEN, HIGH);
    for(uint32_t inner = 0; inner < count; inner++) {
      if(inner == outer) digitalWrite(OLED_VEN, LOW);
    }
  }
  digitalWrite(OLED_VEN, HIGH);
}

// Initializes and turns on screen
void oled_init(void) {

  // Set initial pin states
  digitalWrite(OLED_VEN, LOW);
  digitalWrite(ENABLE_2V8, HIGH);
  digitalWrite(OLED_RESET, HIGH);
  digitalWrite(OLED_A0, HIGH);
  
  // SPI config
  {
    // Enable peripheral clock
    RCC_AHB1PeriphClockCmd(OLED_SPI_SCK->clock | OLED_SPI_MOSI->clock, ENABLE);
    
    // Enable SPI clock
    RCC_APB2PeriphClockCmd(OLED_SPI_CLK, ENABLE);

    //pinInit(OLED_SPI_SCK);
    //pinInit(OLED_SPI_MOSI);
    
    SPI_I2S_DeInit(OLED_SPI);
    
    SPI_InitTypeDef ConfigSPI;
  
    ConfigSPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    ConfigSPI.SPI_Mode = SPI_Mode_Master;
    ConfigSPI.SPI_DataSize = SPI_DataSize_8b;
    ConfigSPI.SPI_CPOL = SPI_CPOL_High;
    ConfigSPI.SPI_CPHA = SPI_CPHA_2Edge;
    ConfigSPI.SPI_NSS = SPI_NSS_Soft;
    ConfigSPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    ConfigSPI.SPI_FirstBit = SPI_FirstBit_MSB;
    
    SPI_Init(OLED_SPI, &ConfigSPI);
    
    SPI_Cmd(OLED_SPI, ENABLE);
  }
  
  // DMA config
  {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitTypeDef ConfigDMA;
    
    ConfigDMA.DMA_Channel = DMA_Channel_3;
    ConfigDMA.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    ConfigDMA.DMA_Memory0BaseAddr = 0;
    ConfigDMA.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    ConfigDMA.DMA_BufferSize = 0;
    ConfigDMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    ConfigDMA.DMA_MemoryInc = DMA_MemoryInc_Enable;
    ConfigDMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    ConfigDMA.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    ConfigDMA.DMA_Mode = DMA_Mode_Normal;
    ConfigDMA.DMA_Priority = DMA_Priority_High;
    ConfigDMA.DMA_FIFOMode = DMA_FIFOMode_Enable;
    ConfigDMA.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    ConfigDMA.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    ConfigDMA.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    DMA_Init(DMA2_Stream3, &ConfigDMA);    
  }

  // Cycle reset
  digitalWrite(OLED_RESET, LOW);
  delay(10);
  digitalWrite(OLED_RESET, HIGH);
  delay(10);

  // Do software reset
  oled_cmd(0x01, 0, 0);
  delay(10);
  
  // Initialization command list
  uint8_t init_cmds[] = {
    0x03, 0x01, 0x00, // Stand-by OFF
    0x07, 0x08, 0x00, 0x00, 0x07, 0x0F, 0x01, 0x0C, 0x09, 0x0C, // Set display size
    0x05, 0x01, 0x0A, // Set RAM write "direction"
    0x06, 0x01, 0x01, // Set RAM scanning "direction"
    0x04, 0x01, 0x02, // Set internal oscillator to 90Hz
    0x1F, 0x01, 0x30, // Set row scan operations = ALL
    0x30, 0x01, 0x12, // Set regulator to ???
    0x1C, 0x01, 0x0F, // Set pre-charge width
    0x1E, 0x01, 0x00, // Set peak pulse delay
    0x0F, 0x03, 0x0A, 0x0A, 0x0A, // Set peak currents (RGB)
    0x1D, 0x03, 0x05, 0x05, 0x05, // Set peak pulse widths (RGB)
    0x08, 0x01, 0x01, // Set interface type to 8-bit
    0x00 // END
  };

  // Send initializiation commands
  uint8_t *p_cmds = init_cmds;
  while(*p_cmds != 0x00) {
    uint8_t cmd = *p_cmds++;
    uint8_t args = *p_cmds++;
    oled_cmd(cmd, args, p_cmds);
    p_cmds += args;
  }
  
  // Set initial power
  oled_power(40);
  
  // Clear screen
  uint16_t blank_line[128];
  memset(blank_line, 0, sizeof(blank_line));
  for(uint8_t y = 0; y < 128; y++) {
    oled_blit(0, y, 128, 1, blank_line);
  }
  
  
  // Turn on display
  uint8_t init_on[] = {0x01};
  oled_cmd(0x02, 1, init_on);

  // Power up LEDs
  oled_vramp();

}

// Shut down display completely (lowest power mode)
void oled_deinit(void) {
  oled_power(0);
  digitalWrite(OLED_VEN, LOW);
  digitalWrite(ENABLE_2V8, LOW);
  digitalWrite(OLED_RESET, LOW);
}

// Sets the memory window
void oled_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  uint8_t window[8];
  y0 += 0x1C; // TODO: Pretty sure it's possible to configure the display
  y1 += 0x1C; // to get rid of this offset... but at least it works now!
  window[0] = x0 >> 4; window[1] = x0 & 15;
  window[2] = x1 >> 4; window[3] = x1 & 15;
  window[4] = y0 >> 4; window[5] = y0 & 15;
  window[6] = y1 >> 4; window[7] = y1 & 15;
  oled_cmd(0x0A, 0x08, window);
}

// Pushes n pixels of specific color (to the window)
void oled_push(uint16_t pixel, uint16_t count) {
  digitalWrite(OLED_NCS, LOW);
  digitalWrite(OLED_A0, LOW);
  SPI_I2S_SendData(OLED_SPI, 0xC);
  oled_wait_spi();
  digitalWrite(OLED_A0, HIGH);
  for(uint16_t n = 0; n < count; n++) {
    SPI_I2S_SendData(OLED_SPI, pixel >> 8);
    oled_wait_spi();
    SPI_I2S_SendData(OLED_SPI, pixel & 255);
    oled_wait_spi();
  }
  digitalWrite(OLED_NCS, HIGH);
}

// Blit a sprite to screen
void oled_blit(uint8_t x, uint8_t y, uint8_t w, uint8_t h, void *buffer) {
  oled_window(x, y, x + w - 1, y + h - 1);

  digitalWrite(OLED_NCS, LOW);

  digitalWrite(OLED_A0, LOW);
  SPI_I2S_SendData(OLED_SPI, 0xC);
  oled_wait_spi();
  digitalWrite(OLED_A0, HIGH);

  oled_dma((uint8_t*)buffer, (w * h) << 1);
  oled_wait_dma();
  oled_wait_spi();

  digitalWrite(OLED_NCS, HIGH);
}

// Changes OLED power (brightness)
void oled_power(uint8_t level) {
  uint8_t p[6];
  if(level > 90) level = 90;
  p[0] = p[2] = p[4] = level >> 4;
  p[1] = p[3] = p[5] = level & 15;
  oled_cmd(0x0E, 6, p);
}
