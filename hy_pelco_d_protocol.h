#ifndef _PELCO_D_PROTOCOL_H_
#define _PELCO_D_PROTOCOL_H_

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;

//????PELCO_D??
typedef struct _pelco_d_protocol{
        unsigned char  flag;                  // 1byte ??????0xff
        unsigned char  addr;                  // 1byte ????
        unsigned char  cmd1;                  // 1byte ???1
        unsigned char  cmd2;                  // 1byte ???2
        unsigned char  data1;                 // 1byte ???1
        unsigned char  data2;                 // 1byte ???2
        unsigned char  check;                 // 1byte ???
}pelco_d_protocol;

//???????
typedef struct _HY_PTZ_DATA_PACK_
{
    unsigned char         head; //????0XFF
    unsigned char         addr; //????
    unsigned char     cmd_type; //????????????????
    unsigned char     cmd_func; //??????????????????
    unsigned short int cmd_len; //??????
    unsigned int      head_crc; //????,????char??
    unsigned char  cmd_data[1]; //???????????????
}HY_PTZ_DATA_PACK;


#define EDIT_IP             0X01   //????IP??
#define EDIT_MAC            0X02   //????MAC??
#define EDIT_NETMASK        0X03   //????NETMASK
#define EDIT_GATEWAY        0X04   //????GATEWAY
/*
#define EDIT_TERM_IP        0X05
#define EDIT_WEB_IP         0X06
#define EDIT_A9_IP          0X07
#define EDIT_HARDWAREVER    0X08
#define EDIT_CMD_ID         0X09
#define EDIT_COMPONENT_ID   0X0A
*/
//????
//?????
unsigned char pelco_d_check(unsigned char *pdata,u_int16_t length);
unsigned int edit_cfg_file_head_crc(HY_PTZ_DATA_PACK *pack);
//??????
void edit_cfg_file(unsigned char addr, char *data, u_int8_t content);
//??????
void turn_up(u_int8_t addr,u_int8_t V_speed);
void turn_down(u_int8_t addr,u_int8_t V_speed);
void turn_left(u_int8_t addr,u_int8_t H_speed);
void turn_right(u_int8_t  addr,u_int8_t H_speed);
void turn_leftup(u_int8_t addr,u_int8_t H_speed,u_int8_t V_speed);
void turn_leftdown(u_int8_t addr,u_int8_t H_speed,u_int8_t V_speed);
void turn_rightup(u_int8_t addr,u_int8_t H_speed,u_int8_t V_speed);
void turn_rightdown(u_int8_t addr,u_int8_t H_speed,u_int8_t V_speed);
void auto_left(u_int8_t addr,u_int8_t H_speed);
void auto_right(u_int8_t addr,u_int8_t H_speed);
void turn_stop(u_int8_t addr);
//????
void get_level_angle(u_int8_t addr);
void get_vertical_angle(u_int8_t addr);
//??????
void control_level_angle(u_int8_t addr,u_int8_t data_high,u_int8_t data_low);
void control_vertical_angle(u_int8_t addr,u_int8_t data_high,u_int8_t data_low);
void control_right_angle(u_int8_t addr,u_int8_t data_high,u_int8_t data_low);
void control_left_angle(u_int8_t addr,u_int8_t data_high,u_int8_t data_low);
void control_right_angle_cycle(u_int8_t addr,u_int8_t H_speed);
void control_left_angle_cycle(u_int8_t addr,u_int8_t H_speed);
//??????
void auto_set_hori_scan_start(u_int8_t addr,u_int8_t Num);
void auto_set_hori_scan_end(u_int8_t addr,u_int8_t Num);
void auto_set_vert_scan_start(u_int8_t addr,u_int8_t Num);
void auto_set_vert_scan_end(u_int8_t addr,u_int8_t Num);
void set_hori_scan_start(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_hori_scan_end(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_vert_scan_start(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_vert_scan_end(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_hori_scan_inter(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_vert_scan_inter(u_int8_t addr,u_int8_t Num,u_int8_t data_high,u_int8_t data_low );
void set_scan_speed(u_int8_t addr,u_int8_t Num,u_int8_t H_speed,u_int8_t V_speed );
void set_scan_stoptime(u_int8_t addr,u_int8_t Num,u_int8_t H_time,u_int8_t L_time );
void start_scan_m(u_int8_t addr,u_int8_t Num1,u_int8_t Num2);
void start_scan_s(u_int8_t addr,u_int8_t Num);
void scan_enable(u_int8_t addr,u_int8_t Num,u_int8_t enable);
void scan_pause_continue(u_int8_t addr,u_int8_t pause);
void scan_close(u_int8_t addr);
void scan_mode(u_int8_t addr,u_int8_t Num,u_int8_t single_step);
//???????
void auto_set_preset_point(u_int8_t addr,u_int8_t PP_num);
void recall_preset_point(u_int8_t addr,u_int8_t PP_num);
void del_preset_point(u_int8_t addr,u_int8_t PP_num);
void set_hori_preset_point(u_int8_t addr,u_int8_t PP_num,u_int8_t data_high,u_int8_t data_low);
void set_vert_preset_point(u_int8_t addr,u_int8_t PP_num,u_int8_t data_high,u_int8_t data_low);
void set_preset_point_stoptime(u_int8_t addr,u_int8_t PP_num,u_int8_t H_time,u_int8_t L_time);
void set_preset_point_speed(u_int8_t addr,u_int8_t PP_num,u_int8_t H_Speed,u_int8_t V_Speed);
void preset_point_start(u_int8_t addr,u_int8_t once,u_int8_t PP_num1,u_int8_t PP_num2);
void preset_point_pause_continue(u_int8_t addr,u_int8_t pause);
void preset_point_close(u_int8_t addr);

void auto_set_zero_posit(u_int8_t addr,u_int8_t dir);
void del_zero_posit(u_int8_t addr);
void reset_reboot(u_int8_t addr);
void work_mode(u_int8_t addr);
void work_state(u_int8_t addr);
void temp_inquire(u_int8_t addr);
void volt_inquire(u_int8_t addr);
void curr_inquire(u_int8_t addr);
void speed_inquire(u_int8_t addr,u_int8_t speed);
void relay_control(u_int8_t addr,u_int8_t relay);
void clean_reset(u_int8_t addr);
void adjust_focal(u_int8_t addr,u_int8_t action );
//????
void auto_return_temp(u_int8_t addr,u_int8_t H_time,u_int8_t L_time);
void auto_return_volt(u_int8_t addr,u_int8_t H_time,u_int8_t L_time);
void auto_return_curr(u_int8_t addr,u_int8_t H_time,u_int8_t L_time);
void auto_return_angle(u_int8_t addr,u_int8_t H_time,u_int8_t L_time);
void auto_return_speed(u_int8_t addr,u_int8_t H_time,u_int8_t L_time);

#endif
