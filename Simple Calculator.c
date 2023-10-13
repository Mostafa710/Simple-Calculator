#define F_CPU 8000000UL
#include <util/delay.h>
#include "keypad.h"
#include "LCD.h"

unsigned char arr[4];
unsigned short num1,num2;
unsigned long result,res_test;

int main(void)
{
	unsigned char i,digits,operation,val,flag;
	keypad_init('D');
	LCD_init();
	while(1)
	{
		arr[0]=arr[1]=arr[2]=arr[3]=NOT_PRESSED;
		num1=num2=result=digits=flag=0;
		//get the first number
		for(i=0;i<=3;i++)
		{
			do
			{
				val=keypad_read('D');
			} while (val == NOT_PRESSED);
			if(val == 'C')
			{
				flag=1;
				LCD_clear_screen();
				break;
			}
			else if( (val == '+') || (val == '-') || (val == '*') || (val == '/') )
			{
				LCD_send_char(val);
				operation = val;
				_delay_ms(250);
				break;
			}
			else
			{
				LCD_send_char(val);
				arr[i] = val-48;
				_delay_ms(250);
			}
		}
		if(flag == 0)
		{
			//check if the number was 4-digits
		    if( (val != '+') && (val != '-') && (val != '*') && (val != '/') )
		    {
			   do
			   {
				   val=keypad_read('D');
			   } while (val == NOT_PRESSED);
			   if(val == 'C')
		 	   {
				   flag=1;
				   LCD_clear_screen();
			   }
			   else
			   {
				   LCD_send_char(val);
				   operation=val;
				   _delay_ms(250);
			   }
			}			
			if(flag == 0)
			{
				//storing the first number
				i=0;
				while( (arr[i] != NOT_PRESSED) && (i<=3) )
				{
					num1 = num1*10;
					num1 = num1+arr[i];
					i++;
				}
				arr[0]=arr[1]=arr[2]=arr[3]=NOT_PRESSED;
				//get the second number
				for(i=0;i<=3;i++)
				{
					do
					{
						val=keypad_read('D');
					} while (val == NOT_PRESSED);
					if(val == 'C')
					{
						flag=1;
						LCD_clear_screen();
					}
					else if(val == '=')
					{
						LCD_send_char(val);
						_delay_ms(250);
						break;
					}
					else
					{
						LCD_send_char(val);
						arr[i] = val-48;
						_delay_ms(250);
					}
				}
				if(flag == 0)
				{
					//check if the second number was 4-digits
					if(val != '=')
					{
						do
						{
							val=keypad_read('D');
						} while (val == NOT_PRESSED);
						if(val == 'C')
						{
							flag=1;
							LCD_clear_screen();
						}
						else
						{
							LCD_send_char(val);
							_delay_ms(250);
						}
					}
					if(flag == 0)
					{
						//storing the second number
						i=0;
						while( (arr[i] != NOT_PRESSED) && (i<=3) )
						{
							num2= num2*10;
							num2= num2+arr[i];
							i++;
						}
						switch(operation)
						{
							case '+':
							result = num1+num2;
							break;
							case '-':
							result = num1-num2;
							break;
							case '*':
							result = (long)num1*num2;
							/* I made promotion here beacuse the
							result may be higher than 65,536 */
							break;
							case '/':
							result = num1/num2;
							break;
						}
						//calculate number of digits of the result
						res_test=result;
						while(res_test != 0)
						{
							res_test = res_test/10;
							digits++;
						}
						//printing the result
						switch(digits)
						{
							case 1:
							LCD_send_char(result+48);
							break;
							case 2:
							LCD_send_char(result/10+48);
							LCD_send_char(result%10+48);
							break;
							case 3:
							LCD_send_char(result/100+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
							case 4:
							LCD_send_char(result/1000+48);
							LCD_send_char((result/100)%10+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
							case 5:
							LCD_send_char(result/10000+48);
							LCD_send_char((result/1000)%10+48);
							LCD_send_char((result/100)%10+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
							case 6:
							LCD_send_char(result/100000+48);
							LCD_send_char((result/10000)%10+48);
							LCD_send_char((result/1000)%10+48);
							LCD_send_char((result/100)%10+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
							case 7:
							LCD_move_cursor(2,1);
							LCD_send_string("Result=");
							LCD_send_char(result/1000000+48);
							LCD_send_char((result/100000)%10+48);
							LCD_send_char((result/10000)%10+48);
							LCD_send_char((result/1000)%10+48);
							LCD_send_char((result/100)%10+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
							case 8:
							LCD_move_cursor(2,1);
							LCD_send_string("Result=");
							LCD_send_char(result/10000000+48);
							LCD_send_char((result/1000000)%10+48);
							LCD_send_char((result/100000)%10+48);
							LCD_send_char((result/10000)%10+48);
							LCD_send_char((result/1000)%10+48);
							LCD_send_char((result/100)%10+48);
							LCD_send_char((result/10)%10+48);
							LCD_send_char(result%10+48);
							break;
						}
					}
				}
			}
		}
		if(flag == 1)
		{
			_delay_ms(250);
		}
		else
		{
			do 
			{
				val=keypad_read('D');
			} while (val == NOT_PRESSED);
			LCD_clear_screen();
			_delay_ms(250);
		}
	}
}
