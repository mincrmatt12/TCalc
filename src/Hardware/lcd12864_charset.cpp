#include "lcd12864_charset.hpp"

namespace lcd {
	const Img * const CHAR_ASCII[] = {
		&CHAR_CRS,
		&CHAR_AGV,
		&CHAR_AVGO,
		&CHAR_ECHG,
		&CHAR_VLIG,
		&CHAR_EE,
		&CHAR_EULR,
		&CHAR_THETA,
		&CHAR_SERR,
		&CHAR_PI,
		&CHAR_MUL, 
		&CHAR_DIV,
		&CHAR_SPC,
		&CHAR_EXMK,
		&CHAR_UNKNOWN,//&CHAR_DBLQ,
		&CHAR_UNKNOWN,//&CHAR_NSGN,
		&CHAR_UNKNOWN,//&CHAR_DLR,
		&CHAR_PCT,
		&CHAR_UNKNOWN,//&CHAR_AND,
		&CHAR_UNKNOWN,//&CHAR_SGLQ,
		&CHAR_LBKT,
		&CHAR_RBKT,
		&CHAR_UNKNOWN,//&CHAR_ASTK,
		&CHAR_PLUS,
		&CHAR_CMMA,
		&CHAR_MNUS,
		&CHAR_DOT,
		&CHAR_UNKNOWN,//&CHAR_SLSH,
		&CHAR_0,
		&CHAR_1,
		&CHAR_2,
		&CHAR_3,
		&CHAR_4,
		&CHAR_5,
		&CHAR_6,
		&CHAR_7,
		&CHAR_8,
		&CHAR_9,
		&CHAR_CLN,
		&CHAR_UNKNOWN,//&CHAR_SMCN,
		&CHAR_LSTN,
		&CHAR_EQL,
		&CHAR_GTTN,
		&CHAR_UNKNOWN,//&CHAR_QNMK,
		&CHAR_UNKNOWN,//&CHAR_ATSB,
		&CHAR_A,
		&CHAR_B,
		&CHAR_C,
		&CHAR_D,
		&CHAR_E,
		&CHAR_F,
		&CHAR_G,
		&CHAR_H,
		&CHAR_I,
		&CHAR_J,
		&CHAR_K,
		&CHAR_L,
		&CHAR_M,
		&CHAR_N,
		&CHAR_O,
		&CHAR_P,
		&CHAR_Q,
		&CHAR_R,
		&CHAR_S,
		&CHAR_T,
		&CHAR_U,
		&CHAR_V,
		&CHAR_W,
		&CHAR_X,
		&CHAR_Y,
		&CHAR_Z,
		&CHAR_LSQB,
		&CHAR_BKSL,
		&CHAR_RSQB,
		&CHAR_CART,
		&CHAR_USCR,
		&CHAR_BKTK,
		&CHAR_LCA,
		&CHAR_LCB,
		&CHAR_LCC,
		&CHAR_LCD,
		&CHAR_LCE,
		&CHAR_LCF,
		&CHAR_LCG,
		&CHAR_LCH,
		&CHAR_LCI,
		&CHAR_LCJ,
		&CHAR_LCK,
		&CHAR_LCL,
		&CHAR_LCM,
		&CHAR_LCN,
		&CHAR_LCO,
		&CHAR_LCP,
		&CHAR_LCQ,
		&CHAR_LCR,
		&CHAR_LCS,
		&CHAR_LCT,
		&CHAR_LCU,
		&CHAR_LCV,
		&CHAR_LCW,
		&CHAR_LCX,
		&CHAR_LCY,
		&CHAR_LCZ,
		&CHAR_LCVB,
		&CHAR_PIPE,
		&CHAR_RCVB,
		&CHAR_TLDE,
		&CHAR_UNKNOWN,
	};

	
	uint8_t asciiToIndex(char c) {
		// If char is outside the range, return 0x7F (CHAR_UNKNOWN) minus the offset
		if(c < LCD_CHARSET_LOWBOUND || c >= 0x7F) {
			return 0x7F - LCD_CHARSET_LOWBOUND;
		}
		// Otherwise subtract the offset
		return c - LCD_CHARSET_LOWBOUND;
	}
	const Img& getChar(char c) {
		return *CHAR_ASCII[asciiToIndex(c)];
	}
}
