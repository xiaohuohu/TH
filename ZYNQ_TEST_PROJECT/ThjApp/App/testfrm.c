# include	"includes.h"

extern		BOOL		g_lHandRun;
extern		INT32S		g_iWorkStatus;
extern 		BASEINI		g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	OS_EVENT	*FreeSem;

WORK_EXT 	BOOL		g_bFirstResetX;							//ָʾX���Ƿ�λ��FALSEΪ��Ҫ��λ
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT 	BOOL		g_bFirstResetV;
WORK_EXT 	BOOL		g_bFirstResetW;

WORK_EXT	INT16S		g_iMoveAxis;
WORK_EXT	FP32 		g_fSpeedBi;
WORK_EXT	BOOL		g_bHandset;
WORK_EXT	BOOL		g_bCylinder; 
//WORK_EXT	BOOL		g_bFunc;

#define		MAXIN	 		54
#define 	MAXOUT			16



static      CRect		g_ptrOutRect[MAXOUT];
BOOL		g_bOutStatus[MAXOUT];
BOOL		g_bTestFrm=FALSE;
char 		g_InputIO[MAXIN];
char 		g_ProStatus[8];
//INT16U		g_iTestAxis;
//INT16U		g_iTestBoxAxis=0;
//BOOL        g_handAxis=FALSE;									//��ʾ���������Ƿ�򿪣���״̬�¿���ѡ������������


/***************************************
	����������

***************************************/

void	CTestFrm_Init()
{
	INT16S i=0;
	
	g_bTestFrm=TRUE;
	
	
		
	for(i=0;i<MAXOUT;i++)
		g_bOutStatus[i]=GetOut(i);	
		

	/*
	for(i=0; i<MAXCH; i++)
	{
		SetCommandPos(i+1,0);
		SetActualPos(i+1,0);
	}*/
				
	AllLedOff();
	LedOut(BOXLED,0);			//�ر�"�ֺ�"ָʾ��,�˳����Խ���ʱ�ٸ���g_bHandset���µƵ�״̬��

}



void    CTestFrm_Frame_Create()
{
//	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),g_TextColor3,1);
	CDC_PutColorBar(0,0,getmaxx(),getmaxy(),DARKGRAYCOLOR,1);
	
	
	CDC_OutText_UTF(6,getmaxy()-31,g_bEnglish?"Msg:":"ѶϢ:",g_TextColor1,DARKGRAYCOLOR,KZK_24);
	CMsg_SetWH(60,566,600,28,g_TextColor11,DARKGRAYCOLOR,DARKGRAYCOLOR);


	CDC_Rectangle(0,0,getmaxx(),getmaxy(),g_TextColor10);
	CDC_Rectangle(1,1,getmaxx()-1,getmaxy()-1,g_TextColor10);
	CDC_Rectangle(2,2,getmaxx()-2,getmaxy()-2,g_TextColor10);
	
	CDC_PutColorBar(3,3,getmaxx()-6,32,g_nTableBk,100);
		
	CDC_Line(3,33,getmaxx()-3,33,g_TextColor10);
	CDC_Line(3,34,getmaxx()-3,34,g_TextColor10);
	CDC_Line(3,35,getmaxx()-3,35,g_TextColor10);
	
	if(g_bEnglish)
		CDC_OutText24(215,4,"Spring Input/Output Check",g_TextColor11,g_nTableBk);
	else
		CDC_OutText_UTF(220,4,"���ɻ�����/������޲���",g_TextColor11,g_nTableBk,KZK_24);
		
		
		
}

void	CTestIoFrm_Create()
{	
	int i;
	
	OSQPost(MsgQueue,g_bEnglish?"Press Key \"X,Y,Z,A Axis\" to Fine Home.":"�� \"X,Y,Z,A��\" ��������");
	
//	CDC_PutColorBar(3,36,getmaxx()-6,530,DARKGRAYCOLOR,1);
	
	CDC_SetRectangle3(10,56,540,500,g_bEnglish?"Input":"����",g_bEnglish?75:55,DARKGRAYCOLOR,1);
	CDC_SetRectangle3(560,56,230,380,g_bEnglish?"Output":"���",g_bEnglish?95:55,DARKGRAYCOLOR,1);



	CDC_OutText_UTF(30,g_bEnglish?492:490, g_bEnglish?"TestKnob:":"������ť:",g_TextColor11,DARKGRAYCOLOR,KZK_22);
	CDC_OutText_UTF(265,g_bEnglish?492:490,g_bEnglish?"HandPulse:":"��ҡ����:",g_TextColor11,DARKGRAYCOLOR,KZK_22);
	CDC_OutText_UTF(30,g_bEnglish?522:520,g_bEnglish?"BoxPulse:":"�ֺ�����:",g_TextColor11,DARKGRAYCOLOR,KZK_22);
//	CDC_OutText_UTF(265,g_bEnglish?522:520,g_bEnglish?"LagPulse:":"�ͺ�����:",g_TextColor11,DARKGRAYCOLOR,KZK_22);
	
	/*
	CDC_Rectangle(560,304,790,556,WHITECOLOR);
	
	for(i=1;i<7;i++)
		CDC_Line(560,304+36*i,790,304+36*i,WHITECOLOR);
	
	
	CDC_Line(675,304,675,556,WHITECOLOR);
	*/
//	CDC_OutText_UTF(571,310,g_bEnglish?"Logical":"�߼�λ��",g_TextColor11,DARKGRAYCOLOR,KZK_24);
//	CDC_OutText_UTF(681,310,g_bEnglish?"Actual":"ʵ��λ��",g_TextColor11,DARKGRAYCOLOR,KZK_24);
//	CDC_OutText_UTF(g_bEnglish?690:720,getmaxy()-30,g_bEnglish?"HandOff ":"���ֹ�",REDCOLOR,DARKGRAYCOLOR,KZK_22);
	
	memset(g_InputIO,2,MAXIN);
	memset(g_ProStatus,2,8);
	
	{
		INT16S k=0,j,i;
		INT16S x=0,y=0,w=100,h=43;
		
		for( j=0; j<8; j++)
		{//����10��������ƾ�����
			for(i=0; i<2; i++,k++)
			{
				x = 580 + i * w;
				y = 80 + j * h;
				
				//Uart_Printf("k=%d\n",k);
				g_ptrOutRect[k].left=x;
				g_ptrOutRect[k].top=y;
				g_ptrOutRect[k].right=x+w-8;
				g_ptrOutRect[k].bottom=y+h-12;
				//SetRect(x,y,x+w-5,y+h-10);
				CTestFrm_DrawOutputStatus(k,g_bOutStatus[k]);//�����������״̬��ʾ
			}
		}

	}
	

}


void	CTestFrm_Destroy()
{
	g_bTestFrm=FALSE;
	CMsg_SetWH(60,566,600,28,g_TextColor11,g_nBk,g_nBk);
	
	BackZeroLed();
	
	LedOut(STOPLED,1);
	LedOut(CYLINDERLED,g_bCylinder?1:0);	
	
//	g_Ini.iHandStatus==0;
	LedOut(HANDLED,g_Ini.iHandStatus==1?1:0);
	
//	g_bHandset=FALSE;
	LedOut(BOXLED,g_bHandset?1:0);	
	
	LedOut(YSWITCHLED,g_Ini.iAxisSwitch[1]?0:1);
	LedOut(ZSWITCHLED,g_Ini.iAxisCount>2?(g_Ini.iAxisSwitch[2]?0:1):0);
	LedOut(USWITCHLED,g_Ini.iAxisCount>3?(g_Ini.iAxisSwitch[3]?0:1):0);
	LedOut(VSWITCHLED,g_Ini.iAxisCount>4?(g_Ini.iAxisSwitch[4]?0:1):0);
	LedOut(WSWITCHLED,g_Ini.iAxisCount>5?(g_Ini.iAxisSwitch[5]?0:1):0);
	UpdateCurAxis();
}


void    CTestFrm_DrawInputStatus(INT16S io,INT16S status)
{
	char str[0x10]="\0";
	char str1[0x10]="\0";
	if(io<=MAXIN)
	{   //һ��Ҫע���ֹ����Խ��
		if(g_InputIO[io]==status)return;
		g_InputIO[io]=status;
		
	//	Uart_Printf("a=%d b=%d\n",io,status);
		
		switch(io)
		{
			case 0:strcpy(str,g_bEnglish?"Pr1":"����1");
					strcpy(str1,g_bEnglish?"":"  PRO1");			break;
			case 1:strcpy(str,g_bEnglish?"Pr2":"����2");
					strcpy(str1,g_bEnglish?"":"  PRO2");			break;
			case 2:strcpy(str,g_bEnglish?"Pr3":"����3");
					strcpy(str1,g_bEnglish?"":"  PRO3");			break;
			case 3:strcpy(str,g_bEnglish?"Pr4":"����4");
					strcpy(str1,g_bEnglish?"":"  PRO4");			break;
			case 4:strcpy(str,g_bEnglish?"XHom":"Xԭ��");
					strcpy(str1,g_bEnglish?"":"XHOME");			break;
			case 5:strcpy(str,g_bEnglish?"YHom":"Yԭ��");
					strcpy(str1,g_bEnglish?"":"YHOME");			break;
			case 6:strcpy(str,g_bEnglish?"Pr5":"����5");
					strcpy(str1,g_bEnglish?"":"    IN5");			break;
			case 7:strcpy(str,g_bEnglish?"Pr6":"����6");
					strcpy(str1,g_bEnglish?"":"    IN6");			break;
			case 8:strcpy(str,g_bEnglish?"Pr7":"����7");
					strcpy(str1,g_bEnglish?"":"    IN7");			break;

			case 9:strcpy(str,g_bEnglish?"safedoor":"��ȫ��");
					strcpy(str1,g_bEnglish?"":"    IN8");			break;
			case 10:strcpy(str,g_bEnglish?"ZHom":"Zԭ��");
					strcpy(str1,g_bEnglish?"":"ZHOME");			break;
			case 11:strcpy(str,g_bEnglish?"AHom":"Aԭ��");
					strcpy(str1,g_bEnglish?"":"AHOME");			break;
			case 12:strcpy(str,g_bEnglish?"BHom":"Bԭ��");
					strcpy(str1,g_bEnglish?"":"BHOME");			break;
			case 13:strcpy(str,g_bEnglish?"CHom":"Cԭ��");
					strcpy(str1,g_bEnglish?"":"CHOME");			break;
			case 14:strcpy(str,g_bEnglish?"XLmt+":"X����");
					strcpy(str1,g_bEnglish?"":" XLMT+");			break;
			case 15:strcpy(str,g_bEnglish?"XLmt-":"X����");
					strcpy(str1,g_bEnglish?"":" XLMT-");			break;
			case 16:strcpy(str,g_bEnglish?"YLmt+":"Y����");
					strcpy(str1,g_bEnglish?"":" YLMT+");			break;
			case 17:strcpy(str,g_bEnglish?"YLmt-":"Y����");
					strcpy(str1,g_bEnglish?"":" YLMT-");			break;
			
			case 18:strcpy(str,g_bEnglish?"ZLmt+":"Z����");
					strcpy(str1,g_bEnglish?"":" ZLMT+");			break;
			case 19:strcpy(str,g_bEnglish?"ZLmt-":"Z����");
					strcpy(str1,g_bEnglish?"":" ZLMT-");			break;
			case 20:strcpy(str,g_bEnglish?"ALmt+":"A����");
					strcpy(str1,g_bEnglish?"":" ALMT+");			break;
			case 21:strcpy(str,g_bEnglish?"ALmt-":"A����");
					strcpy(str1,g_bEnglish?"":" ALMT-");			break;
			case 22:strcpy(str,g_bEnglish?"BLmt+":"B����");
					strcpy(str1,g_bEnglish?"":" BLMT+");			break;
			case 23:strcpy(str,g_bEnglish?"BLmt-":"B����");
					strcpy(str1,g_bEnglish?"":" BLMT-");			break;
			case 24:strcpy(str,g_bEnglish?"CLmt+":"C����");
					strcpy(str1,g_bEnglish?"":" CLMT+");			break;
			case 25:strcpy(str,g_bEnglish?"CLmt-":"C����");
					strcpy(str1,g_bEnglish?"":" CLMT-");			break;
			case 26:strcpy(str,g_bEnglish?"FulW":"����");
					strcpy(str1,g_bEnglish?"":"   ����");			break;
			
			case 27:strcpy(str,g_bEnglish?"BrkW":"����");					break;
			case 28:strcpy(str,g_bEnglish?"XAlm":"X����");				break;
			case 29:strcpy(str,g_bEnglish?"YAlm":"Y����");				break;
			case 30:strcpy(str,g_bEnglish?"ZAlm":"Z����");				break;
			case 31:strcpy(str,g_bEnglish?"AAlm":"A����");				break;
			case 32:strcpy(str,g_bEnglish?"BAlm":"B����");				break;
			case 33:strcpy(str,g_bEnglish?"CAlm":"C����");				break;
			case 34:strcpy(str,g_bEnglish?"XOrg":"X���");				break;
			case 35:strcpy(str,g_bEnglish?"YOrg":"Y���");				break;
			
			case 36:strcpy(str,g_bEnglish?"ZOrg":"Z���");				break;
			case 37:strcpy(str,g_bEnglish?"AOrg":"A���");				break;
			case 38:strcpy(str,g_bEnglish?"BOrg":"B���");				break;
			case 39:strcpy(str,g_bEnglish?"COrg":"C���");				break;
			case 40:strcpy(str,g_bEnglish?"Stop1":"ֹͣ");				break;
			case 41:strcpy(str,g_bEnglish?"HndA":"����A");				break;
			case 42:strcpy(str,g_bEnglish?"HndB":"����B");				break;
			case 43:strcpy(str,g_bEnglish?"BoxA":"�ֺ�A");				break;
			case 44:strcpy(str,g_bEnglish?"BoxB":"�ֺ�B");				break;
			
			case 45:strcpy(str,g_bEnglish?"Low ":"����");					break;
			case 46:strcpy(str,g_bEnglish?"Mid ":"����");					break;
			case 47:strcpy(str,g_bEnglish?"High":"����");					break;
			case 48:strcpy(str,g_bEnglish?"SGL ":"����");					break;
			case 49:strcpy(str,g_bEnglish?"    X":"X��");				break;
			case 50:strcpy(str,g_bEnglish?"    Y":"Y��");				break;
			case 51:strcpy(str,g_bEnglish?"    Z":"Z��");				break;
			case 52:strcpy(str,g_bEnglish?"    A":"A��");				break;
			case 53:strcpy(str,g_bEnglish?"Stop2":"��ͣ2");				break;
//
//			case 54:strcpy(str,g_bEnglish?"Stop2":"��ͣ2");		break;
//			case 55:strcpy(str,g_bEnglish?"22":"22");		break;
//
			
			
		}
		
		{
			INT16S x,y;
			
			{
				CRect rc;
			
				if(io<27)
				{
					x= 20 +(io/8)*90;

					y=80+(io%8)*51;

//					CRect rc1;

					rc.left = x;
					rc.top = y;
					rc.right = x+88;
					rc.bottom = y+24;

//					rc1.left = x;
//					rc1.top = y+24;
//					rc1.right = x+88;
//					rc1.bottom = y+48;
					CDC_PutColorBar(x,y,88,48,status==0?REDCOLOR:BLUECOLOR,1);
				
					if(!g_bEnglish)
					{
						CDC_OutCenTextKzk22(rc, str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR);
						CDC_OutTextLR_UTF(rc.left,rc.top+24,str1,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR,KZK_22,0,1);
						//CDC_OutCenTextKzk22(rc1, str1,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR);
					}
					else
					{
						CDC_OutTextLR_UTF(rc.left+3,rc.top+6,str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR,KZK_22,0,1);
					}

					CDC_Rectangle4(x,y,x+88,y+48,YELLOWCOLOR);
				}
				else if(io<34)
				{
					x= 290;
					y=233+(io-27)*37;
					rc.left = x;
					rc.top = y;
					rc.right = x+88;
					rc.bottom = y+30;

					CDC_PutColorBar(x,y,88,30,status==0?REDCOLOR:BLUECOLOR,1);

					if(!g_bEnglish)
					{
						CDC_OutCenTextKzk22(rc, str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR);
					}
					else
					{
						CDC_OutTextLR_UTF(rc.left+3,rc.top+6,str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR,KZK_22,0,1);
					}

					CDC_Rectangle4(x,y,x+88,y+30,YELLOWCOLOR);

				}
				else
				{
					x= 20 +((io-34)/10)*85+360;

					y=80+((io-34)%10)*41;


					rc.left = x;
					rc.top = y;
					rc.right = x+80;
					rc.bottom = y+35;

					CDC_PutColorBar(x,y,80,35,status==0?REDCOLOR:BLUECOLOR,1);

					if(!g_bEnglish)
					{
						CDC_OutCenTextKzk22(rc, str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR);
					}
					else
					{
						CDC_OutTextLR_UTF(rc.left+3,rc.top+6,str,WHITECOLOR,status==0?REDCOLOR:BLUECOLOR,KZK_22,0,1);
					}

					CDC_Rectangle4(x,y,x+80,y+35,YELLOWCOLOR);
				}
			}
		}
	}
}


void	CTestFrm_DrawOutputStatus(INT16S i,BOOL status)		//���״̬��ʾ����
{
	char str[10]="\0";
	
	switch(i)
	{
		case 0:	strcpy(str,g_bEnglish?"Out1":"����1");	break;
		case 1:	strcpy(str,g_bEnglish?"Out2":"����2");	break;
		case 2:	strcpy(str,g_bEnglish?"Out3":"����3");	break;
		case 3:	strcpy(str,g_bEnglish?"Out4":"����4");	break;
		case 4:	strcpy(str,g_bEnglish?"Out5":"����5");	break;
		case 5:	strcpy(str,g_bEnglish?"Out6":"����6");	break;
		case 6:	strcpy(str,g_bEnglish?"Out7":"����7");	break;
		case 7:	strcpy(str,g_bEnglish?"Out8":"����8");	break;
		case 8:	strcpy(str,g_bEnglish?"Out9":"����9");	break;
		case 9: strcpy(str,g_bEnglish?"Out10":"����10");	break;
		
		case 10:	strcpy(str,g_bEnglish?"Out11":"����11");	break;
		case 11:	strcpy(str,g_bEnglish?"Out12":"ͣ��12");	break;
		case 12:	strcpy(str,g_bEnglish?"Out13":"����13");	break;
		case 13:	strcpy(str,g_bEnglish?"Out14":"����14");	break;
		case 14:	strcpy(str,g_bEnglish?"Out15":"����15");	break;
		case 15:	strcpy(str,g_bEnglish?"Out16":"�߼�16");	break;
		
		
	}
	
	
	CDC_PutColorBar2(g_ptrOutRect[i],status==0?BLUECOLOR:REDCOLOR,1);
	CDC_Rectangle3(g_ptrOutRect[i],YELLOWCOLOR);
	CDC_OutCenTextKzk22(g_ptrOutRect[i],str,WHITECOLOR,status==0?BLUECOLOR:REDCOLOR);
	WriteBit(i,status?1:0);
}


void	CTestFrm_OnKey(INT16U key,INT16U tkey)
{
	INT16S i=-1;//,j=0;
	INT8S buffMsg[80]=""; 
	
	if( (key >='1' && key <='9')||( key == '.')||(key==TZERO))
	{
    	static char disp[0x03]="\0";
    	static int  dispjs=0;
    	
		if(key >='1' && key <='9' && dispjs==0 )
		{
			key -= '1';
			g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
			CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);
		}
		else
		{
			if(key == TZERO)
				key = '0';

    		disp[dispjs++] = key;
    		dispjs = dispjs%2;											//�����һ�ΰ��µ���0�����ټ�¼һ�����֣������ַ�һ��������
    		
        	if(strcmp(disp,".0")==0)    
        		key=9;
         	if(strcmp(disp,".1")==0)   									//����16������� 
        		key=10;
        	if(strcmp(disp,".2")==0)    
        		key=11;
        	if(strcmp(disp,".3")==0)    
        		key=12;   
        	if(strcmp(disp,".4")==0)    
        		key=13; 
        	if(strcmp(disp,".5")==0)    
        		key=14;         		        		
        	if(strcmp(disp,".6")==0)    
        		key=15; 
        		        		    		
            if(dispjs==0)  
            	memset(disp,NULL,3);
            
            if(key>=9 && key<=15)
            {	
				g_bOutStatus[key] = g_bOutStatus[key]?FALSE:TRUE;
				CTestFrm_DrawOutputStatus(key,g_bOutStatus[key]);
			}
		}
	}
	
	switch(key)
	{
	
	case TBACKZERO:
	
		{
		#ifdef	PRINTBMP
			{
				char filename[0x20];
				INT16S i=0;
				
				
				strcpy(filename,"testFrm");
				
				for(i=0;i<strlen(filename);i++)
				{
					if(filename[i]=='.')
					{
						filename[i]='\0';
						break;
					}
				}
				strcat(filename,".bmp");
				CreatBMP(filename,800,600,1);
				
				OSQPost(MsgQueue,"Write Bmp End!");
			}
			
			break;
			#endif
		}
		
		/*
		{
			INT16U	i = 0;
		
			for(i=0; i<MAXCH; i++)
			{
				SetCommandPos(i+1,0);
				SetActualPos(i+1,0);
			}
			
			OSQPost(MsgQueue," ");
		}*/
		break;
	
	
	case TCANCEL: 
		{
				Ok_Exit=TRUE;
		}
		break;
	
	
	
	case TX :
		
		{
			
			{
				CTestFrm_FindZero(1);
			}
		}
		break;
		
	case TY:
			if(g_Ini.iAxisSwitch[1]==0)
				break;
				
			if( g_Ini.iAxisRunMode[1] != 2)
			{
				CTestFrm_FindZero(2);
			}
			else
				OSQPost(MsgQueue,"˿��ģʽ�²���ʹ�������ҵ�Ȧ����");
			
			break;
		
	case TZ :
		
		{
			if(g_Ini.iAxisCount<3 || g_Ini.iAxisSwitch[2]==0)
				break;
			
			if( g_Ini.iAxisRunMode[2] != 2)
			{
				CTestFrm_FindZero(3);
			}
			else
				OSQPost(MsgQueue,"˿��ģʽ�²���ʹ�������ҵ�Ȧ����");
		}
		break;
		
	case TU:
		
		{
			if(g_Ini.iAxisCount<4 || g_Ini.iAxisSwitch[3]==0)
				break;
			
			if( g_Ini.iAxisRunMode[3] != 2)
			{	
				CTestFrm_FindZero(4);
			}
			else
				OSQPost(MsgQueue,"˿��ģʽ�²���ʹ�������ҵ�Ȧ����");
		}
		break;


	
	default:		
		return;
	
	}
}


void	CTestFrm_FindZero(INT16S ch)
{
	char buf[0x30];
	//char axis[0x10];
	SetMM(ch,0,TRUE);
	
	OSQPost(MsgQueue,axisMsg(ch,g_bEnglish?" FindZero...":"Ѱ�����...",TRUE));
	
	
	
	{
		if(CTestFrm_BackZero(ch,g_Ini.iBackDir[ch-1]))
		{
			sprintf(buf,g_bEnglish?"%s FindZero Finished,AxisPulse:%5ld":"%s�������,������:%5ld",axisMsg(ch,"",FALSE),GetPulse(ch,TRUE));
			OSQPost(MsgQueue,buf);
			OSTimeDly(200);
			//GetKey(FALSE);
			SetMM(ch,0,TRUE);
			
		}
		else
		{
			OSQPost(MsgQueue,g_bEnglish?"FindZero Error":"Ѱ��������!");
			OSTimeDly(500);
			//GetKey(FALSE);		
		}
	}
	
	//GetKey(FALSE);		
	//OSQPost(MsgQueue," ");
	
	switch(ch)
	{
		case 1:g_bFirstResetX=FALSE;break;
		case 2:g_bFirstResetY=FALSE;break;
		case 3:g_bFirstResetZ=FALSE;break;
		case 4:g_bFirstResetU=FALSE;break;
		case 5:g_bFirstResetV=FALSE;break;
		case 6:g_bFirstResetW=FALSE;break;
		default:break;
	}
}


INT16S	CTestFrm_DoModal()
{
	INT16U key=0,tkey=0;

	
	CTestFrm_Frame_Create();									//���Խ�����
	CTestIoFrm_Create();							    		//IO��Ͻ���	
	
	while(TRUE)
	{	
		
		{	
						
			CTestFrm_UpdateInputStatuses();						//�����ⲿ�������ʾ״̬
			CTestFrm_UpdateProbeStatuses();						//����̽����ʾ״̬
			CTestFrm_UpdateTestKnob();							//���²�����ť��ʾֵ
			
			CTestFrm_UpdateHandWheelPos();
		}
		
		
		key=KeyMBoxGetKey();
				
		if(key!=0xffff)
		{			
			CTestFrm_OnKey(key,tkey);
			
			if(Ok_Exit)
			{
				break;
			}
		}
		
		OSTimeDly(3);
	}
	
	
	Ok_Exit=FALSE;
	return g_iDlgIDOk;
}


void	CTestFrm_UpdateInputStatuses()
{
	static INT8S js=0;

	switch(js)
		{
		case 0:
			CTestFrm_DrawInputStatus(js,ReadBit(4));		//̽��1//����1
			break;
			
		case 1:
			CTestFrm_DrawInputStatus(js,ReadBit(3));		//̽��2//����2
			break;
		
		case 2:
			CTestFrm_DrawInputStatus(js,ReadBit(9));		//̽��3//����3
			break;
		
		case 3:
			CTestFrm_DrawInputStatus(js,ReadBit(8));		//̽��4//����4
			break;
		
		case 4:
			CTestFrm_DrawInputStatus(js,ReadBit(2));		//X�ŷ�ԭ��
			break;	
			
		case 5:
			CTestFrm_DrawInputStatus(js,ReadBit(7));		//Y�ŷ�ԭ��
			break;
			
		case 6:
			CTestFrm_DrawInputStatus(js,ReadBit(14));		//̽��5//����5
			break;
			
		case 7:
			CTestFrm_DrawInputStatus(js,ReadBit(13));		//̽��6//����6
			break;
			
		case 8:
			CTestFrm_DrawInputStatus(js,ReadBit(19));		//̽��7//����7
			break;
		
		case 9:
			CTestFrm_DrawInputStatus(js,ReadBit(18));		//��ȫ��//����8
			break;	
		
		case 10:
			CTestFrm_DrawInputStatus(js,ReadBit(12));		//Z�ŷ�ԭ��
			break;
			
		case 11:
			CTestFrm_DrawInputStatus(js,ReadBit(17));		//A�ŷ�ԭ��
			break;
		
		case 12:
			CTestFrm_DrawInputStatus(js,ReadBit(39));		//B�ŷ�ԭ��
			break;
			
		case 13:
			CTestFrm_DrawInputStatus(js,ReadBit(42));		//C�ŷ�ԭ��
			break;
		
		case 14:
			CTestFrm_DrawInputStatus(js,ReadBit(24));		//X����λ
			break;	
			
			
		case 15:
			CTestFrm_DrawInputStatus(js,ReadBit(25));		//X����λ
			break;
			
		case 16:
			CTestFrm_DrawInputStatus(js,ReadBit(26));		//Y����λ
			break;
			
		case 17:
			CTestFrm_DrawInputStatus(js,ReadBit(27));		//Y����λ
			break;
			
		case 18:
			CTestFrm_DrawInputStatus(js,ReadBit(28));		//Z����λ
			break;
		
		case 19:
			CTestFrm_DrawInputStatus(js,ReadBit(29));		//Z����λ
			break;	
				
		case 20:
			CTestFrm_DrawInputStatus(js,ReadBit(30));		//A����λ
			break;
			
		case 21:
			CTestFrm_DrawInputStatus(js,ReadBit(31));		//A����λ
			break;
		
		case 22:
			CTestFrm_DrawInputStatus(js,ReadBit(32));		//B����λ
			break;
			
		case 23:
			CTestFrm_DrawInputStatus(js,ReadBit(35));		//B����λ
			break;
			
		case 24:
			CTestFrm_DrawInputStatus(js,ReadBit(37));		//C����λ
			break;
			
		case 25:
			CTestFrm_DrawInputStatus(js,ReadBit(38));		//C����λ
			break;
		
		case 26:
			CTestFrm_DrawInputStatus(js,ReadBit(21));		//����//����
			break;
			
		case 27:
			CTestFrm_DrawInputStatus(js,ReadBit(20));		//����
			break;
		
		case 28:
			CTestFrm_DrawInputStatus(js,ReadBit(41));		//X�ŷ�����
			break;
			
		case 29:
			CTestFrm_DrawInputStatus(js,ReadBit(36));		//Y�ŷ�����
			break;
				
		case 30:
			CTestFrm_DrawInputStatus(js,ReadBit(15));		//Z�ŷ�����
			break;
			
		case 31:
			CTestFrm_DrawInputStatus(js,ReadBit(10));		//A�ŷ�����
			break;
		
		case 32:
			CTestFrm_DrawInputStatus(js,ReadBit(5));		//B�ŷ�����
			break;
			
		case 33:
			CTestFrm_DrawInputStatus(js,ReadBit(0));		//C�ŷ�����
			break;
			
		case 34:
			CTestFrm_DrawInputStatus(js,ReadBit(68));		//X�ŷ����
			break;
			
		case 35:
			CTestFrm_DrawInputStatus(js,ReadBit(69));		//Y�ŷ����
			break;
		
		case 36:
			CTestFrm_DrawInputStatus(js,ReadBit(70));		//Z�ŷ����
			break;
			
		case 37:
			CTestFrm_DrawInputStatus(js,ReadBit(71));		//A�ŷ����
			break;
		
		case 38:
			CTestFrm_DrawInputStatus(js,ReadBit(72));		//B�ŷ����
			break;
			
		case 39:
			CTestFrm_DrawInputStatus(js,ReadBit(73));		//C�ŷ����
			break;
				
		case 40:
			CTestFrm_DrawInputStatus(js,ReadBit(65));	//ֹͣ��ť
			break;
			
		case 41:
			CTestFrm_DrawInputStatus(js,ReadBit(66));		//����A
			break;
			
		case 42:
			CTestFrm_DrawInputStatus(js,ReadBit(67));		//����B
			break;
			
		case 43:
			CTestFrm_DrawInputStatus(js,ReadBit(33));		//�ֺ�A
			break;
			
		case 44:
			CTestFrm_DrawInputStatus(js,ReadBit(34));		//�ֺ�B
			break;
			
		case 45:
			CTestFrm_DrawInputStatus(js,ReadBit(56));	//����
			break;
			
		case 46:
			CTestFrm_DrawInputStatus(js,ReadBit(58));	//����
			break;
			
		case 47:
			CTestFrm_DrawInputStatus(js,ReadBit(60));	//����
			break;
		
		case 48:
			CTestFrm_DrawInputStatus(js,ReadBit(38));	//�ⲿ����
			break;
			
		case 49:
			CTestFrm_DrawInputStatus(js,ReadBit(57));	//X
			break;	
			
		case 50:
			CTestFrm_DrawInputStatus(js,ReadBit(59));	//Y
			break;
			
		case 51:
			CTestFrm_DrawInputStatus(js,ReadBit(61));	//Z
			break;
			
		case 52:
			CTestFrm_DrawInputStatus(js,ReadBit(63));	//A
			break;
			
		case 53:
			CTestFrm_DrawInputStatus(js,ReadBit(23));	//��ͣ2
			break;
			
//			case 54:
//				CTestFrm_DrawInputStatus(js,ReadBit(64));	//B
//				break;
//
//			case 55:
//				CTestFrm_DrawInputStatus(js,ReadBit(22));	//C
//				break;
				

		default:
			CTestFrm_DrawInputStatus(js,ReadBit(js));
			break;
		
	}
	
	js++;
	
	//Uart_Printf("js = %d\n",js++);		
		
	if(js>=MAXIN)
		js=0;
}


void	CTestFrm_UpdateProbeStatuses()
{
	static INT16S i=0;
	
//	{
//		INT16S 	x,y;
//		//char 	str[10]="̽��1";
//		char 	str[10]="����1";
//		char    strE[10]="Prb1";
//		char	status[8]={0,0,0,0,0,0,0,0};
//		{
//
//			{
//				//if (i==2)
//					//x=25 +(i+1+37)/8*85;
//				//else
//					x=25 +5*85;
//
//				y=80+(i+1)*37;
//
//			}
//
//
//			OpenProbe(0,i+1,2);
//
//			if(i == 7)
//			{
//				if(g_bEnglish)
//					strncpy(strE,"safedoor",strlen("safedoor"));
//				else
//					strncpy(str,"��ȫ��",strlen("��ȫ��"));
//			}
//			else
//			{
//				if(g_bEnglish)
//					strE[3]='1'+i;
//				else
//					str[4]='1'+i;
//			}
//
//
//			status[i]=(ReadProbe(0,i+1));
//
//			if(g_ProStatus[i]!=status[i])
//			{
//				{
//					CRect rc ;
//
//					rc.left = x;
//					rc.top = y;
//					rc.right = x+77;
//					rc.bottom = y+30;
//
//					CDC_PutColorBar(x,y,80,30,status[i]==1?REDCOLOR:BLUECOLOR,1);
//					CDC_Rectangle4(x,y,x+80,y+30,YELLOWCOLOR);
//					CDC_OutCenTextKzk24(rc,g_bEnglish?strE:str,WHITECOLOR,status[i]==1?REDCOLOR:BLUECOLOR);
//
//				}
//
//				g_ProStatus[i]=status[i];
//			}
//
//			CloseProbe(0);
//		}
//	}
	
	i++;
	
	if(i>=(g_Ini.iAxisCount<3?4:8))
		i=0;
}


void	CTestFrm_UpdateTestKnob()								//���²�����ť
{	
	INT16S temp=0;
	char   buf[10];
	static INT16S oldad=-1;
					
	temp=CTelCom_GetChar(1);
	
	if(temp!=2000)	
		temp=temp/2;
		
	if(temp!=oldad && temp!=2000)
	{
		oldad=temp;		
		sprintf(buf,"%-4d",temp);
		
		CDC_OutText_UTF(g_bEnglish?150:140,490,buf,g_TextColor11,DARKGRAYCOLOR,MZK_24);
	}
}







void CTestFrm_UpdateHandWheelPos()						//������ʾ��ҡ���塢�ֺ�����
{
	static INT32S  oldpos=-1;
	static INT32S  oldpos1=-1;
	char buf[10];
	
	INT32S p=GetPulse(8,FALSE);
	INT32S p1=GetPulse(7,FALSE);
					
//	if(g_iTestAxis==0)
	{
		if(p!=oldpos)
		{
			oldpos=p;
			sprintf(buf,"%-10d",p);
			
			CDC_OutText_UTF(g_bEnglish?400:380,490,buf,g_TextColor11,DARKGRAYCOLOR,MZK_24);
		}
		
		if(p1!=oldpos1)
		{
			oldpos1=p1;
			sprintf(buf,"%-10d",p1);
			
			CDC_OutText_UTF(g_bEnglish?150:140,520,buf,g_TextColor11,DARKGRAYCOLOR,MZK_24);
		}
	}

}


BOOL	CTestFrm_BackZero(INT16S ch,INT16S dir)				//�滻��TestBackZero����������ͬʱ��ʾ
{
	INT16S 	workstep = 0;
	int 	status=0;
	INT16S 	logic=0;
	INT16S 	value=-1;
	long 	DestPulse = 0, DestPulse1 = 0;

	INT16U 	key;
	
	if(ch>CHMAX)
		return FALSE;

	{
		DestPulse1=GetPulseFromAngle(ch,3650);
		DestPulse=GetPulseFromAngle(ch,3650);
	}
	
	SetMMConSpeed(ch,g_Ini.fBackSpeed[ch-1]);
	Start(ch);
	
	while( TRUE )
	{
		switch( workstep )
		{
		case 0:
			if( CheckHome(ch) )
			{	
				set_stop0_mode(ch,1,logic?0:1);
				pmove(ch,dir?-DestPulse:DestPulse);
				workstep ++;	
			}
			else
			{
				workstep =2;
			}			
			
			break;
			
		case 1:	
			get_status(ch,&status);
			
		
			if( status )
			{
				break;
			}
			
			workstep ++;
			break;
			
		case 2:			
			set_stop0_mode(ch,1,logic);	
			pmove(ch,dir?-DestPulse:DestPulse);
				
			workstep ++;
			break;
			
		case 3:			
			get_status(ch,&status);
			
		
			if( status )
			{
				break;
			}
			
			if( CheckHome(ch) )
			{
				value = 0;
			}
			else
				value = 1;
				
		
			if(value != 0)
			{
				set_stop0_mode(ch,0,logic);
				
				return FALSE;
			}
			workstep ++;

			break;
			
	   case 4:	

	   		set_stop0_mode(ch,0,logic);
	   		
	   		return TRUE;
		}
	   
	   
	   	OSTimeDly(2);
	   	
	  	key=KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			if(key == TSTOP)
			{
				Stop1(ch);
				break;
			}
		}
		
		if(RealIO(STOPIO,g_Ini.StopLevel,3)||(g_bHandset?RealIO(STOPIO2,0,3):0)||(g_bHandset?RealIO(SINGLEIO,0,3):0))
		{
			Stop1(ch);
			break;
		}
	   
	}

	set_stop0_mode(ch,0,logic);
	
	return FALSE;
	   		
}




