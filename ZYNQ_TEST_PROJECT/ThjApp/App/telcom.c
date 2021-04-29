/*************************************************
  Copyright (C), 2004-2005, Adtech Tech. Co., Ltd.
  文件名:        telcom.h
  创建人：       范志州
  创建日期：		2005/08/12
  主要功能描述:   暂时只支持COM1,COM2，8位数据位，1位停止位格式
  		
  修改记录：
   1.   ...
*************************************************/

#include	"includes.h"
//#include	"2440addr.h"

TEST_EXT	OS_EVENT		*COMSem;        
extern	BASEINI	g_Ini;

INT16S		g_iComNum;  
INT32S		g_lBaudRate;


void	CTelCom_Init(INT16S comnum,INT32S baudrate)
{
	CTelCom_SetCom(comnum,baudrate);
}


void	CTelCom_Destroy()
{
	CTelCom_ResetCom();
}


void CTelCom_ResetCom()
{

}

INT16S  CTelCom_SetCom(INT16S comnum,INT32S baud)
{

	g_iComNum=comnum;

	uart_init(g_iComNum, baud, 8, 1, 0);

	uart_enable(g_iComNum);

#if FALSE
	int i;
	int pclk=PCLK; 
	
	g_iComNum=comnum; 
	  
	switch(comnum)
	{
	case 0:
		{
			rUFCON0 = 0x0;
			rUMCON0 = 0x0;
			rULCON0 = 0x3; 
			  
			// Line control register : Normal,No parity,1 stop,8 bits
		    // [10]       [9]     [8]        [7]        [6]      [5]         [4]           [3:2]        [1:0]
     		// Clock Sel,  Tx Int,  Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode
		    // 0          1       0    ,     0          1        0           0     ,       01          01
		    // PCLK       Level    Pulse    Disable    Generate  Normal      Normal        Interrupt or Polling
		    
		    rUCON0  = 0x245;                        	// Control register
		    
			// rUBRDIV0=( (int)(pclk/16./baud) -1 );   //Baud rate divisior register 0
			
		    rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );  //Baud rate divisior register 0
		}
		break;
		
	case 1:
		{
			rUFCON1 = 0x0;   							//UART channel 1 FIFO control register, FIFO disable		
		    rUMCON1 = 0x0;   							//UART chaneel 1 MODEM control register, AFC disable
		    rULCON1 = 0x3;
		    rUCON1  = 0x245;
		    rUBRDIV1=( (int)(pclk/16./baud+0.5) -1 );		    
		}
		break;
			
	case 2:
		{
			rUFCON2 = 0x0;   							//UART channel 1 FIFO control register, FIFO disable		
		    rUMCON2 = 0x0;   							//UART chaneel 1 MODEM control register, AFC disable
		    rULCON2 = 0x3;
		    rUCON2  = 0x245;
		    rUBRDIV2=( (int)(pclk/16./baud+0.5) -1 );		    
		}
		break;
			
	default:
		return 1;
	}
	
    for(i=0;i<100;i++);
    
	return 0;

#endif
}

INT16S 	CTelCom_SendChar(INT8U data,INT32S lTimeout )
{

	INT32S t=0;
	INT8U err;
	int res;

	OSSemPend(COMSem,0,&err);
	if(err != OS_NO_ERR)
		return 2000;

	while(uart_send(g_iComNum,data)==-1)
	{
    	t++;
    	if(t>lTimeout)
		{
			OSSemPost(COMSem);
    		return 2000;
    	}
	}

#if FALSE
	switch(g_iComNum)
	{
	case 0:
        while(!(rUTRSTAT0 & 0x2))   					//Wait until THR is empty.
        {
        	t++;
        	if(t>lTimeout)
			{
				OSSemPost(COMSem);    	
        		return 2000;      	
        	}
        }
		
        WrUTXH0(data);
        break;
        
    case 1:
        while(!(rUTRSTAT1 & 0x2))   					//Wait until THR is empty.
        {
        	t++;
        	
        	if(t>lTimeout)
        	{
        		OSSemPost(COMSem);
        		return 2000;     	
        	}
        }

        rUTXH1 = data;
        break;
        
    case 2:
    	/*
    	 if(data=='\n')
        {
            while(!(rUTRSTAT2 & 0x2));
            Delay(10);                 					//because the slow response of hyper_terminal 
            rUTXH2 = '\r';
        }
        */
        while(!(rUTRSTAT2 & 0x2))   					//Wait until THR is empty.
        {
        	t++;
        	if(t>lTimeout)
        	{
        		OSSemPost(COMSem);
        		return 2000;
        	}
        }
        
 		//Delay(3);
		//Delay(100);20070816 modify by fzz for usbhost operation
		
        rUTXH2 = data;
	}   
#endif


	
	OSSemPost(COMSem);


	return data;

}


INT16S	CTelCom_GetChar(INT32S lTimeout)
{

	INT8U err;
	u8 data;
	int res;
	static INT16S iComVlu =0;

	OSSemPend(COMSem,0,&err);
	
	if(err != OS_NO_ERR)
		return 2000;
	
	if(lTimeout<1)
		lTimeout=1;
	
	while(lTimeout--)
	{
		/*
		switch(g_iComNum)
		{
		case 0:
			if(rUTRSTAT0 & 0x1)    						//Receive data ready
			{
				OSSemPost(COMSem);
        	    return RdURXH0();
        	}
	        break;
    	case 1:
    		if(rUTRSTAT1 & 0x1)    					//Receive data ready
	    	 {
	    	 	OSSemPost(COMSem);
    	        return RdURXH1();
    	     }
        	break;
	    case 2:
    		 if(rUTRSTAT2 & 0x1)    					//Receive data ready
    		 {
    		 	OSSemPost(COMSem);
        	    return RdURXH2();
        	 }
	        break;
		}*/


	//	Uart_Printf("g_iComNum=%d\n",g_iComNum);
		res = uart_recv(g_iComNum,&data);

		if(res>0)
		{
			OSSemPost(COMSem);
			iComVlu = (INT16S)data;

		//	Uart_Printf("iComVlu=%d\n",iComVlu);
			return iComVlu;
		}
	/*	else
		{
			OSSemPost(COMSem);
			return 2000;
		}*/
	}
	
	OSSemPost(COMSem);
	return 2000;		

}


INT16S 	CTelCom_SendString(INT8U *buf,INT32S len,INT32S lTimeout)
{
	INT32S i;
	
	for(i=0;i<len;i++)
	{
		if(CTelCom_SendChar(buf[i],lTimeout)==2000)
			return 2000;
	}
	
	return 0;	
}

INT16S   CTelCom_GetString(INT8U *str, INT32S len,INT32S lTimeout)
{
	INT32S i;
	INT8U ch;
	
	for(i=0;i<len;i++)
	{
		ch=CTelCom_GetChar(lTimeout);
		
		if(ch==2000)
			return 2000;
			
		str[i]=ch;
	}
	
	return 0;
}


