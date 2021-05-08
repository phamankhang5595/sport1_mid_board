#ifndef __POWER_COMMUNICATE_H__
#define __POWER_COMMUNICATE_H__
/*******************************************************************************
* Definition
*******************************************************************************/
#define POWER_COM_CMD_BUFF_MAX_LENGH  (5)
#define POWER_COM_CMD_HEADER_SIZE     (4)

typedef struct
{
    uint8_t command;
    uint8_t type;
    uint8_t sequence;
    uint8_t length;
    uint8_t buff[POWER_COM_CMD_BUFF_MAX_LENGH];
} power_com_cmd_t;

/*-----------------COMMAND-------------------*/
/* Start */
#define START_RUN                       (0x30)
/* Stop */
#define STOP_RUN                        (0x40)
/* SET SPEED MOTOR */
#define SET_SPEED_MOTOR                 (0x80)
/* FLOOR UP DOWN */
#define SET_INCLINE                     (0x90)

                /* TYPE */
enum cmd_t
{
    MASTER_REQUEST_TYPE =  0xB0,
    MASTER_REPONSE_TYE  =  0xB1,
    SLAVE_REQUEST_TYPE  =  0xA0,
    SLAVE_REPONSE_TYPE  =  0xA1,
};

/*******************************************************************************
* API
*******************************************************************************/
uint32_t power_com_init();
uint32_t power_com_send_cmd(power_com_cmd_t *command, uint32_t commandLength);
power_com_cmd_t power_com_data_to_cmd(uint8_t speed, uint8_t incline);


#endif
/*******************************************************************************
* EOF
*******************************************************************************/
