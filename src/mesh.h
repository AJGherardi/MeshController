#ifndef _MESH_H
#define _MESH_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/mesh.h>

// Op codes
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_CONFIG_NODE_RESET BT_MESH_MODEL_OP_2(0x80, 0x49)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS BT_MESH_MODEL_OP_2(0x82, 0x04)
#define BT_MESH_MODEL_OP_SCENE_GET BT_MESH_MODEL_OP_2(0x82, 0x41)
#define BT_MESH_MODEL_OP_SCENE_RECALL BT_MESH_MODEL_OP_2(0x82, 0x42)
#define BT_MESH_MODEL_OP_SCENE_RECALL_UNACK BT_MESH_MODEL_OP_2(0x82, 0x43)
#define BT_MESH_MODEL_OP_SCENE_STATUS BT_MESH_MODEL_OP_1(0x5E)
#define BT_MESH_MODEL_OP_SCENE_REGISTER_GET BT_MESH_MODEL_OP_2(0x82, 0x44)
#define BT_MESH_MODEL_OP_SCENE_REGISTER_STATUS BT_MESH_MODEL_OP_2(0x82, 0x45)
#define BT_MESH_MODEL_OP_SCENE_STORE BT_MESH_MODEL_OP_2(0x82, 0x46)
#define BT_MESH_MODEL_OP_SCENE_STORE_UNACK BT_MESH_MODEL_OP_2(0x82, 0x47)
#define BT_MESH_MODEL_OP_SCENE_DELETE BT_MESH_MODEL_OP_2(0x82, 0x9E)
#define BT_MESH_MODEL_OP_SCENE_DELETE_UNACK BT_MESH_MODEL_OP_2(0x82, 0x9F)

void bt_ready(void);
void write_mesh_state(uint8_t state, uint16_t node_addr, uint16_t app_idx);
void write_mesh_store(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx);
void write_mesh_recall(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx);
void write_mesh_delete(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx);
void write_mesh_config(uint16_t node_addr);
void setup_cdb(void);

#endif