#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	Serial_Init();
	
	while (1)
	{
		if(Serial_RxFlag==1)
		{
			if(Serial_RxPacket[0]==0)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Trafic Light Signal");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==1)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Stop Signal");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==2)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Speedlimit Signal");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==3)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Crosswalk Signal");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==4)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Crosswalk");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==5)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Pedestrian");
				Delay_ms(1000);
				OLED_Clear();
			}
			else if(Serial_RxPacket[0]==6)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Bus");
				Delay_ms(1000);
				OLED_Clear();
			}

			else if(Serial_RxPacket[0]==7)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Car");
				Delay_ms(1000);
				OLED_Clear();
			}

			else if(Serial_RxPacket[0]==8)
			{
				OLED_ShowString(1, 1, "RxData:");
				OLED_ShowString(2, 1, "Truck");
				Delay_ms(1000);
				OLED_Clear();
			}


		}
	}
}
