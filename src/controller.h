#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <power/reboot.h>

void setup();
void add_key(uint16_t app_idx);
void provision(uint8_t uuid[16]);
void configure_node(uint16_t addr, uint16_t app_idx);
void configure_elem(uint16_t group_addr, uint16_t node_addr, uint16_t elem_addr, uint16_t app_idx);
void reset_node(uint16_t addr);
void send_message(uint8_t state, uint16_t addr, uint16_t app_idx);
void send_recall_message(uint16_t scene_number, uint16_t addr, uint16_t app_idx);
void send_store_message(uint16_t scene_number, uint16_t addr, uint16_t app_idx);
void send_delete_message(uint16_t scene_number, uint16_t addr, uint16_t app_idx);
void reset();
void reboot();

#endif