#include "mesh.h"
#include "usb.h"
#include "composition.h"
#include "controller.h"

static void unprovisioned_beacon(uint8_t uuid[16], bt_mesh_prov_oob_info_t oob_info, uint32_t *uri_hash)
{
    // Write uuid to usb
    write_usb(usb, OP_UNPROVISIONED_BEACON, uuid, 16);
}

static void node_added(uint16_t net_idx, uint8_t uuid[16], uint16_t addr, uint8_t num_elem)
{
    // Convert addr and send
    uint8_t write[2];
    write[0] = (addr >> 8);
    write[1] = (addr);
    write_usb(usb, OP_NODE_ADDED, write, 2);
}

static void prov_reset(void)
{
}

static uint8_t dev_uuid[16] = {0xdd, 0xdd};

static const struct bt_mesh_prov prov = {
    .uuid = dev_uuid,
    .unprovisioned_beacon = unprovisioned_beacon,
    .node_added = node_added,
    .reset = prov_reset,
};

void bt_ready(void)
{
    struct bt_le_oob oob;
    int err;
    // Init bluetooth mesh
    err = bt_mesh_init(&prov, &comp);
    // Load data from flash
    settings_load();
    // Use Nordic FICR as UUID
    if (!bt_le_oob_get_local(BT_ID_DEFAULT, &oob))
    {
        memcpy(dev_uuid, oob.addr.a.val, 6);
    }
}

void write_mesh_state(uint8_t state, uint16_t node_addr, uint16_t app_idx)
{
    // Make msg buffer
    NET_BUF_SIMPLE_DEFINE(msg, 2 + 6);
    // Define msg context
    struct bt_mesh_msg_ctx ctx = {
        .net_idx = BT_MESH_NET_PRIMARY,
        .app_idx = app_idx,
        .addr = node_addr,
        .send_ttl = BT_MESH_TTL_DEFAULT,
    };
    // Add paylode to buffer
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_SET);
    net_buf_simple_add_u8(&msg, state);
    net_buf_simple_add_u8(&msg, 1);
    bt_mesh_model_send(&root_models[2], &ctx, &msg, NULL, NULL);
}

void write_mesh_store(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx)
{
    // Make msg buffer
    NET_BUF_SIMPLE_DEFINE(msg, 2 + 6);
    // Define msg context
    struct bt_mesh_msg_ctx ctx = {
        .net_idx = BT_MESH_NET_PRIMARY,
        .app_idx = app_idx,
        .addr = node_addr,
        .send_ttl = BT_MESH_TTL_DEFAULT,
    };
    // Add paylode to buffer
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_SCENE_STORE);
    net_buf_simple_add_le16(&msg, scene_number);
    bt_mesh_model_send(&root_models[5], &ctx, &msg, NULL, NULL);
}

void write_mesh_recall(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx)
{
    // Make msg buffer
    NET_BUF_SIMPLE_DEFINE(msg, 2 + 6 + 4);
    // Define msg context
    struct bt_mesh_msg_ctx ctx = {
        .net_idx = BT_MESH_NET_PRIMARY,
        .app_idx = app_idx,
        .addr = node_addr,
        .send_ttl = BT_MESH_TTL_DEFAULT,
    };
    // Add paylode to buffer
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_SCENE_RECALL);
    net_buf_simple_add_le16(&msg, scene_number);
    net_buf_simple_add_u8(&msg, 1);
    bt_mesh_model_send(&root_models[4], &ctx, &msg, NULL, NULL);
}

void write_mesh_delete(uint16_t scene_number, uint16_t node_addr, uint16_t app_idx)
{
    // Make msg buffer
    NET_BUF_SIMPLE_DEFINE(msg, 2 + 6);
    // Define msg context
    struct bt_mesh_msg_ctx ctx = {
        .net_idx = BT_MESH_NET_PRIMARY,
        .app_idx = app_idx,
        .addr = node_addr,
        .send_ttl = BT_MESH_TTL_DEFAULT,
    };
    // Add paylode to buffer
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_SCENE_DELETE);
    net_buf_simple_add_le16(&msg, scene_number);
    bt_mesh_model_send(&root_models[5], &ctx, &msg, NULL, NULL);
}

void write_mesh_config(uint16_t node_addr)
{
    // Make msg buffer
    NET_BUF_SIMPLE_DEFINE(msg, 2 + 4);
    // Define msg context
    struct bt_mesh_msg_ctx ctx = {
        .app_idx = BT_MESH_KEY_DEV_REMOTE,
        .addr = node_addr,
        .send_ttl = BT_MESH_TTL_DEFAULT,
    };
    // Add paylode to buffer
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_CONFIG_NODE_RESET);
    bt_mesh_model_send(&root_models[1], &ctx, &msg, NULL, NULL);
}