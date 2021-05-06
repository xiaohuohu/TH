/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                            (c) Copyright 1992-2013, Micrium, Weston, FL
*                                           All Rights Reserved
*
* File    : os.h
* By      : Jean J. Labrosse
* Version : V2.92.10
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micrium to properly license
* its use in your product. We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
* 
* Note : This file is included in the uC/OS-II for compatibility with uC/OS-III and should not be used
*        in normal circumstances.
*********************************************************************************************************
*/

#include  <ucos_ii.h>
#include <cpu.h>
#include <os_type.h>


typedef  INT8U  OS_ERR;

/*
************************************************************************************************************************
************************************************************************************************************************
*                                                   # D E F I N E S
************************************************************************************************************************
************************************************************************************************************************
*/

/*
========================================================================================================================
*                                                      TASK STATUS
========================================================================================================================
*/

typedef  void                      (*OS_TASK_PTR)(void *p_arg);

typedef  OS_EVENT OS_SEM;

#define  OS_STATE_OS_STOPPED                    (OS_STATE)(0u)
#define  OS_STATE_OS_RUNNING                    (OS_STATE)(1u)

#define  OS_STATE_NOT_RDY                    (CPU_BOOLEAN)(0u)
#define  OS_STATE_RDY                        (CPU_BOOLEAN)(1u)


                                                                /* ------------------- TASK STATES ------------------ */
#define  OS_TASK_STATE_BIT_DLY               (OS_STATE)(0x01u)  /*   /-------- SUSPENDED bit                          */
                                                                /*   |                                                */
#define  OS_TASK_STATE_BIT_PEND              (OS_STATE)(0x02u)  /*   | /-----  PEND      bit                          */
                                                                /*   | |                                              */
#define  OS_TASK_STATE_BIT_SUSPENDED         (OS_STATE)(0x04u)  /*   | | /---  Delayed/Timeout bit                    */
                                                                /*   | | |                                            */
                                                                /*   V V V                                            */

#define  OS_TASK_STATE_RDY                    (OS_STATE)(  0u)  /*   0 0 0     Ready                                  */
#define  OS_TASK_STATE_DLY                    (OS_STATE)(  1u)  /*   0 0 1     Delayed or Timeout                     */
#define  OS_TASK_STATE_PEND                   (OS_STATE)(  2u)  /*   0 1 0     Pend                                   */
#define  OS_TASK_STATE_PEND_TIMEOUT           (OS_STATE)(  3u)  /*   0 1 1     Pend + Timeout                         */
#define  OS_TASK_STATE_SUSPENDED              (OS_STATE)(  4u)  /*   1 0 0     Suspended                              */
#define  OS_TASK_STATE_DLY_SUSPENDED          (OS_STATE)(  5u)  /*   1 0 1     Suspended + Delayed or Timeout         */
#define  OS_TASK_STATE_PEND_SUSPENDED         (OS_STATE)(  6u)  /*   1 1 0     Suspended + Pend                       */
#define  OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED (OS_STATE)(  7u)  /*   1 1 1     Suspended + Pend + Timeout             */
#define  OS_TASK_STATE_DEL                    (OS_STATE)(255u)

                                                                /* ----------------- PENDING ON ... ----------------- */
#define  OS_TASK_PEND_ON_NOTHING              (OS_STATE)(  0u)  /* Pending on nothing                                 */
#define  OS_TASK_PEND_ON_FLAG                 (OS_STATE)(  1u)  /* Pending on event flag group                        */
#define  OS_TASK_PEND_ON_TASK_Q               (OS_STATE)(  2u)  /* Pending on message to be sent to task              */
#define  OS_TASK_PEND_ON_MULTI                (OS_STATE)(  3u)  /* Pending on multiple semaphores and/or queues       */
#define  OS_TASK_PEND_ON_MUTEX                (OS_STATE)(  4u)  /* Pending on mutual exclusion semaphore              */
#define  OS_TASK_PEND_ON_Q                    (OS_STATE)(  5u)  /* Pending on queue                                   */
#define  OS_TASK_PEND_ON_SEM                  (OS_STATE)(  6u)  /* Pending on semaphore                               */
#define  OS_TASK_PEND_ON_TASK_SEM             (OS_STATE)(  7u)  /* Pending on signal  to be sent to task              */
#define  OS_TASK_PEND_ON_COND_VAR             (OS_STATE)(  8u)  /* Pending on condition variable                      */

/*
------------------------------------------------------------------------------------------------------------------------
*                                                    TASK PEND STATUS
*                                      (Status codes for OS_TCBs field .PendStatus)
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_STATUS_PEND_OK                   (OS_STATUS)(  0u)  /* Pending status OK, !pending, or pending complete   */
#define  OS_STATUS_PEND_ABORT                (OS_STATUS)(  1u)  /* Pending aborted                                    */
#define  OS_STATUS_PEND_DEL                  (OS_STATUS)(  2u)  /* Pending object deleted                             */
#define  OS_STATUS_PEND_TIMEOUT              (OS_STATUS)(  3u)  /* Pending timed out                                  */

/*
========================================================================================================================
*                                                   OS OBJECT TYPES
*
* Note(s) : (1) OS_OBJ_TYPE_&&& #define values specifically chosen as ASCII representations of the kernel
*               object types.  Memory displays of kernel objects will display the kernel object TYPEs with
*               their chosen ASCII names.
========================================================================================================================
*/

#define  OS_OBJ_TYPE_NONE                    (OS_OBJ_TYPE)CPU_TYPE_CREATE('N', 'O', 'N', 'E')
#define  OS_OBJ_TYPE_FLAG                    (OS_OBJ_TYPE)CPU_TYPE_CREATE('F', 'L', 'A', 'G')
#define  OS_OBJ_TYPE_MEM                     (OS_OBJ_TYPE)CPU_TYPE_CREATE('M', 'E', 'M', ' ')
#define  OS_OBJ_TYPE_MUTEX                   (OS_OBJ_TYPE)CPU_TYPE_CREATE('M', 'U', 'T', 'X')
#define  OS_OBJ_TYPE_Q                       (OS_OBJ_TYPE)CPU_TYPE_CREATE('Q', 'U', 'E', 'U')
#define  OS_OBJ_TYPE_SEM                     (OS_OBJ_TYPE)CPU_TYPE_CREATE('S', 'E', 'M', 'A')
#define  OS_OBJ_TYPE_MON                     (OS_OBJ_TYPE)CPU_TYPE_CREATE('M', 'O', 'N', ' ')
#define  OS_OBJ_TYPE_TASK_MSG                (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'M', 'S', 'G')
#define  OS_OBJ_TYPE_TASK_RESUME             (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'R', 'E', 'S')
#define  OS_OBJ_TYPE_TASK_SIGNAL             (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'S', 'I', 'G')
#define  OS_OBJ_TYPE_TASK_SUSPEND            (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'S', 'U', 'S')
#define  OS_OBJ_TYPE_TICK                    (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'I', 'C', 'K')
#define  OS_OBJ_TYPE_TMR                     (OS_OBJ_TYPE)CPU_TYPE_CREATE('T', 'M', 'R', ' ')

/*
========================================================================================================================
*                                           Possible values for 'opt' argument
========================================================================================================================
*/

#define  OS_OPT_NONE                         (OS_OPT)(0x0000u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                    DELETE OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_OPT_DEL_NO_PEND                  (OS_OPT)(0x0000u)
#define  OS_OPT_DEL_ALWAYS                   (OS_OPT)(0x0001u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     PEND OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_OPT_PEND_FLAG_MASK               (OS_OPT)(0x000Fu)
#define  OS_OPT_PEND_FLAG_CLR_ALL            (OS_OPT)(0x0001u)  /* Wait for ALL    the bits specified to be CLR       */
#define  OS_OPT_PEND_FLAG_CLR_AND            (OS_OPT)(0x0001u)

#define  OS_OPT_PEND_FLAG_CLR_ANY            (OS_OPT)(0x0002u)  /* Wait for ANY of the bits specified to be CLR       */
#define  OS_OPT_PEND_FLAG_CLR_OR             (OS_OPT)(0x0002u)

#define  OS_OPT_PEND_FLAG_SET_ALL            (OS_OPT)(0x0004u)  /* Wait for ALL    the bits specified to be SET       */
#define  OS_OPT_PEND_FLAG_SET_AND            (OS_OPT)(0x0004u)

#define  OS_OPT_PEND_FLAG_SET_ANY            (OS_OPT)(0x0008u)  /* Wait for ANY of the bits specified to be SET       */
#define  OS_OPT_PEND_FLAG_SET_OR             (OS_OPT)(0x0008u)

#define  OS_OPT_PEND_FLAG_CONSUME            (OS_OPT)(0x0100u)  /* Consume the flags if condition(s) satisfied        */


#define  OS_OPT_PEND_BLOCKING                (OS_OPT)(0x0000u)
#define  OS_OPT_PEND_NON_BLOCKING            (OS_OPT)(0x8000u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                  PEND ABORT OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_OPT_PEND_ABORT_1                 (OS_OPT)(0x0000u)  /* Pend abort a single waiting task                   */
#define  OS_OPT_PEND_ABORT_ALL               (OS_OPT)(0x0100u)  /* Pend abort ALL tasks waiting                       */

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     POST OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/


#define  OS_OPT_POST_NONE                    (OS_OPT)(0x0000u)

#define  OS_OPT_POST_FLAG_SET                (OS_OPT)(0x0000u)
#define  OS_OPT_POST_FLAG_CLR                (OS_OPT)(0x0001u)

#define  OS_OPT_POST_FIFO                    (OS_OPT)(0x0000u)  /* Default is to post FIFO                            */
#define  OS_OPT_POST_LIFO                    (OS_OPT)(0x0010u)  /* Post to highest priority task waiting              */
#define  OS_OPT_POST_1                       (OS_OPT)(0x0000u)  /* Post message to highest priority task waiting      */
#define  OS_OPT_POST_ALL                     (OS_OPT)(0x0200u)  /* Broadcast message to ALL tasks waiting             */

#define  OS_OPT_POST_NO_SCHED                (OS_OPT)(0x8000u)  /* Do not call the scheduler if this is selected      */

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     TASK OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_OPT_TASK_NONE                    (OS_OPT)(0x0000u)  /* No option selected                                 */
#define  OS_OPT_TASK_STK_CHK                 (OS_OPT)(0x0001u)  /* Enable stack checking for the task                 */
#define  OS_OPT_TASK_STK_CLR                 (OS_OPT)(0x0002u)  /* Clear the stack when the task is create            */
#define  OS_OPT_TASK_SAVE_FP                 (OS_OPT)(0x0004u)  /* Save the contents of any floating-point registers  */
#define  OS_OPT_TASK_NO_TLS                  (OS_OPT)(0x0008u)  /* Specifies the task DOES NOT require TLS support    */

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     TIME OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

//#define  OS_OPT_TIME_DLY                             DEF_BIT_NONE
#define  OS_OPT_TIME_TIMEOUT                ((OS_OPT)DEF_BIT_01)
#define  OS_OPT_TIME_MATCH                  ((OS_OPT)DEF_BIT_02)
#define  OS_OPT_TIME_PERIODIC               ((OS_OPT)DEF_BIT_03)

#define  OS_OPT_TIME_HMSM_STRICT            ((OS_OPT)DEF_BIT_NONE)
#define  OS_OPT_TIME_HMSM_NON_STRICT        ((OS_OPT)DEF_BIT_04)

#define  OS_OPT_TIME_MASK                   ((OS_OPT)(OS_OPT_TIME_DLY      | \
                                                      OS_OPT_TIME_TIMEOUT  | \
                                                      OS_OPT_TIME_PERIODIC | \
                                                      OS_OPT_TIME_MATCH))

#define  OS_OPT_TIME_OPTS_MASK                       (OS_OPT_TIME_DLY            | \
                                                      OS_OPT_TIME_TIMEOUT        | \
                                                      OS_OPT_TIME_PERIODIC       | \
                                                      OS_OPT_TIME_MATCH          | \
                                                      OS_OPT_TIME_HMSM_NON_STRICT)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                    TIMER OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

#define  OS_OPT_TMR_NONE                          (OS_OPT)(0u)  /* No option selected                                 */

#define  OS_OPT_TMR_ONE_SHOT                      (OS_OPT)(1u)  /* Timer will not auto restart when it expires        */
#define  OS_OPT_TMR_PERIODIC                      (OS_OPT)(2u)  /* Timer will     auto restart when it expires        */

#define  OS_OPT_TMR_CALLBACK                      (OS_OPT)(3u)  /* OSTmrStop() option to call 'callback' w/ timer arg */
#define  OS_OPT_TMR_CALLBACK_ARG                  (OS_OPT)(4u)  /* OSTmrStop() option to call 'callback' w/ new   arg */

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     TIMER STATES
------------------------------------------------------------------------------------------------------------------------
*/

//#define  OS_TMR_STATE_UNUSED                    (OS_STATE)(0u)
//#define  OS_TMR_STATE_STOPPED                   (OS_STATE)(1u)
//#define  OS_TMR_STATE_RUNNING                   (OS_STATE)(2u)
//#define  OS_TMR_STATE_COMPLETED                 (OS_STATE)(3u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                       PRIORITY
------------------------------------------------------------------------------------------------------------------------
*/
                                                                    /* Dflt prio to init task TCB                     */
#define  OS_PRIO_INIT                       (OS_PRIO)(OS_CFG_PRIO_MAX)



