#include "controller.h"
#include "mesh.h"
#include "usb.h"

static uint16_t self_addr = 1;

void reboot()
{
    sys_reboot(SYS_REBOOT_WARM);
}

void reset()
{
    bt_mesh_reset();
    bt_mesh_cdb_clear();
}

void reset_node(uint16_t addr)
{
    write_mesh_config(addr);
}

void send_message(uint8_t state, uint16_t addr, uint16_t app_idx)
{
    write_mesh(state, addr, app_idx);
}

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
    // Add app key to node
    err = bt_mesh_cfg_app_key_add(BT_MESH_NET_PRIMARY, node->addr, BT_MESH_NET_PRIMARY, key->app_idx,
                                  key->keys[0].app_key, NULL);
    if (err < 0)
    {
        return;
    }
    // Store node
    bt_mesh_cdb_node_store(node);
}

void configure_elem(uint16_t node_addr, uint16_t elem_addr, uint16_t app_idx)
{
    int err;
    struct bt_mesh_cdb_node *node;
    struct bt_mesh_cdb_app_key *key;
    // Get node from cdb
    node = bt_mesh_cdb_node_get(node_addr);
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
    // Bind app key to elem
    err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, node->addr, elem_addr, key->app_idx,
                                   BT_MESH_MODEL_ID_GEN_ONOFF_SRV, NULL);
    if (err < 0)
    {
        return;
    }
    // Store node
    bt_mesh_cdb_node_store(node);
}

void provision(uint8_t uuid[16])
{
    int err;
    // Provision node with uuid
    err = bt_mesh_provision_adv(uuid, BT_MESH_NET_PRIMARY, 0, 0);
}

void add_key(uint16_t app_idx)
{
    int err;
    struct bt_mesh_cdb_node *self;
    struct bt_mesh_cdb_app_key *key;
    // Get self from cdb
    self = bt_mesh_cdb_node_get(self_addr);
    if (self == NULL)
    {
        return;
    }
    // Get ref to app key
    key = bt_mesh_cdb_app_key_alloc(BT_MESH_NET_PRIMARY, app_idx);
    if (key == NULL)
    {
        return;
    }
    // Set app key
    bt_rand(key->keys[0].app_key, 16);
    // Store app key
    bt_mesh_cdb_app_key_store(key);
    // Add app key to self
    err = bt_mesh_cfg_app_key_add(BT_MESH_NET_PRIMARY, self->addr, BT_MESH_NET_PRIMARY,
                                  key->app_idx, key->keys[0].app_key, NULL);
    if (err < 0)
    {
        return;
    }
    // Bind app key to self
    err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, self->addr, self->addr, key->app_idx,
                                   BT_MESH_MODEL_ID_GEN_ONOFF_SRV, NULL);
    err = bt_mesh_cfg_mod_app_bind(BT_MESH_NET_PRIMARY, self->addr, self->addr, key->app_idx,
                                   BT_MESH_MODEL_ID_GEN_ONOFF_CLI, NULL);
    if (err < 0)
    {
        return;
    }
    // Store self
    bt_mesh_cdb_node_store(self);
    // Convert app key index and send
    uint8_t write[2];
    write[0] = (app_idx >> 8);
    write[1] = (app_idx);
    write_usb(usb, OP_ADD_KEY_STATUS, write, 2);
}

void setup()
{
    int err;
    // Generate dev key
    uint8_t dev_key[16], net_key[16];
    bt_rand(net_key, 16);
    bt_rand(dev_key, 16);
    // Create a config database
    err = bt_mesh_cdb_create(net_key);
    if (err)
    {
        return;
    }
    // Provision self
    err = bt_mesh_provision(net_key, BT_MESH_NET_PRIMARY, 0, 0, 0,
                            dev_key);
    if (err)
    {
        return;
    }
    // Convert addr and send
    uint8_t write[2];
    write[0] = (self_addr >> 8);
    write[1] = (self_addr);
    write_usb(usb, OP_SETUP_STATUS, write, 2);
}