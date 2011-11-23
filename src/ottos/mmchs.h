#include <ottos/types.h>

#define BV(n)      (1 << (n))


#define		MMCHS1 				0x4809C000		//connected to beagle board
#define		MMCHS2				0x480B4000
#define		MMCHS3				0x480AD000

#define 	CM_FCLKEN1_CORE		0x48004A00
#define		CM_ICLKEN1_CORE		0x48004A10

#define		MMCHS1_SYSCONFIG	0x4809C010
#define		MMCHS1_SYSSTATUS	0x4809C014


#define 	MMCHS1_CAPA 		0x4809C140

#define 	MMCHS1_CON			0x4809C02C
#define 	MMCHS1_CMD			0x4809C10C
#define		CONTROL_PADC 		0x48002144

#define		MMCHS1_HCTL			0x4809C128
#define		MMCHS1_SYSCTL		0x4809C12C
#define		MMCHS1_CON			0x4809C02C
#define		MMCHS1_IE			0x4809C134
#define		MMCHS1_ISE			0x4809C138
#define		MMCHS1_ARG			0x4809C108


#define		EN_MCSPI1			BV(24)
#define		EN_MMC1				BV(24)

#define		MMCHS1_STAT			0x4809C130
#define		MMCHS1_SYSSTATUS	0x4809C014

#define		MMCHS_SYSCONFIG		0x10 
#define		MMCHS_SYSSTATUS		0x14
#define		MMCHS_CSRE			0x24
#define		MMCHS_SYSTEST		0x28
#define		MMCHS_CON			0x2C
#define		MMCHS_PWCNT			0x30
#define		MMCHS_BLK			0x104
#define		MMCHS_ARG			0x108
#define		MMCHS_CMD			0x10C
#define		MMCHS_RSP10			0x110
#define		MMCHS_RSP32			0x114
#define		MMCHS_RSP54			0x118
#define		MMCHS_RSP76			0x11C
#define		MMCHS_DATA RW		0x120
#define		MMCHS_PSTATE		0x124
#define		MMCHS_HCTL			0x128
#define		MMCHS_SYSCTL		0x12C
#define		MMCHS_STAT			0x130
#define		MMCHS_IE			0x134
#define		MMCHS_ISE			0x138
#define		MMCHS_AC12			0x13C
#define		MMCHS_CAPA			0x140
#define		MMCHS_CUR_CAPA		0x148
#define		MMCHS_REV			0x1FC


#define		MMCHS_CMD_0			0x00000000
#define		MMCHS_CMD_1			0x01020000
#define		MMCHS_CMD_2			0x02090000
#define		MMCHS_CMD_3			0x031a0000
#define		MMCHS_CMD_5			0x05020000
#define		MMCHS_CMD_8			0x81a0000
#define		MMCHS_CMD_9			0x09090000
#define		MMCHS_CMD_55		0x371a0000


void mmchs_init();
void mmchs_cardIdent();
