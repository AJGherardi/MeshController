#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>

void add_key(uint8_t app_key[16], uint16_t app_idx);
void provision(uint8_t uuid[16]);
void setup(uint8_t net_key[16], uint16_t addr, uint32_t iv_index);

#endif