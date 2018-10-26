#include "lcd1602.hpp"
namespace lcd {
	
	void LCD1602::clear() {
		writeCommand(Command::CLEAR);
	}
	void LCD1602::home() {
		writeCommand(Command::HOME);
	}
	
	void LCD1602::init() {
		if(!FOUR_WIRE_INTERFACE) {
			delay::ms(15);
			writeCommandNoWait(Command::EIGHT_BIT_TWO_LINES);
			delay::ms(5);
			writeCommandNoWait(Command::EIGHT_BIT_TWO_LINES);
			delay::ms(1);
			writeCommandNoWait(Command::EIGHT_BIT_TWO_LINES);
		}
		else {
			delay::ms(15);
			writeCommandNoWait(Command::FOUR_BIT_TWO_LINES);
			delay::ms(5);
			writeCommandNoWait(Command::FOUR_BIT_TWO_LINES);
			delay::ms(1);
			writeCommandNoWait(Command::FOUR_BIT_TWO_LINES);
		}
		
		writeCommand(Command::DISPLAY_OFF);
		writeCommand(Command::CLEAR);
		writeCommand(Command::ENTRY_CURSOR_SHIFT_RIGHT);
		writeCommand(Command::DISPLAY_ON_CURSOR_OFF);
	}
	
	void LCD1602::setCursor(uint8_t row, uint8_t col) {
		if(row != 0) {
			col += 0x40;
		}
		writeCommand(0x80 | col);
	}
}
