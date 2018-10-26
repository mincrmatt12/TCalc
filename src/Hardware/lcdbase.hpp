#ifndef __LCD_H__
#define __LCD_H__
#include "stm32f10x.h"
#include "delay.hpp"
#include "gpiopin.hpp"

#define LCD_ENABLE_DELAY 10 //Cycles

namespace lcd {
	
	class LCDException {
	public:
		LCDException(const char *msg) : msg(msg) {}
		
		virtual const char* what() const noexcept {
			return msg;
		}
			
	private:
		const char *msg;
	};
	
	class LCDBase {
	public:
		LCDBase(GPIOPin RS, GPIOPin RW, GPIOPin E, GPIOPin D0, GPIOPin D1, GPIOPin D2, GPIOPin D3, GPIOPin D4,
				GPIOPin D5, GPIOPin D6, GPIOPin D7, uint32_t timeout = 1000000) :
				RS(RS), RW(RW), E(E), D0(D0), D1(D1), D2(D2), D3(D3), D4(D4), D5(D5), D6(D6), D7(D7), timeout(timeout), FOUR_WIRE_INTERFACE(false) {
			initGPIO();
		}
		LCDBase(GPIOPin RS, GPIOPin RW, GPIOPin E, GPIOPin D4, GPIOPin D5, GPIOPin D6, GPIOPin D7, uint32_t timeout = 1000000) :
				RS(RS), RW(RW), E(E), D4(D4), D5(D5), D6(D6), D7(D7), timeout(timeout), FOUR_WIRE_INTERFACE(true) {
			initGPIO();
		}
	
		virtual void init() = 0;
		virtual void setCursor(uint8_t, uint8_t) = 0;
		virtual void clear() = 0;
		virtual void home() = 0;
		
		virtual uint32_t getTimeout() const noexcept;
		virtual void setTimeout(uint32_t) noexcept;
	
		virtual void writeCommand(uint8_t cmd);
		virtual void writeData(uint8_t data);
		virtual void readData(uint8_t &out);
		virtual void writeString(const char *str);
	
	protected:
		GPIOPin RS, RW, E;
		GPIOPin D0, D1, D2, D3, D4, D5, D6, D7;
		uint32_t timeout;
	
		virtual void waitForBusyFlag();
		
		virtual void writeCommandNoWait(uint8_t) noexcept;
	
		virtual void setDataPort(uint8_t) noexcept;
		virtual uint8_t readDataPort() noexcept;
	
		virtual void setGPIOMode(const GPIOConfig&) noexcept;
		
		const bool FOUR_WIRE_INTERFACE;
	
		static const GPIOConfig READ_CONFIG;
		static const GPIOConfig WRITE_CONFIG;
		
	
	private:
		void initGPIO() noexcept;
	};
}

#endif
