#include "mesh.h"
#include "usb.h"
#include "composition.h"
#include "controller.h"

static void unprovisioned_beacon(uint8_t uuid[16], bt_mesh_prov_oob_info_t oob_info, uint32_t *uri_hash)
{
    write_data(usb, 0x01, uuid, 16);
}

static void node_added(uint16_t net_idx, uint8_t uuid[16], uint16_t addr, uint8_t num_elem)
{
    // Configure nodes
    bt_mesh_cdb_node_foreach(check_unconfigured, NULL);
    // TODO: Address this
    // send_msg(1, addr);
}

static void prov_reset(void)
{
    // TODO: send a reset ack
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
    // Use Nordic FICR UUID
    if (!bt_le_oob_get_local(BT_ID_DEFAULT, &oob))
    {
        memcpy(dev_uuid, oob.addr.a.val, 6);
    }
}

void send_msg(uint8_t state, uint16_t node_addr, uint16_t app_idx)
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
    bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK);
    net_buf_simple_add_u8(&msg, state);
    net_buf_simple_add_u8(&msg, 1);
    bt_mesh_model_send(&root_models[3], &ctx, &msg, NULL, NULL);
}

// void configure_node(struct bt_mesh_cdb_node *node)
// {
//     struct bt_mesh_cdb_app_key *key;
//     struct bt_mesh_cfg_mod_pub pub;
//     uint8_t status;
//     int err;
//     // Get app key from cdb
//     key = bt_mesh_cdb_app_key_get(app_idx);
//     if (key == NULL)
//     {
//         return;
//     }
//     // Add app key to self
//     err = bt_mesh_cfg_app_key_add(BT_MESH_NET_PRIMARY, node->addr, BT_MESH_NET_PRIMARY, app_idx,
//                                   key->keys[0].app_key, NULL);
//     if (err < 0)
//     {
//         return;
//     }
//     // Bind app key to self
//     err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, node->addr, node->addr, app_idx,
//                                    BT_MESH_MODEL_ID_GEN_ONOFF_SRV, NULL);
//     if (err < 0)
//     {
//         return;
//     }
//     // Mark node Configured
//     atomic_set_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED);
// }

uint8_t check_unconfigured(struct bt_mesh_cdb_node *node, void *data)
{
    // Check if configured
    if (!atomic_test_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED))
    {
        // Check if node is self
        if (node->addr != 1)
        {
            // configure_node(node);
        }
    }
    return BT_MESH_CDB_ITER_CONTINUE;
}