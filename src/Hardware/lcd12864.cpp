#include "lcd12864.hpp"

namespace lcd {
	
	void LCD12864::clear() {
		writeCommand(Command::CLEAR);
	}
	void LCD12864::home() {
		writeCommand(Command::HOME);
	}
			
	void LCD12864::init() {
		if(!FOUR_WIRE_INTERFACE) {
			delay::ms(15);
			writeCommandNoWait(Command::NORMAL_CMD_8BIT);
			delay::ms(5);
			writeCommandNoWait(Command::NORMAL_CMD_8BIT);
			delay::ms(5);
		}
		else {
			delay::ms(15);
			writeCommandNoWait(Command::NORMAL_CMD_4BIT);
			delay::ms(5);
			writeCommandNoWait(Command::NORMAL_CMD_4BIT);
			delay::ms(5);
		}
		
		writeCommand(Command::ENTRY_CURSOR_SHIFT_RIGHT);
		writeCommand(Command::CLEAR);
		writeCommand(Command::DISPLAY_ON_CURSOR_OFF);
	}
	
	void LCD12864::setCursor(uint8_t row, uint8_t col) {
		//If using extended command set, first set to use basic, write the address and change back
		if(isUsingExtended()) {
			useBasic();
			switch(row){
			case 0: col += 0x80; break;
			case 1: col += 0x90; break;
			case 2: col += 0x88; break;
			case 3: col += 0x98; break;
			default: break;
			}
			//Make the first bit 1 and second bit 0 to match the command requirements
			col |= 0x80; //1000 0000
			col &= 0xBF; //1011 1111
			writeCommand(col);
			useExtended();
		}
		else {
			switch(row){
			case 0: col += 0x80; break;
			case 1: col += 0x90; break;
			case 2: col += 0x88; break;
			case 3: col += 0x98; break;
			default: break;
			}
			//Make the first bit 1 and second bit 0 to match the command requirements
			col |= 0x80; //1000 0000
			col &= 0xBF; //1011 1111
			writeCommand(col);
		}
	}
	
	bool LCD12864::isUsingExtended() {
		return extendedCmd;
	}
	void LCD12864::useExtended() {
		if(extendedCmd) 
			return;
		writeCommand(FOUR_WIRE_INTERFACE ? Command::EXT_CMD_4BIT : Command::EXT_CMD_8BIT);
		extendedCmd = true;
	}
	void LCD12864::useBasic() {
		if(!extendedCmd)
			return;
		writeCommand(FOUR_WIRE_INTERFACE ? Command::NORMAL_CMD_4BIT : Command::NORMAL_CMD_8BIT);
		extendedCmd = false;
	}
	
	bool LCD12864::isDrawing() {
		return drawing;
	}
	void LCD12864::startDraw() {
		
	}
}
