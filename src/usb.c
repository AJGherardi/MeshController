#include "usb.h"
#include "controller.h"

struct device *usb;

static void handler(struct device *dev)
{
    // Process interrupts
    uart_irq_update(dev);
    // Return if not ready
    if (!uart_irq_rx_ready(dev))
    {
        return;
    }
    // Read into buffer
    uint8_t buffer[32];
    int len = uart_fifo_read(dev, buffer, 32);
    // Map to appropriate function
    switch (buffer[0])
    {
    case 0x00:
        setup();
        break;
    case 0x01:
        provision(buffer);
        break;
    }
}

int slice(void *dest, void *source, size_t offset, size_t count)
{
    memcpy(((char *)dest), ((const char *)source) + offset, count);
    return 0;
}

void write_data(struct device *dev, const uint8_t *buf, int len)
{
    uart_fifo_fill(dev, buf, len);
}

void init_usb(void)
{
    // Usb vars
    uint32_t dtr = 0U;
    // Get cdc device
    usb = device_get_binding("CDC_ACM_0");
    // Enable usb
    usb_enable(NULL);
    // Wait for dtr
    while (true)
    {
        uart_line_ctrl_get(usb, UART_LINE_CTRL_DTR, &dtr);
        if (dtr)
        {
            break;
        }
        else
        {
            k_sleep(K_MSEC(100));
        }
    }
    // Set callback
    uart_irq_callback_set(usb, handler);
    // Enable interrupts
    uart_irq_rx_enable(usb);
    uart_irq_tx_enable(usb);
}