#ifndef _USB_H
#define _USB_H

#include <usb/usb_device.h>
#include <drivers/uart.h>

extern struct device *usb;

void write_data(struct device *dev, uint8_t op, uint8_t *buf, int len);
void init_usb(void);
uint8_t *slice(uint8_t *source, size_t offset, size_t count);
uint16_t join2(uint8_t d1, uint8_t d2);
uint32_t join4(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);

#endif