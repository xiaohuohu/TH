/*
*********************************************************************************************************
*                                       APPLICATION CONFIGURATION
*
*                             (c) Copyright 2014, Micrium, Inc., Weston, FL
*                                          All Rights Reserved
*
*                                            ZYNQ 7000 EPP
*                                               on the
*                                       ZC702 development board
*
*
* File          : app_cfg.h
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


#include  <cpu.h>
#include  <stdio.h>

#include  <lib_def.h>


//#include  <xparameters.h>

#define UCOS_AMP_MASTER				DEF_ENABLED

#define UCOS_ZYNQ_CONFIG_CACHES		DEF_DISABLED

#define UCOS_ZYNQ_ENABLE_CACHES		DEF_ENABLED

#define  APP_CPU_ENABLED DEF_ENABLED

#define  APP_LIB_ENABLED DEF_ENABLED

#define  APP_COMMON_ENABLED DEF_ENABLED

#define  APP_SHELL_ENABLED DEF_DISABLED

#define  APP_CLK_ENABLED DEF_DISABLED

#define  APP_OSIII_ENABLED DEF_ENABLED

#define  APP_OSII_ENABLED  DEF_DISABLED

#define  APP_TCPIP_ENABLED  DEF_DISABLED

#define  APP_DHCPC_ENABLED  DEF_DISABLED

#define  APP_DNSC_ENABLED  DEF_DISABLED

#define  APP_HTTPC_ENABLED  DEF_DISABLED

#define  APP_MQTTC_ENABLED  DEF_DISABLED

#define  APP_TELNETS_ENABLED  DEF_DISABLED

#define  APP_IPERF_ENABLED  DEF_DISABLED

#define  APP_FS_ENABLED  DEF_DISABLED

#define  APP_USBD_ENABLED  DEF_DISABLED

#define  APP_USBH_ENABLED  DEF_DISABLED

#define  APP_OPENAMP_ENABLED  DEF_DISABLED

//#define OS_TASK_TMR_PRIO 3


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define  APP_CFG_CAN_EN                             DEF_ENABLED
#define  APP_CFG_SERIAL_EN                          DEF_DISABLED


/*
*********************************************************************************************************
*                                            START TASK
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                       10u//8u
#define  APP_CFG_START_TASK_STK_SIZE                  1024//256u

#define  OS_TASK_TMR_PRIO                       (OS_LOWEST_PRIO - 2u)

/*
*********************************************************************************************************
*                                              CAN APP
*********************************************************************************************************
*/

#define  APP_CAN_DEMO_ECHO_TASK                         0u      /* Definition for APP_CAN_DEMO_SELECT, choose.. */
#define  APP_CAN_DEMO_RX_TX_TASK                        1u      /* .. between Echo Task or Tx/Rx Tasks          */

                                                                /* Select CAN Demo to run in Application        */
#define  APP_CAN_DEMO_SELECT      APP_CAN_DEMO_RX_TX_TASK



#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_ECHO_TASK)             /* Demo 'Echo' Task to Rx & Tx same Msg back   */
#define  APP_CAN0_ECHO_TASK_PRIO                        4u
#define  APP_CAN1_ECHO_TASK_PRIO                        5u

#define  APP_CAN0_ECHO_TASK_STK_SIZE                  256u
#define  APP_CAN1_ECHO_TASK_STK_SIZE                  256u
#endif


#if (APP_CAN_DEMO_SELECT == APP_CAN_DEMO_RX_TX_TASK)            /* Main Rx_Task for Incomming CAN BUS Msg(s)   */
#define  APP_CAN0_RX_TASK_PRIO                          4u
#define  APP_CAN0_TX_TASK_PRIO                          5u

#define  APP_CAN1_RX_TASK_PRIO                          6u
#define  APP_CAN1_TX_TASK_PRIO                          7u

#define  APP_CAN0_RX_TASK_STK_SIZE                    128u
#define  APP_CAN0_TX_TASK_STK_SIZE                    128u

#define  APP_CAN1_RX_TASK_STK_SIZE                    128u
#define  APP_CAN1_TX_TASK_STK_SIZE                    128u
#endif


/*
*********************************************************************************************************
*                                             TRACING
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                                0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                               1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                                2u
#endif

#define  APP_CFG_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_CFG_TRACE                                  printf

#define  APP_TRACE_INFO(x)    ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO) ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)  ? (void)(APP_CFG_TRACE x) : (void)0)


#endif /* __APP_CFG_H__ */
