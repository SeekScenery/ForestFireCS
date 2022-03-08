#include "hy_pelco_d_protocol.h"

/******************************************************************************
** �������ƣ�u_int8_t pelco_d_check(u_int8_t *pdata,u_int16_t length)
** ��    �ܣ��ۼӺ�У��(addr��cmd1��cmd2��data1��data2�ۼӺ�)
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
** �������ƣ�edit_cfg_file_head_crc
** ��    �ܣ�Э���ͷУ�������
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-04
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
** �������ƣ�edit_cfg_file
** ��    �ܣ��޸������ļ�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-04
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
  case EDIT_IP:  //�޸���̨IP��ַ
    p->cmd_func = EDIT_IP;
    break;
    
  case EDIT_MAC:  //�޸���̨MAC��ַ
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(send_buff, len2);
  
}

/*****************************************************************************
**��̨�������
**     H-speed����̨ˮƽת�ٷŴ�10����ȡ��(��λ��r/min)
**     V_speed����̨��ֱת�ٷŴ�10����ȡ��
******************************************************************************/

/******************************************************************************
** �������ƣ�turn_up
** ��    �ܣ�����ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
  
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** �������ƣ�turn_down
** ��    �ܣ�����ת��**  
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
  
  
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�turn_left
** ��    �ܣ�����ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** �������ƣ�turn_right
** ��    �ܣ�����ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
  
}

/******************************************************************************
** �������ƣ�turn_leftup
** ��    �ܣ�������ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�turn_leftdown
** ��    �ܣ�������ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�turn_rightup
** ��    �ܣ�������ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�turn_rightup
** ��    �ܣ�������ת��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
  
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�auto_left
** ��    �ܣ��Զ���ת
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
/***************************�ù��ܽ������ڻ�Դ��λ��**************************/
void auto_left(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x04;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�auto_right
** ��    �ܣ��Զ���ת
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
/***************************�ù��ܽ������ڻ�Դ��λ��**************************/
void auto_right(unsigned char addr,unsigned char H_speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0x00;
  pelco_d_pack.cmd2  = 0x02;
  pelco_d_pack.data1 = H_speed;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�turn_stop
** ��    �ܣ�ֹͣ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/*****************************************************************************
**�ǶȲ�ѯ
******************************************************************************/
/******************************************************************************
** �������ƣ�
** ��    �ܣ���ѯˮƽ�Ƕ�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
/******************************************************************************
** ��    �ܣ�ˮƽ�ǶȻش�
** �ش����ݣ�{0xff,addr,0x00,0x59,H_HAngle,H_LAngle,crc}
** H_HAngle���ش��Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
** H_LAngle���ش��Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
***  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/


/******************************************************************************
** �������ƣ�
** ��    �ܣ���ѯ��ֱ�Ƕ�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
/******************************************************************************
** ��    �ܣ���ֱ�ǶȻش�
** �ش����ݣ�{0xff,addr,0x00,0x5b,V_HAngle,V_LAngle,crc}
** V_HAngle���ش��Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
** V_LAngle���ش��Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/



/*****************************************************************************
**�Ƕȿ���
******************************************************************************/

/******************************************************************************
**  �������ƣ�control_level_angle
**  ��    �ܣ�ˮƽ�Ƕȿ���
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data( pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  �������ƣ�control_vertical_angle
**  ��    �ܣ���ֱ�Ƕȿ���
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  �������ƣ�control_right_angle
**  ��    �ܣ�ˮƽ��ת�Ƕȿ���
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  �������ƣ�control_left_angle
**  ��    �ܣ�ˮƽ��ת�Ƕȿ���
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  �������ƣ�control_right_angle_cycle
**  ��    �ܣ�ˮƽ��תһȦ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**  �������ƣ�control_left_angle_cycle
**  ��    �ܣ�ˮƽ��תһȦ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/*************************************************************************************************************************************
**����ɨ��
**************************************************************************************************************************************/
/******************************************************************************
** �������ƣ�auto_set_hori_scan_start
** ��    �ܣ���Ƶ����ˮƽ����ɨ����ʼ�߽�
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�auto_set_hori_scan_end
** ��    �ܣ���Ƶ����ˮƽ����ɨ������߽�
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�auto_set_vert_scan_start
** ��    �ܣ���Ƶ���ô�ֱ����ɨ����ʼ�߽�
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�auto_set_vert_scan_end
** ��    �ܣ���Ƶ���ô�ֱ����ɨ������߽�
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�set_hori_scan_start
** ��    �ܣ��Ƕ�����ˮƽ����ɨ����ʼ�߽�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_hori_scan_end
** ��    �ܣ��Ƕ�����ˮƽ����ɨ������߽�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_vert_scan_start
** ��    �ܣ��Ƕ����ô�ֱ����ɨ����ʼ�߽�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_vert_scan_end
** ��    �ܣ��Ƕ����ô�ֱ����ɨ������߽�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_hori_scan_inter
** ��    �ܣ�ˮƽɨ�����Ƕ�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_vert_scan_inter
** ��    �ܣ���ֱɨ�����Ƕ�
** data_high���Ƕȸ߷Ŵ�100��ȡ����ĸ�8λ
**  data_low���Ƕȸ߷Ŵ�100��ȡ����ĵ�8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_scan_speed
** ��    �ܣ�ɨ���ٶ�
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_scan_stoptime
** ��    �ܣ�����ɨ��ֹͣʱ��
**   H_time��ֹͣʱ���8λ
**   L_time��ֹͣʱ���8λ
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�start_scan_m
** ��    �ܣ�����������ɨ��
**     Num1��ɨ����ʼ���
**     Num2��ɨ��������
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�start_scan_s
** ��    �ܣ�����������ɨ��
**      Num��ɨ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�scan_enable
** ��    �ܣ�ʹ������ɨ��
**      Num��ɨ����
** enable=1��ʹ����Ӧ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void scan_enable(unsigned char addr,unsigned char Num,unsigned char enable)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf4;
  pelco_d_pack.cmd2  = Num;
  if (enable==1){              // 1 ʹ��
      pelco_d_pack.data1 = 0x01;
  }else{                       // 0 ʧ��
      pelco_d_pack.data1 = 0x00;
  }
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�scan_pause_continue
** ��    �ܣ�ֹͣ/�ָ�����ɨ��
**  pause=1������ɨ��ֹͣ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void scan_pause_continue(unsigned char addr,unsigned char pause)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  if (pause==1){              // 1 ֹͣɨ��
      pelco_d_pack.cmd2 = 0x03;
  }else{                      // 0 �ָ�ɨ��
      pelco_d_pack.cmd2 = 0x04;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�scan_close
** ��    �ܣ��ر�����ɨ��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���  
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
**      �������ƣ�scan_mode
**      ��    �ܣ�ɨ��ģʽ������/����ɨ�裩
** single_step=1������ɨ��ģʽ
**           Num��������
**       @author���Ĵ���Դ��ͨ��
**      �޸���־��2021-09-02
******************************************************************************/
void scan_mode(unsigned char addr,unsigned char Num,unsigned char single_step)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf5;
  pelco_d_pack.cmd2  = 0x06;
  pelco_d_pack.data1 = Num;
  if (single_step==1){         // 1 ����ɨ��
      pelco_d_pack.data2 = 0x02;
  }else{                       // 0 ����ɨ��
      pelco_d_pack.data2 = 0x01;
  }  
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** Ԥ��λɨ��
******************************************************************************/
/******************************************************************************
** �������ƣ�auto_set_preset_point
** ��    �ܣ���Ƶ����Ԥ��λ
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�recall_preset_point
** ��    �ܣ��ٻأ����ã�Ԥ��λ
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�del_preset_point
** ��    �ܣ�ɾ��Ԥ��λ
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�set_hori_preset_point
** ��    �ܣ��Ƕ�����ˮƽԤ��λ
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_vert_preset_point
** ��    �ܣ��Ƕ����ô�ֱԤ��λ
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_preset_point_stoptime
** ��    �ܣ�����Ԥ��λֹͣʱ��
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_preset_point_speed
** ��    �ܣ����õ���Ԥ��λ�ٶ�
**   PP_num��Ԥ��λ���
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�preset_point_start
** ��    �ܣ�����Ԥ��λɨ��
**   once=1������һ��Ԥ��λɨ��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void preset_point_start(unsigned char addr,unsigned char once,unsigned char PP_num1,unsigned char PP_num2)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf0;
  if (once==1){               // 1 ����һ��
      pelco_d_pack.cmd2 = 0x05;
  }else{                      // 0 ����������
      pelco_d_pack.cmd2 = 0x01;
  }
  pelco_d_pack.data1 = PP_num1;
  pelco_d_pack.data2 = PP_num2;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�preset_point_pause_continue
** ��    �ܣ�ֹͣ/�ָ�Ԥ��λɨ��
**  pause=1��Ԥ��λɨ��ֹͣ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void preset_point_pause_continue(unsigned char addr,unsigned char pause)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xf0;
  if (pause==1){              // 1 ֹͣ
      pelco_d_pack.cmd2 = 0x02;
  }else{                      // 0 �ָ�
      pelco_d_pack.cmd2 = 0x03;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�preset_point_close
** ��    �ܣ��ر�Ԥ��λɨ��
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}





/******************************************************************************
** ��̨�Զ���0λ
******************************************************************************/

/******************************************************************************
** �������ƣ�auto_set_zero_posit
** ��    �ܣ���Ƶ���û�׼0λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void auto_set_zero_posit(unsigned char addr,unsigned char dir)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe3;
  switch (dir)
  {             
  case 1:                     //ˮƽ 
     pelco_d_pack.cmd2  = 0x01; break;
  case 2:                     //��ֱ
     pelco_d_pack.cmd2  = 0x02; break;
  case 3:                     //ˮƽ�ʹ�ֱ
     pelco_d_pack.cmd2  = 0x03; break;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�set_zero_posit
** ��    �ܣ��Ƕ����û�׼0λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
/******************************���а汾�ݲ�֧�ָù���**************************
void set_zero_posit(unsigned char addr��unsigned char data_high, unsigned char data_low��unsigned char dir)
{
  unsigned char hori = 1;
  unsigned char vert = 0;
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xe3;
  if (dir==hori){             //ˮƽ 
      pelco_d_pack.cmd2  = 0x04;  
  }
  if (dir==vert){              //��ֱ
      pelco_d_pack.cmd2  = 0x05;     
  }
  pelco_d_pack.data1 = data_high;
  pelco_d_pack.data2 = data_low;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}
******************************************************************************/

/******************************************************************************
** �������ƣ�del_zero_posit
** ��    �ܣ�ɾ����׼0λ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}




/******************************************************************************
** �������ƣ�reset_reboot
** ��    �ܣ���̨��λ����
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�work_mode
** ��    �ܣ���ѯ����ģʽ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�work_state
** ��    �ܣ���ѯ����״̬
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�temp_inquire
** ��    �ܣ���ѯ�¶�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�volt_inquire
** ��    �ܣ���ѯ������ѹ
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}

/******************************************************************************
** �������ƣ�curr_inquire
** ��    �ܣ���ѯ��������
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�speed_inquire
** ��    �ܣ���ѯ�ٶ�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void speed_inquire(unsigned char addr,unsigned char speed)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xd0;
  switch (speed)
  {
  case 1://ˮƽ�봹ֱ�ٶ�
    pelco_d_pack.cmd2  = 0x00; break;
  case 2://ˮƽ�ٶ�
    pelco_d_pack.cmd2  = 0x01; break;
  case 3://��ֱ�ٶ�
    pelco_d_pack.cmd2  = 0x02; break;  
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�relay_control
** ��    �ܣ��̵������أ�������ʹ��:�̵���������ʱʹ�ã�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void relay_control(unsigned char addr, unsigned char relay)
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  pelco_d_pack.cmd1  = 0xc0;
  if (relay==1){             //�򿪼̵���
    pelco_d_pack.cmd2  = 0x01;
  }else{                     //�رռ̵�����Ĭ�ϳ��أ�
    pelco_d_pack.cmd2  = 0x00;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�clean_reset
** ��    �ܣ��������λ������
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}


/******************************************************************************
** �������ƣ�adjust_focal
** ��    �ܣ��۽�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-02
******************************************************************************/
void adjust_focal(unsigned char addr,unsigned char action )
{
  pelco_d_protocol  pelco_d_pack = {0xff,0x00,0x00,0x00,0x00,0x00,0x00};

  pelco_d_pack.addr  = addr;
  if (action == 1){             // 1 �۽�+
      pelco_d_pack.cmd1  = 0x01; 
      pelco_d_pack.cmd2  = 0x00;
  } else {                      // 0 �۽�-
      pelco_d_pack.cmd1  = 0x00;
      pelco_d_pack.cmd2  = 0x80;
  }
  pelco_d_pack.data1 = 0x00;
  pelco_d_pack.data2 = 0x00;
  pelco_d_pack.check = pelco_d_check(&pelco_d_pack.flag,sizeof(pelco_d_pack));
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));
}




/******************************************************************************
****ʵʱ�ش�
******************************************************************************/

/******************************************************************************
** �������ƣ�auto_return_temp
** ��    �ܣ��¶�ʵʱ�ش�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-06
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�auto_return_volt
** ��    �ܣ�������ѹʵʱ�ش�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-06
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�auto_return_curr
** ��    �ܣ���������ʵʱ�ش�
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-06
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�auto_return_angle
** ��    �ܣ��Ƕ�ʵʱ�ش�(ˮƽ�ʹ�ֱ)
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-06
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}

/******************************************************************************
** �������ƣ�auto_return_speed
** ��    �ܣ��ٶ�ʵʱ�ش�(ˮƽ�ʹ�ֱ)
**  @author���Ĵ���Դ��ͨ��
** �޸���־��2021-09-06
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
    
  // ͨ����̫��/485/422�ӿڷ���
  //send_data(pelco_d_pack, sizeof(pelco_d_pack));

}


