#include "includes.h"
#include "faceColorStyle.h"

extern BOOL g_gridStyle; 
	
FACE_CLR_STYLE FCS;

void SetFCS(INT16S mode)
{
	switch(mode)
	{
	case 1:
		FCS.fcsTextColor1 = LIGHTWHITECOLOR;
		FCS.fcsTextColor2 = YELLOWCOLOR;
		FCS.fcsTextColor3 = BLACKCOLOR;//MAGENTACOLOR;
		FCS.fcsTextColor4 = CYANCOLOR; 
		FCS.fcsTextColor5 = BLACKCOLOR;
		FCS.fcsTextColor6 = WHITECOLOR;
		FCS.fcsTextColor7 = DARKGRAYCOLOR;
		FCS.fcsTextColor8 = LIGHTGRAYCOLOR;
		FCS.fcsTextColor9 = BLUECOLOR;
		FCS.fcsTextColor10= LIGHTGRAYCOLOR;
		FCS.fcsTextColor11= YELLOWCOLOR;
		FCS.fcsColumnBk   = DARKBROWNCOLOR;
		FCS.fcsTableLine  = DARKGREENCOLOR2;
		FCS.fcsTechfrmBk  = DARKGREENCOLOR4;
		FCS.fcsTitleBk	  = LIGHTREDCOLOR;//CYANCOLOR; 
		FCS.fcsTableBk	  = SKYBLUECOLOR; 
		
		g_gridStyle=TRUE;
		break;
	case 2:
		FCS.fcsTextColor1 = LIGHTWHITECOLOR;
		FCS.fcsTextColor2 = YELLOWCOLOR;
		FCS.fcsTextColor3 = BLACKCOLOR;//MAGENTACOLOR;
		FCS.fcsTextColor4 = CYANCOLOR; 
		FCS.fcsTextColor5 = REDCOLOR;
		FCS.fcsTextColor6 = WHITECOLOR;
		FCS.fcsTextColor7 = DARKGRAYCOLOR;
		FCS.fcsTextColor8 = LIGHTGRAYCOLOR;
		FCS.fcsTextColor9 = BLUECOLOR;
		FCS.fcsTextColor10= LIGHTGRAYCOLOR;
		FCS.fcsTextColor11= GREENCOLOR;
		FCS.fcsColumnBk   = BLACKCOLOR;
		FCS.fcsTableLine  = GRAYCOLOR;
		FCS.fcsTechfrmBk  = BLACKCOLOR;
		FCS.fcsTitleBk	  = DARKBLUECOLOR;//CYANCOLOR; 
		FCS.fcsTableBk	  = BLACKCOLOR; 
		
		g_gridStyle=FALSE;
		break;
	}

	
	
	 g_TextColor1 = FCS.fcsTextColor1;
	 g_TextColor2 = FCS.fcsTextColor2;
	 g_TextColor3 = FCS.fcsTextColor3;
	 g_TextColor4 = FCS.fcsColumnBk;
	 g_TextColor5 = FCS.fcsTextColor5;
	 g_TextColor6 = FCS.fcsTextColor6;
	 g_TextColor7 = FCS.fcsTextColor7;
	 g_TextColor8 = FCS.fcsTextColor8;
	 g_TextColor9 = FCS.fcsTextColor9;
	 g_TextColor10= FCS.fcsTextColor10;
	 g_TextColor11= FCS.fcsTextColor11;
	
	 g_TableLine  = FCS.fcsTableLine ;
	 g_nBk		  = FCS.fcsTechfrmBk;
	 g_nTechfrmBk = FCS.fcsTechfrmBk;
	 g_nTitleBk	  = FCS.fcsTitleBk;//CYANCOLOR; 
	 g_nTableBk	  = FCS.fcsTableBk; 


}

