#include "controller.h"
#include "mesh.h"
#include "usb.h"

static uint16_t self_addr;

void configure_node(uint16_t addr, uint16_t app_idx)
{
    int err;
    struct bt_mesh_cdb_node *node;
    struct bt_mesh_cdb_app_key *key;
    // Get node from cdb
    node = bt_mesh_cdb_node_get(addr);
    if (node == NULL)
    {
        return;
    }
    // Get app key from cdb
    key = bt_mesh_cdb_app_key_get(app_idx);
    if (key == NULL)
    {
        return;
    }
    // Add app key to self
    err = bt_mesh_cfg_app_key_add(BT_MESH_NET_PRIMARY, node->addr, BT_MESH_NET_PRIMARY, app_idx,
                                  key->keys[0].app_key, NULL);
    if (err < 0)
    {
        return;
    }
    // Bind app key to self
    err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, node->addr, node->addr, app_idx,
                                   BT_MESH_MODEL_ID_GEN_ONOFF_SRV, NULL);
    if (err < 0)
    {
        return;
    }
}

void add_key(uint8_t app_key[16], uint16_t app_idx)
{
    int err;
    struct bt_mesh_cdb_app_key *key;
    // Get ref to app key
    key = bt_mesh_cdb_app_key_alloc(BT_MESH_NET_PRIMARY, app_idx);
    if (key == NULL)
    {
        return;
    }
    // Set app key
    *key->keys[0].app_key = *app_key;
    // Add app key to self
    err = bt_mesh_cfg_app_key_add(BT_MESH_NET_PRIMARY, self_addr, BT_MESH_NET_PRIMARY,
                                  app_idx, key->keys[0].app_key, NULL);
    if (err < 0)
    {
        return;
    }
    // Bind app key to self
    err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, self_addr, self_addr, app_idx,
                                   BT_MESH_MODEL_ID_GEN_ONOFF_SRV, NULL);
    if (err < 0)
    {
        return;
    }
    write_data(usb, 0x05, app_key, 16);
}

void provision(uint8_t uuid[16])
{
    int err;
    // Provision node with uuid
    err = bt_mesh_provision_adv(uuid, BT_MESH_NET_PRIMARY, 0, 0);
}

void setup(uint8_t net_key[16], uint16_t addr, uint32_t iv_index)
{
    int err;
    // Generate dev key
    uint8_t dev_key[16];
    bt_rand(dev_key, 16);
    // Create a config database
    err = bt_mesh_cdb_create(net_key);
    if (err)
    {
        return;
    }
    // Provision self
    err = bt_mesh_provision(net_key, BT_MESH_NET_PRIMARY, 0, iv_index, addr,
                            dev_key);
    if (err)
    {
        return;
    }
    // Store addr
    self_addr = addr;
    write_data(usb, 0x03, net_key, 16);
}