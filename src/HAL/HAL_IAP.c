#include "HAL_IAP.h"

xdata IapConfig IAP_CON={0xff,0};

void IAP_disable()
{
	IAP_CONTR = 0;			//禁止IAP/IAP操作
	IAP_CMD   = 0;			//去除IAP/IAP命令
	IAP_TRIG  = 0;			//防止IAP/IAP命令误触发
	IAP_ADDRH = 0xff;		//清0地址高字节
	IAP_ADDRL = 0xff;		//清0地址低字节，指向非EEPROM区，防止误操作
}

void IAP_reads(unsigned int EE_address,unsigned char *DataAddress,unsigned int number)
{
	EA = 0;		//禁止中断
	EE_address += ADDRESS;
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//设置等待时间，允许IAP/IAP操作，送一次就够
	IapRead();									//送字节读命令，命令不需改变时，不需重新送命令
	do
	{
		IAP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		IAP_ADDRL = EE_address % 256;		//送地址低字节
		IapTrig();							//先送5AH，再送A5H到IAP/IAP触发寄存器，每次都需要如此
											//送完A5H后，IAP/IAP命令立即被触发启动
											//CPU等待IAP完成后，才会继续执行程序。
		_nop_();
		*DataAddress = IAP_DATA;			//读出的数据送往
		EE_address++;
		DataAddress++;
	}while(--number);

	IAP_disable();
	EA = 1;		//重新允许中断
}

unsigned char IAP_read(unsigned int EE_address)
{
	unsigned char c;
	EA = 0;		//禁止中断
	EE_address += ADDRESS;
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//设置等待时间，允许IAP/IAP操作，送一次就够
	IapRead();									//送字节读命令，命令不需改变时，不需重新送命令
	IAP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
	IAP_ADDRL = EE_address % 256;		//送地址低字节
	IapTrig();							//先送5AH，再送A5H到IAP/IAP触发寄存器，每次都需要如此
											//送完A5H后，IAP/IAP命令立即被触发启动
											//CPU等待IAP完成后，才会继续执行程序。
	_nop_();
	c = IAP_DATA;			//读出的数据送往
	IAP_disable();
	EA = 1;		//重新允许中断
	return c;
}

void IAP_erase(unsigned int EE_address)
{
	EA = 0;		//禁止中断
	EE_address += ADDRESS;
											//只有扇区擦除，没有字节擦除，512字节/扇区。
											//扇区中任意一个字节地址都是扇区地址。
	IAP_ADDRH = EE_address / 256;			//送扇区地址高字节（地址需要改变时才需重新送地址）
	IAP_ADDRL = EE_address % 256;			//送扇区地址低字节
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//设置等待时间，允许IAP/IAP操作，送一次就够
	IapErase();							//送扇区擦除命令，命令不需改变时，不需重新送命令
	IapTrig();
	_nop_();
	IAP_disable();
	if(IAP_CON.IapSectorNum==(EE_address/512))
		IAP_CON.IapSectorNum=0xff;
	EA = 1;		//重新允许中断
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
	EA = 0;		//禁止中断
	EE_address=IAP_CON.IapSectorNum*512+ADDRESS;
	IAP_erase(IAP_CON.IapSectorNum*512);
	IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY);	//设置等待时间，允许IAP/IAP操作，送一次就够
	IapWrite();							//送字节写命令，命令不需改变时，不需重新送命令
	for(i=0;i<512;i++)
	{
		IAP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		IAP_ADDRL = EE_address % 256;		//送地址低字节
		IAP_DATA  = IAP_CON.IapSectorBuffer[i];			//送数据到IAP_DATA，只有数据改变时才需重新送
		IapTrig();
		_nop_();
		EE_address++;
	}

	IAP_disable();
	EA = 1;		//重新允许中断
}