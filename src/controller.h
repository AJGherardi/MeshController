#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>

void provision(uint8_t buffer[32]);
void setup();

#endif