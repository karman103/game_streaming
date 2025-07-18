
 /****************************************************************************
 *                       THIS FILE WAS GENERATED                             *
 * Script: common/generate_packets.py                                        *
 * Input:  common/networking/packets.def                                     *
 *                       DO NOT CHANGE THIS FILE                             *
 ****************************************************************************/


#ifndef FC__PACKHAND_GEN_H
#define FC__PACKHAND_GEN_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* utility */
#include "shared.h"

/* common */
#include "packets.h"

bool client_handle_packet(enum packet_type type, const void *packet);

void handle_processing_started(void);
void handle_processing_finished(void);
void handle_investigate_started(int unit_id16, int unit_id32, int city_id16, int city_id32);
void handle_investigate_finished(int unit_id16, int unit_id32, int city_id16, int city_id32);
void handle_server_join_reply(bool you_can_join, const char *message, const char *capability, const char *challenge_file, int conn_id);
void handle_authentication_req(enum authentication_type type, const char *message);
void handle_server_shutdown(void);
struct packet_endgame_report;
void handle_endgame_report(const struct packet_endgame_report *packet);
struct packet_endgame_player;
void handle_endgame_player(const struct packet_endgame_player *packet);
struct packet_tile_info;
void handle_tile_info(const struct packet_tile_info *packet);
struct packet_game_info;
void handle_game_info(const struct packet_game_info *packet);
struct packet_calendar_info;
void handle_calendar_info(const struct packet_calendar_info *packet);
void handle_timeout_info(float seconds_to_phasedone, float last_turn_change_time);
void handle_map_info(int xsize, int ysize, int topology_id);
void handle_nuke_tile_info(int tile);
void handle_team_name_info(int team_id, const char *team_name);
void handle_achievement_info(int id, bool gained, bool first);
struct packet_chat_msg;
void handle_chat_msg(const struct packet_chat_msg *packet);
struct packet_early_chat_msg;
void handle_early_chat_msg(const struct packet_early_chat_msg *packet);
void handle_connect_msg(const char *message);
void handle_server_info(const char *version_label, int major_version, int minor_version, int patch_version, int emerg_version);
void handle_city_remove(int city_id16, int city_id32);
struct packet_city_info;
void handle_city_info(const struct packet_city_info *packet);
struct packet_city_nationalities;
void handle_city_nationalities(const struct packet_city_nationalities *packet);
struct packet_city_short_info;
void handle_city_short_info(const struct packet_city_short_info *packet);
struct packet_trade_route_info;
void handle_trade_route_info(const struct packet_trade_route_info *packet);
void handle_city_name_suggestion_info(int unit_id16, int unit_id32, const char *name);
struct packet_city_sabotage_list;
void handle_city_sabotage_list(const struct packet_city_sabotage_list *packet);
struct packet_city_rally_point;
void handle_city_rally_point(const struct packet_city_rally_point *packet);
struct packet_worker_task;
void handle_worker_task(const struct packet_worker_task *packet);
void handle_player_remove(int playerno);
struct packet_player_info;
void handle_player_info(const struct packet_player_info *packet);
struct packet_player_attribute_chunk;
void handle_player_attribute_chunk(const struct packet_player_attribute_chunk *packet);
struct packet_player_diplstate;
void handle_player_diplstate(const struct packet_player_diplstate *packet);
struct packet_research_info;
void handle_research_info(const struct packet_research_info *packet);
void handle_unknown_research(int id);
void handle_unit_remove(int unit_id16, int unit_id32);
struct packet_unit_info;
void handle_unit_info(const struct packet_unit_info *packet);
struct packet_unit_short_info;
void handle_unit_short_info(const struct packet_unit_short_info *packet);
struct packet_unit_combat_info;
void handle_unit_combat_info(const struct packet_unit_combat_info *packet);
struct packet_unit_action_answer;
void handle_unit_action_answer(const struct packet_unit_action_answer *packet);
struct packet_unit_actions;
void handle_unit_actions(const struct packet_unit_actions *packet);
void handle_diplomacy_init_meeting(int counterpart, int initiated_from);
void handle_diplomacy_cancel_meeting(int counterpart, int initiated_from);
void handle_diplomacy_create_clause(int counterpart, int giver, enum clause_type type, int value);
void handle_diplomacy_remove_clause(int counterpart, int giver, enum clause_type type, int value);
void handle_diplomacy_accept_treaty(int counterpart, bool I_accepted, bool other_accepted);
void handle_page_msg(const char *caption, const char *headline, enum event_type event, int len, int parts);
void handle_page_msg_part(const char *lines);
struct packet_conn_info;
void handle_conn_info(const struct packet_conn_info *packet);
void handle_conn_ping_info(int connections, const int *conn_id, const float *ping_time);
void handle_conn_ping(void);
void handle_end_phase(void);
void handle_start_phase(int phase);
void handle_new_year(int year, int fragments, int turn);
void handle_begin_turn(void);
void handle_end_turn(void);
void handle_freeze_client(void);
void handle_thaw_client(void);
struct packet_spaceship_info;
void handle_spaceship_info(const struct packet_spaceship_info *packet);
struct packet_ruleset_unit;
void handle_ruleset_unit(const struct packet_ruleset_unit *packet);
struct packet_ruleset_unit_bonus;
void handle_ruleset_unit_bonus(const struct packet_ruleset_unit_bonus *packet);
struct packet_ruleset_unit_flag;
void handle_ruleset_unit_flag(const struct packet_ruleset_unit_flag *packet);
struct packet_ruleset_unit_class_flag;
void handle_ruleset_unit_class_flag(const struct packet_ruleset_unit_class_flag *packet);
struct packet_ruleset_game;
void handle_ruleset_game(const struct packet_ruleset_game *packet);
struct packet_ruleset_specialist;
void handle_ruleset_specialist(const struct packet_ruleset_specialist *packet);
struct packet_ruleset_government_ruler_title;
void handle_ruleset_government_ruler_title(const struct packet_ruleset_government_ruler_title *packet);
struct packet_ruleset_tech;
void handle_ruleset_tech(const struct packet_ruleset_tech *packet);
struct packet_ruleset_tech_class;
void handle_ruleset_tech_class(const struct packet_ruleset_tech_class *packet);
struct packet_ruleset_tech_flag;
void handle_ruleset_tech_flag(const struct packet_ruleset_tech_flag *packet);
struct packet_ruleset_government;
void handle_ruleset_government(const struct packet_ruleset_government *packet);
struct packet_ruleset_terrain_control;
void handle_ruleset_terrain_control(const struct packet_ruleset_terrain_control *packet);
void handle_rulesets_ready(void);
struct packet_ruleset_nation_sets;
void handle_ruleset_nation_sets(const struct packet_ruleset_nation_sets *packet);
struct packet_ruleset_nation_groups;
void handle_ruleset_nation_groups(const struct packet_ruleset_nation_groups *packet);
struct packet_ruleset_nation;
void handle_ruleset_nation(const struct packet_ruleset_nation *packet);
void handle_nation_availability(int ncount, const bool *is_pickable, bool nationset_change);
struct packet_ruleset_style;
void handle_ruleset_style(const struct packet_ruleset_style *packet);
struct packet_ruleset_city;
void handle_ruleset_city(const struct packet_ruleset_city *packet);
struct packet_ruleset_building;
void handle_ruleset_building(const struct packet_ruleset_building *packet);
struct packet_ruleset_terrain;
void handle_ruleset_terrain(const struct packet_ruleset_terrain *packet);
struct packet_ruleset_terrain_flag;
void handle_ruleset_terrain_flag(const struct packet_ruleset_terrain_flag *packet);
struct packet_ruleset_unit_class;
void handle_ruleset_unit_class(const struct packet_ruleset_unit_class *packet);
struct packet_ruleset_extra;
void handle_ruleset_extra(const struct packet_ruleset_extra *packet);
struct packet_ruleset_extra_flag;
void handle_ruleset_extra_flag(const struct packet_ruleset_extra_flag *packet);
struct packet_ruleset_base;
void handle_ruleset_base(const struct packet_ruleset_base *packet);
struct packet_ruleset_road;
void handle_ruleset_road(const struct packet_ruleset_road *packet);
struct packet_ruleset_goods;
void handle_ruleset_goods(const struct packet_ruleset_goods *packet);
struct packet_ruleset_disaster;
void handle_ruleset_disaster(const struct packet_ruleset_disaster *packet);
struct packet_ruleset_achievement;
void handle_ruleset_achievement(const struct packet_ruleset_achievement *packet);
struct packet_ruleset_trade;
void handle_ruleset_trade(const struct packet_ruleset_trade *packet);
struct packet_ruleset_action;
void handle_ruleset_action(const struct packet_ruleset_action *packet);
struct packet_ruleset_action_enabler;
void handle_ruleset_action_enabler(const struct packet_ruleset_action_enabler *packet);
struct packet_ruleset_action_auto;
void handle_ruleset_action_auto(const struct packet_ruleset_action_auto *packet);
struct packet_ruleset_music;
void handle_ruleset_music(const struct packet_ruleset_music *packet);
struct packet_ruleset_multiplier;
void handle_ruleset_multiplier(const struct packet_ruleset_multiplier *packet);
struct packet_ruleset_clause;
void handle_ruleset_clause(const struct packet_ruleset_clause *packet);
struct packet_ruleset_control;
void handle_ruleset_control(const struct packet_ruleset_control *packet);
struct packet_ruleset_summary;
void handle_ruleset_summary(const struct packet_ruleset_summary *packet);
struct packet_ruleset_description_part;
void handle_ruleset_description_part(const struct packet_ruleset_description_part *packet);
void handle_single_want_hack_reply(bool you_have_hack);
struct packet_ruleset_choices;
void handle_ruleset_choices(const struct packet_ruleset_choices *packet);
void handle_game_load(bool load_successful, const char *load_filename);
struct packet_server_setting_control;
void handle_server_setting_control(const struct packet_server_setting_control *packet);
struct packet_server_setting_const;
void handle_server_setting_const(const struct packet_server_setting_const *packet);
struct packet_server_setting_bool;
void handle_server_setting_bool(const struct packet_server_setting_bool *packet);
struct packet_server_setting_int;
void handle_server_setting_int(const struct packet_server_setting_int *packet);
struct packet_server_setting_str;
void handle_server_setting_str(const struct packet_server_setting_str *packet);
struct packet_server_setting_enum;
void handle_server_setting_enum(const struct packet_server_setting_enum *packet);
struct packet_server_setting_bitwise;
void handle_server_setting_bitwise(const struct packet_server_setting_bitwise *packet);
void handle_set_topology(int topology_id);
struct packet_ruleset_effect;
void handle_ruleset_effect(const struct packet_ruleset_effect *packet);
struct packet_ruleset_resource;
void handle_ruleset_resource(const struct packet_ruleset_resource *packet);
struct packet_scenario_info;
void handle_scenario_info(const struct packet_scenario_info *packet);
void handle_scenario_description(const char *description);
struct packet_vote_new;
void handle_vote_new(const struct packet_vote_new *packet);
void handle_vote_update(int vote_no, int yes, int no, int abstain, int num_voters);
void handle_vote_remove(int vote_no);
void handle_vote_resolve(int vote_no, bool passed);
struct packet_edit_startpos;
void handle_edit_startpos(const struct packet_edit_startpos *packet);
struct packet_edit_startpos_full;
void handle_edit_startpos_full(const struct packet_edit_startpos_full *packet);
void handle_edit_object_created(int tag, int id);
void handle_play_music(const char *tag);
void handle_web_city_info_addition(int id16, int id32, int granary_size, int granary_turns);
void handle_web_player_info_addition(int playerno, int expected_income);
void handle_web_ruleset_unit_addition(Unit_type_id id, bv_actions utype_actions);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FC__PACKHAND_GEN_H */
