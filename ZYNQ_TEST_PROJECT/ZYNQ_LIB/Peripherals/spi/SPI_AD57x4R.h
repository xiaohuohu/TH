#ifndef __SPI_AD57X4R_H__
#define __SPI_AD57X4R_H__

/* AD5754R Register Map */
#define AD5724R_REG_DAC             0x00 // DAC register
#define AD5724R_REG_RANGE_SELECT    0x01 // Output range select register
#define AD5724R_REG_POWER_CONTROL   0x02 // Power control register
#define AD5724R_REG_CONTROL         0x03 // Control register

/* AD5754R Channel Address */
#define AD5724R_DAC_A               0x00 // Address of channel A
#define AD5724R_DAC_B               0x01 // Address of channel B
#define AD5724R_DAC_C               0x02 // Address of channel C
#define AD5724R_DAC_D               0x03 // Address of channel D
#define AD5724R_DAC_ALL             0x04 // All four DACs



/* AD5754R Range Bits */
#define AD5754R_UNIPOLAR_5_RANGE    0x00 // 0..+5(V)
#define AD5754R_UNIPOLAR_10_RANGE   0x01 // 0..+10(V)
#define AD5754R_UNIPOLAR_10_8_RANGE 0x02 // 0..+10.8(V)
#define AD5754R_BIPOLAR_5_RANGE     0x03 // -5..+5(V)
#define AD5754R_BIPOLAR_10_RANGE    0x04 // -10...+10(V)
#define AD5754R_BIPOLAR_10_8_RANGE  0x05 // -10.8...+10.8(V)

/* Control Functions */
#define AD5754R_NOP_OPERATION       0x00 // No operation instruction used in readback operations.
#define AD5754R_SDO_DISABLE         0x11 // Set by the user to disable the SDO output.
#define AD5754R_CLR_SELECT          0x12 // Selects the clear code: 0V, Midscale or Negative full scale.
#define AD5754R_CLAMP_ENABLE        0x14 // Set by the user to enable the current-limit clamp.
#define AD5754R_TSD_ENABLE          0x18 // Set by the user to enable the thermal shutdown feature.
#define AD5754R_CLEAR               0x40 // Sets the DAC registers to the clear code and updates the outputs.
#define AD5754R_LOAD                0x50 // Updates the DAC registers and, consequently, the DAC outputs.

/*AD5724 Power on status*/
#define AD5724R_DAC_A_POWERON       0x01 //DAC A Power on
#define AD5724R_DAC_B_POWERON       0x02 //DAC B Power on
#define AD5724R_DAC_C_POWERON       0x04 //DAC C Power on
#define AD5724R_DAC_D_POWERON       0x08 //DAC D Power on
#define AD5724R_DAC_ALL_POWERON     0X0F  //DAC ALL Power on
#define AD5724R_DAC_ALL_POWERON_REF   0x1F
#define AD5724R_DAC_A_C_POWERON_REF   0x15

/*************************************
 * DA spi��������ʼ����ʹ��SPI 1 ƬѡΪ 1
 * spi_AD57x4R_init
 * ����ֵ��
 * 		1 Ϊ��ʼ���ɹ���0 Ϊ��ʼ��ʧ��
 *************************************/
int spi_AD57x4R_init(void);

/*************************************
 * ��DAͨ����Դ
 * AD5724R_SetPowerControl
 * ���� �� DA5724R ʹ�� AD5724R_DAC_A_C_POWERON_REF ʹ���ڲ���׼��ѹ
 * ����ֵ��
 * 		1 Ϊ�򿪳ɹ���0 Ϊ��ʧ��
 *************************************/
int AD5724R_SetPowerControl(u8 PowerStatus);

/*************************************
 * AD5724R_OutputRangeSelect
 * ���� �����Χ  5V  10V  10.8V
 * ���� ��  AD5754R Range Bits ���к궨��
 *
 *************************************/
void AD5724R_OutputRangeSelect(u8 DACAddr,u8 RangeSelect);

/*************************************
 * AD5724R_SetDACRegisterValue
 * ���DAֵ
 * ���� ��
 *    @DACAddr��     AD5754R Channel Address ���к궨��
 *    @Data��             -32768 - 32768 ����AD5724˵����
 *
 *************************************/
void AD5724R_SetDACRegisterValue(u8 DACAddr,u16 Data);



/*************************************
 * AD5724R_GetDACRegisterValue
 * ��ȡDA�Ĵ�����ֵ
 * ���� ��
 *    @DACAddr��     AD5754R Channel Address ���к궨��
 *    @buff_r��        ��ȡ��ֵ
 *    @len��                AD5724R ��ȡ�ĳ���Ϊ 2 ���Ĵ�����16λΪ����λ��������Ϊ 2��
 *************************************/
void AD5724R_GetDACRegisterValue(u8 DACAddr,u8* buff_r,u8 len);


/*������������Ӧ�ó�����Ҫ*/
void AD5724R_SetControlReg(u8 DACAddr,u16 Data);
int spi_AD57x4R_write(u8 RegisterAddr,u8 DACAddr,u8 *buffer, u32 len);
int spi_AD57x4R_read(u8 RegisterAddr,u8 DACAddr,u8 *buffer, u32 len);


//extern unsigned int clock_spi;

#endif
