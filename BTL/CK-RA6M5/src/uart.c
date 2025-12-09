

#include "uart.h"
#include "hal_data.h"
#define ICU_SCI5_IRQn (IRQn_Type) 0



extern ReadSensorState RSState = TYPE_INIT;
extern int S0Value = 0;
extern int S1Value = 0;
void printUART(const char *str)
{
    if (!str) return;
    while (*str)
    {
        uint8_t ch = (uint8_t)*str++;
        if (ch == '\n')
        {
            while (R_SCI5->SSR_b.TDRE == 0) { __NOP(); }
            R_SCI5->TDR_b.TDR = '\r';
        }
        while (R_SCI5->SSR_b.TDRE == 0) { __NOP(); }
        R_SCI5->TDR_b.TDR = ch;
    }
    while (R_SCI5->SSR_b.TEND == 0) { __NOP(); }
}
void UART5_Handler(void){
	R_ICU->IELSR_b[0].IR=0;
	char c = R_SCI5->RDR_b.RDR;
	R_SCI5->TDR_b.TDR = c;
	switch(RSState){
	case TYPE_INIT:
		if(c=='S')
			RSState = TYPE_SENSOR;
		break;
	case TYPE_SENSOR:
		if(c=='0')
			RSState = TYPE_ID_0;
		else if (c == '1')
			RSState = TYPE_ID_1;
		else{
			RSState = TYPE_INIT;
			printUART("Loi nhap, xin nhap lai \n");
		}
		break;
	case TYPE_ID_0:
		if(c=='0'){
			RSState = TYPE_INIT;
			S0Value = 0;
		}
		else if(c == '1'){
			RSState = TYPE_INIT;
			S0Value = 1;
		}
		else{
			RSState = TYPE_INIT;
			printUART("\nLoi nhap, xin nhap lai \n");
		}
		break;
	case TYPE_ID_1:
		if(c=='0'){
			RSState = TYPE_INIT;
			S1Value = 0;
		}
		else if(c == '1'){
			RSState = TYPE_INIT;
			S1Value = 1;
		}
		else{
			RSState = TYPE_INIT;
			printUART("\nLoi nhap, xin nhap lai \n");
		}
		break;
	}
}

/*
 * PCLKA 100Mhz , n = 1, N = 80, baud rate = 9600
 * SCI5-->UART5-->J20 (nhớ nối dây micro vào J20 nhó)
 */
void uart_init (void){


	R_MSTP->MSTPCRB_b.MSTPB26 = 0;		// enable module SCI5
	R_SCI5->SCR = 0;					// tắt Tx, Rx
	R_SCI5->SMR_b.CM = 0;				// chọn mode async
	R_SCI5->SCMR_b.CHR1 |= 1;			// CHR1 = 1, CHR = 0: 8 bit data
	R_SCI5->SMR_b.CHR = 0;
	R_SCI5->SMR_b.PE = 0;				// không dùng parity
	R_SCI5->SMR_b.STOP = 0;				// 1 stop bit
	R_SCI5->SMR_b.CKS = 0b01;			// n = 1
	R_SCI5->BRR_b.BRR = 80;				// N = 80
	R_SCI5->SCR_b.RE = 1;				// cho phép Rx
	R_SCI5->SCR_b.RIE = 1;				// Cho phép ngắt Rx
	R_SCI5->SCR_b.TE = 1;				// Cho phép Tx

	R_PMISC->PWPR_b.B0WI = 0; // Enable writable PFS
	R_PMISC->PWPR_b.PFSWE = 1;
	R_PFS->PORT[5].PIN[01].PmnPFS_b.PMR=1;				// Peripheral mode phải cấu hình thành chế độ ngoại vi để nó chạy được mode ngoại vi, như trong trường hợp này là cho SCI
	R_PFS->PORT[5].PIN[01].PmnPFS_b.PSEL=0b00101;
	R_PFS->PORT[5].PIN[02].PmnPFS_b.PMR=1;
	R_PFS->PORT[5].PIN[02].PmnPFS_b.PSEL=0b00101;

	R_SYSTEM->PRCR = (0xA5u<<8)|(1<<4);						// mở khóa SAIELSRn
	R_CPSCU->ICUSARG_b.SAIELSRn &= ~((uint32_t)0b1);		// secure vùng ICU cho 3 handler
	NVIC->ITNS[0] &= ~((uint32_t)0b1);					// secure vùng NVIC

	R_ICU->IELSR_b[0].IELS = 0x19E;
	NVIC_SetPriority(ICU_SCI5_IRQn, 3);
	NVIC_EnableIRQ(ICU_SCI5_IRQn);
	NVIC_SetVector(ICU_SCI5_IRQn, (uint32_t)&UART5_Handler);
}
