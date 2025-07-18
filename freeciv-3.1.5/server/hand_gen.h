
 /****************************************************************************
 *                       THIS FILE WAS GENERATED                             *
 * Script: common/generate_packets.py                                        *
 * Input:  common/networking/packets.def                                     *
 *                       DO NOT CHANGE THIS FILE                             *
 ****************************************************************************/


#ifndef FC__HAND_GEN_H
#define FC__HAND_GEN_H

/* utility */
#include "shared.h"

/* common */
#include "fc_types.h"
#include "packets.h"

struct connection;

bool server_handle_packet(enum packet_type type, const void *packet,
                          struct player *pplayer, struct connection *pconn);

void handle_nation_select_req(struct connection *pc, int player_no, Nation_type_id nation_no, bool is_male, const char *name, int style);
void handle_player_ready(struct player *pplayer, int player_no, bool is_ready);
void handle_chat_msg_req(struct connection *pc, const char *message);
void handle_city_sell(struct player *pplayer, int city_id16, int city_id32, int build_id);
void handle_city_buy(struct player *pplayer, int city_id16, int city_id32);
void handle_city_change(struct player *pplayer, int city_id16, int city_id32, int production_kind, int production_value);
void handle_city_worklist(struct player *pplayer, int city_id16, int city_id32, const struct worklist *worklist);
void handle_city_make_specialist(struct player *pplayer, int city_id16, int city_id32, int tile_id);
void handle_city_make_worker(struct player *pplayer, int city_id16, int city_id32, int tile_id);
void handle_city_change_specialist(struct player *pplayer, int city_id16, int city_id32, Specialist_type_id from, Specialist_type_id to);
void handle_city_rename(struct player *pplayer, int city_id16, int city_id32, const char *name);
void handle_city_options_req(struct player *pplayer, int city_id16, int city_id32, bv_city_options options);
void handle_city_refresh(struct player *pplayer, int city_id16, int city_id32);
void handle_city_name_suggestion_req(struct player *pplayer, int unit_id16, int unit_id32);
struct packet_city_rally_point;
void handle_city_rally_point(struct player *pplayer, const struct packet_city_rally_point *packet);
struct packet_worker_task;
void handle_worker_task(struct player *pplayer, const struct packet_worker_task *packet);
void handle_player_phase_done(struct player *pplayer, int turn);
void handle_player_rates(struct player *pplayer, int tax, int luxury, int science);
void handle_player_change_government(struct player *pplayer, Government_type_id government);
void handle_player_place_infra(struct player *pplayer, int tile, int extra);
void handle_player_attribute_block(struct player *pplayer);
struct packet_player_attribute_chunk;
void handle_player_attribute_chunk(struct player *pplayer, const struct packet_player_attribute_chunk *packet);
void handle_player_multiplier(struct player *pplayer, int count, const int *multipliers);
void handle_player_research(struct player *pplayer, int tech);
void handle_player_tech_goal(struct player *pplayer, int tech);
void handle_unit_sscs_set(struct player *pplayer, int unit_id16, int unit_id32, enum unit_ss_data_type type, int value);
struct packet_unit_orders;
void handle_unit_orders(struct player *pplayer, const struct packet_unit_orders *packet);
void handle_unit_server_side_agent_set(struct player *pplayer, int unit_id16, int unit_id32, enum server_side_agent agent);
void handle_unit_action_query(struct connection *pc, int actor_id16, int actor_id32, int target_id, action_id action_type, int request_kind);
void handle_unit_type_upgrade(struct player *pplayer, Unit_type_id type);
struct packet_unit_do_action;
void handle_unit_do_action(struct player *pplayer, const struct packet_unit_do_action *packet);
struct packet_unit_get_actions;
void handle_unit_get_actions(struct connection *pc, const struct packet_unit_get_actions *packet);
void handle_unit_change_activity(struct player *pplayer, int unit_id16, int unit_id32, enum unit_activity activity, int target);
void handle_diplomacy_init_meeting_req(struct player *pplayer, int counterpart);
void handle_diplomacy_cancel_meeting_req(struct player *pplayer, int counterpart);
void handle_diplomacy_create_clause_req(struct player *pplayer, int counterpart, int giver, enum clause_type type, int value);
void handle_diplomacy_remove_clause_req(struct player *pplayer, int counterpart, int giver, enum clause_type type, int value);
void handle_diplomacy_accept_treaty_req(struct player *pplayer, int counterpart);
void handle_diplomacy_cancel_pact(struct player *pplayer, int other_player_id, enum clause_type clause);
void handle_report_req(struct connection *pc, enum report_type type);
void handle_conn_pong(struct connection *pc);
void handle_client_heartbeat(struct connection *pc);
void handle_client_info(struct connection *pc, enum gui_type gui, int emerg_version, const char *distribution);
void handle_spaceship_launch(struct player *pplayer);
void handle_spaceship_place(struct player *pplayer, enum spaceship_place_type type, int num);
struct packet_single_want_hack_req;
void handle_single_want_hack_req(struct connection *pc, const struct packet_single_want_hack_req *packet);
void handle_save_scenario(struct connection *pc, const char *name);
void handle_vote_submit(struct connection *pc, int vote_no, int value);
void handle_edit_mode(struct connection *pc, bool state);
void handle_edit_recalculate_borders(struct connection *pc);
void handle_edit_check_tiles(struct connection *pc);
void handle_edit_toggle_fogofwar(struct connection *pc, int player);
void handle_edit_tile_terrain(struct connection *pc, int tile, Terrain_type_id terrain, int size);
void handle_edit_tile_extra(struct connection *pc, int tile, int extra_type_id, bool removal, int eowner, int size);
struct packet_edit_startpos;
void handle_edit_startpos(struct connection *pc, const struct packet_edit_startpos *packet);
struct packet_edit_startpos_full;
void handle_edit_startpos_full(struct connection *pc, const struct packet_edit_startpos_full *packet);
struct packet_edit_tile;
void handle_edit_tile(struct connection *pc, const struct packet_edit_tile *packet);
void handle_edit_unit_create(struct connection *pc, int owner, int tile, Unit_type_id type, int count, int tag);
void handle_edit_unit_remove(struct connection *pc, int owner, int tile, Unit_type_id type, int count);
void handle_edit_unit_remove_by_id(struct connection *pc, int id16, int id32);
struct packet_edit_unit;
void handle_edit_unit(struct connection *pc, const struct packet_edit_unit *packet);
void handle_edit_city_create(struct connection *pc, int owner, int tile, int size, int tag);
void handle_edit_city_remove(struct connection *pc, int id16, int id32);
struct packet_edit_city;
void handle_edit_city(struct connection *pc, const struct packet_edit_city *packet);
void handle_edit_player_create(struct connection *pc, int tag);
void handle_edit_player_remove(struct connection *pc, int id);
struct packet_edit_player;
void handle_edit_player(struct connection *pc, const struct packet_edit_player *packet);
void handle_edit_player_vision(struct connection *pc, int player, int tile, bool known, int size);
struct packet_edit_game;
void handle_edit_game(struct connection *pc, const struct packet_edit_game *packet);
void handle_edit_scenario_desc(struct connection *pc, const char *scenario_desc);

#endif /* FC__HAND_GEN_H */
