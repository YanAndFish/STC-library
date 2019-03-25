#include "HAL_IAP.h"

xdata IapConfig IAP_CON={0xff,0};

void IAP_disable()
{
	IAP_CONTR = 0;			//��ֹIAP/IAP����
	IAP_CMD   = 0;			//ȥ��IAP/IAP����
	IAP_TRIG  = 0;			//��ֹIAP/IAP�����󴥷�
	IAP_ADDRH = 0xff;		//��0��ַ���ֽ�
	IAP_ADDRL = 0xff;		//��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
}

void IAP_reads(unsigned int EE_address,unsigned char *DataAddress,unsigned int number)
{
	EA = 0;		//��ֹ�ж�
	EE_address += ADDRESS;
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����IAP/IAP��������һ�ξ͹�
	IapRead();									//���ֽڶ���������ı�ʱ����������������
	do
	{
		IAP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		IapTrig();							//����5AH������A5H��IAP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
											//����A5H��IAP/IAP������������������
											//CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
		_nop_();
		*DataAddress = IAP_DATA;			//��������������
		EE_address++;
		DataAddress++;
	}while(--number);

	IAP_disable();
	EA = 1;		//���������ж�
}

unsigned char IAP_read(unsigned int EE_address)
{
	unsigned char c;
	EA = 0;		//��ֹ�ж�
	EE_address += ADDRESS;
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����IAP/IAP��������һ�ξ͹�
	IapRead();									//���ֽڶ���������ı�ʱ����������������
	IAP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	IAP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
	IapTrig();							//����5AH������A5H��IAP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
											//����A5H��IAP/IAP������������������
											//CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
	_nop_();
	c = IAP_DATA;			//��������������
	IAP_disable();
	EA = 1;		//���������ж�
	return c;
}

void IAP_erase(unsigned int EE_address)
{
	EA = 0;		//��ֹ�ж�
	EE_address += ADDRESS;
											//ֻ������������û���ֽڲ�����512�ֽ�/������
											//����������һ���ֽڵ�ַ����������ַ��
	IAP_ADDRH = EE_address / 256;			//��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	IAP_ADDRL = EE_address % 256;			//��������ַ���ֽ�
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����IAP/IAP��������һ�ξ͹�
	IapErase();							//������������������ı�ʱ����������������
	IapTrig();
	_nop_();
	IAP_disable();
	if(IAP_CON.IapSectorNum==(EE_address/512))
		IAP_CON.IapSectorNum=0xff;
	EA = 1;		//���������ж�
}

void IAP_write(unsigned int EE_address,unsigned char Data)
{
	if(IAP_CON.IapSectorNum != (EE_address/512))
	{
		IAP_CON.IapSectorNum = (EE_address/512);
		IAP_reads((IAP_CON.IapSectorNum*512),IAP_CON.IapSectorBuffer,512);
	}
	IAP_CON.IapSectorBuffer[EE_address-(IAP_CON.IapSectorNum*512)]=Data;
}

void IAP_update()
{
	unsigned int i,EE_address;
	EA = 0;		//��ֹ�ж�
	EE_address=IAP_CON.IapSectorNum*512+ADDRESS;
	IAP_erase(IAP_CON.IapSectorNum*512);
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����IAP/IAP��������һ�ξ͹�
	IapWrite();							//���ֽ�д��������ı�ʱ����������������
	for(i=0;i<512;i++)
	{
		IAP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		IAP_DATA  = IAP_CON.IapSectorBuffer[i];			//�����ݵ�IAP_DATA��ֻ�����ݸı�ʱ����������
		IapTrig();
		_nop_();
		EE_address++;
	}

	IAP_disable();
	EA = 1;		//���������ж�
}