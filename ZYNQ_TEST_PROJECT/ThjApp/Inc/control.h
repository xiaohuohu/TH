# ifndef __CONTROL_STRUCT___
# define __CONTROL_STRUCT___
////////////////////////��ť���������ݽṹ///////////////
# define CONTROL_MAX 24	//���ť��

///��ť�ϵ�������ʾ��ʽ///////////////
/*fzz 07/24/2004����*/
# define LEFT_ALGIN			0
# define MIDE_ALGIN			1
# define CENTER_ALGIN		1
# define RIGHT_ALGIN		2

# define	CTROL_STYLE_EMPTY			0
# define 	CTROL_STYLE_BUTTON			1		/*����*/
# define 	CTROL_STYLE_EDIT			2		/*�༭*/
# define	CTROL_STYLE_STATIC			3		/*��̬*/
# define 	CTROL_STYLE_LISTEDIT		4
# define	CTROL_STYLE_DESKTOP			5
# define	CTROL_STYLE_STATICLINK		6
typedef struct CONTROL
{
	INT16U 	style;		/*��ť����*/
	INT16U	ID;				/*ID���*/
	INT8S	name[13];           /*����*/
	INT16S	x,y;		/*λ��*/
	INT16S	w,h;
	INT16U 	key;
	INT8U   algin;		/*���뷽ʽ*/
}CTROL;

typedef enum
{
	Reset=0,Active=1,Draw=2,Normal=3,Foucs=4,UnFoucs=5,MoveActive=6
}DRAWSTATUS;
//typedef enum { Reset=0,Active=1,Draw=2} STATUS;
# endif