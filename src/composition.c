#include "composition.h"

static struct bt_mesh_cfg_srv cfg_srv = {
    .relay = BT_MESH_RELAY_ENABLED,
    .beacon = BT_MESH_BEACON_DISABLED,
    .frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
    .default_ttl = 7,
    .net_transmit = BT_MESH_TRANSMIT(2, 20),
    .relay_retransmit = BT_MESH_TRANSMIT(3, 20),
};

static struct bt_mesh_cfg_cli cfg_cli = {};

static const struct bt_mesh_model_op gen_onoff_srv_op[] = {
    BT_MESH_MODEL_OP_END,
};

struct bt_mesh_model root_models[] = {
    BT_MESH_MODEL_CFG_SRV(&cfg_srv),
    BT_MESH_MODEL_CFG_CLI(&cfg_cli),
    BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
                  NULL, NULL),
};

static struct bt_mesh_elem elements[] = {
    BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
};

const struct bt_mesh_comp comp = {
    .cid = BT_COMP_ID_LF,
    .elem = elements,
    .elem_count = ARRAY_SIZE(elements),
};
