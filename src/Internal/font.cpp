#include "lcd12864_charset.hpp"

namespace lcd {
    /* NUMBERS */
    const uint8_t CHAR_0_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x98,
        0xa8,
        0xc8,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_0(CHAR_0_DATA, 1, 5, 9);
    const uint8_t CHAR_1_DATA[] = {
        0x20,
        0x60,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x70,
    };
    const Img CHAR_1(CHAR_1_DATA, 1, 5, 9);
    const uint8_t CHAR_2_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x08,
        0x10,
        0x20,
        0x40,
        0x80,
        0xf8,
    };
    const Img CHAR_2(CHAR_2_DATA, 1, 5, 9);
    const uint8_t CHAR_3_DATA[] = {
        0x70,
        0x88,
        0x08,
        0x08,
        0x70,
        0x08,
        0x08,
        0x88,
        0x70,
    };
    const Img CHAR_3(CHAR_3_DATA, 1, 5, 9);
    const uint8_t CHAR_4_DATA[] = {
        0x10,
        0x30,
        0x50,
        0x50,
        0x90,
        0xf8,
        0x10,
        0x10,
        0x10,
    };
    const Img CHAR_4(CHAR_4_DATA, 1, 5, 9);
    const uint8_t CHAR_5_DATA[] = {
        0xf8,
        0x80,
        0x80,
        0xf0,
        0x88,
        0x08,
        0x08,
        0x88,
        0x70,
    };
    const Img CHAR_5(CHAR_5_DATA, 1, 5, 9);
    const uint8_t CHAR_6_DATA[] = {
        0x70,
        0x88,
        0x80,
        0x80,
        0xf0,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_6(CHAR_6_DATA, 1, 5, 9);
    const uint8_t CHAR_7_DATA[] = {
        0xf8,
        0x08,
        0x08,
        0x10,
        0x10,
        0x20,
        0x20,
        0x20,
        0x20,
    };
    const Img CHAR_7(CHAR_7_DATA, 1, 5, 9);
    const uint8_t CHAR_8_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x88,
        0x70,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_8(CHAR_8_DATA, 1, 5, 9);
    const uint8_t CHAR_9_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x88,
        0x78,
        0x08,
        0x08,
        0x88,
        0x70,
    };
    const Img CHAR_9(CHAR_9_DATA, 1, 5, 9);

    /* ASCII 32-126 */
    const uint8_t CHAR_SPC_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_SPC(CHAR_SPC_DATA, 1, 5, 9);
    const uint8_t CHAR_EXMK_DATA[] = {
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x00,
        0x00,
        0x20,
    };
    const Img CHAR_EXMK(CHAR_EXMK_DATA, 1, 5, 9);
    // const uint8_t CHAR_DBLQ_DATA[] = {
    //     0x50,
    //     0x50,
    //     0x50,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    // };
    // const Img CHAR_DBLQ(CHAR_DBLQ_DATA, 1, 5, 9);
    // const uint8_t CHAR_NSGN_DATA[] = {
    //     0x50,
    //     0x50,
    //     0xf8,
    //     0x50,
    //     0x50,
    //     0x50,
    //     0xf8,
    //     0x50,
    //     0x50,
    // };
    // const Img CHAR_NSGN(CHAR_NSGN_DATA, 1, 5, 9);
    // const uint8_t CHAR_DLR_DATA[] = {
    //     0x20,
    //     0x70,
    //     0xa8,
    //     0xa0,
    //     0x70,
    //     0x28,
    //     0xa8,
    //     0x70,
    //     0x20,
    // };
    // const Img CHAR_DLR(CHAR_DLR_DATA, 1, 5, 9);
    const uint8_t CHAR_PCT_DATA[] = {
        0x08,
        0xc8,
        0xd0,
        0x10,
        0x20,
        0x20,
        0x40,
        0x58,
        0x98,
    };
    const Img CHAR_PCT(CHAR_PCT_DATA, 1, 5, 9);
    // const uint8_t CHAR_AND_DATA[] = {
    //     0x60,
    //     0x90,
    //     0x90,
    //     0x90,
    //     0x60,
    //     0xa8,
    //     0x90,
    //     0x90,
    //     0x68,
    // };
    // const Img CHAR_AND(CHAR_AND_DATA, 1, 5, 9);
    // const uint8_t CHAR_SGLQ_DATA[] = {
    //     0x20,
    //     0x20,
    //     0x20,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    // };
    // const Img CHAR_SGLQ(CHAR_SGLQ_DATA, 1, 5, 9);
    const uint8_t CHAR_LBKT_DATA[] = {
        0x10,
        0x20,
        0x40,
        0x40,
        0x40,
        0x40,
        0x40,
        0x20,
        0x10,
    };
    const Img CHAR_LBKT(CHAR_LBKT_DATA, 1, 5, 9);
    const uint8_t CHAR_RBKT_DATA[] = {
        0x40,
        0x20,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x20,
        0x40,
    };
    const Img CHAR_RBKT(CHAR_RBKT_DATA, 1, 5, 9);
    // const uint8_t CHAR_ASTK_DATA[] = {
    //     0x00,
    //     0x20,
    //     0xa8,
    //     0x70,
    //     0xa8,
    //     0x20,
    //     0x00,
    //     0x00,
    //     0x00,
    // };
    // const Img CHAR_ASTK(CHAR_ASTK_DATA, 1, 5, 9);
    const uint8_t CHAR_PLUS_DATA[] = {
        0x00,
        0x00,
        0x20,
        0x20,
        0xf8,
        0x20,
        0x20,
        0x00,
        0x00,
    };
    const Img CHAR_PLUS(CHAR_PLUS_DATA, 1, 5, 9);
    const uint8_t CHAR_CMMA_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x60,
        0x60,
        0x20,
        0x40,
    };
    const Img CHAR_CMMA(CHAR_CMMA_DATA, 1, 5, 9);
    const uint8_t CHAR_MNUS_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0xf8,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_MNUS(CHAR_MNUS_DATA, 1, 5, 9);
    const uint8_t CHAR_DOT_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x60,
        0x60,
    };
    const Img CHAR_DOT(CHAR_DOT_DATA, 1, 5, 9);
    // const uint8_t CHAR_SLSH_DATA[] = {
    //     0x08,
    //     0x08,
    //     0x10,
    //     0x10,
    //     0x20,
    //     0x20,
    //     0x40,
    //     0x40,
    //     0x80,
    // };
    // const Img CHAR_SLSH(CHAR_SLSH_DATA, 1, 5, 9);
    const uint8_t CHAR_CLN_DATA[] = {
        0x00,
        0x60,
        0x60,
        0x00,
        0x00,
        0x60,
        0x60,
        0x00,
        0x00,
    };
    const Img CHAR_CLN(CHAR_CLN_DATA, 1, 5, 9);
    // const uint8_t CHAR_SMCN_DATA[] = {
    //     0x00,
    //     0x60,
    //     0x60,
    //     0x00,
    //     0x00,
    //     0x60,
    //     0x60,
    //     0x20,
    //     0x40,
    // };
    // const Img CHAR_SMCN(CHAR_SMCN_DATA, 1, 5, 9);
    const uint8_t CHAR_LSTN_DATA[] = {
        0x08,
        0x10,
        0x20,
        0x40,
        0x80,
        0x40,
        0x20,
        0x10,
        0x08,
    };
    const Img CHAR_LSTN(CHAR_LSTN_DATA, 1, 5, 9);
    const uint8_t CHAR_EQL_DATA[] = {
        0x00,
        0x00,
        0x00,
        0xf8,
        0x00,
        0xf8,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_EQL(CHAR_EQL_DATA, 1, 5, 9);
    const uint8_t CHAR_GTTN_DATA[] = {
        0x80,
        0x40,
        0x20,
        0x10,
        0x08,
        0x10,
        0x20,
        0x40,
        0x80,
    };
    const Img CHAR_GTTN(CHAR_GTTN_DATA, 1, 5, 9);
    // const uint8_t CHAR_QNMK_DATA[] = {
    //     0x70,
    //     0x88,
    //     0x88,
    //     0x08,
    //     0x10,
    //     0x20,
    //     0x20,
    //     0x00,
    //     0x20,
    // };
    // const Img CHAR_QNMK(CHAR_QNMK_DATA, 1, 5, 9);
    // const uint8_t CHAR_ATSB_DATA[] = {
    //     0x70,
    //     0x88,
    //     0x88,
    //     0x08,
    //     0x68,
    //     0xa8,
    //     0xa8,
    //     0xa8,
    //     0x70,
    // };
    // const Img CHAR_ATSB(CHAR_ATSB_DATA, 1, 5, 9);
    const uint8_t CHAR_A_DATA[] = {
        0x20,
        0x50,
        0x50,
        0x88,
        0x88,
        0xf8,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_A(CHAR_A_DATA, 1, 5, 9);
    const uint8_t CHAR_B_DATA[] = {
        0xf0,
        0x88,
        0x88,
        0x88,
        0xf0,
        0x88,
        0x88,
        0x88,
        0xf0,
    };
    const Img CHAR_B(CHAR_B_DATA, 1, 5, 9);
    const uint8_t CHAR_C_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x80,
        0x80,
        0x80,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_C(CHAR_C_DATA, 1, 5, 9);
    const uint8_t CHAR_D_DATA[] = {
        0xe0,
        0x90,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x90,
        0xe0,
    };
    const Img CHAR_D(CHAR_D_DATA, 1, 5, 9);
    const uint8_t CHAR_E_DATA[] = {
        0xf8,
        0x80,
        0x80,
        0x80,
        0xf0,
        0x80,
        0x80,
        0x80,
        0xf8,
    };
    const Img CHAR_E(CHAR_E_DATA, 1, 5, 9);
    const uint8_t CHAR_F_DATA[] = {
        0xf8,
        0x80,
        0x80,
        0x80,
        0xf0,
        0x80,
        0x80,
        0x80,
        0x80,
    };
    const Img CHAR_F(CHAR_F_DATA, 1, 5, 9);
    const uint8_t CHAR_G_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x80,
        0xb0,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_G(CHAR_G_DATA, 1, 5, 9);
    const uint8_t CHAR_H_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x88,
        0xf8,
        0x88,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_H(CHAR_H_DATA, 1, 5, 9);
    const uint8_t CHAR_I_DATA[] = {
        0x70,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x70,
    };
    const Img CHAR_I(CHAR_I_DATA, 1, 5, 9);
    const uint8_t CHAR_J_DATA[] = {
        0x78,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x90,
        0x90,
        0x60,
    };
    const Img CHAR_J(CHAR_J_DATA, 1, 5, 9);
    const uint8_t CHAR_K_DATA[] = {
        0x88,
        0x88,
        0x90,
        0x90,
        0xe0,
        0x90,
        0x90,
        0x88,
        0x88,
    };
    const Img CHAR_K(CHAR_K_DATA, 1, 5, 9);
    const uint8_t CHAR_L_DATA[] = {
        0x80,
        0x80,
        0x80,
        0x80,
        0x80,
        0x80,
        0x80,
        0x80,
        0xf8,
    };
    const Img CHAR_L(CHAR_L_DATA, 1, 5, 9);
    const uint8_t CHAR_M_DATA[] = {
        0x88,
        0xd8,
        0xa8,
        0xa8,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_M(CHAR_M_DATA, 1, 5, 9);
    const uint8_t CHAR_N_DATA[] = {
        0x88,
        0xc8,
        0xc8,
        0xa8,
        0xa8,
        0x98,
        0x98,
        0x88,
        0x88,
    };
    const Img CHAR_N(CHAR_N_DATA, 1, 5, 9);
    const uint8_t CHAR_O_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_O(CHAR_O_DATA, 1, 5, 9);
    const uint8_t CHAR_P_DATA[] = {
        0xf0,
        0x88,
        0x88,
        0x88,
        0xf0,
        0x80,
        0x80,
        0x80,
        0x80,
    };
    const Img CHAR_P(CHAR_P_DATA, 1, 5, 9);
    const uint8_t CHAR_Q_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0xa8,
        0x90,
        0x68,
    };
    const Img CHAR_Q(CHAR_Q_DATA, 1, 5, 9);
    const uint8_t CHAR_R_DATA[] = {
        0xf0,
        0x88,
        0x88,
        0x88,
        0xf0,
        0xa0,
        0x90,
        0x88,
        0x88,
    };
    const Img CHAR_R(CHAR_R_DATA, 1, 5, 9);
    const uint8_t CHAR_S_DATA[] = {
        0x70,
        0x88,
        0x80,
        0x80,
        0x70,
        0x08,
        0x08,
        0x88,
        0x70,
    };
    const Img CHAR_S(CHAR_S_DATA, 1, 5, 9);
    const uint8_t CHAR_T_DATA[] = {
        0xf8,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
    };
    const Img CHAR_T(CHAR_T_DATA, 1, 5, 9);
    const uint8_t CHAR_U_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_U(CHAR_U_DATA, 1, 5, 9);
    const uint8_t CHAR_V_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0x50,
        0x50,
        0x50,
        0x20,
    };
    const Img CHAR_V(CHAR_V_DATA, 1, 5, 9);
    const uint8_t CHAR_W_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x88,
        0x88,
        0xa8,
        0xa8,
        0xa8,
        0x50,
    };
    const Img CHAR_W(CHAR_W_DATA, 1, 5, 9);
    const uint8_t CHAR_X_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x50,
        0x20,
        0x50,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_X(CHAR_X_DATA, 1, 5, 9);
    const uint8_t CHAR_Y_DATA[] = {
        0x88,
        0x88,
        0x88,
        0x50,
        0x50,
        0x20,
        0x20,
        0x20,
        0x20,
    };
    const Img CHAR_Y(CHAR_Y_DATA, 1, 5, 9);
    const uint8_t CHAR_Z_DATA[] = {
        0xf8,
        0x08,
        0x10,
        0x10,
        0x20,
        0x40,
        0x40,
        0x80,
        0xf8,
    };
    const Img CHAR_Z(CHAR_Z_DATA, 1, 5, 9);
    const uint8_t CHAR_LSQB_DATA[] = {
        0x70,
        0x40,
        0x40,
        0x40,
        0x40,
        0x40,
        0x40,
        0x40,
        0x70,
    };
    const Img CHAR_LSQB(CHAR_LSQB_DATA, 1, 5, 9);
    const uint8_t CHAR_BKSL_DATA[] = {
        0x80,
        0x80,
        0x40,
        0x40,
        0x20,
        0x20,
        0x10,
        0x10,
        0x08,
    };
    const Img CHAR_BKSL(CHAR_BKSL_DATA, 1, 5, 9);
    const uint8_t CHAR_RSQB_DATA[] = {
        0x70,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x70,
    };
    const Img CHAR_RSQB(CHAR_RSQB_DATA, 1, 5, 9);
    const uint8_t CHAR_CART_DATA[] = {
        0x20,
        0x50,
        0x88,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_CART(CHAR_CART_DATA, 1, 5, 9);
    const uint8_t CHAR_USCR_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xf8,
    };
    const Img CHAR_USCR(CHAR_USCR_DATA, 1, 5, 9);
    const uint8_t CHAR_BKTK_DATA[] = {
        0x40,
        0x20,
        0x10,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_BKTK(CHAR_BKTK_DATA, 1, 5, 9);
    const uint8_t CHAR_LCA_DATA[] = {
        0x00,
		0x00,
		0x00,
		0x00,
		0x78,
		0x88,
		0x88,
		0x98,
		0x68,
    };
    const Img CHAR_LCA(CHAR_LCA_DATA, 1, 5, 9);
    const uint8_t CHAR_LCB_DATA[] = {
        0x80,
        0x80,
        0x80,
        0x80,
        0xb0,
        0xc8,
        0x88,
        0x88,
        0xf0,
    };
    const Img CHAR_LCB(CHAR_LCB_DATA, 1, 5, 9);
    const uint8_t CHAR_LCC_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x70,
        0x80,
        0x80,
        0x88,
        0x70,
    };
    const Img CHAR_LCC(CHAR_LCC_DATA, 1, 5, 9);
    const uint8_t CHAR_LCD_DATA[] = {
        0x08,
        0x08,
        0x08,
        0x08,
        0x68,
        0x98,
        0x88,
        0x88,
        0x78,
    };
    const Img CHAR_LCD(CHAR_LCD_DATA, 1, 5, 9);
    const uint8_t CHAR_LCE_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x70,
        0x88,
        0xf8,
        0x80,
        0x70,
    };
    const Img CHAR_LCE(CHAR_LCE_DATA, 1, 5, 9);
    const uint8_t CHAR_LCF_DATA[] = {
        0x00,
        0x10,
        0x28,
        0x20,
        0x70,
        0x20,
        0x20,
        0x20,
        0x20,
    };
    const Img CHAR_LCF(CHAR_LCF_DATA, 1, 5, 9);
    const uint8_t CHAR_LCG_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x78,
        0x88,
        0x88,
        0x78,
        0x08,
        0x70,
    };
    const Img CHAR_LCG(CHAR_LCG_DATA, 1, 5, 9);
    const uint8_t CHAR_LCH_DATA[] = {
        0x80,
        0x80,
        0x80,
        0x80,
        0xb0,
        0xc8,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_LCH(CHAR_LCH_DATA, 1, 5, 9);
    const uint8_t CHAR_LCI_DATA[] = {
        0x00,
        0x00,
        0x20,
        0x00,
        0x60,
        0x20,
        0x20,
        0x20,
        0x70,
    };
    const Img CHAR_LCI(CHAR_LCI_DATA, 1, 5, 9);
    const uint8_t CHAR_LCJ_DATA[] = {
        0x00,
        0x00,
        0x10,
        0x00,
        0x30,
        0x10,
        0x10,
        0x90,
        0x60,
    };
    const Img CHAR_LCJ(CHAR_LCJ_DATA, 1, 5, 9);
    const uint8_t CHAR_LCK_DATA[] = {
        0xc0,
        0x40,
        0x40,
        0x40,
        0x48,
        0x50,
        0x60,
        0x50,
        0x48,
    };
    const Img CHAR_LCK(CHAR_LCK_DATA, 1, 5, 9);
    const uint8_t CHAR_LCL_DATA[] = {
        0x60,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x70,
    };
    const Img CHAR_LCL(CHAR_LCL_DATA, 1, 5, 9);
    const uint8_t CHAR_LCM_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0xd0,
        0xa8,
        0xa8,
        0xa8,
        0xa8,
    };
    const Img CHAR_LCM(CHAR_LCM_DATA, 1, 5, 9);
    const uint8_t CHAR_LCN_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0xb0,
        0xc8,
        0x88,
        0x88,
        0x88,
    };
    const Img CHAR_LCN(CHAR_LCN_DATA, 1, 5, 9);
    const uint8_t CHAR_LCO_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x70,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_LCO(CHAR_LCO_DATA, 1, 5, 9);
    const uint8_t CHAR_LCP_DATA[] = {
        0x00,
        0x00,
        0x00,
        0xf0,
        0x88,
        0x88,
        0xf0,
        0x80,
        0x80,
    };
    const Img CHAR_LCP(CHAR_LCP_DATA, 1, 5, 9);
    const uint8_t CHAR_LCQ_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x68,
        0x98,
        0x88,
        0x78,
        0x08,
        0x08,
    };
    const Img CHAR_LCQ(CHAR_LCQ_DATA, 1, 5, 9);
    const uint8_t CHAR_LCR_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0xb0,
        0xc8,
        0x80,
        0x80,
        0x80,
    };
    const Img CHAR_LCR(CHAR_LCR_DATA, 1, 5, 9);
    const uint8_t CHAR_LCS_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x78,
        0x80,
        0x70,
        0x08,
        0xf0,
    };
    const Img CHAR_LCS(CHAR_LCS_DATA, 1, 5, 9);
    const uint8_t CHAR_LCT_DATA[] = {
        0x00,
        0x00,
        0x20,
        0x20,
        0x70,
        0x20,
        0x20,
        0x28,
        0x10,
    };
    const Img CHAR_LCT(CHAR_LCT_DATA, 1, 5, 9);
    const uint8_t CHAR_LCU_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x88,
        0x88,
        0x88,
        0x98,
        0x68,
    };
    const Img CHAR_LCU(CHAR_LCU_DATA, 1, 5, 9);
    const uint8_t CHAR_LCV_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x88,
        0x88,
        0x88,
        0x50,
        0x20,
    };
    const Img CHAR_LCV(CHAR_LCV_DATA, 1, 5, 9);
    const uint8_t CHAR_LCW_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x88,
        0x88,
        0xa8,
        0xa8,
        0x50,
    };
    const Img CHAR_LCW(CHAR_LCW_DATA, 1, 5, 9);
    const uint8_t CHAR_LCX_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x88,
        0x50,
        0x20,
        0x50,
        0x88,
    };
    const Img CHAR_LCX(CHAR_LCX_DATA, 1, 5, 9);
    const uint8_t CHAR_LCY_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x88,
        0x88,
        0x78,
        0x08,
        0x70,
    };
    const Img CHAR_LCY(CHAR_LCY_DATA, 1, 5, 9);
    const uint8_t CHAR_LCZ_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0xf8,
        0x10,
        0x20,
        0x40,
        0xf8,
    };
    const Img CHAR_LCZ(CHAR_LCZ_DATA, 1, 5, 9);
    const uint8_t CHAR_LCVB_DATA[] = {
        0x10,
        0x20,
        0x20,
        0x20,
        0x40,
        0x20,
        0x20,
        0x20,
        0x10,
    };
    const Img CHAR_LCVB(CHAR_LCVB_DATA, 1, 5, 9);
    const uint8_t CHAR_PIPE_DATA[] = {
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
        0x20,
    };
    const Img CHAR_PIPE(CHAR_PIPE_DATA, 1, 5, 9);
    const uint8_t CHAR_RCVB_DATA[] = {
        0x40,
        0x20,
        0x20,
        0x20,
        0x10,
        0x20,
        0x20,
        0x20,
        0x40,
    };
    const Img CHAR_RCVB(CHAR_RCVB_DATA, 1, 5, 9);
    const uint8_t CHAR_TLDE_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x40,
        0xa8,
        0x10,
        0x00,
        0x00,
        0x00,
    };
    const Img CHAR_TLDE(CHAR_TLDE_DATA, 1, 5, 9);
	const uint8_t CHAR_UNKNOWN_DATA[] = {
		0xe0, 0x10, 0x20, 0x1c,
		0x10, 0x20, 0x10, 0x20,
		0x10, 0x42, 0x48, 0x20,
		0x10, 0x49, 0x48, 0x20,
		0x08, 0x44, 0x48, 0x40,
		0x08, 0x40, 0x48, 0x40,
		0x04, 0x40, 0x88, 0x80,
		0x04, 0x21, 0x10, 0x80,
		0x03, 0x96, 0x27, 0x00,
	};
	const Img CHAR_UNKNOWN(CHAR_UNKNOWN_DATA, 4, 30, 9);
	
	// Special characters
    const uint8_t CHAR_AGV_DATA[] = {
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0x78, 0x00,
        0x88, 0x00,
        0x89, 0x00,
        0x7a, 0x80,
        0x0a, 0x80,
        0x71, 0x00,
    };
    const Img CHAR_AGV(CHAR_AGV_DATA, 2, 9, 9);
    const uint8_t CHAR_AVGO_DATA[] = {
        0x88, 0x00,
        0xc8, 0x00,
        0xc8, 0x00,
        0xa8, 0x00,
        0xa8, 0x00,
        0x99, 0x00,
        0x9a, 0x80,
        0x8b, 0x80,
        0x8a, 0x80,
    };
    const Img CHAR_AVGO(CHAR_AVGO_DATA, 2, 9, 9);
    const uint8_t CHAR_ECHG_DATA[] = {
        0x00,
        0x00,
        0x07,
        0x00,
        0x70,
        0x88,
        0xf8,
        0x80,
        0x70,
    };
    const Img CHAR_ECHG(CHAR_ECHG_DATA, 1, 8, 9);
    const uint8_t CHAR_VLIG_DATA[] = {
        0x00,
        0x00,
        0x30,
        0x48,
        0x80,
        0x80,
        0x80,
        0x88,
        0x70,
    };
    const Img CHAR_VLIG(CHAR_VLIG_DATA, 1, 5, 9);
    const uint8_t CHAR_EE_DATA[] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x70,
        0x40,
        0x70,
        0x40,
        0x70,
    };
    const Img CHAR_EE(CHAR_EE_DATA, 1, 5, 9);
    const uint8_t CHAR_EULR_DATA[] = {
        0x00,
        0x00,
        0x30,
        0x48,
        0xd0,
        0xe0,
        0xc0,
        0xc8,
        0x70,
    };
    const Img CHAR_EULR(CHAR_EULR_DATA, 1, 5, 9);
    const uint8_t CHAR_THETA_DATA[] = {
        0x70,
        0x88,
        0x88,
        0x88,
        0xf8,
        0x88,
        0x88,
        0x88,
        0x70,
    };
    const Img CHAR_THETA(CHAR_THETA_DATA, 1, 5, 9);
	const uint8_t CHAR_SERR_DATA[] = {
		0x28, 0x00, 0x82, 0x00, 0x00,
		0x49, 0x02, 0x42, 0x00, 0x88,
		0x8a, 0x85, 0x22, 0x00, 0x88,
		0xb1, 0x02, 0x2c, 0x00, 0x88,
		0x80, 0x78, 0x20, 0x03, 0xfe,
		0x80, 0x48, 0x20, 0x00, 0x00,
		0x80, 0x48, 0x20, 0xf0, 0x00,
		0x40, 0x78, 0x41, 0x08, 0x00,
		0x20, 0x00, 0x82, 0x04, 0x00,
	};
	const Img CHAR_SERR(CHAR_SERR_DATA, 5, 39, 9);
	const uint8_t CHAR_PI_DATA[] = {
		0x00,
		0x00,
		0x00,
		0x00,
		0xf8,
		0x50,
		0x50,
		0x50,
		0x98,
	};
	const Img CHAR_PI(CHAR_PI_DATA, 1, 5, 9);
	const uint8_t CHAR_MUL_DATA[] = {
		0x00,
		0x00,
		0x00,
		0x60,
		0x60,
		0x00,
		0x00,
		0x00,
		0x00,
	};
	const Img CHAR_MUL(CHAR_MUL_DATA, 1, 5, 9);
	const uint8_t CHAR_DIV_DATA[] = {
		0x00,
		0x00,
		0x20,
		0x00,
		0xf8,
		0x00,
		0x20,
		0x00,
		0x00,
	};
	const Img CHAR_DIV(CHAR_DIV_DATA, 1, 5, 9);

	const uint8_t CHAR_SUMMATION_DATA[] = {
		0xff, 0xc0,
		0xff, 0xc0,
		0xc0, 0x40,
		0xe0, 0x00,
		0x70, 0x00,
		0x38, 0x00,
		0x1c, 0x00,
		0x0e, 0x00,
		0x1c, 0x00,
		0x38, 0x00,
		0x70, 0x00,
		0xe0, 0x00,
		0xc0, 0x40,
		0xff, 0xc0,
		0xff, 0xc0,
	};
	const Img CHAR_SUMMATION(CHAR_SUMMATION_DATA, 2, 10, 15);
	const uint8_t CHAR_PRODUCT_DATA[] = {
		0xff, 0xc0,
		0xff, 0xc0,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0x61, 0x80,
		0xf3, 0xc0,
	};
	const Img CHAR_PRODUCT(CHAR_PRODUCT_DATA, 2, 10, 15);
	
} // namespace lcd
