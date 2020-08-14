#ifndef _MESH_H
#define _MESH_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/mesh.h>

// Op codes
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x03)

void bt_ready(void);
void send_msg(uint8_t state, uint16_t node_addr, uint16_t app_idx);
uint8_t check_unconfigured(struct bt_mesh_cdb_node *node, void *data);
void configure_node(struct bt_mesh_cdb_node *node);
void configure_self(struct bt_mesh_cdb_node *self);
void setup_cdb(void);

#endif