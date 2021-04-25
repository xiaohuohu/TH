# include	"includes.h"

extern 		BASEINI		g_Ini;
TEST_EXT  	OS_EVENT 	*MsgQueue;
TEST_EXT	BOOL		g_bEnglish;
TEST_EXT	BOOL		Ok_Exit;
TEST_EXT	INT16S		g_iDlgIDOk;
TEST_EXT	OS_EVENT	*FreeSem;

WORK_EXT 	BOOL		g_bFirstResetX;											//ָʾX���Ƿ�λ��FALSEΪ��Ҫ��λ
WORK_EXT 	BOOL		g_bFirstResetY;
WORK_EXT 	BOOL		g_bFirstResetZ;
WORK_EXT 	BOOL		g_bFirstResetU;
WORK_EXT	INT32S 		g_TextColor1;
WORK_EXT	INT32S 		g_TextColor2;
WORK_EXT	INT32S 		g_TextColor3;
WORK_EXT	INT32S 		g_TextColor4;
WORK_EXT	INT32S 		g_TextColor5;
WORK_EXT	INT32S 		g_TableLine;
WORK_EXT	INT32S 		g_nBk;
WORK_EXT	INT32S 		g_nTitleBk;
WORK_EXT	INT32S 		g_nTableBk;
WORK_EXT	INT32S 		g_Color1;

#define	   	MAXTESTKEY	48

CRect		g_ptrKeyRect[MAXTESTKEY];
BOOL		g_bKeyStatus[MAXTESTKEY];
BOOL		g_bTestKey=FALSE;
INT16S  	CR = 0;
BOOL    	LED =0;

/***************************************
	�������

***************************************/

void	CTestKey_Init()
{
	INT16S i=0;
	
	g_bTestKey=TRUE;
	
	for( i=0; i<MAXTESTKEY; i++)
		g_bKeyStatus[i]=FALSE;	
}


void	CTestKey_Destroy()
{
	g_bTestKey=FALSE;
}


void	CTestKey_Create()
{
	CDC_PutColorBar(0,0,getmaxx(),getmaxy(), g_nBk,1);
	CDC_Rectangle(0,0,getmaxx(),getmaxy(),SILVERCOLOR);
	CDC_Rectangle(1,1,getmaxx()-1,getmaxy()-1,SILVERCOLOR);
	CDC_Rectangle(2,2,getmaxx()-2,getmaxy()-2,SILVERCOLOR);
	CDC_Rectangle(3,3,getmaxx()-3,getmaxy()-3,SILVERCOLOR);
	
	CDC_PutColorBar(4,4,getmaxx()-8,40,g_nBk,1);
	CDC_Line(4,41,getmaxx()-4,41,SILVERCOLOR);
	CDC_Line(4,42,getmaxx()-4,42,SILVERCOLOR);
	CDC_Line(4,43,getmaxx()-4,43,SILVERCOLOR);

	if(g_bEnglish)
		CDC_OutText24Ex(280,10,"TH08HB Key/LCD Check",g_TextColor2,g_nBk);
	else
		CDC_OutText24Ex(280,10,"���ɻ�(����)���޲���",g_TextColor2,g_nBk);
		
	
	CDC_OutText24Ex(15,445,g_bEnglish?"Keep press key <<TEACH>> to exit!":"��ʾ:����<<�̵�>>���˳����޽���!",g_TextColor2,g_nBk);

	{
		INT16S k=0,j,i;
		INT16S x=0,y=0,w=85,h=62;
		
		y = 60;
		k = 0;
		
		for(i=0; i<6; i++)
		{
			for(j=0; j<8; j++,k++)
			{
				x = 30 + j * w;
				
				if(j > 4)
					x += 50;
			
				g_ptrKeyRect[k].left  = x;
				g_ptrKeyRect[k].top   = y;
				g_ptrKeyRect[k].right = x+w;
				g_ptrKeyRect[k].bottom= y+h;
								
				CTestKey_UpdateKeyStatus(k,g_bKeyStatus[k],FALSE);
			}
			
			y += h;
		}
	}

}


void	CTestKey_UpdateKeyStatus(INT16S i,BOOLEAN status,BOOLEAN ledout)		//����״̬��ʾ����
{
	
	char    str[10]="\0";
	INT16S 	l = 18,h = 18;
	
	switch(i)
	{
		case 0:		
			strcpy(str,g_bEnglish?"Scr":"����");												
			break;
			
		case 1:	
			strcpy(str,"*");
			l=35;								
			break;
		
		case 2:		
			strcpy(str,g_bEnglish?"Probe":"̽��");
			l=g_bEnglish?8:18;											
			break;
			
		case 3:		
			strcpy(str,g_bEnglish?"UP":"��");									
			l = 28,h = 18;	
			break;
		
		case 4:
			strcpy(str,g_bEnglish?"Air":"����");
			break;
			
		case 5:		
			strcpy(str,"7");
			l=35;													
			break;
			
		case 6:		
			strcpy(str,"8");	
			l=35;												
			break;
			
		case 7:		
			strcpy(str,"9");	
			l=35;												
			break;
		
		case 8:	
			strcpy(str,g_bEnglish?"PUp":"��ҳ");										
			break;
		
		case 9:	
			strcpy(str,g_bEnglish?"PDown":"��ҳ");		
			l=g_bEnglish?8:18;									
			break;
			
		case 10:		
			strcpy(str,g_bEnglish?"LEFT":"��");									
			l=g_bEnglish?12:28,h = 18;	
			break;
			
		case 11: 	
			strcpy(str,"-");	
			l=35;							
			break;
			
		case 12:	
			strcpy(str,g_bEnglish?"RIGHT":"��");								
			l=g_bEnglish?8:28,h = 18;	
			break;
			
		case 13:	
			strcpy(str,"4");	
			l=35;												
			break;
			
		case 14:	
			strcpy(str,"5");	
			l=35;												
			break;
			
		case 15:	
			strcpy(str,"6");
			l=35;													
			break;
		
		case 16:	
			strcpy(str,g_bEnglish?"Count\n Clr":"����\n���");	
			l = 10,h = 5;								
			break;
			
		case 17:	
			strcpy(str,g_bEnglish?"PRO\nRST":"̽��\n����");		
			l = 20,h = 5;									
			break;
		
		case 18:
			strcpy(str,g_bEnglish?"Rate":"�ٱ�");										
			break;
			
		case 19:	
			strcpy(str,g_bEnglish?"DOWN":"��");									
			l = g_bEnglish?12:28,h = 18;	
			break;
			
		case 20:	
			strcpy(str," ");									
			break;
			
		case 21:	
			strcpy(str,"1");
			l=35;													
			break;
			
		case 22: 	
			strcpy(str,"2");	
			l=35;												
			break;
			
		case 23:	
			strcpy(str,"3");
			l=35;													
			break;
		
		case 24:
			strcpy(str,g_bEnglish?"HOME":"����");									
			break;
			
		case 25:	
			
			strcpy(str,g_bEnglish?"INS":"����");								
			break;
			
		case 26:	
			strcpy(str,g_bEnglish?"DEL":"ɾ��");								
			break;
			
		case 27:	
			strcpy(str,g_bEnglish?"DALL":"ȫɾ");								
			break;
			
		case 28:	
			strcpy(str,g_bEnglish?"Loop":"ѭ��");													
			break;
			
		case 29:	
			strcpy(str,".");	
			l=35;												
			break;
			
		case 30:	
			strcpy(str,"0");	
			l=35;												
			break;
			
		case 31:	
			strcpy(str,g_bEnglish?"ENT":"�س�");								
			break;
		
		case 32:	
			strcpy(str,g_bEnglish?"START":"����");	
			l=g_bEnglish?4:18;	
			break;
			
		case 33: 
			strcpy(str,g_bEnglish?"Single":"����");
			l=g_bEnglish?4:18;	
			break;
			
		case 34:	
			strcpy(str,g_bEnglish?"StopY":"ͣ��");	
			l=g_bEnglish?10:18;		
			break;
			
		case 35:	
			strcpy(str,g_bEnglish?"ZSwch":"Z����");	
			l=g_bEnglish?10:18;		
			break;	
			
		case 36:
			strcpy(str,g_bEnglish?"CLR":"���");
			break;
			
		case 37:	
			strcpy(str,g_bEnglish?"Z":"Z��");
			l=g_bEnglish?28:18;										
			break;
			
		case 38:	
			strcpy(str,g_bEnglish?"Box":"����");								
			break;
			
		case 39:	
			strcpy(str,g_bEnglish?"Teach":"�̵�");
			l=g_bEnglish?8:18;									
			break;
		
		case 40:	
			strcpy(str,g_bEnglish?"STOP":"ֹͣ");								
			break;
			
		case 41:	
			strcpy(str,g_bEnglish?"Air1":"����1");								
			break;
			
		case 42:	
			strcpy(str,g_bEnglish?"Air2":"����2");								
			break;
			
		case 43:	
			strcpy(str,g_bEnglish?"Air3":"����3");								
			break;
		
		case 44:	
			strcpy(str,g_bEnglish?"Air4":"����4");								
			break;
			
		case 45:	
			strcpy(str,g_bEnglish?"Hand":"�ֶ�");									
			break;
			
		case 46: 	
			strcpy(str,g_bEnglish?"X":"X��");
			l=g_bEnglish?28:18;											
			break;
			
		case 47:	
			strcpy(str,g_bEnglish?"Y":"Y��");	
			l=g_bEnglish?28:18;								
			break;
			
		default:
			break;
	
	}

	if(i == 32)																							//��ɫ����
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:GREENCOLOR,1);						
		CDC_OutText24Ex(g_ptrKeyRect[i].left+l,g_ptrKeyRect[i].top+h,str,BLACKCOLOR,status?MAGENTACOLOR:GREENCOLOR);
	}
	else if(i==0 || i==1 || i==2 || i==4 || i==8|| i==9 || i==18 || i==20 || (i>=25 && i<=28))			//��ɫ����
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:BLUECOLOR,1);
		CDC_OutText24Ex(g_ptrKeyRect[i].left+l,g_ptrKeyRect[i].top+h,str,WHITECOLOR,status?MAGENTACOLOR:BLUECOLOR);
	}
	else if((i>=37 && i<=47) || i==33 || i==34 || i==35 || i==31 || i==24 )								//����ɫ����
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:LIGHTBLUECOLOR,1);
		CDC_OutText24Ex(g_ptrKeyRect[i].left+l,g_ptrKeyRect[i].top+h,str,WHITECOLOR,status?MAGENTACOLOR:LIGHTBLUECOLOR);
	}
	else if(i==32)
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:REDCOLOR,1);						
		CDC_OutText24Ex(g_ptrKeyRect[i].left+l,g_ptrKeyRect[i].top+h,str,BLACKCOLOR,status?MAGENTACOLOR:REDCOLOR);
	}
	else																								//��ɫ����
	{
		CDC_PutColorBar2(g_ptrKeyRect[i],status?MAGENTACOLOR:WHITECOLOR,1);
		CDC_OutText24Ex(g_ptrKeyRect[i].left+l,g_ptrKeyRect[i].top+h,str,BLACKCOLOR,status?MAGENTACOLOR:WHITECOLOR);
	}

	CDC_Rectangle2(g_ptrKeyRect[i],BLACKCOLOR); 

}


void	CTestKey_OnKey(INT16U key,INT16U tkey)															//ͨ����ťҲ�ܴ����������
{
	INT16S i=-1,j=0;
	
	switch(key)
	{
	
		case F5:																						//��Ļ��ͼ ������
			#ifdef	PRINTBMP
				{
					char   filename[0x20];
					INT16S i=0;
					
//					CBmp_Init();
					strcpy(filename,"keytest");
					
					for(i=0; i<strlen(filename); i++)
					{
						if(filename[i]=='.')
						{
							filename[i]='\0';
							break;
						}
					}
					
					strcat(filename,".bmp");
					CreatBMP(filename,800,480,1);
//					CBmp_Destroy();
					OSQPost(MsgQueue,"Write Bmp End!");
				}
			#else
				i=32;
			#endif
			break;

		case F1:
			i=0;
			break;
			
		case F7:
			i=1;
			break;
			
		case 's':
			i=2;
			break;	
			
		case UP:
			i=3;
			break;
			
		case 'p':
			i=4;
			break;
			
		case '7':
			i=5;
			break;
			
		case '8':
			i=6;
			break;
			
		case '9':
			i=7;
			break;
		
		case F10:
			i=8;
			break;
		
		case F11:
			i=9;
			break;
			
		case LEFT:
			i=10;
			break;
		
		case '-':
			i=11;
			break;
			
			
		case RIGHT:
			i=12;
			break;
			
		case '4':
			i=13;
			break;
			
		case '5':
			i=14;
			break;
			
		case '6':
			i=15;
			break;
		
		case F3:
			i=16;
			break;
			
		case F8:
			i=17;
			break;
		
		case 'i':
			i=18;
			break;
			
		case DOWN:
			i=19;
			break;
			
		case 'j':
			i=20;
			break;
			
		case '1':
			i=21;
			break;
			
		case '2':
			i=22;
			break;
			
		case '3':
			i=23;
			break;
		
		case F4:
			i=24;
			break;
			
		case F9:
			i=25;
			break;
			
		case ESC:
			i=26;
			break;
			
		case '=':
			i=27;
			break;
			
		case 'q':
			i=28;
			break;
			
		case '.':
			i=29;
			break;
			
		case '0':
			i=30;
			break;
			
		case ENTER:
			i=31;
			break;
		
//		case F5:
//			i=32;
//			break;
			
		case 'w':
			i=33;
			break;
			
		case 't':
			i=34;
			break;
			
		case 'r':
			i=35;
			break;
			
		case F12:
			i=36;
			break;
			
		case 'z':
			i=37;
			break;
		
		case 'h':
			i=38;
			break;	
			
		case 'e':
        	if(PressKeyTime(250))			
        		Ok_Exit=TRUE;
        	else
        		i=39;
    		break;
		
		case F6:
			i=40;
			break;
			
		case 'a':
			i=41;
			break;
			
		case 'b':
			i=42;
			break;
			
		case 'c':
			i=43;
			break;
			
		case 'd':
			i=44;
			break;
		
		case 'm':
			i=45;
			break;
			
		case 'x':
			i=46;
			break;
			
		case 'y':
			i=47;
			break;

		default:
			return;
	}
	
	Uart_Printf("i=%d\n",i);
	
	if(i != -1)
	{
		g_bKeyStatus[i]=g_bKeyStatus[i]?FALSE:TRUE;
		
		CTestKey_UpdateKeyStatus(i,g_bKeyStatus[i],TRUE);
		
		if(i>=32 && i<=47)
		{
			key_set_led(i-32,g_bKeyStatus[i]);
		}
	}
	
}


INT16S	CTestKey_DoModal()
{
	INT16U key,tkey=0;
	INT8S  js=0;
	INT16S i=0;
	
	CTestKey_Create();
	
	while(TRUE)
	{
		key = KeyMBoxGetKey();
		
		if(key!=0xffff)
		{
			CTestKey_OnKey(key,tkey);
		
			if(Ok_Exit)
				break;
		}

		OSTimeDly(10);
	}
	
	{
		INT16S i =0;			
	
		for(i = 0 ; i<16 ; i++)
		{
			key_set_led(i,0);
		}
	}
	
	Ok_Exit=FALSE;
	
	return g_iDlgIDOk;
}
