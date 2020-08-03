#include "controller.h"
#include "mesh.h"
#include "usb.h"

void provision(uint8_t buffer[32])
{
    int err;
    // Get uuid from buffer
    uint8_t uuid[16];
    slice(uuid, buffer, 1, 16);
    // Provision node with uuid
    err = bt_mesh_provision_adv(uuid, BT_MESH_NET_PRIMARY, 0, 0);
}

void setup()
{
    int err;
    // Generate keys
    uint8_t net_key[16], dev_key[16];
    bt_rand(net_key, 16);
    bt_rand(dev_key, 16);
    // Create a config database
    err = bt_mesh_cdb_create(net_key);
    if (err)
    {
        return;
    }
    // Add an app key to the cdb
    setup_cdb();
    // Provision self
    err = bt_mesh_provision(net_key, BT_MESH_NET_PRIMARY, 0, 0, 1,
                            dev_key);
    if (err)
    {
        return;
    }
    // Configure self
    bt_mesh_cdb_node_foreach(check_unconfigured, NULL);
}