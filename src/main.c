#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>

#include "usb.h"
#include "mesh.h"

void main(void)
{
	// Init bluetooth
	bt_enable(NULL);
	bt_ready();
	// Init usb
	init_usb();
}
