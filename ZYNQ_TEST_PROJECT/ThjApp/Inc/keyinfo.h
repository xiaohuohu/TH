#ifndef	____KEYINFO_H___
#define	____KEYINFO_H___

#include	"key.h"

// ********基出键值定义******************************  //



# define	NULL_KEY	0
# define	EMPTYKEY	-1

//#ifdef  CNC835

#define ENTER			0x0d
#define ESC 			0x1b
#define SPACE			0x20
#define TAB 			0x09
#define SHIFT_TAB		0x10f
#define BACK			0x08

#define UP				0x148
#define DOWN			0x150
#define LEFT			0x14b
#define RIGHT 			0x14d
#define INSERT			0x152
#define DEL				0x153
#define HOME			0x147
#define END				0x14f
#define PAGEUP			0x149
#define PAGEDOWN		0x151

#define CTRL_HOME		0x177
#define CTRL_END		0x175
#define CTRL_PAGEUP	    0x184
#define CTRL_PAGEDOWN	0x176

#define RIGHT_SHIFT		0x01
#define LEFT_SHIFT		0x02
#define CTRL			0x04
#define ALT				0x08

#define	CAPS_LUCK		0x10
#define SCROLL_LUCK		0x20
#define NUM_LUCK		0x40
#define INS				0x80


#define F1				0x13b
#define F2				0x13c
#define F3				0x13d
#define F4				0x13e
#define F5				0x13f
#define F6				0x140
#define F7				0x141
#define F8				0x142
#define F9				0x143
#define F10				0x144
#define	F11				0x185
#define	F12				0x186
#define ALT_Q			0x110
#define ALT_W			0x111
#define ALT_E           0x112
#define ALT_R           0x113
#define ALT_T           0x114
#define ALT_Y           0x115
#define ALT_U           0x116
#define ALT_I           0x117
#define ALT_O           0x118
#define ALT_P           0x119

#define ALT_A			0x11e
#define ALT_S			0x11f
#define ALT_D           0x120
#define ALT_F           0x121
#define ALT_G           0x122
#define ALT_H           0x123
#define ALT_J           0x124
#define ALT_K           0x125
#define ALT_L           0x126

#define ALT_Z           0x12c
#define ALT_X			0x12d

#define ALT_C			0x12e
#define ALT_V           0x12f
#define ALT_B           0x130
#define ALT_N           0x131
#define ALT_M           0x132

#define CTRL_A			0x01
#define CTRL_B			0x02
#define CTRL_C			0x03
#define CTRL_D			0x04
#define CTRL_E			0x05
#define CTRL_F			0x06
#define CTRL_G			0x07

#define CTRL_H			0x08
#define CTRL_I			0x09
#define CTRL_J			0x0a
#define CTRL_K			0x0b
#define CTRL_L			0x0c
#define CTRL_M			0x0d
#define CTRL_N			0x0e

#define CTRL_O			0x0f
#define CTRL_P			0x10
#define CTRL_Q			0x11

#define CTRL_R			0x12
#define CTRL_S			0x13
#define CTRL_T			0x14

#define CTRL_U			0x15
#define CTRL_V			0x16
#define CTRL_W			0x17

#define CTRL_X			0x18
#define CTRL_Y			0x19
#define CTRL_Z			0x1a

#define ALT_1           0x178
#define ALT_2           0x179
#define ALT_3           0x17a
#define ALT_4           0x17b
#define ALT_5           0x17c
#define ALT_6           0x17d
#define ALT_7           0x17e
#define ALT_8           0x17f
#define ALT_9           0x180
#define ALT_0           0x181

#define SHIFT_F1		0x154
#define SHIFT_F2		0x155
#define SHIFT_F3		0x156
#define SHIFT_F4		0x157
#define SHIFT_F5		0x158
#define SHIFT_F6		0x159
#define SHIFT_F7		0x15a
#define SHIFT_F8		0x15b
#define SHIFT_F9		0x15c
#define SHIFT_F10		0x15d

#define CTRL_F1			0x15e
#define CTRL_F2			0x15f
#define CTRL_F3			0x160
#define CTRL_F4			0x161
#define CTRL_F5			0x162
#define CTRL_F6			0x163
#define CTRL_F7			0x164
#define CTRL_F8			0x165
#define CTRL_F9			0x166
#define CTRL_F10		0x167

#define ALT_F1			0x168
#define ALT_F2			0x169
#define ALT_F3			0x16a
#define ALT_F4			0x16b
#define ALT_F5			0x16c
#define ALT_F6			0x16d
#define ALT_F7			0x16e
#define ALT_F8			0x16f
#define ALT_F9			0x170
#define ALT_F10			0x171

//#endif






// ***********控制器键值定义 *******************  //



//#ifdef		CNC835

# define	THOME		0XFF+1
# define	TEND		0XFF+2
//# define	TCANCEL			'#'

# define	TBACKZERO		'x'
# define	TYSWITCH		'y'
# define	TZSWITCH		'z'
# define	TUSWITCH		'u'
# define	TVSWITCH		'v'
# define	TWSWITCH		'w'
# define	TCYLINDER  		'd'			//气缸
# define	TSTOP			' '
# define	TTEST 			'a'

# define	TREAD			'r'
# define    TSCOMMAND       's'
# define    TGCOMMAND       'g'
# define    TMCOMMAND       'm'
//# define	TNCOMMAND		'f'


# define    TPROBECLR       'c'
# define	TPROBESET    	'k'
# define    TJCOMMAND       'j' 
# define	TECOMMAND       'e'
# define	TCANCEL			'o'			//取消

# define	TCOUNTCLR		'b'
# define	TCOUNTSET		'i'
# define	TINSERT			INSERT
# define	TLCOMMAND		'l'
# define    TMINUS			'-'

# define	TSENDCLR    	'q'
# define	TWORKPARA     	'p'
# define	TDEL			DEL
# define	TNCOMMAND		'n'
# define	TUP				UP
# define	TDOT			'.'

# define	TENTER			ENTER

# define	TBEILV   		't'
# define	TSTEPSPEED		';'
# define	TDELALL	','
# define	TSYSMANGE		'='				//系统管理
# define	TDOWN			DOWN
# define	TBACKSPACE		'h'
# define	TLEFT			LEFT
# define	TRIGHT			RIGHT

# define	TXPLUS			F1
# define	TYPLUS			F3
# define	TZPLUS			F5
# define	TUPLUS			F7
# define	TVPLUS			F9
# define	TWPLUS			F11
# define	TPAGEDOWN       ']'          //下页       TCLEAR			0x5b
# define    TPAGEUP         0x5b
# define	TSTARTINPUT		HOME
# define	TENDINPUT		END


# define	TXMINUS			F2
# define	TYMINUS			F4
# define	TZMINUS			F6
# define	TUMINUS			F8
# define	TVMINUS			F10
# define	TWMINUS			F12

# define    THANDBOX        '\''
# define	THANDSWITCH		ESC
# define	TSAVE			0x66

//#endif




#endif
