#include <string.h>
#include <usbd_core.h>
#include <usbd_cdc_core.h>
#include <usbd_usr.h>
#include "usbd_desc.h"
#include "system.h"
#include "driver_vcp.h"

// USB device handle
USB_OTG_CORE_HANDLE USB_OTG_dev;

// Transmit buffer (or receive buffer, as seen from CDC)
extern uint8_t  APP_Rx_Buffer []; // Outgoing data buffer
extern uint32_t APP_Rx_ptr_in;    // Buffer index/byte count

#define VCP_TX_SIZE APP_RX_DATA_SIZE
#define txbuff APP_Rx_Buffer
#define txbuff_ix APP_Rx_ptr_in

static uint8_t rxbuff[VCP_RX_SIZE];
static uint32_t rxbuff_ix;

void vcp_init(void) {
  USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
}

uint32_t vcp_send(uint8_t *data, uint32_t count) {
  if(txbuff_ix + count > VCP_TX_SIZE) {
    // TODO: wait until free
    count = VCP_TX_SIZE - txbuff_ix; // Discard overflows
  }
  memcpy(&txbuff[txbuff_ix], data, count);
  di();
  txbuff_ix += count;
  ei(); // TODO: unsafe
  return count;
}

uint32_t vcp_available(void) {
  return rxbuff_ix;
}

void vcp_recv(uint8_t *data, uint32_t count) {
  if(count > rxbuff_ix) count = rxbuff_ix;
  if(count == 0) return;
  di();
  rxbuff_ix -= count;
  memcpy(data, rxbuff, count);
  memmove(rxbuff, &rxbuff[count], rxbuff_ix);
  ei();  // TODO: unsafe
}

uint32_t vcp_buffer(uint8_t *data, uint32_t count) {
  if(rxbuff_ix + count > VCP_RX_SIZE) {
    // TODO: what to do?
    count = VCP_RX_SIZE - rxbuff_ix; // Silently discard overflows
  }
  memcpy(&rxbuff[rxbuff_ix], data, count);
  di();
  rxbuff_ix += count;
  ei();  // TODO: unsafe
  return count;
}