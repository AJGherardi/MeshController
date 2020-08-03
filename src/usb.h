#ifndef _USB_H
#define _USB_H

#include <usb/usb_device.h>
#include <drivers/uart.h>

extern struct device *usb;

void write_data(struct device *dev, const uint8_t *buf, int len);
int slice(void *dest, void *source, size_t offset, size_t count);
void init_usb(void);

#endif