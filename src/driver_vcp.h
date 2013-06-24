// Davey Taylor, Arduino Verkstad AB
// Accelerometer driver
#ifndef _DRIVER_VCP_H_
#define _DRIVER_VCP_H_
#ifdef __cplusplus
extern "C" {
#endif

#define VCP_RX_SIZE 2048

void vcp_init(void);
uint32_t vcp_send(uint8_t *data, uint32_t count);
uint32_t vcp_available(void);
void vcp_recv(uint8_t *data, uint32_t count);

#ifdef __cplusplus
}
#endif
#endif
