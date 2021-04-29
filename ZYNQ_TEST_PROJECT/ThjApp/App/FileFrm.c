#include "includes.h"

TEST_EXT	INT16S 		g_iDlgLeft;										//边框的左侧坐标
TEST_EXT	INT16S		g_iDlgTop;										//边框顶端坐标
TEST_EXT	INT16S		g_iDlgWidth;									//边框的宽度
TEST_EXT	INT16S		g_iDlgHeight;									//边框的高度
TEST_EXT	INT16S		g_iDlgIDOk;										//Do_Modal程序结束的返回值
TEST_EXT	BOOL		Ok_Exit;										//退出标志量
TEST_EXT	BOOL		g_bEnglish;		
TEST_EXT    OS_EVENT 	*MsgQueue;


extern      BOOL		g_bInsertNum;
extern 		BASEINI		g_Ini;
BOOL		s_bCylinder = FALSE;


#define 	MAXFILE    1000

#define     FILECOUNT_COL  	15											//每一列的行数
#define     FILEOBJECT      (15+7)										//每一个界面的控件数23,按钮控件8个;
#define		CEDITHOFFSET	32											//垂直偏移值
#define		CEDITWOFFSET	399											//水平偏移值

static		INT16S		s_FileStartObj=0;									//指示显示起始控件编号
static		INT16S		s_FileCurObj=0;										//当前控件编号

FILENOTE   	g_FileNote[MAXFILE];

INT32S g_nPreBk = 0;



INT16S    	CFileFrm_Init(INT16S left,INT16S top,INT16S width,INT16S height)
{
	CTROL temp;
	INT16S i=0,k=0,m=0,n=0;
	
	g_nPreBk = g_nBk;
	g_nBk=DARKGRAYCOLOR;												//该模块临时改变背景颜色
	Ok_Exit=FALSE;
	g_iDlgIDOk=1;
	s_bCylinder=FALSE;
	g_iDlgLeft=left;
	g_iDlgTop=top;
	g_iDlgWidth=width;
	g_iDlgHeight=height;


	g_bInsertNum=FALSE;
	
	
	s_FileStartObj= g_Ini.iFileName/FILECOUNT_COL*FILECOUNT_COL;
	
	s_FileCurObj =g_Ini.iFileName%FILECOUNT_COL;

	if(!ReadFileNote())
	{
		memset(g_FileNote,NULL,sizeof(g_FileNote));
		WriteFileNote();
	}
		
	CMainFrm_Init();
	CMainFrm_SetArraySize(FILEOBJECT);

	temp.style=CTROL_STYLE_EDIT;
	strcpy(temp.name,"");
	
	temp.h=28;
	temp.key=0;
	temp.algin=LEFT_ALGIN;
	
	for(m=0;m<FILECOUNT_COL;m++)
	{
		temp.ID=0x9000+k++;
		temp.x=g_iDlgLeft+287;
		temp.y=g_iDlgTop+40+(m)*CEDITHOFFSET;
		temp.w=180;
		CMainFrm_Add(i++,&temp);
	}	


	temp.style=CTROL_STYLE_BUTTON;
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Copy":"复制");
	temp.x=g_iDlgLeft+4;
	temp.y=g_iDlgTop+g_iDlgHeight-43;
	temp.w=113;
	temp.h=40;
	temp.key=NULL;
	temp.algin=CENTER_ALGIN;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"":"");
	temp.x=g_iDlgLeft+4+temp.w*1;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);

	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"":"");
	temp.x=g_iDlgLeft+4+temp.w*2;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
		
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"PrePg":"上 页");
	temp.x=g_iDlgLeft+4+temp.w*3;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"NextPg":"下 页");
	temp.x=g_iDlgLeft+4+temp.w*4;
	temp.key=NULL;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Load":"加载");
	temp.x=g_iDlgLeft+4+temp.w*5;
	temp.key=TSAVE;
	CMainFrm_Add(i++,&temp);
	
	temp.ID=0x9000+k++;
	strcpy(temp.name,g_bEnglish?"Exit":"退出");
	temp.x=g_iDlgLeft+4+temp.w*6;
	temp.key=TCANCEL;
	CMainFrm_Add(i++,&temp);	
	
	return TRUE;
}


void 	CFileFrm_Create()
{
	INT16S i=0;
	CTROL   *ptr;
	
	CDC_DrawWin24(g_iDlgLeft,g_iDlgTop,g_iDlgWidth,g_iDlgHeight,g_bEnglish?"File Management":"文件注释与查询");

	CMainFrm_ShowAllControl();
	
	for(i=FILECOUNT_COL;i<FILEOBJECT;i++)
	{
		ptr=CMainFrm_GetItemID(0x9000+i);
		CButton_SetCtrl(ptr);
		CButton_Create();
	}		
	
	CFileFrm_Message(NULL);	
	CFileFrm_UpdateView(s_FileStartObj,TRUE);
	CFileFrm_DrawButtonInfo(0,0);
	
	CFileFrm_UpdateNowFile();

}


INT16S    CFileFrm_DoModal()
{
	INT16U key=0,tkey=0;
	
	CFileFrm_Create();
	
	while(TRUE)
	{
		key=KeyMBoxGetKey();
		if(key!=0xffff)
		{
			CFileFrm_OnKey(key,tkey);
			
			if(Ok_Exit)
				break;
		}
		OSTimeDly(10);
	}
	
	Ok_Exit=FALSE;
	
	return g_iDlgIDOk;

	
}


void	CFileFrm_Destroy()
{
	CMainFrm_Destroy();
	g_nBk=g_nPreBk;
}


void 	CFileFrm_UpdateView(INT16S iStartObj,BOOL bFreshFile)		//bFreshFile表示该界面的内容是否需要重新刷新显示
{

	INT16S j=0,n=0;
	CTROL   *ptr;
	char buffer[0x20];
	

	
	for(j=0;j<FILECOUNT_COL;j++)										//显示一个界面内15行和30个编辑框的内容
	{		
		if(iStartObj+j>= MAXFILE)
		{	
			CDC_PutColorBar(g_iDlgLeft+10,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL)-1,780,32,g_nBk,1);
			continue;
		}
	
	
		ptr=CMainFrm_GetItemID(0x9000+j);								//一行有两个编辑框控件。首行为0x9000+0,0x9000+1;以此类推
		CEdit_SetCtrl(ptr);
		
		
		memset(buffer,0,sizeof(buffer));
		
		if(g_bEnglish)
			sprintf(buffer,"FileNo:%03d\n",iStartObj+j);
		else
			sprintf(buffer,"文件编号:%03d\n",iStartObj+j);
		
		
		
		if(j==s_FileCurObj)
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(LIGHTBLUECOLOR);
			CEdit_SetFirst(TRUE);
			CEdit_SetLen(12);	
			
			CDC_PutColorBar(g_iDlgLeft+10,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),162,26,LIGHTBLUECOLOR,1);
			CDC_PutColorBar(g_iDlgLeft+185,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),100,26,LIGHTBLUECOLOR,1);
			CDC_PutColorBar(g_iDlgLeft+480,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),310,26,LIGHTBLUECOLOR,1);
		
		
			CDC_OutText24(g_iDlgLeft+12,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),buffer,BLACKCOLOR,LIGHTBLUECOLOR);
			CDC_OutText24(g_iDlgLeft+187,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),g_bEnglish?"Info:":"文件注释",BLACKCOLOR,LIGHTBLUECOLOR);
			CDC_OutText24(g_iDlgLeft+482,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),g_FileNote[iStartObj+j].ModifyTime,BLACKCOLOR,LIGHTBLUECOLOR);
		}
		else
		{
			CEdit_SetTextColor(BLACKCOLOR);
			CEdit_SetBackColor(WHITECOLOR);
			
			CDC_PutColorBar(g_iDlgLeft+10,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),162,26,LIGHTGRAYCOLOR,1);
			CDC_PutColorBar(g_iDlgLeft+185,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),100,26,LIGHTGRAYCOLOR,1);
			CDC_PutColorBar(g_iDlgLeft+480,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),310,26,LIGHTGRAYCOLOR,1);
			
			
			CDC_OutText24(g_iDlgLeft+12,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),buffer,BLUECOLOR,LIGHTGRAYCOLOR);
			CDC_OutText24(g_iDlgLeft+187,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),g_bEnglish?"Info:":"文件注释",BLUECOLOR,LIGHTGRAYCOLOR);
			CDC_OutText24(g_iDlgLeft+482,g_iDlgTop+41+CEDITHOFFSET*(j%FILECOUNT_COL),g_FileNote[iStartObj+j].ModifyTime,BLUECOLOR,LIGHTGRAYCOLOR);			
		}
		
		CEdit_SetString(g_FileNote[iStartObj+j].Note);	
		CEdit_UpdateData(TRUE);
	}
	
	CEdit_SetString(g_FileNote[iStartObj+s_FileCurObj].Note);				//循环执行完成，m_bString需要重新赋值，否则是最后一个编辑框的内容而非当前编辑框的内容。
	
	CFileFrm_Message(g_bEnglish?"HELP As *,SAVE As Load,CANCEL As exit":"帮助键代替符号*,存储键为加载当前行文件,取消键退出");	

}


void	CFileFrm_UpdateObject(INT16S dk,BOOL IsEnter)
{
	CTROL *ptr;
	char buffer[0x20];

	ptr = CMainFrm_GetItemID(0x9000+s_FileCurObj);

	if(s_FileCurObj < FILECOUNT_COL)
	{
		CEdit_SetCtrl(ptr);
		
		if(IsEnter)
		{		
			INT8S  tempNote[0x20]="";
			
			strcpy(tempNote,g_FileNote[s_FileStartObj+s_FileCurObj].Note);
			
			CEdit_String(g_FileNote[s_FileStartObj+s_FileCurObj].Note);	
			
			if(strcmp(g_FileNote[s_FileStartObj+s_FileCurObj].Note , tempNote) !=0)//按确认时，输入字符串不一样时，更新时间
			{
				INT8S  		ti[10],da[10];
				INT8S 		filemessage[0x20]= "";

				RtcGetDateStr(da);						
				RtcGetTimeStr(ti);
				strcat(filemessage,da); 
				strcat(filemessage," "); 
				strcat(filemessage,ti); 
				
				strcpy(g_FileNote[s_FileStartObj+s_FileCurObj].ModifyTime,filemessage);	
			}
		}

		CEdit_SetString(g_FileNote[s_FileStartObj+s_FileCurObj].Note);

	}
	
	if(s_FileStartObj+s_FileCurObj+dk> MAXFILE-1)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);		
		CEdit_UpdateData(FALSE);
		return;
	}		
	else if(s_FileStartObj+s_FileCurObj+dk< 0)
	{
		CEdit_SetTextColor(BLACKCOLOR);
		CEdit_SetBackColor(LIGHTBLUECOLOR);
		CEdit_SetFirst(TRUE);		
		CEdit_UpdateData(FALSE);
		return;
	}	 
	
	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(WHITECOLOR);
	CEdit_UpdateData(FALSE);
	
	CDC_PutColorBar(g_iDlgLeft+10,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),162,26,LIGHTGRAYCOLOR,1);
	CDC_PutColorBar(g_iDlgLeft+185,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),100,26,LIGHTGRAYCOLOR,1);
	CDC_PutColorBar(g_iDlgLeft+480,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),310,26,LIGHTGRAYCOLOR,1);
	
	memset(buffer,0,sizeof(buffer));
		
	if(g_bEnglish)
		sprintf(buffer,"FileNo:%03d\n",s_FileStartObj+s_FileCurObj);
	else
		sprintf(buffer,"文件编号:%03d\n",s_FileStartObj+s_FileCurObj);
	
	CDC_OutText24(g_iDlgLeft+12,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),buffer,BLUECOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24(g_iDlgLeft+187,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),g_bEnglish?"Info:":"文件注释",BLUECOLOR,LIGHTGRAYCOLOR);
	CDC_OutText24(g_iDlgLeft+482,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),g_FileNote[s_FileStartObj+s_FileCurObj].ModifyTime,BLUECOLOR,LIGHTGRAYCOLOR);

	s_FileCurObj+=dk;

	if(s_FileCurObj >= FILECOUNT_COL)
	{
		s_FileStartObj += FILECOUNT_COL;
		s_FileCurObj=0;
		CFileFrm_UpdateView(s_FileStartObj,FALSE);
		return;
	}	
		
	if(s_FileCurObj<0 )
	{
		s_FileStartObj -= FILECOUNT_COL;
	
		if(s_FileStartObj<0)
			s_FileStartObj =0;
		
		s_FileCurObj=FILECOUNT_COL-1;
		CFileFrm_UpdateView(s_FileStartObj,FALSE);
		return;
	}

	ptr = CMainFrm_GetItemID(0x9000+s_FileCurObj);

	if(s_FileCurObj < FILECOUNT_COL)
	{
		CEdit_SetCtrl(ptr);
		CEdit_SetFirst(TRUE);
	
		CEdit_SetString(g_FileNote[s_FileStartObj+s_FileCurObj].Note);

	}
	
	CEdit_SetLen(12);

	CEdit_SetTextColor(BLACKCOLOR);
	CEdit_SetBackColor(LIGHTBLUECOLOR);
	CEdit_UpdateData(FALSE);
	
	CDC_PutColorBar(g_iDlgLeft+10,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),162,26,LIGHTBLUECOLOR,1);
	CDC_PutColorBar(g_iDlgLeft+185,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),100,26,LIGHTBLUECOLOR,1);
	CDC_PutColorBar(g_iDlgLeft+480,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),310,26,LIGHTBLUECOLOR,1);
	
	memset(buffer,0,sizeof(buffer));
	
	if(g_bEnglish)
		sprintf(buffer,"FileNo:%03d\n",s_FileStartObj+s_FileCurObj);
	else
		sprintf(buffer,"文件编号:%03d\n",s_FileStartObj+s_FileCurObj);
	
	CDC_OutText24(g_iDlgLeft+12,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),buffer,BLACKCOLOR,LIGHTBLUECOLOR);
	CDC_OutText24(g_iDlgLeft+187,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),g_bEnglish?"Info:":"文件注释",BLACKCOLOR,LIGHTBLUECOLOR);
	CDC_OutText24(g_iDlgLeft+482,g_iDlgTop+41+CEDITHOFFSET*(s_FileCurObj%FILECOUNT_COL),g_FileNote[s_FileStartObj+s_FileCurObj].ModifyTime,BLACKCOLOR,LIGHTBLUECOLOR);
}


void CFileFrm_OnKey(INT16U key,INT16U tkey)
{
	CTROL *ptr;
	g_bInsertNum = FALSE;
	ptr=CMainFrm_GetItemID(0x9000+s_FileCurObj);
	CEdit_SetCtrl(ptr);
		
	switch(key)
	{
		#ifdef	PRINTBMP
		case TRUN:		
			{
				char   filename[0x20];
				INT16S i=0;
				
				strcpy(filename,"111");
				
				for(i=0; i<strlen(filename); i++)
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
		case TCYLINDER:
			{
				s_bCylinder =s_bCylinder?FALSE:TRUE;		
				LedOut(CYLINDERLED,s_bCylinder?1:0);
				CFileFrm_DrawButtonInfo(s_bCylinder,-1);
			}
			break;	
	
		case TUP:
			CFileFrm_UpdateObject(-1,FALSE);
			break;
			
		case TDOWN:
			CFileFrm_UpdateObject(1,FALSE);
			break;		
			
		case TLEFT:
			CFileFrm_UpdateObject(-1,FALSE);
			break;
			
	    case TRIGHT:
				CFileFrm_UpdateObject(1,FALSE);
			break;
		
		case TPAGEUP:          					 						//上页
				CFileFrm_OnButtonPageUp();
			break;

		case TPAGEDOWN:           										//下页                
				CFileFrm_OnButtonPageDown();
			break;	

		case TENTER:
			{	
				CFileFrm_UpdateObject(1,TRUE);	
			}
			break;	
			
		case TCANCEL:
				CFileFrm_OnButtonOk();
			break;	
			
		case TSAVE:
			{
				CFileFrm_OnButtonLoad();
			}
			break;


		case TZERO:case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '.':case TMINUS:case TCLEAR:case THELP:
			if(s_bCylinder)
			{
				CFileFrm_DrawButtonInfo(s_bCylinder,key-'0');
				
				if(key=='1'||key=='6'||key=='7')
				{
					s_bCylinder=FALSE;
					LedOut(CYLINDERLED,s_bCylinder?1:0);
					CFileFrm_DrawButtonInfo(s_bCylinder,-1);
				}
				
				switch(key)
				{
					case '1':
						{
							CFileFrm_OnButtonCopy();
						}
						break;
					case '4':
						{
							CFileFrm_OnButtonPageUp();
						}
						break;
					case '5':
						{
							CFileFrm_OnButtonPageDown();
						}
						break;
					case '6':
						{
							CFileFrm_OnButtonLoad();
						}
						break;
					case '7':
						{
							CFileFrm_OnButtonOk();
						}
						break;
						
					default:
						break;
				}
				
				break;
			}
			
			{
				g_bInsertNum = TRUE;
				CEdit_OnKey(key,tkey);
			}	
			break;
						
		default:
			break;
	}

}



void CFileFrm_DrawButtonInfo(INT16S iCliyer,INT16S NumStatus)
{

	INT16S i=0;
	INT16S x=0;
	INT16S y=0;
	INT8S buf[0x10];
	
	x = g_iDlgLeft+10;
	y = g_iDlgTop+g_iDlgHeight-40;
	
	CDC_OutText(x,y,g_bEnglish?"Cly":"缸",iCliyer==0?YELLOWCOLOR:REDCOLOR,g_nBk);

	if(NumStatus ==0)
	{
		for(i=0;i<7;i++)
		{		
			sprintf(buf,"%d",i+1);
			CDC_OutText(x+90+i*113,y,buf,YELLOWCOLOR,g_nBk);
		}
	}
	else if(NumStatus>0 && NumStatus<8)
	{
		sprintf(buf,"%d",NumStatus);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,REDCOLOR,g_nBk);
		OSTimeDly(100);
		CDC_OutText(x+90+(NumStatus-1)*113,y,buf,YELLOWCOLOR,g_nBk);
	}
}


void CFileFrm_Message(INT8S *ParaInfo)
{
	CRect rect;
	
	rect.left	= g_iDlgLeft+10;
	rect.top	= g_iDlgTop+g_iDlgHeight-80;
	rect.right	= g_iDlgLeft+10+618;
	rect.bottom = g_iDlgTop+g_iDlgHeight-80+30;
	
	if(ParaInfo==NULL)
	{
		INT16S x1 = rect.left-1;
		INT16S x2 = rect.right+1;
		INT16S y1 = rect.top-1;
		INT16S y2 = rect.bottom+1;

		INT16S x=x1,y=y1,xx=x2,yy=y2;

		CDC_Line(x,y,x,yy,BLACKCOLOR);
		CDC_Line(x,y,xx,y,BLACKCOLOR);

		CDC_Line(x,yy,xx,yy,WHITECOLOR);
		CDC_Line(xx,y,xx,yy,WHITECOLOR);

		CDC_Line(x+1,yy-1,xx-1,yy-1,WHITECOLOR);
		CDC_Line(xx-1,y+1,xx-1,yy-1,WHITECOLOR);
	}
	else
	{
		CDC_PutColorBar(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,GRAYCOLOR,1);
		//CDC_OutText_UTF(rect.left+3,rect.top+2,ParaInfo,BLACKCOLOR,GRAYCOLOR,MZK_24);
		CDC_OutText24(rect.left+3,rect.top+2,ParaInfo,BLACKCOLOR,GRAYCOLOR);	
	}
}

void	CFileFrm_UpdateNowFile(void)
{
	INT8S buf[0x20];
	memset(buf,NULL,sizeof(buf));
	sprintf(buf,"当前文件:%03d\n",g_Ini.iFileName);
	CDC_PutColorBar(g_iDlgLeft+10+610+12,g_iDlgTop+g_iDlgHeight-77,160,30,g_nBk,1);
	CDC_OutText24(g_iDlgLeft+10+610+12,g_iDlgTop+g_iDlgHeight-75,buf,YELLOWCOLOR,-1);	

}


void	CFileFrm_OnButtonOk()
{	
	WriteFileNote();
	Ok_Exit=TRUE;
	g_iDlgIDOk=1;
}


void	CFileFrm_OnButtonCancel()
{
	WriteFileNote();

   	Ok_Exit = TRUE;
   	g_iDlgIDOk=0;
}


void 	CFileFrm_OnButtonPageDown(void)
{
	CFileFrm_UpdateObject( FILECOUNT_COL-s_FileCurObj,FALSE);
}


void 		CFileFrm_OnButtonPageUp(void)
{
	CFileFrm_UpdateObject( -s_FileCurObj-1,FALSE);
}


void 	CFileFrm_OnButtonCopy(void)
{
	INT8S buf[0x40];
	
	memset(buf,NULL,sizeof(buf));

	if(s_FileStartObj+s_FileCurObj<MAXFILE)
	{
		sprintf(buf,"当前文件成功复制到%d号文件",(s_FileStartObj+s_FileCurObj));
		CFileFrm_Message(buf);
		
		WriteDataToFile(s_FileStartObj+s_FileCurObj);
	}
	else
	{
		CFileFrm_Message("文件复制失败");
	}
}


void 	CFileFrm_OnButtonDel(void)
{
}


void 	CFileFrm_OnButtonLoad(void)
{

	INT8S buf[0x40];
	memset(buf,NULL,sizeof(buf));

	if(s_FileStartObj+s_FileCurObj<MAXFILE)
	{

		if(ReadDataFromFile(s_FileStartObj+s_FileCurObj))
		{					
			sprintf(buf,"加载%d号文件",(s_FileStartObj+s_FileCurObj));
			CFileFrm_Message(buf);
		}
		else
		{
			g_Ini.lRunCount = 0;
			g_Ini.lDestCount = 100000;
			g_Ini.lProbeCount=10;
			
			sprintf(buf,"加载%d号文件,文件为空",(s_FileStartObj+s_FileCurObj));
			CFileFrm_Message(buf);
		}
		
		
		
		WriteRunCount();
		
		g_Ini.iFileName =s_FileStartObj+s_FileCurObj;
		SysParaWrite();								//当前文件号写入
		

		CFileFrm_UpdateNowFile();
	}
	else
	{
		CFileFrm_Message("加载新文件失败");
	}

}


void	WriteFileNote(void)	
{
/*	int  		fw;
	INT8U 		err;
	INT8S 		tmpbuf[0x20]="/flash/PROG/fnote.ini";
	
	fw = yaffs_open(tmpbuf,O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

	if( fw == -1 )
	{
		Uart_Printf("WriteFileNote 写文件错误!!\n");
		goto Err;
	}	
	
	yaffs_lseek(fw,0,SEEK_SET);

	err=yaffs_write(fw,g_FileNote,sizeof(g_FileNote));
	
Err:
	yaffs_close(fw);*/
}


BOOL	ReadFileNote(void)
{/*
	int  		fr;

	INT8S 		tmpbuf[0x20]="/flash/PROG/fnote.ini";
	
	fr=yaffs_open(tmpbuf,O_RDONLY , S_IREAD | S_IWRITE);

	if( fr == -1 )
	{
		Uart_Printf("ReadFileNote 未找到文件\n");
		yaffs_close(fr);
		return FALSE;
	}	
	
	yaffs_lseek(fr,0,SEEK_SET);

	yaffs_read(fr,g_FileNote,sizeof(g_FileNote));
	
	yaffs_close(fr);
	*/
	return TRUE;
}


