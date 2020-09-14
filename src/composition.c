#include "composition.h"
#include "mesh.h"
#include "usb.h"

static void gen_onoff_status(struct bt_mesh_model *model,
                             struct bt_mesh_msg_ctx *ctx,
                             struct net_buf_simple *buf);

static void scene_status(struct bt_mesh_model *model,
                         struct bt_mesh_msg_ctx *ctx,
                         struct net_buf_simple *buf);

static void scene_register_status(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf);

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

static const struct bt_mesh_model_op gen_onoff_cli_op[] = {
    {BT_MESH_MODEL_OP_GEN_ONOFF_STATUS, 1, gen_onoff_status},
    BT_MESH_MODEL_OP_END,
};

static const struct bt_mesh_model_op scene_srv_op[] = {
    {BT_MESH_MODEL_OP_SCENE_STATUS, 0, scene_status},
    {BT_MESH_MODEL_OP_SCENE_REGISTER_STATUS, 0, scene_register_status},
    BT_MESH_MODEL_OP_END,
};

static const struct bt_mesh_model_op scene_setup_srv_op[] = {
    BT_MESH_MODEL_OP_END,
};

static void gen_onoff_status(struct bt_mesh_model *model,
                             struct bt_mesh_msg_ctx *ctx,
                             struct net_buf_simple *buf)
{
    uint8_t state;
    uint16_t addr;
    state = net_buf_simple_pull_u8(buf);
    addr = ctx->addr;
    // Write state and addr to usb
    uint8_t write[3];
    write[0] = (addr);
    write[1] = (addr >> 8);
    write[2] = state;
    write_usb(usb, OP_STATE, write, 3);
}
static void scene_status(struct bt_mesh_model *model,
                         struct bt_mesh_msg_ctx *ctx,
                         struct net_buf_simple *buf)
{
}
static void scene_register_status(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf)
{
}

struct bt_mesh_model root_models[] = {
    BT_MESH_MODEL_CFG_SRV(&cfg_srv),
    BT_MESH_MODEL_CFG_CLI(&cfg_cli),
    BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
                  NULL, NULL),
    BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
                  NULL, NULL),
    BT_MESH_MODEL(BT_MESH_MODEL_ID_SCENE_SRV, scene_srv_op,
                  NULL, NULL),
    BT_MESH_MODEL(BT_MESH_MODEL_ID_SCENE_SETUP_SRV, scene_setup_srv_op,
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
