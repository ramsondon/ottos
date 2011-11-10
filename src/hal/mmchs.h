#define BV(n)      (1 << (n))

#define		MMCHS1 				0x4809C000
#define		MMCHS2				0x480B4000
#define		MMCHS3				0x480AD000

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