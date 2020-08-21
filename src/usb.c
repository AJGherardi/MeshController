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
    uart_fifo_read(dev, buffer, 32);
    // Map to appropriate function
    switch (buffer[0])
    {
    case OP_SETUP:
    {
        setup();
        break;
    }
    case OP_ADD_KEY:
    {
        uint16_t app_idx = join2(buffer[1], buffer[2]);
        add_key(app_idx);
        break;
    }
    case OP_PROVISION:
    {
        uint8_t uuid[16] = {};
        memcpy(uuid, buffer + 1, 16);
        provision(uuid);
        break;
    }
    case OP_CONFIGURE_NODE:
    {
        uint16_t addr = join2(buffer[1], buffer[2]);
        uint16_t app_idx = join2(buffer[3], buffer[4]);
        configure_node(addr, app_idx);
        break;
    }
    case OP_SEND_MESSAGE:
    {
        uint16_t addr = join2(buffer[2], buffer[3]);
        uint16_t app_idx = join2(buffer[4], buffer[5]);
        send_message(buffer[1], addr, app_idx);
        break;
    }
    }
}

uint16_t join2(uint8_t d1, uint8_t d2)
{
    uint16_t output;
    union {
        uint8_t bytes[2];
        uint16_t n;
    } join;
    join.bytes[0] = d2;
    join.bytes[1] = d1;
    output = join.n;
    return output;
}

uint32_t join4(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
    uint32_t output;
    union {
        uint8_t bytes[4];
        uint32_t n;
    } join;
    join.bytes[0] = d4;
    join.bytes[1] = d3;
    join.bytes[2] = d2;
    join.bytes[3] = d1;
    output = join.n;
    return output;
}

void write_usb(struct device *dev, uint8_t op, uint8_t *buf, int len)
{
    if (!uart_irq_tx_ready(dev))
    {
        return;
    }
    static uint8_t write[32] = {};
    // Set op code
    write[0] = op;
    // Copy buffer
    memcpy(write + 1, buf, len);
    // Write to uart
    uart_fifo_fill(dev, write, (len + 1));
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
    uart_line_ctrl_set(usb, UART_LINE_CTRL_DCD, 1);
    uart_line_ctrl_set(usb, UART_LINE_CTRL_DSR, 1);
    k_busy_wait(1000000);
    // Set callback
    uart_irq_callback_set(usb, handler);
    // Enable interrupts
    uart_irq_rx_enable(usb);
    uart_irq_tx_enable(usb);
}