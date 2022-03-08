#include "hy_pelco_d_protocol.h"

/******************************************************************************
** 函数名称：u_int8_t pelco_d_check(u_int8_t *pdata,u_int16_t length)
** 功    能：累加和校验(addr、cmd1、cmd2、data1、data2累加和)
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
unsigned char pelco_d_check(unsigned char *data,unsigned short int length)
{
  unsigned char  cs = 0;
  unsigned short int i = 0;
  for(i = 1; i < length -1; i++)
  {
    cs += *(data +i);
  }
  return cs;
}

/******************************************************************************
** 函数名称：edit_cfg_file_head_crc
** 功    能：协议包头校验码计算
**  @author：四川汇源光通信
** 修改日志：2021-09-04
******************************************************************************/
unsigned int edit_cfg_file_head_crc(HY_PTZ_DATA_PACK *pack)
{
  unsigned short int len = 0,len_count = 0;
  unsigned int crc = 0;
  len = sizeof(HY_PTZ_DATA_PACK) - sizeof(unsigned int)- sizeof(unsigned char);
  for(len_count = 0; len_count < len; len_count++)
  {
    crc = crc + *(((unsigned char *)pack) + len_count);
  }
  return crc;
}

/******************************************************************************
** 函数名称：edit_cfg_file
** 功    能：修改配置文件
**  @author：四川汇源光通信
** 修改日志：2021-09-04
******************************************************************************/
void edit_cfg_file(unsigned char addr, char* data, unsigned char content)
{
  static char send_buff [500];

  unsigned short int len ;
  unsigned short int len1 = 0;
  unsigned short int len2 = 0;
  len = strlen(data);
  
  HY_PTZ_DATA_PACK *p = (HY_PTZ_DATA_PACK *)send_buff;

  memset(send_buff, 0, sizeof(send_buff));
  p->head = 0xff;
  p->addr = addr;
  p->cmd_type = 0x01;
  switch (content)
  {
  case EDIT_IP:  //修改云台IP地址
    p->cmd_func = EDIT_IP;
    break;
    
  case EDIT_MAC:  //修改云台MAC地址
    p->cmd_func = EDIT_MAC;
    break;
    
  case EDIT_NETMASK: 
    p->cmd_func = EDIT_NETMASK;
    break;
    
  case EDIT_GATEWAY: 
    p->cmd_func = EDIT_GATEWAY;
    break;
    /*********************************************
  case EDIT_TERM_IP: 
    p->cmd_func = EDIT_TERM_IP;
    break;
    
  case EDIT_WEB_IP: 
    p->cmd_func = EDIT_WEB_IP;
    break;
    
  case EDIT_A9_IP: 
    p->cmd_func = EDIT_A9_IP;
    break;
    
  case EDIT_HARDWAREVER: 
    p->cmd_func = EDIT_HARDWAREVER;
    break;
    
  case EDIT_CMD_ID: 
    p->cmd_func = EDIT_CMD_ID;
    break;
    
  case EDIT_COMPONENT_ID: 
    p->cmd_func = EDIT_COMPONENT_ID;
    break;  ************************************/
  }
    
  p->cmd_len = len;
  p->head_crc = ptz_data_pack_a_head_crc(p);
  if(len > 0){
    memcpy(p->cmd_data, data, len);
  }
  len1 =  sizeof(HY_PTZ_DATA_PACK) - 1 + len;
  *((unsigned int *)(((unsigned char *)p) + len1)) =
    ptz_udp_data_pack_a_crc(p);
  len1 = len1 + sizeof(int);
  *((unsigned char *)p + len1) = 0x16;
  len2 = (len1 + sizeof(char));
    
  // 通过以太网/485/422接口发送
  //send_data(send_buff, len2);
  
}

/*****************************************************************************
**云台方向控制
**     H-speed：云台水平转速放大10倍后取整(单位：r/min)
**     V_speed：云台垂直转速放大10倍后取整
******************************************************************************/

/******************************************************************************
** 函数名称：turn_up
** 功    能：向上转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/

void turn_up(unsigned char addr,unsigned char V_speed)
{
  
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};
  
  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x08;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
  
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** 函数名称：turn_down
** 功    能：向下转动**  
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_down(unsigned char addr,unsigned char V_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x10;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
  
  
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：turn_left
** 功    能：向左转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_left(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x04;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** 函数名称：turn_right
** 功    能：向右转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_right(unsigned char  addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x02;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** 函数名称：turn_leftup
** 功    能：向左上转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_leftup(unsigned char addr,unsigned char H_speed,unsigned char V_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x0c;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：turn_leftdown
** 功    能：向左下转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_leftdown(unsigned char addr,unsigned char H_speed,unsigned char V_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x14;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：turn_rightup
** 功    能：向右上转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_rightup(unsigned char addr,unsigned char H_speed,unsigned char V_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x0a;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：turn_rightup
** 功    能：向右下转动
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_rightdown(unsigned char addr,unsigned char H_speed,unsigned char V_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x12;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
  
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：auto_left
** 功    能：自动左转
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
/***************************该功能仅适用于汇源上位机**************************/
void auto_left(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x04;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：auto_right
** 功    能：自动右转
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
/***************************该功能仅适用于汇源上位机**************************/
void auto_right(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x02;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：turn_stop
** 功    能：停止
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void turn_stop(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/*****************************************************************************
**角度查询
******************************************************************************/
/******************************************************************************
** 函数名称：
** 功    能：查询水平角度
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void get_level_angle(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x51;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
/******************************************************************************
** 功    能：水平角度回传
** 回传数据：{0xff,addr,0x00,0x59,H_HAngle,H_LAngle,crc}
** H_HAngle：回传角度高放大100倍取整后的高8位
** H_LAngle：回传角度高放大100倍取整后的低8位
***  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/


/******************************************************************************
** 函数名称：
** 功    能：查询垂直角度
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void get_vertical_angle(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x53;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
/******************************************************************************
** 功    能：垂直角度回传
** 回传数据：{0xff,addr,0x00,0x5b,V_HAngle,V_LAngle,crc}
** V_HAngle：回传角度高放大100倍取整后的高8位
** V_LAngle：回传角度高放大100倍取整后的低8位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/



/*****************************************************************************
**角度控制
******************************************************************************/

/******************************************************************************
**  函数名称：control_level_angle
**  功    能：水平角度控制
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/

void control_level_angle(unsigned char addr,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x4b;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag ,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data( pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  函数名称：control_vertical_angle
**  功    能：垂直角度控制
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void control_vertical_angle(unsigned char addr,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x4d;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  函数名称：control_right_angle
**  功    能：水平右转角度控制
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void control_right_angle(unsigned char addr,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xbd;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  函数名称：control_left_angle
**  功    能：水平左转角度控制
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void control_left_angle(unsigned char addr,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xbd;
  pelco_d_pack.cmd2  = 0x01;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  函数名称：control_right_angle_cycle
**  功    能：水平右转一圈
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void control_right_angle_cycle(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xbd;
  pelco_d_pack.cmd2  = 0x02;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  函数名称：control_left_angle_cycle
**  功    能：水平左转一圈
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void control_left_angle_cycle(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xbd;
  pelco_d_pack.cmd2  = 0x03;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/*************************************************************************************************************************************
**区域扫描
**************************************************************************************************************************************/
/******************************************************************************
** 函数名称：auto_set_hori_scan_start
** 功    能：视频设置水平区域扫描起始边界
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_hori_scan_start(unsigned char addr,unsigned char Num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe6;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：auto_set_hori_scan_end
** 功    能：视频设置水平区域扫描结束边界
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_hori_scan_end(unsigned char addr,unsigned char Num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe7;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：auto_set_vert_scan_start
** 功    能：视频设置垂直区域扫描起始边界
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_vert_scan_start(unsigned char addr,unsigned char Num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe8;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：auto_set_vert_scan_end
** 功    能：视频设置垂直区域扫描结束边界
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_vert_scan_end(unsigned char addr,unsigned char Num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe9;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：set_hori_scan_start
** 功    能：角度设置水平区域扫描起始边界
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_hori_scan_start(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf7;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_hori_scan_end
** 功    能：角度设置水平区域扫描结束边界
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_hori_scan_end(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf8;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_vert_scan_start
** 功    能：角度设置垂直区域扫描起始边界
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_vert_scan_start(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf9;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_vert_scan_end
** 功    能：角度设置垂直区域扫描结束边界
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_vert_scan_end(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xfa;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_hori_scan_inter
** 功    能：水平扫描间隔角度
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_hori_scan_inter(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xfb;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_vert_scan_inter
** 功    能：垂直扫描间隔角度
** data_high：角度高放大100倍取整后的高8位
**  data_low：角度高放大100倍取整后的低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_vert_scan_inter(unsigned char addr,unsigned char Num,unsigned char data_high,unsigned char data_low )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xfc;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_scan_speed
** 功    能：扫描速度
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_scan_speed(unsigned char addr,unsigned char Num,unsigned char H_speed,unsigned char V_speed )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xfd;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = V_speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_scan_stoptime
** 功    能：单步扫描停止时间
**   H_time：停止时间高8位
**   L_time：停止时间低8位
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_scan_stoptime(unsigned char addr,unsigned char Num,unsigned char H_time,unsigned char L_time )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xfd;
  pelco_d_pack.cmd2  = Num;
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：start_scan_m
** 功    能：启动多区域扫描
**     Num1：扫描起始编号
**     Num2：扫描结束编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void start_scan_m(unsigned char addr,unsigned char Num1,unsigned char Num2)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  pelco_d_pack.cmd2  = 0x02;
  pelco_d_pack.data1 = Num1;
  pelco_d_pack.data2 = Num2;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：start_scan_s
** 功    能：启动单区域扫描
**      Num：扫描编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void start_scan_s(unsigned char addr,unsigned char Num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  pelco_d_pack.cmd2  = 0x01;
  pelco_d_pack.data1 = Num;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：scan_enable
** 功    能：使能区域扫描
**      Num：扫描编号
** enable=1：使能相应区域
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void scan_enable(unsigned char addr,unsigned char Num,unsigned char enable)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf4;
  pelco_d_pack.cmd2  = Num;
  if (enable==1){              // 1 使能
      pelco_d_pack.data1 = 0x01;
  }else{                       // 0 失能
      pelco_d_pack.data1 = 0x00;
  }
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：scan_pause_continue
** 功    能：停止/恢复区域扫描
**  pause=1：区域扫描停止
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void scan_pause_continue(unsigned char addr,unsigned char pause)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  if (pause==1){              // 1 停止扫描
      pelco_d_pack.cmd2 = 0x03;
  }else{                      // 0 恢复扫描
      pelco_d_pack.cmd2 = 0x04;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：scan_close
** 功    能：关闭区域扫描
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void scan_close(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  pelco_d_pack.cmd2  = 0x05;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送  
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**      函数名称：scan_mode
**      功    能：扫描模式（单步/连续扫描）
** single_step=1：单步扫描模式
**           Num：区域编号
**       @author：四川汇源光通信
**      修改日志：2021-09-02
******************************************************************************/
void scan_mode(unsigned char addr,unsigned char Num,unsigned char single_step)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  pelco_d_pack.cmd2  = 0x06;
  pelco_d_pack.data1 = Num;
  if (single_step==1){         // 1 单步扫描
      pelco_d_pack.data2 = 0x02;
  }else{                       // 0 连续扫描
      pelco_d_pack.data2 = 0x01;
  }  
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 预置位扫描
******************************************************************************/
/******************************************************************************
** 函数名称：auto_set_preset_point
** 功    能：视频设置预置位
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_preset_point(unsigned char addr,unsigned char PP_num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x03;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = PP_num;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：recall_preset_point
** 功    能：召回（调用）预置位
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void recall_preset_point(unsigned char addr,unsigned char PP_num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x07;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = PP_num;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：del_preset_point
** 功    能：删除预置位
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void del_preset_point(unsigned char addr,unsigned char PP_num)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x05;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = PP_num;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：set_hori_preset_point
** 功    能：角度设置水平预置位
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_hori_preset_point(unsigned char addr,unsigned char PP_num,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe4;
  pelco_d_pack.cmd2  = PP_num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_vert_preset_point
** 功    能：角度设置垂直预置位
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_vert_preset_point(unsigned char addr,unsigned char PP_num,unsigned char data_high,unsigned char data_low)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe5;
  pelco_d_pack.cmd2  = PP_num;
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_preset_point_stoptime
** 功    能：设置预置位停止时间
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_preset_point_stoptime(unsigned char addr,unsigned char PP_num,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf1;
  pelco_d_pack.cmd2  = PP_num;
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_preset_point_speed
** 功    能：设置到达预置位速度
**   PP_num：预置位编号
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void set_preset_point_speed(unsigned char addr,unsigned char PP_num,unsigned char H_Speed,unsigned char V_Speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf2;
  pelco_d_pack.cmd2  = PP_num;
  pelco_d_pack.data1 = H_Speed;
  pelco_d_pack.data2 = V_Speed;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：preset_point_start
** 功    能：启动预置位扫描
**   once=1：启动一次预置位扫描
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void preset_point_start(unsigned char addr,unsigned char once,unsigned char PP_num1,unsigned char PP_num2)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf0;
  if (once==1){               // 1 启动一次
      pelco_d_pack.cmd2 = 0x05;
  }else{                      // 0 启动且连续
      pelco_d_pack.cmd2 = 0x01;
  }
  pelco_d_pack.data1 = PP_num1;
  pelco_d_pack.data2 = PP_num2;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：preset_point_pause_continue
** 功    能：停止/恢复预置位扫描
**  pause=1：预置位扫描停止
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void preset_point_pause_continue(unsigned char addr,unsigned char pause)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf0;
  if (pause==1){              // 1 停止
      pelco_d_pack.cmd2 = 0x02;
  }else{                      // 0 恢复
      pelco_d_pack.cmd2 = 0x03;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：preset_point_close
** 功    能：关闭预置位扫描
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void preset_point_close(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf0;
  pelco_d_pack.cmd2  = 0x04;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}





/******************************************************************************
** 云台自定义0位
******************************************************************************/

/******************************************************************************
** 函数名称：auto_set_zero_posit
** 功    能：视频设置基准0位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void auto_set_zero_posit(unsigned char addr,unsigned char dir)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe3;
  switch (dir)
  {             
  case 1:                     //水平 
     pelco_d_pack.cmd2  = 0x01; break;
  case 2:                     //垂直
     pelco_d_pack.cmd2  = 0x02; break;
  case 3:                     //水平和垂直
     pelco_d_pack.cmd2  = 0x03; break;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：set_zero_posit
** 功    能：角度设置基准0位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
/******************************现有版本暂不支持该功能**************************
void set_zero_posit(unsigned char addr，unsigned char data_high, unsigned char data_low，unsigned char dir)
{
  unsigned char hori = 1;
  unsigned char vert = 0;
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe3;
  if (dir==hori){             //水平 
      pelco_d_pack.cmd2  = 0x04;  
  }
  if (dir==vert){              //垂直
      pelco_d_pack.cmd2  = 0x05;     
  }
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
******************************************************************************/

/******************************************************************************
** 函数名称：del_zero_posit
** 功    能：删除基准0位
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void del_zero_posit(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe3;
  pelco_d_pack.cmd2  = 0x06;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}




/******************************************************************************
** 函数名称：reset_reboot
** 功    能：云台复位重启
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void reset_reboot(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xde;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：work_mode
** 功    能：查询工作模式
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void work_mode(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe0;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：work_state
** 功    能：查询工作状态
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void work_state(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xdd;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：temp_inquire
** 功    能：查询温度
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void temp_inquire(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xd6;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：volt_inquire
** 功    能：查询工作电压
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void volt_inquire(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xcd;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** 函数名称：curr_inquire
** 功    能：查询工作电流
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void curr_inquire(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xc8;
  pelco_d_pack.cmd2  = 0x00;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：speed_inquire
** 功    能：查询速度
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void speed_inquire(unsigned char addr,unsigned char speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xd0;
  switch (speed)
  {
  case 1://水平与垂直速度
    pelco_d_pack.cmd2  = 0x00; break;
  case 2://水平速度
    pelco_d_pack.cmd2  = 0x01; break;
  case 3://垂直速度
    pelco_d_pack.cmd2  = 0x02; break;  
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：relay_control
** 功    能：继电器开关（不建议使用:继电器被触发时使用）
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void relay_control(unsigned char addr, unsigned char relay)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xc0;
  if (relay==1){             //打开继电器
    pelco_d_pack.cmd2  = 0x01;
  }else{                     //关闭继电器（默认常关）
    pelco_d_pack.cmd2  = 0x00;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：clean_reset
** 功    能：清除掉电位置数据
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void clean_reset(unsigned char addr)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xbb;
  pelco_d_pack.cmd2  = 0x01;
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** 函数名称：adjust_focal
** 功    能：聚焦
**  @author：四川汇源光通信
** 修改日志：2021-09-02
******************************************************************************/
void adjust_focal(unsigned char addr,unsigned char action )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  if (action == 1){             // 1 聚焦+
      pelco_d_pack.cmd1  = 0x01; 
      pelco_d_pack.cmd2  = 0x00;
  } else {                      // 0 聚焦-
      pelco_d_pack.cmd1  = 0x00;
      pelco_d_pack.cmd2  = 0x80;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}




/******************************************************************************
****实时回传
******************************************************************************/

/******************************************************************************
** 函数名称：auto_return_temp
** 功    能：温度实时回传
**  @author：四川汇源光通信
** 修改日志：2021-09-06
******************************************************************************/
void auto_return_temp(unsigned char addr,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xd4; 
  pelco_d_pack.cmd2  = 0x01;  
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：auto_return_volt
** 功    能：工作电压实时回传
**  @author：四川汇源光通信
** 修改日志：2021-09-06
******************************************************************************/
void auto_return_volt(unsigned char addr,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xcc; 
  pelco_d_pack.cmd2  = 0x01;  
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：auto_return_curr
** 功    能：工作电流实时回传
**  @author：四川汇源光通信
** 修改日志：2021-09-06
******************************************************************************/
void auto_return_curr(unsigned char addr,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xc1; 
  pelco_d_pack.cmd2  = 0x01;  
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：auto_return_angle
** 功    能：角度实时回传(水平和垂直)
**  @author：四川汇源光通信
** 修改日志：2021-09-06
******************************************************************************/
void auto_return_angle(unsigned char addr,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe1; 
  pelco_d_pack.cmd2  = 0x01;  
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** 函数名称：auto_return_speed
** 功    能：速度实时回传(水平和垂直)
**  @author：四川汇源光通信
** 修改日志：2021-09-06
******************************************************************************/
void auto_return_speed(unsigned char addr,unsigned char H_time,unsigned char L_time)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xdc; 
  pelco_d_pack.cmd2  = 0x01;  
  pelco_d_pack.data1 = H_time;
  pelco_d_pack.data2 = L_time;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // 通过以太网/485/422接口发送
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}


