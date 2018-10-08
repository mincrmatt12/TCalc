#include "stm32f10x.h"
#include "sys.hpp"
#include "delay.hpp"
#include "usart.hpp"
#include "gpiopin.hpp"
#include "lcd1602.hpp"

using namespace lcd;

int main() {
	
	sys::initRCC();
	sys::initNVIC();
	delay::init();
	usart::init(115200);
	
	GPIOPin led(GPIOC, GPIO_Pin_13);
	led.init(GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	led = 0;
	
	//Disable JTAG so we can use PB3 and 4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	
	delay::sec(3);
	
	GPIOPin RS(GPIOB, GPIO_Pin_8), RW(GPIOB, GPIO_Pin_9), E(GPIOB, GPIO_Pin_10), BUSY(GPIOA, GPIO_Pin_7);
	LCD1602 lcd(RS, RW, E, BUSY, GPIOA);
	lcd.init();
	lcd.writeData(0xFF);
	
    while(true) {
    }
}
