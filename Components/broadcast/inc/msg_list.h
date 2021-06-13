/** ************************************************************* *
 * @file        msg_list.h
 * @brief       
 * 
 * @date        2021-05-08
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __MSG_LIST__
#define __MSG_LIST__

/* ------------------------------------------------------------- --
   constant
-- ------------------------------------------------------------- */
/* phases ID */
#define MSG_ID_phase_wait           (uint8_t)0xA1
#define MSG_ID_phase_ascend         (uint8_t)0xA2
#define MSG_ID_phase_deploy         (uint8_t)0xA3
#define MSG_ID_phase_descend        (uint8_t)0xA4
#define MSG_ID_phase_landed         (uint8_t)0xA5

/* jack ID */
#define MSG_ID_HW_jack_plugged      (uint8_t)0xB1
#define MSG_ID_HW_jack_unplugged    (uint8_t)0xB2

/* recovery system ID */
#define MSG_ID_recsys_unlocked      (uint8_t)0xC1
#define MSG_ID_recsys_unlocking     (uint8_t)0xC2
#define MSG_ID_recsys_locked        (uint8_t)0xC3
#define MSG_ID_recsys_locking       (uint8_t)0xC4
#define MSG_ID_recsys_reverse       (uint8_t)0xC5
#define MSG_ID_recsys_start_motor   (uint8_t)0xC6
#define MSG_ID_recsys_stop_motor    (uint8_t)0xC7

/* alim monitoring ID */
#define MSG_ID_alim_motor_ok        (uint8_t)0xD1
#define MSG_ID_alim_motor_default   (uint8_t)0xD2
#define MSG_ID_alim_seq_ok          (uint8_t)0xD3
#define MSG_ID_alim_seq_default     (uint8_t)0xD4

/* temporal window ID */
#define MSG_ID_unlock_window_IT     (uint8_t)0xF1
#define MSG_ID_unlock_window_POOL   (uint8_t)0xF2
#define MSG_ID_relock_window_IT     (uint8_t)0xF3
#define MSG_ID_relock_window_POOL   (uint8_t)0xF4

#endif