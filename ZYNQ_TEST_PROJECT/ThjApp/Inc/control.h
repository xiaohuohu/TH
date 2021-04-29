# ifndef __CONTROL_STRUCT___
# define __CONTROL_STRUCT___
////////////////////////按钮的所有数据结构///////////////
# define CONTROL_MAX 24	//最大按钮数

///按钮上的文字显示方式///////////////
/*fzz 07/24/2004更新*/
# define LEFT_ALGIN			0
# define MIDE_ALGIN			1
# define CENTER_ALGIN		1
# define RIGHT_ALGIN		2

# define	CTROL_STYLE_EMPTY			0
# define 	CTROL_STYLE_BUTTON			1		/*正常*/
# define 	CTROL_STYLE_EDIT			2		/*编辑*/
# define	CTROL_STYLE_STATIC			3		/*静态*/
# define 	CTROL_STYLE_LISTEDIT		4
# define	CTROL_STYLE_DESKTOP			5
# define	CTROL_STYLE_STATICLINK		6
typedef struct CONTROL
{
	INT16U 	style;		/*按钮类型*/
	INT16U	ID;				/*ID标号*/
	INT8S	name[13];           /*名称*/
	INT16S	x,y;		/*位置*/
	INT16S	w,h;
	INT16U 	key;
	INT8U   algin;		/*对齐方式*/
}CTROL;

typedef enum
{
	Reset=0,Active=1,Draw=2,Normal=3,Foucs=4,UnFoucs=5,MoveActive=6
}DRAWSTATUS;
//typedef enum { Reset=0,Active=1,Draw=2} STATUS;
# endif