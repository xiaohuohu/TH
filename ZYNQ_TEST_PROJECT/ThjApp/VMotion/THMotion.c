/*************************************************
  Copyright (C), 2013-2015, Adtech Tech. Co., Ltd.
  文件名:        THMotion.c
  创建人：       杨基鹏
  创建日期：	 2013/08/19
  主要功能描述:  

*************************************************/
#define 	VM_PUBLIC
#include   	"includes.h"
#include 	"os_cpu.h"
#include 	"THMotion.h"
#include 	"VMQUEUE_OP.h"

#define 	Bool 		BOOL
#define 	CALRATE		1000.
#define 	VMAX 		VM.SpdMax

#define 	SET_RANGE(rate)	{FPGARATE=rate;set_range(rate,0);}
#define 	GET_RANGE()		FPGARATE
#define 	min3(a,b,c)		(min(a,b)<min(b,c)?min(a,b):min(b,c))

int 	vm_setrange1(float rate);
static 	INT8U 	err;
static 	int 	eventtag=0;

static 	int 	FIFOCOUNT=0;
static 	TIT 	FIFOBUF[2100];

int 	MethodFlag = -1;


int g_ivm_errNo=0;												//用于记录VM运行过程中的错误代码
int g_iStopMode=0;												//停止模式，0表示减速停止，1表示急停

int g_lastpulsereg[MAXAXIS];


int vm_get_errNo()
{
	return g_ivm_errNo;
}


int vm_clear_errNo()
{
	g_ivm_errNo = 0;
}


int	ErrRecord(int No)
{
	SYS_ErrNo=No;
	return 1;
}


int SetMethodFlag(int No)										//0是从轴连续，1是从轴不连续
{
	MethodFlag=No;
	return 1;
}


static unsigned int T_POSSETCountPush,T_POSSETCountPop;			//定义POSSET的设定和生效计数器，用于区别多组POSSET操作

static int SpeedPretreatment( MP *pData,Queue *q)				//速度预处理
{
	float 		acc,ad;
	int 		i,j,k,N;
	float 		VectorL1,VectorL2;
	int 		i_temp,i_temp2;
	float 		t,tt;
	static 	MP 	*pLastMP,*pa[200+10];

	ad=VM.aconf[INPA].acc;

	if(pData)
	{
		t=0;

		for(i=Ax;i<MAXAXIS;i++)									//求最大时间t
		{
			if(i==pData->maxis)continue;
			
			tt=2*sqrt(SYS_acc(i)*fabsf(pData->axis[i].relpos))/SYS_acc(i);  //以设定加速度做三角加减速走完行程（pData->axis[i].relpos）需要的时间
			
			if(t<tt)t=tt;
		    
		    tt=fabsf(pData->axis[i].relpos)/VMAX[i];			//VMAX各轴的最大速度下走完行程需要的时间
		    
		    if(t<tt)t=tt;
		}
		
		if(pData->vmax>fabsf(pData->relpos)/t)					//主轴限速 
			pData->vmax=fabsf(pData->relpos)/t;
	}

	pLastMP=GetMPDataIn(0,q);									//获取队列尾数据

	if(pLastMP==NULL)											//本次是第一段运动信息，做直线加减速
	{
		pData->vs=pData->ve=0;
		goto Suc;
	}
	else
	{
		VectorL1=fabsf(pLastMP->relpos);	
	}

	VectorL2=fabsf(pData->relpos);	

	pData->v0=pow(pData->vs,2);
	pData->b=2*ad*VectorL2;
	pData->d=pow(pData->vmax,2);								//公式Vt平方-V0平方=2as
	
	pData->e=2*pData->d;
	pData->c=min(pData->e,pData->d);
	pData->ve=0;

	if(pLastMP)
	{
		pLastMP->v0=pow(pLastMP->vs,2);
		pLastMP->b=2*ad*VectorL1;

		if((pLastMP->maxis!=pData->maxis)||(pLastMP->relpos>0 && pData->relpos<0)||(pLastMP->relpos<0 && pData->relpos>0))
			pLastMP->e=0.;
		else
			pLastMP->e=1000000.;
			
		pLastMP->d=pow(pLastMP->vmax,2);
		pLastMP->c=min3(pLastMP->e,pLastMP->d,pData->d);		//min(pLastMP->e,pLastMP->d);由原来的只考虑本线段和下轨迹拐角和本线段的最高速增加考虑下段最高速

	}

	N=NNN-1;													//减去1个当前数据

	pLastMP=GetMPDataIn(N,q);

	if(!pLastMP)												//缓冲内无足够的线段进行分析
	{
		//Uart_SendByte('1');
		N=MotionQueue.QueueCount;								//取出缓冲内第一个数据
	}
	
	j=N;
	memset(pa,0x00,sizeof(MP *)*(NNN+1));
	
	for(i=0;i<j;i++)
	{	
		pa[i]=GetMPDataIn(j-i-1,q);
		
		if(!pa[i])goto Err;
	}	
	
	pa[i]=pData;												//pa[]存放最多30条插补数据，从队头0到队尾N

	N+=1;														//还原实际的N数
	
	for(i=0;i<N;i++)
	{
		j=0;

		for(;;)
		{
			if(j>=N || (j+i+1)>=N)
			{
				acc=0;

				for(k=i+j;k>=i+1;k--)
				{
					acc+=pa[k]->b;
					if(acc>pa[k-1]->c)acc=pa[k-1]->c;
				}
				
				i_temp=pa[i]->v0+pa[i]->b;
				i_temp2=min(min(acc,i_temp),min(acc,pa[i]->c));
				pa[i]->ve=sqrt(i_temp2);

				if((i+1)<N)
				{
					pa[i+1]->vs=pa[i]->ve;
					pa[i+1]->v0=pow(pa[i+1]->vs,2);

//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i+1]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);
				}
				else
				{
//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);
				}

				break;
			}
			
			j++;

			if((pa[i+j]->b > pa[i+j-1]->c))						//本次线段存在拐角
			{
				acc=pa[i+j-1]->c;
				
				for(k=i+j-1;k>=i+1;k--)
				{
					acc+=pa[k]->b;
				
					if(acc>pa[k-1]->c)acc=pa[k-1]->c;
				}
				
				i_temp=pa[i]->v0+pa[i]->b;
				i_temp2=min(min(acc,i_temp),min(acc,pa[i]->c));
				pa[i]->ve=sqrt(i_temp2);
				
				if((i+1)<N)
				{
					pa[i+1]->vs=pa[i]->ve;
					pa[i+1]->v0=pow(pa[i+1]->vs,2);

//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i+1]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);
				}
				else
				{
//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);				
				}

				break;
			}

			acc=0;

			for(k=i+j;k>=i+1;k--)
			{
				acc+=pa[k]->b;
			
				if(acc>pa[k-1]->c)acc=pa[k-1]->c;
			}
			
			if(acc>=pa[i]->c)									//后面线段足够本次加速
			{
				i_temp=pa[i]->v0+pa[i]->b;
				i_temp2=min(i_temp,pa[i]->c);
				pa[i]->ve=sqrt(i_temp2);
			
				if((i+1)<N)
				{
					pa[i+1]->vs=pa[i]->ve;
					pa[i+1]->v0=pow(pa[i+1]->vs,2);

//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i+1]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);
				}
				else
				{
//					pa[i]->speedv=sqrt((pa[i]->v0+pa[i]->b)/2);
//					pa[i]->speedv=min(pa[i]->speedv,pa[i]->vmax);
				}
				
				//if(pa[i]->vc<max(pa[i]->vs,pa[i]->ve))debugout("3pa:%f %f %f %f\r\n",pa[i]->vs,pa[i]->vc,pa[i]->ve,pa[i]->vmax);

				break;
			}
		}
	}

//	debugout("angle:%d %x %d %d %d %d %d %d\n",angle,QNodeHead->MD.MType,pData->pulse[Ax],pData->pulse[Ay],pData->pulse[Az],QNodeHead->MD.vs,QNodeHead->MD.vc,QNodeHead->MD.ve);
//	debugout("angle:%d %x %d %d %d %d %d %d\n",angle,QNodeHead->MD.MType,pData->pulse[Ax],pData->pulse[Ay],pData->pulse[Az],QNodeHead->MD.vs,QNodeHead->MD.vc,QNodeHead->MD.ve);
//	debugout("SpeedPretreatment success\r\n");

Suc:
	return SUCCESS;

Err:
	debugout("SpeedPretreatment Err\r\n");
	return FAIL;	
}


static void FollowSpeedPretreatment( Queue *q)
{
  	int 		i,j,N;
 	double 		Vavrc,acc,vs,vc,ve,ftemp,maxpos;				//,MinSpeed
 	float		t1,t3;
 	double 		I;
 	static 	MP 	*Currentp,*Lastp;  								//当前指针和下一个指针
 
	for(i=0;i<=NNN;i++)											//得到队列中从轴速度规划数据个数，最大30个
  	{
   		N=i;
  		Currentp=GetMPDataIn(i,  q);
  	    if(Currentp==NULL) break;  
  	}
  	
  //Uart_Printf("%d\r\n",N);
  	if(N==0) return;
  	
  	for(j=N-1;j>=0;j--)											//重新计算主轴分段时间
  	{
		Currentp=GetMPDataIn(j,  q);  
		Currentp->speedv=max(max(Currentp->vs,Currentp->ve),max(Currentp->ve,Currentp->vmax));
	 	vs=Currentp->vs;		
	 	ve=Currentp->ve;
		maxpos=fabsf(Currentp->relpos);
 		//vc=sqrt((2*SYS_acc(INPA)*SYS_acc(INPA)*pP->maxpos+SYS_acc(INPA)*vs*vs+SYS_acc(INPA)*ve*ve)/(SYS_acc(INPA)+SYS_acc(INPA)));
	 	vc=sqrt((2*SYS_acc(INPA)*maxpos+vs*vs+ve*ve)*0.5);

	 	if(vc<Currentp->speedv)									//若最高加速低于驱动速度，则没有匀速段
		{
	 		Currentp->speedv=vc;
	 	}
		
		vc=Currentp->speedv;
	 	
	  	t1=(vc-vs)/SYS_acc(INPA);
		t3=(vc-ve)/SYS_acc(INPA);
		ftemp=maxpos-((vc+vs)*t1+(vc+ve)*t3)/2;
		/*T=t1+t3+ftemp/vc;
		fn=T/0.001;
		n=(int)fn;
		if(fn>(float)n)
		{n=n+1; }
		Currentp->T=n*0.001;*/
		Currentp->T=t1+t3+ftemp/vc;
	 	
	 	if(j>=1)
	 		Lastp=GetMPDataIn(j-1,  q);
	 	else
	 		Lastp=NULL;		
	 			
		for(i=Ax;i<MAXAXIS;i++)
		{  
			acc=SYS_acc(i);
			
			if(i==Currentp->maxis) continue;
		 	
			if(fabs(Currentp->axis[i].relpos)>0.0001)
			{
		 		if(Lastp==NULL||(Lastp->axis[i].relpos*Currentp->axis[i].relpos)<=0||Lastp->maxis!=Currentp->maxis)
		 		{
		 			Vavrc=fabsf(Currentp->axis[i].relpos/Currentp->T);
		    		Currentp->axis[i].ve=0;
		    		
		    		if(Vavrc>=Currentp->axis[i].vs)
		    		{
		    			I=acc*acc*Currentp->T*Currentp->T+2*acc*Currentp->axis[i].vs*Currentp->T-
					  			 4*acc*fabsf(Currentp->axis[i].relpos)-Currentp->axis[i].vs*Currentp->axis[i].vs;//ok
					 
					  	if(I<0)																	//设定加速度过小需要重新调整加速度
				     	{
				        	I=0;
				        	acc=(-(2*Currentp->axis[i].vs*Currentp->T-4*fabsf(Currentp->axis[i].relpos))+sqrt((2*Currentp->axis[i].vs*Currentp->T-4*fabsf(Currentp->axis[i].relpos))*(2*Currentp->axis[i].vs*Currentp->T-4*fabsf(Currentp->axis[i].relpos))+
					 		4*Currentp->axis[i].vs*Currentp->axis[i].vs*Currentp->T*Currentp->T))/(2*Currentp->T*Currentp->T);//ok
				      	}
				 		
				 		Currentp->axis[i].vc=((acc*Currentp->T+Currentp->axis[i].vs)-sqrt(I))/2;//ok
				 		//Uart_Printf("11111\r\n");	 
		    		}
		    		else
		    		{
		    			Currentp->axis[i].vc=(2*acc*fabsf(Currentp->axis[i].relpos)-Currentp->axis[i].vs*Currentp->axis[i].vs)/(2*(acc*Currentp->T-Currentp->axis[i].vs));//计算匀速段ok
			            
			            if(Currentp->axis[i].vc<=0||(Currentp->axis[i].vs/acc)>Currentp->T)		//计算出匀速段时间为负值需要重新调整加速度ok
		           		{
			             	Currentp->axis[i].vc=0.5*Vavrc;
			                acc=(Currentp->axis[i].vs*Currentp->axis[i].vs-2*Currentp->axis[i].vc*Currentp->axis[i].vs)/(2*(fabsf(Currentp->axis[i].relpos)-Currentp->axis[i].vc*Currentp->T));//ok
		                }
		    		}
		    		
		 		}
		 		else
		 		{
		 		    Vavrc=fabsf(Currentp->axis[i].relpos/Currentp->T);
		 			
		 			if(Vavrc>=Currentp->axis[i].vs)
		        	{
		          		I=acc*acc*Currentp->T*Currentp->T+2*acc*Currentp->axis[i].vs*Currentp->T-
			        	2*acc*fabsf(Currentp->axis[i].relpos);//ok
			  		  
			  		  	if(I<0)																	//设定加速度过小需要重新调整加速度
			   			{
				   			I=0;
				   			acc=(2*fabsf(Currentp->axis[i].relpos)-2*Currentp->axis[i].vs*Currentp->T)/(Currentp->T*Currentp->T);//ok
			  		    }
		        
		              	Currentp->axis[i].vc=(acc*Currentp->T+Currentp->axis[i].vs)-sqrt(I); 	//计算匀速段
		              //Uart_Printf("33333\r\n");
		        	}
		       		else
		        	{
		        	 	I=acc*acc*Currentp->T*Currentp->T-2*acc*Currentp->axis[i].vs*Currentp->T+
		               		2*acc*fabsf(Currentp->axis[i].relpos);//ok

			        	if(I<0)																	//设定加速度过小需要重新调整加速度
			         	{
				      		I=0;
				     		acc=(2*Currentp->axis[i].vs*Currentp->T-2*fabsf(Currentp->axis[i].relpos))/(Currentp->T*Currentp->T);//ok
			         	}
			   
			       		Currentp->axis[i].vc=(Currentp->axis[i].vs-acc*Currentp->T)+sqrt(I);	//计算匀速段ok
			      		
			      		if (Currentp->axis[i].vc<=0||(Currentp->axis[i].vs/acc)>Currentp->T)	//计算出匀速段为负值需要重新调整加速度
			         	{
				     	 	Currentp->axis[i].vc=0.5*Vavrc;										//设定最小运行速度
				      		acc=(Currentp->axis[i].vs-Currentp->axis[i].vc)*(Currentp->axis[i].vs-Currentp->axis[i].vc)/(2*(fabsf(Currentp->axis[i].relpos)-Currentp->axis[i].vc*Currentp->T));//ok
		        		}
		     			//Uart_Printf("44444\r\n");
		 		  	}
		 		   	
		 		   	Currentp->axis[i].ve=Currentp->axis[i].vc;
		 	       	Lastp->axis[i].vs=Currentp->axis[i].ve;
		 	  	}
			} 	   
		 	else
		 	{
		 		Currentp->axis[i].vs=0;
		    	Currentp->axis[i].vc=0;
		    	Currentp->axis[i].ve=0;	
		 	}	
		 	
		 	//Uart_Printf("%f,%f,%f\r\n",Currentp->axis[i].vs,Currentp->axis[i].vc,Currentp->axis[i].ve);	
		 	Currentp->axis[i].len1=fabsf(Currentp->axis[i].vs*Currentp->axis[i].vs-Currentp->axis[i].vc*Currentp->axis[i].vc)/(2*acc);
		 	Currentp->axis[i].ad=Currentp->axis[i].dd=acc;
		 	Currentp->axis[i].t1=fabsf(Currentp->axis[i].vs-Currentp->axis[i].vc)/acc;
		 	
		 	if(Currentp->axis[i].vc!=Currentp->axis[i].ve)
		 	{
		 		Currentp->axis[i].t3=fabsf(Currentp->axis[i].vc-Currentp->axis[i].ve)/acc;
		 		Currentp->axis[i].t2=Currentp->T-Currentp->axis[i].t3;
		 		Currentp->axis[i].t3=Currentp->T;
		 		Currentp->axis[i].len2=fabsf(Currentp->axis[i].relpos)-fabsf(Currentp->axis[i].ve*Currentp->axis[i].ve-Currentp->axis[i].vc*Currentp->axis[i].vc)/(2*acc);
		 	}
		 	else
		 	{
		 		Currentp->axis[i].t3=Currentp->axis[i].t2=Currentp->T;
		 		Currentp->axis[i].len2=fabsf(Currentp->axis[i].relpos);
		 	}
		 	
		 	/* if(i==2)
	            Uart_Printf("%f,%f,%f\r\n",Currentp->axis[i].vs,Currentp->axis[i].vc,Currentp->axis[i].ve);
	            Uart_Printf("%f,%f,%f\r\n",Currentp->axis[i].t1,Currentp->axis[i].t2,Currentp->axis[i].t3);*/
	        //Uart_Printf("%f,%f,%f\r\n",Currentp->axis[i].t1,Currentp->axis[i].t2,Currentp->axis[i].t3);
		 	 //Uart_Printf("%f,%f,%f\r\n",Currentp->axis[i].vs,Currentp->axis[i].vc,Currentp->axis[i].ve); 	*/
		}
		//Uart_Printf("%f,%f,%f\r\n",Currentp->axis[1].vs,Currentp->axis[1].vc,Currentp->axis[1].ve);
	}
}

#if FALSE
static int SP( MP *pData,Queue *q)								//速度预处理
{
	float 	Lc,ad;
	int 	i;
	MP 		*pCurMP,*pLastMP,*pLLMP;
	float 	t,tt;
	int 	paMaxCount;
	
	ad 		= VM.aconf[INPA].acc;
	pCurMP  = pData;
	pLastMP = GetMPDataIn(0,q);    								//获取队列尾数据
	pLLMP   = GetMPDataIn(1,q);

	if(pData)
	{
		t=0;
	
		for(i=Ax;i<MAXAXIS;i++)
		{
//			tt=fabsf(pCurMP->axis[i].relpos)/VMAX[i];
			if(i==pData->maxis)continue;
			
			tt=2*sqrt(SYS_acc(i)*fabsf(pCurMP->axis[i].relpos))/SYS_acc(i);
			
			if(t<tt)t=tt;
		}
		
		if(pCurMP->vmax>fabsf(pCurMP->relpos)/t)
			pCurMP->vmax=fabsf(pCurMP->relpos)/t;
	}
	//计算拐角衔接速度
	if(pCurMP && pLastMP)
	{
		if((pLastMP->maxis!=pCurMP->maxis)||(pLastMP->relpos>0 && pCurMP->relpos<0)||(pLastMP->relpos<0 && pCurMP->relpos>0))
			pLastMP->vlimit=0.;
		else
			pLastMP->vlimit=10000.;
			
		pLastMP->vlimit=pCurMP->vs=min3(pLastMP->vlimit,pCurMP->vmax,pLastMP->vmax);
	}
	
	paMaxCount=100;
	
	/***************************************/

	i = 0;
	pCurMP 	= pData;											//当前轨迹段
	pLastMP = GetMPDataIn(i+0,q);
	pLLMP 	= GetMPDataIn(i+1,q);

	if(pData)
	{
		pData->vlimit=0;
		pData->vs=0;
		pData->ve=pData->vmax;
		
		if(!pData->relpos)pCurMP=NULL;
	}
	
	if(pLastMP && pCurMP && pCurMP->relpos)						//非第一段轨迹和最后一段轨迹 开始令i+1为当前轨迹
	{
		//debugout("@@@@@@@@@@@@@@@@@@ %d\r\n",MotionQueue.QueueCount);
		if(pLastMP)
		{
			pCurMP->vs = pLastMP->vlimit;						//令后一线段的起始速度为拐角速度
			
			if(pCurMP->vs < pLastMP->ve)						//如果本线段存在拐角进行往前面轨迹反算
			{
	 			pLastMP->ve = pCurMP->vs;						//令当前迹轨迹的终止速度为后一线段的起始速度

				if(i==0)										//后一线段计算根据起始速度和合成位移计算终止速度和驱动速度
				{
					pCurMP->ve = sqrt(pow(pCurMP->vs,2)+fabsf(pCurMP->relpos)*2*ad);//V2^2 = V1^2+2aL b=2.0*ad*relpos
					pCurMP->ve = min(pCurMP->ve,pCurMP->vmax);
					//debugout("3 i+0 %d pCurMPLine %d startv %f endv %f speedv %f c %f\r\n",i,pCurMP->LinePos,pCurMP->vs,pCurMP->ve,pCurMP->vc,pCurMP->c);
				}
				
	Loop1:
				if(pLastMP->ve<pLastMP->vs)						//如果当前线段的终止速度小于起始速度时根据此线段的终止速度反算起始速度(此线段需减速)
				{
					Lc = fabsf(pow(pLastMP->ve,2) - pow(pLastMP->vs,2))*0.5/ad;//(2.0*ad));//Lc = (V2^2 - V1^2)/2a ad_temp = 2.0*ad
					//fTemp = fabsf((pow(pLastMP->ve,2) - pow(pLastMP->vs,2))/(2.0*ad));//Lc = (V2^2 - V1^2)/2a
					
					if(Lc < fabsf(pLastMP->relpos))
					{
						//debugout("4 i+x %d pCurMPLine %d startv %f endv %f speedv %f c %f\r\n",i+1,pLastMP->LinePos,pLastMP->vs,pLastMP->ve,pLastMP->vc,pLastMP->c);
						goto MainSuc;
					}
					
					pLastMP->vs = sqrt(pow(pLastMP->ve,2)+fabsf(pLastMP->relpos)*2*ad);//V2^2 = V1^2+2aL b=2.0*ad*relpos
					pLastMP->vs = min(pLastMP->vs,pLastMP->vmax);
					
					if(pLLMP)
					{
						pLLMP->ve = pLastMP->vs;				//令前一线段的终止速的为当前线段的起始速度(此线段需加速)
					}
					else
					{
						goto MainSuc;
					}
					
					i++;
					
					if(i>paMaxCount)goto MainSuc;
					
					pLastMP= GetMPDataIn(i+0,q);
					pLLMP  = GetMPDataIn(i+1,q);				//此处的pLLMPi++之前上次循环pLLMP的后一段轨迹
					
					if(pLastMP)goto Loop1;						//此处的pLastMPi++之前上次循环的pLLMP的轨迹
				}
				else											//如果当前线段的终止速度依然大于起始速度
				{
					Lc = fabsf(pow(pLastMP->ve,2) - pow(pLastMP->vs,2))*0.5/ad;//(2.0*ad));//Lc = (V2^2 - V1^2)/2a ad_temp = 2.0*ad
					
					if(Lc < fabsf(pLastMP->relpos))
					{
						goto MainSuc;
					}
					
					pLastMP->ve = sqrt(pow(pLastMP->vs,2)+fabsf(pLastMP->relpos)*2*ad);//V2^2 = V1^2+2aL b=2.0*ad*relpos
					pLastMP->ve = min(pLastMP->ve,pLastMP->vmax);
					
					pCurMP->vs = pLastMP->ve;
					pCurMP->vs = min(pCurMP->vs,pCurMP->vmax);
				}
			}
			else// if(i==0)										//如果是当前的线段计算根据起始速度和合成位移计算终止速度和驱动速度
			{
				pCurMP->vs = pLastMP->ve;						//后一线段的起始度为当前线段的终速度
				pCurMP->ve = sqrt(pow(pCurMP->vs,2)+fabsf(pCurMP->relpos)*2*ad);//b=2.0*ad*relpos
				pCurMP->ve = min(pCurMP->ve,pCurMP->vmax);
			}
		}
	}
	else if(pCurMP && pLastMP==NULL && pCurMP->relpos)			//第一段运动轨迹
	{
		pCurMP->vs = 0;
		Lc = fabsf(pow(pCurMP->ve,2) - pow(pCurMP->vs,2))*0.5/ad;//(2.0*ad));//ad_temp = 2.0*ad
		//debugout("first1 i %d Line %d Lc %d relpos %d startv %f endv %f speedv %f\r\n",i,pCurMP->LinePos,Lc,pCurMP->relpos,pCurMP->vs,pCurMP->ve,pCurMP->vc);
		
		if(Lc > fabsf(pCurMP->relpos))
		{
			pCurMP->ve = sqrt(fabsf(pCurMP->relpos)*2*ad + pow(pCurMP->vs,2));//V2^2 = V1^2 + 2*a*L b=2.0*ad*relpos
			pCurMP->ve = min(pCurMP->ve,pCurMP->vmax);
		}
	}
	else														//最后一段运动轨迹
	{
		i = 0;
		pCurMP  = GetMPDataIn(i,q);
		pLastMP = GetMPDataIn(i+1,q);
		pLLMP 	= GetMPDataIn(i+2,q);
		
		if(pLastMP)
		{
			
			pCurMP->ve = 0;										//令最后一段运动轨迹的终止速度为0
			pCurMP->vs = sqrt(pow(pCurMP->ve,2)+2*ad*fabsf(pCurMP->relpos));//V2^2 = V1^2+2aL b=2.0*ad*relpos
			pCurMP->vs = min(pCurMP->vs,pCurMP->vmax);
			
			if(pCurMP->vs > pLastMP->ve)
			{
				pCurMP->vs = pLastMP->ve;
				goto MainSuc;
			}
			
			pLastMP->ve = pCurMP->vs;							//令当前迹轨迹的终止速度为最后一线段的起始速度

	Loop2:
			//debugout("@@@@@@@@@@@@@@@@@@@@\r\n");
			debugout("pCurMp:%x pLastMP:%x\r\n",pCurMP,pLastMP);
			
			if(pLastMP->ve<pLastMP->vs)							//如果当前线段的终止速度小于起始速度时根据此线段的终止速度反算起始速度
			{
				Lc = fabsf(pow(pLastMP->ve,2) - pow(pLastMP->vs,2))*0.5/ad;//(2.0*ad));//Lc = (V2^2 - V1^2)/2a ad_temp = 2.0*ad
				
				if(Lc < fabsf(pLastMP->relpos))
				{
					goto MainSuc;
				}
				
				pLastMP->vs = sqrt(pow(pLastMP->ve,2)+fabsf(pLastMP->relpos)*2*ad);//V2^2 = V1^2+2aL b=2.0*ad*relpos
				pLastMP->vs = min(pLastMP->vs,pLastMP->vmax);
				
				if(pLLMP)
				{
					pLLMP->ve = pLastMP->vs;					//令前一线段的终止速的为当前线段的起始速度
					
					//Lc = fabsf((pow(pLLMP->ve,2) - pow(pLLMP->vs,2))/(2.0*ad));//Lc = (V2^2 - V1^2)/2a
					//if(Lc>pLLMP->relpos)
					//{
					//	pLLMP->ve = sqrt(2.0*ad*pLLMP->relpos + pow(pLLMP->vs,2));//V2^2 = V1^2 + 2*a*L;
					//}
				}
				else
				{
					goto MainSuc;
				}
				
				i++;
				
				if(i>paMaxCount)
				{
					goto MainSuc;
				}
				
				debugout("%d @@pLLMP:%x pLastMP:%x @%x~%x\r\n",i,pLLMP,pLastMP,(int)(&MPDataBuf[0]),(int)(&MPDataBuf[MaxMParaPart-1]));
				
				pLastMP = GetMPDataIn(i+0,q);
				pLLMP 	= GetMPDataIn(i+1,q);					//此处的pLLMP i++之前上次循环pLLMP的后一段轨迹
				
				debugout("@@pLLMP:%x pLastMP:%x @%x~%x\r\n",pLLMP,pLastMP,(int)(&MPDataBuf[0]),(int)(&MPDataBuf[MaxMParaPart-1]));
				
				if(pLastMP)goto Loop2;							//此处的pLastMPi++之前上次循环的pLLMP的轨迹

			}
			else												//如果当前线段的终止速度依然大于起始速度
			{

				debugout("pCurMp:%x pLastMP:%x @%x~%x\r\n",pCurMP,pLastMP,(int)(&MPDataBuf[0]),(int)(&MPDataBuf[MaxMParaPart-1]));
				Lc = fabsf(pow(pLastMP->ve,2) - pow(pLastMP->vs,2))*0.5/ad;//(2.0*ad));//Lc = (V2^2 - V1^2)/2a ad_temp = 2.0*ad
				
				if(Lc < fabsf(pLastMP->relpos))
				{
					goto MainSuc;
				}
				
				pLastMP->ve = sqrt(pow(pLastMP->vs,2)+fabsf(pLastMP->relpos)*ad*2);//V2^2 = V1^2+2aL b=2.0*ad*relpos
				pLastMP->ve = min(pLastMP->ve,pLastMP->vmax);
				pCurMP->vs = pLastMP->ve;
				pCurMP->vs = min(pCurMP->vs,pCurMP->vmax);
			}
		}
		else
		{
			debugout("pCurMp:%x\r\n",pCurMP);
			pCurMP->ve = 0;										//令最后一段运动轨迹的终止速度为0
		}
	}

MainSuc:
//Suc:
	if(!pData)goto tag1;

	return SUCCESS;
	
//Err:
	debugout("SpeedPretreatment Err\r\n");
	return FAIL;
	
tag1:
	return SUCCESS;
/*
{
	float vs,vc,ve,vm,len;
	float t,t1,t2,t3,L,L1,L2,L3;
	TFM *p,*lp;

	pCurMP  = GetMPDataOut(0,q);;

	len=fabsf(pCurMP->relpos);
	
	if(len>0.001)
	{
		pCurMP->speedv=max(max(pCurMP->vs,pCurMP->ve),max(pCurMP->ve,pCurMP->vmax));
		vs=pCurMP->vs;		
		ve=pCurMP->ve;
		vc=sqrt((2*SYS_acc(INPA)*SYS_acc(INPA)*len+SYS_acc(INPA)*vs*vs+SYS_acc(INPA)*ve*ve)/(SYS_acc(INPA)+SYS_acc(INPA)));
		if(vc<pCurMP->speedv)		//若最高加速低于驱动速度，则没有匀速段
			pCurMP->speedv=vc;
		else
			vc=pCurMP->speedv;	
		t1=(vc-vs)/SYS_acc(INPA);
		t3=(vc-ve)/SYS_acc(INPA);
		L=len-((vc+vs)*t1+(vc+ve)*t3)/2;
		pCurMP->T=t1+t3+L/vc;
	}
	else
		pCurMP->T=0;

	for(i=0;;i++)
	{
		pLastMP = GetMPDataOut(i+1,q);    	//获取队列尾数据
		pLLMP   = GetMPDataOut(i+2,q);

		if(pLastMP)
		{
			len=fabsf(pLastMP->relpos);
			if(len>0.001)
			{
				pLastMP->speedv=max(max(pLastMP->vs,pLastMP->ve),max(pLastMP->ve,pLastMP->vmax));
				vs=pLastMP->vs;		
				ve=pLastMP->ve;
				vc=sqrt((2*SYS_acc(INPA)*SYS_acc(INPA)*len+SYS_acc(INPA)*vs*vs+SYS_acc(INPA)*ve*ve)/(SYS_acc(INPA)+SYS_acc(INPA)));
				if(vc<pLastMP->speedv)		//若最高加速低于驱动速度，则没有匀速段
					pLastMP->speedv=vc;
				else
					vc=pLastMP->speedv;
				t1=(vc-vs)/SYS_acc(INPA);
				t3=(vc-ve)/SYS_acc(INPA);
				L=len-((vc+vs)*t1+(vc+ve)*t3)/2;
				pLastMP->T=t1+t3+L/vc;
			}
			else
				pLastMP->T=0;
		}
		for(j=Ax;j<MAXAXIS;j++)
		{
			if(j==pCurMP->maxis)continue;
			p=&pCurMP->axis[j];
			lp=&pLastMP->axis[j];
			if(!p->relpos)continue;
			
			p->vs=0;
			p->ve=0;
			p->endcoder=pCurMP->pos;
			p->startcoder=pCurMP->pos-pCurMP->relpos;
			p->ad=SYS_acc(j);
			p->dd=SYS_acc(j);
//			p->cps=fabsf(pCurMP->relpos)/pCurMP->T;
			if(pCurMP->T>0)
				p->cps=fabsf(pCurMP->relpos)/pCurMP->T;
			else
				p->cps=pCurMP->speedv;
			p->vcps=1./p->cps;
//			t=fabsf(p->endcoder-p->startcoder)*p->vcps;
			t=pCurMP->T;

			len=fabsf(p->relpos);								//长度计算全部用正值计算
		//debugout("%f v:%f\r\n",p->ad*t*t/4,v);
			if((p->ad*t*t/4)<len)
			{
				p->dd=p->ad=len/(t*t/4);						//不足长度，则改变加速度满足加工长度
				t1=t2=t/2;
				vm=p->ad*t1;
				p->len1=p->len2=len/2;
//		debugout("ad:%f vm:%f\r\n",p->ad,vm);
			}
			else
			{
				t1=(-t+sqrt(t*t-4*len/p->ad))/-2;
//				t1=(-t-sqrt(t*t-4*len/p->ad))/-2;				//该表达式有两个根，后继调试需要注意
				t2=t-t1;
				vm=p->ad*t1;
				p->len1=p->ad*pow(t1,2)/2;
				p->len2=len-p->len1;
//		debugout("vm:%f ad:%f\r\n",vm, p->ad);
			}

			p->vc=vm;
			p->t1=t1;
			p->t2=t2;
			p->t3=t;
debugout("vc:%5.3f %5.3f %5.3f %5.3f pos%5.3f\r\n",vm,p->t1,p->t2,p->t3,p->relpos);			
debugout("len:%5.3f %5.3f %5.3f\r\n",len,p->len1,p->len2);
debugout("ad:%5.3f dd%5.3f \r\n",p->ad,p->dd);
		}
		pCurMP=pLastMP;
		if(!pCurMP)break;
	}
	return SUCCESS;
}
*/	return SUCCESS;

}
#endif


int Init_coderfollow_T(MP *pMD,int axis)
{
	double 	t,t1,t2;
	float	vm,len;
	double 	ftemp;
	TFM 	*p;

	p=&pMD->axis[axis];
	
	p->vs=0;
	p->ve=0;
	p->endcoder=pMD->pos;
	p->startcoder=pMD->pos-pMD->relpos;
	p->ad=SYS_acc(axis);
	p->dd=SYS_acc(axis);
	
//	p->cps=fabsf(pCurMP->relpos)/pCurMP->T;
	
	if(pMD->T>0)
		p->cps=fabsf(pMD->relpos)/pMD->T;
	else
		p->cps=pMD->speedv;
	
	p->vcps=1./p->cps;

//	t=fabsf(p->endcoder-p->startcoder)*p->vcps;

	t=pMD->T;

	len=fabsf(p->relpos);										//长度计算全部用正值计算

	//	debugout("%f v:%f\r\n",p->ad*t*t/4,v);
	
	ftemp=p->ad*p->ad*t*t-4*len*p->ad;
	
	//	Uart_Printf("ftemp:%5.5f ",ftemp);
	//	if((p->ad*t*t/4-len)>0.001)
	
	if(ftemp<0.001)
	{
		p->dd=p->ad=len/(t*t/4);								//不足长度，则改变加速度满足加工长度
		t1=t2=t/2;
		vm=p->ad*t1;
		p->len1=p->len2=len/2;
//		debugout("ad:%f vm:%f\r\n",p->ad,vm);
	}
	else
	{
		ftemp=sqrt(ftemp);
		t1=(p->ad*t-ftemp)*0.5/p->ad;
		
		//Uart_Printf("t1:%5.5f ",t1);
//		if(fabsf(p->ad*t1*t1+p->ad*t1*(t-2*t1)-len)>0.001)
		
		if((2*t1)>t)
		{	
			t1=(p->ad*t+ftemp)*0.5/p->ad;
			//Uart_Printf("@t1:%5.5f ",t1);
		}
		
//		t1=(-t+sqrt(t*t-4*len/p->ad))/-2;
//		t1=(-t-sqrt(t*t-4*len/p->ad))/-2;						//该表达式有两个根，后继调试需要注意
		
		t2=t-t1;
		vm=p->ad*t1;
		p->len1=p->ad*pow(t1,2)/2;
		p->len2=len-p->len1;
//		debugout("vm:%f ad:%f\r\n",vm, p->ad);
	}
	
//Uart_Printf("axis:%d ad:%5.3f vm:%5.3f len:%5.3f t:%5.5f\r\n",axis,p->ad,vm,len,t);
	p->vc=vm;
	p->t1=t1;
	p->t2=t2;
	p->t3=t;
//Err:
	return 0;
}


#if 0

float Get_follow_pos_T(TFM *p,float t)
{
	#define PI2  9.8696044
	#define PI	 3.1415926

	volatile float pos;
	volatile float ftemp,f_temp2,mainpos;
	volatile float coder;
	volatile float logicpos,coderpos,curpos,acc;
	
	pos=0;

	if(!p->relpos)goto Err;
	//debugout("t:%5.3f t1:%5.3f %5.3f %5.3f\r\n",t,p->t1,p->t2,p->t3);
	
	if(t<p->t1)													//加速
	{	
//		pos=p->vs*t+p->ad*t*t/2;
		pos=p->ad*(t*t*0.5+p->t1*p->t1*0.25/PI2*cos(t/p->t1*2*PI)-p->t1*p->t1*0.25/PI2);
	}
	else if(t<p->t2)											//匀速
	{	
		pos=p->len1+p->vc*(t-p->t1);
	}
	else if(t<p->t3)											//减速
	{ 
		ftemp=p->t3-t;
//		pos=p->len2+p->vc*ftemp-p->dd*ftemp*ftemp*0.5;
		pos=fabsf(p->relpos)-p->dd*(ftemp*ftemp*0.5+p->t1*p->t1*0.25/PI2*cos(ftemp/p->t1*2*PI)-p->t1*p->t1*0.25/PI2);
	}
	else														//尾部速度处理
	{
		pos=fabsf(p->relpos);
		t=p->t3;
	}
	
	if(p->relpos<0)pos=-pos;
Err:	
	return pos;

}

#else

float Get_follow_pos_T(TFM *p,float t)
{
	#define 	PI2  	9.8696044
	#define 	PI	 	3.1415926

	volatile 	float 	pos;
	volatile 	float 	ftemp;										

	pos=0;
    
    //Uart_Printf("%f,%f,%f,%f\r\n",p->vs,p->vc,p->ve,p->ad);
	
	if(!p->relpos)goto Err;
	//debugout("t:%5.3f t1:%5.3f %5.3f %5.3f\r\n",t,p->t1,p->t2,p->t3);
	
	if(t<p->t1)													//加速
	{	
	  	if(p->vs<=p->vc)
			//pos=p->vs*t+p->ad*t*t/2;
			//pos=p->ad*(t*t*0.5+p->t1*p->t1*0.25/PI2*cos(t/p->t1*2*PI)-p->t1*p->t1*0.25/PI2);
			pos=0.5*(p->vc+p->vs)*t-0.5*(p->vc-p->vs)*sin(t*PI/p->t1)*p->t1/PI;
	 	else
	    	//pos=p->vs*t-p->ad*t*t/2;
	    	//ftemp=p->t1-t;
	   		// pos=fabsf(p->relpos)-p->dd*(ftemp*ftemp*0.5+p->t1*p->t1*0.25/PI2*cos(ftemp/p->t1*2*PI)-p->t1*p->t1*0.25/PI2);
	      	pos=0.5*(p->vc+p->vs)*t+0.5*(p->vs-p->vc)*sin(t*PI/p->t1)*p->t1/PI;
	    	//pos=p->len1-(0.5*(p->vs+p->vc)*(p->t1-t)-0.5*(p->vs-p->vc)*sin((p->t1-t)*PI/(p->t1))*(p->t1)/PI);
			//pos=p->ad*(t*t*0.5+p->t1*p->t1*0.25/PI2*cos(t/p->t1*2*PI)-p->t1*p->t1*0.25/PI2);
	}
	else if(t<p->t2)											//匀速
	{	
		pos=p->len1+p->vc*(t-p->t1);
	}
	else if(t<p->t3)											//减速
	{ 
        ftemp=p->t3-t;
		pos=fabsf(p->relpos)-(0.5*(p->vc+p->ve)*ftemp-0.5*(p->vc-p->ve)*sin(ftemp*PI/(p->t3-p->t2))*(p->t3-p->t2)/PI);
	}
	else														//尾部速度处理
	{
		pos=fabsf(p->relpos);
		t=p->t3;
	}
	
	if(p->relpos<0)pos=-pos;
	 		 		
	/*if(t<=p->t1)												//加速
	{	
	    if(p->vs<=p->vc)
			pos=p->vs*t+p->ad*t*t/2;
	   else
	    	pos=p->vs*t-p->ad*t*t/2;
	}
	else if(t<p->t2)
	{
	    pos=p->len1+p->vc*(t-p->t1);	
	 		
	}
	else if(t<p->t3)											//减速
	{ 
		ftemp=t-p->t2;
		pos=p->len2+p->vc*ftemp-p->dd*ftemp*ftemp*0.5;
	}
	else														//尾部速度处理
	{
		pos=fabsf(p->relpos);
		t=p->t3;
	}*/

	//Uart_Printf("%f,%f,%f\r\n",p->vs,p->vc,p->ve);
Err:	
	return pos;

}

#endif



//该函数用于在缓存数据正常走完后，对剩下的TAG前为执行的事件进行执行
static int event_end(void)					
{
	EP 		*pEvent;
	int iaxis;

	for(;;)
	{
		pEvent = GetEPDataOut(0,pEPQ);    						//获取队列头数据

		if(pEvent)
		{
		//	Uart_Printf("EvTp=%d\n",pEvent->Type);
		
			switch(pEvent->Type)
			{	
				case _T_GPIO:
					WriteBit(pEvent->Data.Gpio.ioport,pEvent->Data.Gpio.iolevel);
					break;
				
				case _T_POSSET:
				//	SetPulse((pEvent->Data.Posset.axis+1),0,TRUE);
					
					iaxis=pEvent->Data.Posset.axis;
					BaseSetCommandOffset(iaxis,pEvent->Data.Posset.pos - g_lastpulsereg[iaxis]);
					
					break;
					
				case _T_TAGSET:
					{
						CurTag=pEvent->Data.Tagset.tag;
						
						CurIndex = 0;//加工结束，时CurIndex设置为0,(当前的执行行号)
						if(SYS_State==VM_TAGSTOP)
							return 1;							//注意在第一个_T_TAGSET退出，因为缓存了3条弹簧
						else
							break;								//延时退出
					}
			}

			DelQueue(pEPQ);
		}
		else
		{
			return 1;
		}
	}
	
	return 1;
}



static int event_proc(int bOver,MP *pCur)
{
	int 	itemp,i,T_POSSET_AXISMAP;
	EP 		*pEvent;
	INT32U 	index;
	#define INVALLIMIT	15										//滞后间隔最大限定段数
	static 	long 		lastpulsereg[INVALLIMIT+1][MAXAXIS];
	long 	ltemp[MAXAXIS];
	int 	T_POSSET_TAG=0;
	
	if(bOver)
	{
		for(i=0;;i++)
		{
			pEvent = GetEPDataOut(i,pEPQ);    					//获取队列头数据
		
			if(!pEvent)break;
			
			if(!pEvent->PCnt)break;
		}
	
	//	Uart_Printf("@%d @%d %d %d %d\r\n",eventtag,pEvent->Type,pEvent->QueueIndex , mdata[INPA].Info.index.QueueIndex,vm_getqueueindex());	
	
		if(pEvent && pEvent->QueueIndex == pCur->Info.index.QueueIndex)					//提前处理的缓存事件
		{
			T_POSSET_TAG=0;
			T_POSSET_AXISMAP=0;
	
			for(;;i++)			
			{
				pEvent = GetEPDataOut(i,pEPQ);    				//获取队列头数据
			//	Uart_Printf(" @%d %d %d ",pEvent->Type,pEvent->QueueIndex , mdata[INPA].Info.index.QueueIndex);	
			//	Uart_Printf("@%d %x %x %x %x \r\n",pCur->Info.index.Index,pEvent, pEvent->QueueIndex, pCur->Info.index.QueueIndex, pEvent->PCnt);
				if(!pEvent || pEvent->QueueIndex != pCur->Info.index.QueueIndex || pEvent->PCnt)break;
				
				pEvent->PCnt=1;
				
				switch(pEvent->Type)
				{
					case _T_POSSET:
						itemp=pEvent->Data.Posset.axis;
						
						if(T_POSSET_AXISMAP & (0x01<<itemp))	//如果一个轴在同一个运动后被复位多次，则取最后的复位值
						{

						}
						else
						{	
						//	Uart_Printf("lastpulse:%d\r\n",lastpulse[itemp]);
							g_lastpulsereg[itemp]=lastpulsereg[T_POSSETCountPush&0x0F][itemp]=lastpulse[itemp];
							T_POSSET_AXISMAP |= (0x01<<itemp);
						}
						
						lastpulse[itemp]=pEvent->Data.Posset.pos;
						T_POSSET_TAG=1;
						//Uart_Printf("@&&%d %d 0x%x %d %d\r\n",T_POSSETCountPush&0x07,itemp,T_POSSET_AXISMAP,lastpulse[itemp],lastpulsereg[T_POSSETCountPush&0x07][itemp]);
						break;
						
					case _T_TAGSET:
						CurTag=pEvent->Data.Tagset.tag;
						
						if(SYS_State==VM_TAGSTOPCMD)
							SYS_State=VM_TAGSTOP;
						
						//	Uart_Printf("CurTag:%d\r\n",CurTag);
						break;
				}
			}
			
			if(T_POSSET_TAG)
			{
				T_POSSETCountPush++;
			}
		}
	}
	
	T_POSSET_TAG=0;
	
	pEvent 	= GetEPDataOut(0,pEPQ);    							//获取队列头数据
	index	= vm_getqueueindex();
	
	if(pEvent)
	{
		/*
		{
			static int tag;
			
			if(tag!=index)
			{
			//	if(pEvent->QueueIndex<index)
				Uart_Printf("&&%d %d %d\r\n",eventtag,pEvent->QueueIndex,index);		
				tag=index;
			}
		}
		*/
		
//		if((eventtag && pEvent->QueueIndex != index) || pEvent->QueueIndex==0)		//实时处理的缓存事件
		if((eventtag && ((((int)pEvent->QueueIndex-(int)index)<0 && (index-pEvent->QueueIndex)<INVALLIMIT) ||((MAXMPCOUNT-pEvent->QueueIndex)<3 && index<3)))
			|| pEvent->QueueIndex==0)												//实时处理的缓存事件,为防止执行过快漏掉一些数据
		{
			//Uart_Printf("&&... %d %d %d\r\n",eventtag,pEvent->QueueIndex,index);
			T_POSSET_TAG=0;
			memset(ltemp,0x00,sizeof(ltemp));
			
			for(;;)
			{
				pEvent = GetEPDataOut(0,pEPQ);    				//获取队列头数据
			
				if(pEvent && (pEvent->QueueIndex == eventtag))
				{
				//	Uart_Printf("&&--- %d %d %d\r\n",eventtag,pEvent->QueueIndex,pEvent->Type);
					
					switch(pEvent->Type)
					{	
						case _T_GPIO:
							WriteBit(pEvent->Data.Gpio.ioport,pEvent->Data.Gpio.iolevel);
							break;
						
						case _T_PROBE:
						//	Uart_Printf("probe %d \r\n",pEvent->Data.Gpio.ioport);
							OpenProbe(0,pEvent->Data.Probe.pro,pEvent->Data.Probe.port);
							break;
						
						case _T_POSSET:
							itemp=pEvent->Data.Posset.axis;
						//	Uart_Printf("AA=%d %ld\r\n",pEvent->Data.Posset.pos,lastpulsereg[T_POSSETCountPop&0x07][itemp]);
							ltemp[itemp]=pEvent->Data.Posset.pos-lastpulsereg[T_POSSETCountPop&0x0f][itemp];
							T_POSSET_TAG=1;
						//	BaseSetCommandOffset(itemp,pEvent->Data.Posset.pos-lastpulsereg[T_POSSETCountPop&0x07][itemp]);
						//	BaseSetCommandOffset(itemp,pEvent->Data.Posset.pos-lastpulse[itemp]);
						//	lastpulse[itemp]=pEvent->Data.Posset.pos;
						//	Uart_Printf("#%d %d %d\r\n",itemp,lastpulse[itemp],lastpulsereg[itemp]);
							break;
							
						case _T_TAGSET:
						//	CurTag=pEvent->Data.Tagset.tag;
						//	if(SYS_State==VM_TAGSTOPCMD)
						//		SYS_State=VM_TAGSTOP;
						//	Uart_Printf("CurTag:%d\r\n",CurTag);
							break;
					}

					DelQueue(pEPQ);
					continue;
				}
				
				break;
			}
			
			if(T_POSSET_TAG)
			{
			//	Uart_Printf("\t&&%d %d %d %d \r\n",ltemp[Ax],ltemp[Ay],ltemp[Az],ltemp[Aa]);
				BaseSetCommandOffset(Ax,ltemp[Ax]);
				BaseSetCommandOffset(Ay,ltemp[Ay]);
				BaseSetCommandOffset(Az,ltemp[Az]);
				BaseSetCommandOffset(Aa,ltemp[Aa]);
				BaseSetCommandOffset(Ab,ltemp[Ab]);
				BaseSetCommandOffset(Ac,ltemp[Ac]);

			//	Uart_Printf("\t&&%d %d %d %d \r\n",ltemp[Ax],ltemp[Ay],ltemp[Az],ltemp[Aa]);

				T_POSSETCountPop++;
			//	Uart_Printf("@@@%x %x %x\r\n",index,eventtag,pEvent->QueueIndex);
			//	Uart_Printf("%d Pop=%d,push=%d\r\n",pCur->Info.index.Index,T_POSSETCountPop,T_POSSETCountPush);
				
				if(T_POSSETCountPop>T_POSSETCountPush || abs(T_POSSETCountPop-T_POSSETCountPush)>=INVALLIMIT)
				{
					Uart_Printf("T_POSSETCountPush err\r\n");
					return FAIL;
				}
			}
		}
		
	//	if(pEvent->QueueIndex <= index)
	//	Uart_Printf("$$%d %d %d\r\n",eventtag,index,pEvent->QueueIndex);		
	//	if(((int)pEvent->QueueIndex-(int)index)<=0 && (index-pEvent->QueueIndex)<INVALLIMIT)
		
		if((((int)pEvent->QueueIndex-(int)index)<=0 && (index-pEvent->QueueIndex)<INVALLIMIT)
			||((MAXMPCOUNT-pEvent->QueueIndex)<3 && index<3))
		{
			//if(eventtag!=index)
			//Uart_Printf("\t$$%d %d %d\r\n",eventtag,index,pEvent->QueueIndex);		
			eventtag=pEvent->QueueIndex;
		}
		else
			eventtag=0;
	}
	
	return SUCCESS;
}


static int inp_t_move(int inpg,MP *p,MP *pNextMP,T_PreParamPack *pP)
{
	float 		pos;
	int 		i,iFlag_temp;
	float 		vs,vc,ve;
	float 		ftemp;
	int 		itemp,temp[MAXAXIS];
	int 		ErrNo;
	unsigned 	short 	sta;
	INT8U 		err;

//	switch(p->Info.bDataTag)
	{
//		case 0:
			pP->maxpos=fabsf(p->relpos);
			
			if(pP->maxpos<0.001)
			{
				p->T=0;
				goto Suc;
			}

			pP->axismap=0;

////////////////////////////////////////////

			p->speedv=max(max(p->vs,p->ve),max(p->ve,p->vmax));
			vs=p->vs;		
			ve=p->ve;
//			vc=sqrt((2*SYS_acc(INPA)*SYS_acc(INPA)*pP->maxpos+SYS_acc(INPA)*vs*vs+SYS_acc(INPA)*ve*ve)/(SYS_acc(INPA)+SYS_acc(INPA)));
			vc=sqrt((2*SYS_acc(INPA)*pP->maxpos+vs*vs+ve*ve)*0.5);

			if(vc<p->speedv)									//若最高加速低于驱动速度，则没有匀速段
				p->speedv=vc;
			else
				vc=p->speedv;
				
			pP->t1=(vc-vs)/SYS_acc(INPA);
			pP->t3=(vc-ve)/SYS_acc(INPA);
			ftemp=pP->maxpos-((vc+vs)*pP->t1+(vc+ve)*pP->t3)/2;
			p->T=pP->t1+pP->t3+ftemp/vc;
			
////////////////////////////////////////////

			pP->vs=p->vs;		
			pP->ve=p->ve;
			pP->vc=p->speedv;
			pP->acc=SYS_acc(inpg);
			pP->dec=SYS_acc(inpg);

//			pP->t1=(pP->vc-pP->vs)/SYS_acc(INPA);
//			pP->t3=(pP->vc-pP->ve)/SYS_acc(INPA);
			
			pP->L1=(pP->vc+pP->vs)*pP->t1/2;
			pP->L3=(pP->vc+pP->ve)*pP->t3/2;
			pP->L3=(pP->vc+pP->ve)*pP->t3/2;
			pP->L2=pP->maxpos-pP->L1-pP->L3;
			
			if(pP->L2<0.001)
			{
				if(pP->L1>pP->L3)
					pP->L1=pP->maxpos-pP->L3;
				else 
					pP->L3=pP->maxpos-pP->L1;
				
				pP->L2=0;
			}

		//	pP->axismap=0x07;
		//	debugout("%d %f %f %f\r\n",p->maxis,pP->regpos[p->maxis],p->pos,p->relpos);
		
		//	if(pP->t1>0.001)
		//		pP->acc=(pP->vc-pP->vs)/(pP->t1);

		//	if(pP->t3>0.001)
		//		pP->dec=(pP->vc-pP->ve)/(pP->t3);
			
			pP->t2=pP->L2/pP->vc;
			pP->t2=pP->t1+pP->t2;
			pP->t3=pP->t2+pP->t3;

			pP->time=pP->t=0;
			p->Info.bDataTag=1;
			p->T=pP->t3;
		
		//	Uart_Printf("%f,%f,%f\r\n",p->axis[1].vs,p->axis[1].vc,p->axis[1].ve);
		//	Uart_Printf("inp vs:%f vc:%f ve:%f t:%5.3f pos:%f acc:%5.3f dec:%5.3f\r\n",pP->vs,pP->vc,pP->ve,p->T,pP->maxpos,pP->acc,pP->dec);
		//	debugout("G01X%3.3f Y%3.3f Z%3.3f\r\n",pP->SYS_RMacPos[Ax]+p->relpos[Ax],pP->SYS_RMacPos[Ay]+p->relpos[Ay],pP->SYS_RMacPos[Az]+p->relpos[Az]);
		
			for(i=Ax;i<MAXAXIS;i++)
			{
				if(i!=p->maxis)
				{
					if(fabsf(p->axis[i].relpos)>0.001)
						pP->axismap|=(0x01<<i);
					
					pP->lastpos[i]=pP->regpos[i]=p->axis[i].pos-p->axis[i].relpos;

				/*		
					p->axis[i].cps=pP->vc;
					p->axis[i].vs=0;
					p->axis[i].ve=0;
					p->axis[i].endcoder=p->pos;
					p->axis[i].startcoder=p->pos-p->relpos;
					p->axis[i].ad=SYS_acc(i);
					p->axis[i].dd=SYS_acc(i);
					*/
					
				    if(MethodFlag==1)
                    {
						Init_coderfollow_T(p,i);
					}
				}
				else
				{
					pP->lastpos[i]=pP->regpos[i]=p->pos-p->relpos;
					
					if(fabsf(p->relpos)>0.001)
						pP->axismap|=(0x01<<i);
				}
				
				/*if(i==1)
				{
               	 	Uart_Printf("%f,%f,%f %f,%f,%f\r\n",p->axis[i].vs,p->axis[i].vc,p->axis[i].ve,p->axis[i].ad,p->axis[i].relpos,p->T);
                	//Uart_Printf("%f,%f,%f\r\n",p->axis[i].t1,p->axis[i].t2,p->axis[i].t3);
                }*/
			}
//			break;

//		case 1:
			pos=0;
			iFlag_temp=0;
			
			//debugout("test1\n");
			
			/*
			for(i=Ax;i<MAXAXIS;i++)
			{
			 	if(i==p->maxis) continue;
			 	if(i==1)
			 	Uart_Printf("%f,%f,%f\r\n",p->axis[i].vs,p->axis[i].vc,p->axis[i].ve);
		    }
		    */
		    
		    // Uart_Printf("%f\r\n",p->axis[2].relpos);
            // Uart_Printf("%f,%f,%f\r\n",p->axis[2].vs,p->axis[2].vc,p->axis[2].ve);
            // Uart_Printf("%f,%f,%f\r\n",p->axis[2].t1,p->axis[2].t2,p->axis[2].t3);
		 	
		 	for(;;)
			{
				pP->t+=dt;

				if(pP->t<pP->t1)								//加速
				{	
					pos=pP->vs*pP->t+pP->acc*pP->t*pP->t/2;
				//	Uart_Printf("@1 %5.5f ",pP->t);
		 		}
				else if(pP->t<pP->t2)							//匀速
				{
				//	pP->t=pP->t2;
				//	pos=pP->L1+pP->L2;
					pos=pP->L1+pP->vc*(pP->t-pP->t1);
				//	Uart_Printf("@2 %5.5f ",pP->t);
		 		}
				else if(pP->t<pP->t3)							//减速
				{
					ftemp=pP->t-pP->t2;
					pos=pP->L1+pP->L2+pP->vc*ftemp-pP->dec*ftemp*ftemp/2;
				//	Uart_Printf("@3 %5.5f ",pP->t);
		 		}
				else											//尾部速度处理
				{
					pos=pP->maxpos;
					pP->t=pP->t3;
					iFlag_temp=1;
				//	Uart_Printf("@4 %5.5f ",pP->t);
				}
				
				if(p->relpos<0)pos=-pos;

				pP->descpos[p->maxis]=pos+pP->regpos[p->maxis];
				
				//Uart_Printf("%f\r\n",pP->descpos[p->maxis]);
				
				for(i=Ax;i<MAXAXIS;i++)
				{
					if(i!=p->maxis)
					{
					//	pP->descpos[i]=Get_follow_pos_T(&(p->axis[i]),pP->descpos[p->maxis])+pP->regpos[i];
						pP->descpos[i]=Get_follow_pos_T(&(p->axis[i]),pP->t)+pP->regpos[i];
						
						debugout("%d@%f %f\r\n",i,pP->descpos[i],pP->regpos[i]);
					}
				}
				
				ftemp=pP->t-pP->time; 
				
		//		if(ftemp>=0.00001)
				if(ftemp>=0.000002)
				{
					//debugout("G01X%fY%fZ%f(%x)\r\n",pP->descpos[Ax],pP->descpos[Ay],pP->descpos[Az],pP->axismap);
					//if(ErrNo=inpmove(pP,pP->axismap,ftemp,pP->lastpos,pP->descpos))goto Err;
					//debugout("time:%f:%f %f %f\r\n",ftemp,pP->descpos[p->maxis]-pP->lastpos[p->maxis],pos,(pP->descpos[p->maxis]-pP->lastpos[p->maxis])/ftemp);

					temp[Ax]=(int)(pP->descpos[Ax]*CALRATE)-lastpulse[Ax];
					temp[Ay]=(int)(pP->descpos[Ay]*CALRATE)-lastpulse[Ay];
					temp[Az]=(int)(pP->descpos[Az]*CALRATE)-lastpulse[Az];
					temp[Aa]=(int)(pP->descpos[Aa]*CALRATE)-lastpulse[Aa];
					temp[Ab]=(int)(pP->descpos[Ab]*CALRATE)-lastpulse[Ab];
					temp[Ac]=(int)(pP->descpos[Ac]*CALRATE)-lastpulse[Ac];

					//a+=temp[Ax];
					//Uart_Printf("%f\r\n",pP->descpos[Ax]);
                    //Uart_Printf("%f,%f\r\n",pP->descpos[Az],temp[Az]/ftemp);
					//Uart_Printf("^^%d %d\r\n",lastpulse[Az],p->Info.index.QueueIndex);
					//Uart_Printf("%5.3f %5.3f %5.3f %5.3f \r\n",pP->descpos[Ax],pP->descpos[Ay],pP->descpos[Az],pP->descpos[Aa]);
					
					itemp=max( max(abs(temp[Ax]),abs(temp[Ay])), max( max(abs(temp[Az]),abs(temp[Aa])),max(abs(temp[Ab]),abs(temp[Ac])) ) );

					lastpulse[Ax]+=temp[Ax];
					lastpulse[Ay]+=temp[Ay];
					lastpulse[Az]+=temp[Az];
					lastpulse[Aa]+=temp[Aa];
					lastpulse[Ab]+=temp[Ab];
					lastpulse[Ac]+=temp[Ac];

					/*
					lastpulse[Ax]=(int)(lastpulse[Ax]+temp[Ax]);
					lastpulse[Ay]=(int)(lastpulse[Ay]+temp[Ay]);
					lastpulse[Az]=(int)(lastpulse[Az]+temp[Az]);
					lastpulse[Aa]=(int)(lastpulse[Aa]+temp[Aa]);
					*/
					
/******************************************************/
loop:
					if(SYS_State==VM_STOP)						//异常退出
					{
						ErrNo=_ERR_AXISSTOP;
						goto Err;
					}				

					read_fifo(&sta);
					
			//		if((sta&0x7ff)>(6*10))
					if((sta&0x7ff)>(8*10))						//fifo_inp_move6 1条缓存占用8个空间
					{
						OSSemSet (gMoveStopSEM,0,&err);
						OSSemPend(gMoveStopSEM,20,&err);
					//	OSTimeDly(1);
					//	debugout("err:%d\r\n",err);
					//	Uart_Printf("@%d %x\r\n",FPGARATE,sta);
						goto loop;
					}
					
					if(itemp/ftemp > 500000)					//限制发送频率在500K内，用于异常处理
					{
						ErrNo=_ERR_SPEEDERR;
						g_ivm_errNo=1;
						Uart_Printf("speed too max:%d %5.5f\r\n",itemp,ftemp);
						SYS_State=VM_STOP;
						goto Err;
					}
					
/******************************************************/
					
					event_proc(iFlag_temp,p);
					vm_setindex(p->Info.index);
				//	fifo_inp_move4(temp[Ax],temp[Ay],temp[Az],temp[Aa],4*itemp/ftemp);
					fifo_inp_move6(temp[Ax],temp[Ay],temp[Az],temp[Aa],temp[Ab],temp[Ac],4*itemp/ftemp);


				//	{
				//		Uart_Printf("Sed=%f \n",4*itemp/ftemp);

				//	}


				/*	{
						static int tempall[4]={0};
						tempall[0] +=temp[Ax];
						tempall[1] +=temp[Ay];
						tempall[2] +=temp[Az];
						tempall[3] +=temp[Aa];

						Uart_Printf("ALL Ax=%d,Ay=%d,Az=%d,Aa=%d \n",tempall[0],tempall[1],tempall[2],tempall[3]);

					}*/


					/*
					Uart_Printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",\
					icount++,\
					lastpulse[Ax],(int)((temp[Ax])/ftemp),\
					lastpulse[Ay],(int)((temp[Ay])/ftemp),\
					lastpulse[Az],(int)((temp[Az])/ftemp),\
					lastpulse[Aa],(int)((temp[Aa])/ftemp));
					*/

				//	memcpy(pP->lastpos,pP->descpos,sizeof(pP->lastpos));
				//	pP->lastpos[p->maxis]=pP->descpos[p->maxis];
					pP->time=pP->t;
				}
				else
				{	
					event_proc(iFlag_temp,p);
				}

				if(iFlag_temp)
				{
					p->Info.bDataTag=2;
					break;
				}
				
		 	} //end for(;;) 以时间为单位进行拆分
	
		//	debugout("test2\n");
		//	break;
	}
	
Suc:
	return _ERR_NO_ERR;
	
Err:
	debugout("inp move error\r\n");
	return ErrNo;

}


OS_EVENT  *VM_START;
OS_EVENT  *VM_PV;												//VM互斥信号量
OS_EVENT  *QUEUE_PV;											//Queue互斥信号量

void vm_motion_task(void *a)
{
	static 	MP 	*pLastMP;
	int 		ErrNo;
	INT8U 		err;
	
	EH.EHCount	=0;
	
	debugout("vm_motion system startup...\r\n ");
	VM_START=OSSemCreate (0);
	VM_PV=OSSemCreate (1);
	QUEUE_PV=OSSemCreate (1);
//	Enable_ADT834INT();   //这个函数死机
	vm_setrange1(1);
	vm_setrange2(8000);
	
	for(;;)
	{
		if(!Get_QueueCount(pMQ))
		{
			while(vm_get_status())								//需在结束时等待缓存发完
			{
				OSTimeDly(1);
				
			//	if(FPGARATE<100)
			//		SET_RANGE(100);
		
				if(SYS_State==VM_STOP)
				{
					int 	status;

					reset_fifo();								//探针等急停时采用缓存自然走完，不采用Stop1，防止Stop1造成的脉冲丢失
					
					while(FPGARATE>0)
					{
						get_inp_status(&status);
						if(!status)break;
						OSTimeDly(1);
					}

					reset_fifo();
					
					Stop1(Ax+1);
					Stop1(Ay+1);
					Stop1(Az+1);
					Stop1(Aa+1);
					Stop1(Ab+1);
					Stop1(Ac+1);
					
					for(;;)												//增加插补状态确认，避免最后一个脉冲因为倍率问题导致无法退出
					{
						SET_RANGE(8000);
						get_inp_status(&status);
						if(!status)break;
						OSTimeDly(1);
					}
					
					InitQueue(pMQ);
					InitQueue(pEPQ);
					mdata[INPA].Info.bDataTag=3;
					eventtag=0;
					vm_setrange1(1);
					
					
				}
			
		
			}
		
			event_end();										//处理缓存发完后的事件
		
			InitQueue(pEPQ);
			T_POSSETCountPush=0;
			T_POSSETCountPop=0;
			SYS_State=VM_IDLE; 
			OSSemSet (VM_START,0,&err);
			
		//	Uart_Printf("motion over\r\n");

			OSSemPend(VM_START,0,&err);

		//	Uart_Printf("pend start\r\n");
			
			if(SYS_State!=VM_STOP)
				SYS_State=VM_RUN;
			else
				Uart_Printf("@err1\r\n");
				
			eventtag=0;
			CurQueueIndex=0;
			memset(FIFOBUF,0x00,sizeof(FIFOBUF));
		//	Uart_Printf("testpoint8\n");
		}
	
		if(Get_QueueCount(pMQ))
		{
		//	debugout("%d \r\n",Get_QueueCount(pMQ));
		//	Uart_Printf("testpoint9\n");

		//	if(mdata[INPA].Info.bDataTag!=1)
			{
				if(!DeQueue(sizeof(MP),&mdata[INPA],pMQ))
				{
					pLastMP=GetMPDataOut(0,pMQ);
				//	debugout("%d %f\r\n",icount,mdata[INPA].pos[Ax]);
				}
				else continue;
			}
		}
		else continue;

//		CurIndex=mdata[INPA].Info.index;

		ErrNo=inp_t_move(INPA,&mdata[INPA],pLastMP,&ParamTemp[INPA]);
		
		if(ErrNo)
		{
			ErrRecord(ErrNo);
			
			switch(ErrNo)
			{
				case _ERR_SPEEDERR:
/*					while(1)
					{
						OSTimeDly(10);
					}
*/					break;
				
				case _ERR_AXISSTOP:
//				case _ERR_MOTIONCONFLICT:						//运动冲突
//					InitQueue(pMQ);
//					InitQueue(pEPQ);
//					mdata[INPA].Info.bDataTag=3;
				//	ebugout("%d AA_ERR_AXISSTOP!\r\n",ErrNo);
					break;
				
				default:
					debugout("%d _ERR_AXISSTOP!\r\n",ErrNo);
					break;
			}
		}


		if(SYS_State==VM_STOP)									//运动停止，用于处理急停，停止等操作
		{
			float 	rate,unitrate;
			int 	status;
			INT16U 	sta,laststa;
			
		//	Uart_Printf("VM_STOP ...\r\n");
		
			if(g_iStopMode==0)
			{	
				read_fifo(&sta);
				rate=RANGERATE1;
				
				if((sta&0x7ff)>0)
				{
				//	unitrate=rate/((sta&0x7ff)/6);
					unitrate=rate/((sta&0x7ff)/8);					//6轴缓存占用8个空间
				}
				else
					unitrate=1;
				
				laststa=0;
				
				for(;;)
				{
					OSSemPend(gMoveStopSEM,5,&err);
					read_fifo(&sta);
					
					sta=(sta&0x7ff)/8;

				//	Uart_Printf("time STOP:%d %5.3f\r\n",OSTimeGet(),sta*unitrate);
					
					vm_setrange1(sta*unitrate);
					
					if(FPGARATE<100)
						break;
				}
			}
			else					//探针等急停时采用缓存自然走完，不采用Stop1，防止Stop1造成的脉冲丢失
			{				
				reset_fifo();
				
				while(FPGARATE>0)
				{
					get_inp_status(&status);
					if(!status)break;
					OSTimeDly(1);
				}
			}
		
		
		
		//	Uart_Printf("VM STOP OVER\r\n");
		
			vm_getindex();										//在急停VM_STOP时将getindex定位到最后执行的位置
			
			reset_fifo();
			
			Stop1(Ax+1);
			Stop1(Ay+1);
			Stop1(Az+1);
			Stop1(Aa+1);
			Stop1(Ab+1);
			Stop1(Ac+1);

			for(;;)												//增加插补状态确认，避免最后一个脉冲因为倍率问题导致无法退出
			{
				SET_RANGE(8000);
				get_inp_status(&status);
				if(!status)break;
				OSTimeDly(1);
				
			}
			
			InitQueue(pMQ);
			InitQueue(pEPQ);
			mdata[INPA].Info.bDataTag=3;
			eventtag=0;
			vm_setrange1(1);
			continue;
		}
		
		if(SYS_State==VM_TAGSTOP)								//运动数据组停止标记，例如用于处理当条弹簧运行完退出，清空缓存
		{
			float 	rate,unitrate;
			int 	status;
			INT16U 	sta,laststa;
			
			Uart_Printf("VM_TAGSTOP ...\r\n");
			read_fifo(&sta);
			rate=RANGERATE1;
			
			if((sta&0x7ff)>0)
			{
			//	unitrate=rate/((sta&0x7ff)/6);
				unitrate=rate/((sta&0x7ff)/8);					//6轴缓存占用8个空间
			}
			else
				unitrate=1;
			
			laststa=0;
			
			for(;;)
			{
				OSSemPend(gMoveStopSEM,5,&err);
				event_proc(0,&mdata[INPA]);
				read_fifo(&sta);
				
				sta=(sta&0x7ff)/8;

			//	Uart_Printf("time TAGSTOP:%d %d\r\n",OSTimeGet(),sta);
				
				vm_setrange1(sta*unitrate);
			
				if(FPGARATE<100)
					SET_RANGE(100);
					
				get_inp_status(&status);
				
				if(!sta && !status)
					break;
					
				if(SYS_State==VM_STOP)
					break;
			}
			
			reset_fifo();
			sudden_stop(Ax+1);
			sudden_stop(Ay+1);
			sudden_stop(Az+1);
			sudden_stop(Aa+1);
			sudden_stop(Ab+1);
			sudden_stop(Ac+1);

			for(;;)
			{
				SET_RANGE(8000);
				OSTimeDly(1);
				get_inp_status(&status);
				if(!status)break;
			}

			event_end();

			InitQueue(pMQ);
			InitQueue(pEPQ);
			mdata[INPA].Info.bDataTag=3;
			eventtag=0;
			vm_setrange1(1);
			continue;
		}
		
	}
}


int vm_motion_init(void)
{
	volatile int i;

	debugout("vm_motion_init\r\n");

	memset(&VM,0x00,sizeof(VM));
	vm_efunc_reset();
	SYS_State=VM_IDLE;
	SYS_ErrNo=0;

	for(i=Ax;i<MAXAXIS;i++)
	{
		VM.Amax[i]=3000;
		VM.SpdMax[i]=100;
		SYS_vc(i)=20;
		SYS_vs(i)=0;
		SYS_ve(i)=0;
		SYS_acc(i)=50;
		SYS_acc(i)=50;
		SYS_admode(i)=1;
		SYS_MacPos[i]=0;
		SYS_RMacPos[i]=0;

		SYS_Rate2[i]=1.;
//		SYS_Gear[i]=1000.;
//		AxisGear[i]=1./1000.;
		SYS_CoderGear[i]=CALRATE;								//默认为脉冲一样
		CoderGear[i]=1./CALRATE;
		SYS_LogicPos[i]=0;
		fSyncTime[i]=0;
		SYS_MODEPULSEDIR[i]=1;
		SYS_MODECODERDIR[i]=1;
	}

	fSyncTime[INPA]=0;
	fSyncTime[INPB]=0;

	SYS_Rate1=1;
	SYS_Rate2[INPA]=1.;
	SYS_Rate2[INPB]=1.;

	SYS_admode(INPA)=1;
	SYS_admode(INPB)=1;
	SYS_vc(INPA)=20;
	SYS_vc(INPB)=20;
	SYS_vs(INPA)=0;
	SYS_vs(INPB)=0;
	SYS_ve(INPA)=0;
	SYS_ve(INPB)=0;
	SYS_acc(INPA)=500;
	SYS_acc(INPB)=500;
	SYS_acc(INPA)=500;
	SYS_acc(INPB)=500;
	VM.SpdMax[INPA]=100;
	VM.SpdMax[INPB]=100;
	SYS_Iterations=30;
	SYS_PreCodeBufs=0;
	
	SYS_RMacPos[0]=0;
	SYS_RMacPos[1]=0;
	SYS_RMacPos[2]=0;
	SYS_RMacPos[3]=0;
	SYS_RMacPos[4]=0;
	SYS_RMacPos[5]=0;

	MethodFlag = 1;					//初始化默认从轴不连续
	
	return 0;
}


int vm_efunc_reset(void)
{
	int i;
 	
	SYS_ErrNo=_ERR_NO_ERR;
	EH.EHCount=0;
	SYS_State=VM_IDLE;
	memset(ParamTemp,0x00,sizeof(ParamTemp));

	memset((void*)MPDataBuf,0x00,sizeof(MP)*MaxMParaPart);
	MotionQueue.QNode=MPDataBuf;
	pMQ=&MotionQueue;
	pMQ->MaxCount=MaxMParaPart;
	InitQueue(pMQ);

	memset((void*)EPDataBuf,0x00,sizeof(EPDataBuf));
	EPQueue.QNode=EPDataBuf;
	pEPQ=&EPQueue;
	pEPQ->MaxCount=MaxEParaPart;
	InitQueue(pEPQ);

	memset(mdata,0x00,sizeof(mdata));
	
	for(i=Ax;i<(MAXAXIS+MAXINPGROUP);i++)
		mdata[i].Info.bDataTag=3;
	MPCount=0;
	MPCOUNTOFFSET=0;
	return 0;
}


	/*********************************************
	函数名:       	vm_inp_move
	主要功能:    	？？？
	输入参数:       
					index---------- 索引值,必须从1开始算起，另外不能送两个连续相同的index，
									或者尽量不要在一组连续运动过程中送两个重复的index
			 		maxis----------主轴
			 		pos1,pos2------相对位置
					pos3,pos4------相对位置
					speed----------主轴运行速度
	输出参数:       无
	返回值:         无
	*********************************************/
	
int vm_inp_move(int index,int maxis,int pos1,int pos2,int pos3,int pos4,int speed)  //未用
{
	static 	float 	ftemp,ftemp2; 
	static 	int 	i,j;

	float 	relpos[4];
	MP 		Data;
	
	debugout("rel %d %d %d %d %d speed:%d\r\n",maxis,pos1,pos2,pos3,pos4,speed);
	
	OSSemPend(VM_PV,0,&err);
	
	if(maxis<0)
	{	
		goto INPASUC;
	}
	
	relpos[0]=pos1/CALRATE;
	relpos[1]=pos2/CALRATE;
	relpos[2]=pos3/CALRATE;
	relpos[3]=pos4/CALRATE;
	
	ftemp=0;
	j=0;
	
	for(i=Ax;i<MAXAXIS;i++)
	{	
		ftemp2=fabsf(relpos[i]);
		
		if(ftemp<ftemp2)
		{	j=i;
			ftemp=ftemp2;
		}
	}
	
	if(fabsf(ftemp)<0.001)
		goto INPASUC;
		
	if(fabsf(relpos[maxis])<0.001)
	{
		maxis=j;
	}
/*
	if(!relpos[maxis])
	{	SpeedPretreatment(NULL,pMQ);
		goto INPASUC;
	}
*/
//	memset(relpos,0x00,sizeof(relpos));
	
	memset(&Data,0x00,sizeof(Data));
	Data.maxis=maxis;
	Data.Info.index.Index=index;
	MPCount++;
	MPCount&=MAXMPCOUNT;
	Data.Info.index.QueueIndex=MPCount?MPCount:++MPCount;		//MPCount 永远不能为0，0是特殊处理的标记

	Data.vmax=speed/CALRATE;
	Data.Info.bDataTag=0;

	for(i=Ax;i<MAXAXIS;i++)
	{
		SYS_RMacPos[i]+=relpos[i];
		Data.axis[i].pos=SYS_RMacPos[i];
		Data.axis[i].relpos=relpos[i];
	}


	Data.pos=Data.axis[Data.maxis].pos;
	Data.relpos=Data.axis[Data.maxis].relpos;

	if(!SpeedPretreatment(&Data,pMQ))
	{
		EnQueue(sizeof(MP),&Data,pMQ);
	}
	else
		goto INPAERR;
		
INPASUC:
	OSSemPost(VM_PV);
	return SUCCESS;
	
INPAERR:
	OSSemPost(VM_PV);
	return FAIL;
}




	/*********************************************
	函数名:       	vm_abs_inp_move
	主要功能:    	？？？
	输入参数:       
					index---------- 索引值,必须从1开始算起，另外不能送两个连续相同的index，
									或者尽量不要在一组连续运动过程中送两个重复的index
			 		axismap--------映射的轴
			 		maxis----------主轴
			 		pos1,pos2------绝对位置
					pos3,pos4------绝对位置
					pos5,pos6------绝对位置

					speed------运行速度
	输出参数:       无
	返回值:         无
	*********************************************/
int vm_abs_inp_move(int index,int axismap,int maxis,INT32S pos1,INT32S pos2,INT32S pos3,INT32S pos4,INT32S pos5,INT32S pos6,INT32S speed)
{
	static 	float 	ftemp,ftemp2; 
	static 	int 	i,j;
	float 	pos[6]={0,0,0,0,0,0};
	MP 		Data;

//Uart_Printf("abs %d %d %d %d %d speed:%d\t",maxis,pos1,pos2,pos3,pos4,speed);

//Uart_Printf("qq=%d\r\n",pos1);
	
	OSSemPend(VM_PV,0,&err);
	
	if(maxis<0)
	{	
		goto INPASUC;
	}
	
	if(pos1>0)
		pos[Ax]=0.0005+pos1/CALRATE;

//	Uart_Printf("%f\r\n",pos[Ax]);
	
	if(pos1<0)
		pos[Ax]=-0.0005+pos1/CALRATE;

	if(pos2>0)
		pos[Ay]=0.0005+pos2/CALRATE;

	if(pos2<0)
		pos[Ay]=-0.0005+pos2/CALRATE;

	if(pos3>0)
		pos[Az]=0.0005+pos3/CALRATE;

	if(pos3<0)
		pos[Az]=-0.0005+pos3/CALRATE;

	if(pos4>0)
		pos[Aa]=0.0005+pos4/CALRATE;

	if(pos4<0)
		pos[Aa]=-0.0005+pos4/CALRATE;

	if(pos5>0)
		pos[Ab]=0.0005+pos5/CALRATE;

	if(pos5<0)
		pos[Ab]=-0.0005+pos5/CALRATE;

	if(pos6>0)
		pos[Ac]=0.0005+pos6/CALRATE;

	if(pos6<0)
		pos[Ac]=-0.0005+pos6/CALRATE;


	memset(&Data,0x00,sizeof(Data));
	Data.maxis=maxis;											//该段缓存插补的主轴标示
	Data.vmax=speed/CALRATE;									//该段缓存插补的速度单位1000脉冲
	Data.Info.bDataTag=0;

	for(i=Ax;i<MAXAXIS;i++)
	{
		if(axismap & (0x01<<i))
		{
			Data.axis[i].pos=pos[i];							//存放该段缓存插补绝对目标位置						
			Data.axis[i].relpos=pos[i]-SYS_RMacPos[i];			//存放当前坐标到缓存插补绝对目标位置的距离
			SYS_RMacPos[i]=pos[i];
		}
		else
		{
			Data.axis[i].pos=SYS_RMacPos[i];
			Data.axis[i].relpos=0;
		}
	}

	ftemp=0;
	j=0;

	for(i=Ax;i<MAXAXIS;i++)										//求该段缓存插补最大移动脉冲轴和最大移动轴的脉冲
	{
		ftemp2=fabsf(Data.axis[i].relpos);
	
		if(ftemp2>ftemp)
		{	
			j=i;
			ftemp=ftemp2;
		}
	}
	
	if(fabsf(ftemp)<0.001)
		goto INPASUC;
		
	if(fabsf(Data.axis[maxis].relpos)<0.001)
	{
		maxis=j;
		Data.maxis=maxis;
	}
	
	//Uart_Printf("%5.3f %5.3f %5.3f \r\n",Data.axis[Ax].relpos,Data.axis[Ay].relpos,Data.axis[Az].relpos);
	Data.pos=Data.axis[Data.maxis].pos;
	Data.relpos=Data.axis[Data.maxis].relpos;
	Data.Info.index.Index=index;

	MPCount++;
	MPCount&=MAXMPCOUNT;
	Data.Info.index.QueueIndex=MPCount?MPCount:++MPCount;		//MPCount 永远不能为0，0是特殊处理的标记

	if(!SpeedPretreatment(&Data,pMQ))
	{
		EnQueue(sizeof(MP),&Data,pMQ);

		if(MethodFlag==0)
		{
			FollowSpeedPretreatment(pMQ);
		}
	}
	else
		goto INPAERR;
	
INPASUC:
	OSSemPost(VM_PV);
	return SUCCESS;

INPAERR:
	OSSemPost(VM_PV);
	return FAIL;
}


int vm_start(void)
{
	OSSemPend(VM_PV,0,&err);
	
	if(SYS_State!=VM_IDLE)
		goto Err;

//	SpeedPretreatment(NULL,pMQ);
//	Uart_Printf("testpoint5\n");

	OSSemPost(VM_START);
//	Uart_Printf("testpoint6\n");

Err:
	OSSemPost(VM_PV);
	return SUCCESS;
}


int vm_stop(int mode)
{
	int i=0;

	#if OS_CRITICAL_METHOD == 3                      							/* Allocate storage for CPU status register           */
    	OS_CPU_SR  cpu_sr;
	    cpu_sr = 0;                                 	 						/* Prevent compiler warning                           */
	#endif  
	
	g_iStopMode = mode;
	
	OS_ENTER_CRITICAL();
	
	if(SYS_State==VM_RUN  || SYS_State==VM_TAGSTOPCMD)
	{
		SYS_State=VM_STOP;
		OS_EXIT_CRITICAL();
		
		for(;;)
		{
			OSTimeDly(1);
			
			i++;
			
			if(i>1000 )
			{
				Uart_Printf("vm_stop timeout!!\n ");
				SYS_State=VM_IDLE;
				return FAIL;
			}


			if(SYS_State==VM_IDLE)break;

		}
	}
	else
		OS_EXIT_CRITICAL();

	return 0;
}


int vm_tag_stop(void)
{
	OSSemPend(VM_PV,0,&err);
	Uart_Printf("vm_tag_stop\r\n");
	
	if(SYS_State==VM_RUN)
	{
		SYS_State=VM_TAGSTOPCMD;
		
		/*		
		for(;;)
		{
			OSTimeDly(1);
			if(SYS_State==VM_IDLE)break;
		}
		*/
	}
	
	OSSemPost(VM_PV);
	
	return SUCCESS;
}


int vm_reset(void)
{
	OSSemPend(VM_PV,0,&err);
	vm_efunc_reset();
	OSSemPost(VM_PV);

	return SUCCESS;
}


int vm_set_io(int port,int level)
{
	EP d;
	MP *pMP;
	
//	Uart_Printf("port=%d,level=%d\n",port,level);
	
	OSSemPend(VM_PV,0,&err);
	
	pMP = GetMPDataIn(0,pMQ);    								//获取队列尾数据
	memset(&d,0x00,sizeof(d));
	d.Type=_T_GPIO;
	d.Data.Gpio.ioport =port;
	d.Data.Gpio.iolevel=level;	
	if(!pMP)
		d.QueueIndex=0;
	else
		d.QueueIndex=pMP->Info.index.QueueIndex;

	EnQueue(sizeof(EP),&d,pEPQ);
	OSSemPost(VM_PV);
	
	return SUCCESS;
}


int vm_set_probe(int pro,int port)
{
	EP d;
	MP *pMP;

	OSSemPend(VM_PV,0,&err);
	
	pMP = GetMPDataIn(0,pMQ);    								//获取队列尾数据
	memset(&d,0x00,sizeof(d));
	d.Type=_T_PROBE;
	d.Data.Probe.pro =pro;
	d.Data.Probe.port=port;	
	
	if(!pMP)
		d.QueueIndex=0;
	else
		d.QueueIndex=pMP->Info.index.QueueIndex;

	EnQueue(sizeof(EP),&d,pEPQ);
	OSSemPost(VM_PV);
	
	return SUCCESS;
	
}


int vm_set_position(int axis,int pos)
{
	EP d;
	MP *pMP;
	
	if(axis<Ax || axis>Ac)return FAIL;
	
	OSSemPend(VM_PV,0,&err);
	pMP = GetMPDataIn(0,pMQ);    								//获取队列尾数据
	memset(&d,0x00,sizeof(d));
	d.Type=_T_POSSET;
	d.Data.Posset.axis =axis;
	d.Data.Posset.pos=pos;	
	
	if(!pMP)
	{
		d.QueueIndex=0;
		
		BaseSetCommandPos(axis+1,pos);
		BaseGetCommandPos(axis+1,&lastpulse[axis]);
		SYS_RMacPos[axis]=lastpulse[axis]/1000.;
	//	Uart_Printf("v_s_p MPCot=%d\n",MPCount);

		goto VM_SET_POS_END;
	}
	else
		d.QueueIndex=pMP->Info.index.QueueIndex;

	EnQueue(sizeof(EP),&d,pEPQ);
//	Uart_Printf("@%d %d\r\n",axis,pos);
	SYS_RMacPos[axis]=pos/CALRATE;
	
VM_SET_POS_END:	

	OSSemPost(VM_PV);
	
	return SUCCESS;
}


int vm_set_tag(int tag)
{
	EP d;
	MP *pMP;
	
	OSSemPend(VM_PV,0,&err);
	pMP = GetMPDataIn(0,pMQ);    								//获取队列尾数据
	memset(&d,0x00,sizeof(d));
	d.Type=_T_TAGSET;
	d.Data.Tagset.tag=tag;	
	
	if(!pMP)
	{
		d.QueueIndex=0;
		CurTag=tag;
		goto VM_SET_TAG_ENG;
	}
	else
		d.QueueIndex=pMP->Info.index.QueueIndex;

	EnQueue(sizeof(EP),&d,pEPQ);
	
VM_SET_TAG_ENG:	
//	Uart_Printf("vm_set_tag =%ld\n",tag);
	OSSemPost(VM_PV);
	return SUCCESS;
	
}


int vm_get_status(void)
{
	INT16U 	value;
	int 	status=-1;
	
	OSSemPend(VM_PV,0,&err);
	read_fifo(&value);
	value&=0x7ff;
	get_inp_status(&status);

	if (pMQ->QueueCount==0 && value==0 && !status)	
		status= 0;
	else
		status= 1;
		
	OSSemPost(VM_PV);
	return status;
}


int vm_getindex(void)
{
	INT16U 	count;
	INT8U 	err;
	int 	sta;

	OSSemPend(VM_PV,0,&err);
	read_fifo(&count);
	count&=0x7ff;

	if(count==0)
	{
		get_inp_status(&sta);

		if(sta==0)
		{
			FIFOCOUNT=0;
			goto Err;
		}

		count=1;
	}

//	CurIndex=FIFOBUF[(FIFOCOUNT-count)&0x7ff].Index;
	CurIndex=FIFOBUF[(FIFOCOUNT-count-1)&0x7ff].Index;

Err:

	OSSemPost(VM_PV);
	return CurIndex;
	
}


int vm_getqueueindex(void)
{
	INT16U 	count;
	INT8U 	err;
	int 	sta;

	OSSemPend(VM_PV,0,&err);

	read_fifo(&count);
	count&=0x7ff;

	if(count==0)
	{
		get_inp_status(&sta);

		if(sta==0)
		{
			FIFOCOUNT=0;
			CurQueueIndex=0;
			goto Err;
		}

		count=1;
	}

	CurQueueIndex=FIFOBUF[(FIFOCOUNT-count)&0x7ff].QueueIndex;
Err:
	OSSemPost(VM_PV);
	return CurQueueIndex;
}


int vm_setindex(TIT index)
{
	int count;

	OSSemPend(VM_PV,0,&err);

//	FIFOCOUNT=FIFOCOUNT+4+2;									//四轴插补一条指令的缓存空间是6个，六轴插补是8个
	FIFOCOUNT=FIFOCOUNT+8;										//四轴插补一条指令的缓存空间是6个，六轴插补是8个

//	for(count=4+2;count>0;count--)
	for(count=8;count>0;count--)
	{
//		Uart_Printf("\t%d\n",(FIFOCOUNT-count+1)&0x7ff);
		FIFOBUF[(FIFOCOUNT-count)&0x7ff]=index;
	}

	OSSemPost(VM_PV);
	return SUCCESS;
}


int vm_setrange1(float rate)
{
//	OSSemPend(VM_PV,0,&err);
	RANGERATE1=rate;
	FPGARATE=8000*RANGERATE1*RANGERATE2;
	SET_RANGE(FPGARATE);
//	OSSemPost(VM_PV);

	return 0;
}


int vm_setrange2(int rate)
{
//	OSSemPend(VM_PV,0,&err);
	RANGERATE2=rate/8000.;
	FPGARATE=8000*RANGERATE1*RANGERATE2;
	SET_RANGE(FPGARATE);
//	OSSemPost(VM_PV);
	return 0;
}
