#ifndef _USB_H
#define _USB_H

#include <usb/usb_device.h>
#include <drivers/uart.h>

// Define usb op codes
#define OP_SETUP 0x00
#define OP_SETUP_STATUS 0x01
#define OP_ADD_KEY 0x02
#define OP_ADD_KEY_STATUS 0x03
#define OP_UNPROVISIONED_BEACON 0x04
#define OP_PROVISION 0x05
#define OP_NODE_ADDED 0x06
#define OP_CONFIGURE_NODE 0x07
#define OP_CONFIGURE_NODE_STATUS 0x08
#define OP_SEND_MESSAGE 0x09
#define OP_RESET 0x10
#define OP_REBOOT 0x11
#define OP_NODE_RESET 0x12
#define OP_STATE 0x13
#define OP_CONFIGURE_ELEM 0x14
#define OP_CONFIGURE_ELEM_STATUS 0x15
#define OP_SEND_RECALL_MESSAGE 0x16
#define OP_SEND_STORE_MESSAGE 0x17
#define OP_SEND_DELETE_MESSAGE 0x18

extern struct device *usb;

void write_usb(struct device *dev, uint8_t op, uint8_t *buf, int len);
void init_usb(void);
uint8_t *slice(uint8_t *source, size_t offset, size_t count);
uint16_t join2(uint8_t d1, uint8_t d2);
uint32_t join4(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);

#endif