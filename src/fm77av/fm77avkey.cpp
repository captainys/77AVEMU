#include <vector>
#include <string.h>
#include "cpputil.h"
#include "fm77avkey.h"

typedef struct KeyCodeAndLabelStruct
{
	int code;
	const char *label;
} KeyCodeAndLabel;

static KeyCodeAndLabel bitPattern[]=
{
	{AVKEY_BREAK				,"101010101010101001001010110101"},
	{AVKEY_PF1					,"101010101010101001001010101001"},
	{AVKEY_PF2					,"101010101010101001001001010101"},
	{AVKEY_PF3					,"101010101010101001001001001001"},
	{AVKEY_PF4					,"101010101001010110110110110101"},
	{AVKEY_PF5					,"101010101001010110110110101001"},
	{AVKEY_PF6					,"101010101001010110110101010101"},
	{AVKEY_PF7					,"101010101001010110110101001001"},
	{AVKEY_PF8					,"101010101001010110101010110101"},
	{AVKEY_PF9					,"101010101001010110101010101001"},
	{AVKEY_PF10					,"101010101001010110101001010101"},
	{AVKEY_EL					,"101010101010110101010110101001"},
	{AVKEY_CLS					,"101010101010110101010101010101"},
	{AVKEY_DUP					,"101010101010110101001010110101"},
	{AVKEY_HOME					,"101010101010110101001001010101"},
	{AVKEY_INS					,"101010101010110101010110110101"},
	{AVKEY_DEL					,"101010101010110101010101001001"},
	{AVKEY_LEFT					,"101010101010110101001001001001"},
	{AVKEY_RIGHT				,"101010101010101010110110101001"},
	{AVKEY_UP					,"101010101010110101001010101001"},
	{AVKEY_DOWN					,"101010101010101010110110110101"},

	{AVKEY_ESC					,"101010110110110110110110101001"},
	{AVKEY_0					,"101010110110110101010101001001"},
	{AVKEY_1					,"101010110110110110110101010101"},
	{AVKEY_2					,"101010110110110110110101001001"},
	{AVKEY_3					,"101010110110110110101010110101"},
	{AVKEY_4					,"101010110110110110101010101001"},
	{AVKEY_5					,"101010110110110110101001010101"},
	{AVKEY_6					,"101010110110110110101001001001"},
	{AVKEY_7					,"101010110110110101010110110101"},
	{AVKEY_8					,"101010110110110101010110101001"},
	{AVKEY_9					,"101010110110110101010101010101"},
	{AVKEY_MINUS				,"101010110110110101001010110101"},
	{AVKEY_HAT					,"101010110110110101001010101001"},
	{AVKEY_YEN					,"101010110110110101001001010101"},
	{AVKEY_BACKSPACE			,"101010110110110101001001001001"},

	{AVKEY_TAB					,"101010110110101010110110110101"},
	{AVKEY_Q					,"101010110110101010110110101001"},
	{AVKEY_W					,"101010110110101010110101010101"},
	{AVKEY_E					,"101010110110101010110101001001"},
	{AVKEY_R					,"101010110110101010101010110101"},
	{AVKEY_T					,"101010110110101010101010101001"},
	{AVKEY_Y					,"101010110110101010101001010101"},
	{AVKEY_U					,"101010110110101010101001001001"},
	{AVKEY_I					,"101010110110101001010110110101"},
	{AVKEY_O					,"101010110110101001010110101001"},
	{AVKEY_P					,"101010110110101001010101010101"},
	{AVKEY_AT					,"101010110110101001010101001001"},
	{AVKEY_LEFT_SQUARE_BRACKET	,"101010110110101001001010110101"},
	{AVKEY_RETURN				,"101010110110101001001010101001"},

	{AVKEY_CTRL					,"101010101010101010110101010101"},
	{AVKEY_A					,"101010110110101001001001010101"},
	{AVKEY_S					,"101010110110101001001001001001"},
	{AVKEY_D					,"101010110101010110110110110101"},
	{AVKEY_F					,"101010110101010110110110101001"},
	{AVKEY_G					,"101010110101010110110101010101"},
	{AVKEY_H					,"101010110101010110110101001001"},
	{AVKEY_J					,"101010110101010110101010110101"},
	{AVKEY_K					,"101010110101010110101010101001"},
	{AVKEY_L					,"101010110101010110101001010101"},
	{AVKEY_SEMICOLON			,"101010110101010110101001001001"},
	{AVKEY_COLON				,"101010110101010101010110110101"},
	{AVKEY_RIGHT_SQUARE_BRACKET	,"101010110101010101010110101001"},

	{AVKEY_LEFT_SHIFT			,"101010101010101010110101001001"},
	{AVKEY_Z					,"101010110101010101010101010101"},
	{AVKEY_X					,"101010110101010101010101001001"},
	{AVKEY_C					,"101010110101010101001010110101"},
	{AVKEY_V					,"101010110101010101001010101001"},
	{AVKEY_B					,"101010110101010101001001010101"},
	{AVKEY_N					,"101010110101010101001001001001"},
	{AVKEY_M					,"101010110101001010110110110101"},
	{AVKEY_COMMA				,"101010110101001010110110101001"},
	{AVKEY_DOT					,"101010110101001010110101010101"},
	{AVKEY_SLASH				,"101010110101001010110101001001"},
	{AVKEY_DOUBLE_QUOTE			,"101010110101001010101010110101"},
	{AVKEY_RIGHT_SHIFT			,"101010101010101010101010110101"},

	{AVKEY_CAPS					,"101010101010101010101010101001"},
	{AVKEY_GRAPH				,"101010101010101010101001010101"},
	{AVKEY_LEFT_SPACE			,"101010101010101010101001001001"},
	{AVKEY_MID_SPACE			,"101010101010101001010110110101"},
	{AVKEY_RIGHT_SPACE			,"101010110101001010101010101001"},
	{AVKEY_KANA					,"101010101010101001010101010101"},

	{AVKEY_NUM_STAR				,"101010110101001010101001010101"},
	{AVKEY_NUM_SLASH			,"101010110101001010101001001001"},
	{AVKEY_NUM_PLUS				,"101010110101001001010110110101"},
	{AVKEY_NUM_MINUS			,"101010110101001001010110101001"},
	{AVKEY_NUM_EQUAL			,"101010110101001001001010101001"},
	{AVKEY_NUM_COMMA			,"101010101010110110110110101001"},
	{AVKEY_NUM_RETURN			,"101010101010110110101010101001"},
	{AVKEY_NUM_DOT				,"101010101010110110101001001001"},
	{AVKEY_NUM_0				,"101010101010110110101001010101"},
	{AVKEY_NUM_1				,"101010101010110110110101010101"},
	{AVKEY_NUM_2				,"101010101010110110110101001001"},
	{AVKEY_NUM_3				,"101010101010110110101010110101"},
	{AVKEY_NUM_4				,"101010110101001001001001010101"},
	{AVKEY_NUM_5				,"101010110101001001001001001001"},
	{AVKEY_NUM_6				,"101010101010110110110110110101"},
	{AVKEY_NUM_7				,"101010110101001001010101010101"},
	{AVKEY_NUM_8				,"101010110101001001010101001001"},
	{AVKEY_NUM_9				,"101010110101001001001010110101"},
};



static KeyCodeAndLabel keyCodeLabelTable[]=
{
	{AVKEY_NULL,                     "NULL"},

	{AVKEY_BREAK,                    "BREAK"},
	{AVKEY_PF1,                      "PF1"},
	{AVKEY_PF2,                      "PF2"},
	{AVKEY_PF3,                      "PF3"},
	{AVKEY_PF4,                      "PF4"},
	{AVKEY_PF5,                      "PF5"},
	{AVKEY_PF6,                      "PF6"},
	{AVKEY_PF7,                      "PF7"},
	{AVKEY_PF8,                      "PF8"},
	{AVKEY_PF9,                      "PF9"},
	{AVKEY_PF10,                     "PF10"},
	{AVKEY_EL,                       "EL"},
	{AVKEY_CLS,                      "CLS"},
	{AVKEY_DUP,                      "DUP"},
	{AVKEY_HOME,                     "HOME"},
	{AVKEY_INS,                      "INS"},
	{AVKEY_DEL,                      "DEL"},
	{AVKEY_LEFT,                     "LEFT"},
	{AVKEY_RIGHT,                    "RIGHT"},
	{AVKEY_UP,                       "UP"},
	{AVKEY_DOWN,                     "DOWN"},

	{AVKEY_ESC,                      "ESC"},
	{AVKEY_0,                        "0"},
	{AVKEY_1,                        "1"},
	{AVKEY_2,                        "2"},
	{AVKEY_3,                        "3"},
	{AVKEY_4,                        "4"},
	{AVKEY_5,                        "5"},
	{AVKEY_6,                        "6"},
	{AVKEY_7,                        "7"},
	{AVKEY_8,                        "8"},
	{AVKEY_9,                        "9"},
	{AVKEY_MINUS,                    "MINUS"},
	{AVKEY_HAT,                      "HAT"},
	{AVKEY_YEN,                      "YEN"},
	{AVKEY_BACKSPACE,                "BACKSPACE"},
                                    
	{AVKEY_TAB,                      "TAB"},
	{AVKEY_Q,                        "Q"},
	{AVKEY_W,                        "W"},
	{AVKEY_E,                        "E"},
	{AVKEY_R,                        "R"},
	{AVKEY_T,                        "T"},
	{AVKEY_Y,                        "Y"},
	{AVKEY_U,                        "U"},
	{AVKEY_I,                        "I"},
	{AVKEY_O,                        "O"},
	{AVKEY_P,                        "P"},
	{AVKEY_AT,                       "AT"},
	{AVKEY_LEFT_SQUARE_BRACKET,      "LEFT_SQUARE_BRACKET"},
	{AVKEY_RETURN,                   "RETURN"},
                                    
	{AVKEY_CTRL,                     "CTRL"},
	{AVKEY_A,                        "A"},
	{AVKEY_S,                        "S"},
	{AVKEY_D,                        "D"},
	{AVKEY_F,                        "F"},
	{AVKEY_G,                        "G"},
	{AVKEY_H,                        "H"},
	{AVKEY_J,                        "J"},
	{AVKEY_K,                        "K"},
	{AVKEY_L,                        "L"},
	{AVKEY_SEMICOLON,                "SEMICOLON"},
	{AVKEY_COLON,                    "COLON"},
	{AVKEY_RIGHT_SQUARE_BRACKET,     "RIGHT_SQUARE_BRACKET"},
                                    
	{AVKEY_LEFT_SHIFT,               "LEFT_SHIFT"},
	{AVKEY_Z,                        "Z"},
	{AVKEY_X,                        "X"},
	{AVKEY_C,                        "C"},
	{AVKEY_V,                        "V"},
	{AVKEY_B,                        "B"},
	{AVKEY_N,                        "N"},
	{AVKEY_M,                        "M"},
	{AVKEY_COMMA,                    "COMMA"},
	{AVKEY_DOT,                      "DOT"},
	{AVKEY_SLASH,                    "SLASH"},
	{AVKEY_DOUBLE_QUOTE,             "DOUBLE_QUOTE"},
	{AVKEY_RIGHT_SHIFT,              "RIGHT_SHIFT"},
                                    
	{AVKEY_CAPS,                     "CAPS"},
	{AVKEY_GRAPH,                    "GRAPH"},
	{AVKEY_LEFT_SPACE,               "LEFT_SPACE"},
	{AVKEY_MID_SPACE,                "MID_SPACE"},
	{AVKEY_RIGHT_SPACE,              "RIGHT_SPACE"},
	{AVKEY_KANA,                     "KANA"},
                                    
	{AVKEY_NUM_STAR,                 "NUM_STAR"},
	{AVKEY_NUM_SLASH,                "NUM_SLASH"},
	{AVKEY_NUM_PLUS,                 "NUM_PLUS"},
	{AVKEY_NUM_MINUS,                "NUM_MINUS"},
	{AVKEY_NUM_EQUAL,                "NUM_EQUAL"},
	{AVKEY_NUM_COMMA,                "NUM_COMMA"},
	{AVKEY_NUM_RETURN,               "NUM_RETURN"},
	{AVKEY_NUM_DOT,                  "NUM_DOT"},
	{AVKEY_NUM_0,                    "NUM_0"},
	{AVKEY_NUM_1,                    "NUM_1"},
	{AVKEY_NUM_2,                    "NUM_2"},
	{AVKEY_NUM_3,                    "NUM_3"},
	{AVKEY_NUM_4,                    "NUM_4"},
	{AVKEY_NUM_5,                    "NUM_5"},
	{AVKEY_NUM_6,                    "NUM_6"},
	{AVKEY_NUM_7,                    "NUM_7"},
	{AVKEY_NUM_8,                    "NUM_8"},
	{AVKEY_NUM_9,                    "NUM_9"},

	{0,NULL}
};


int FM77AVKeyLabelToKeyCode(std::string str)
{
	cpputil::Capitalize(str);
	for(auto &l : keyCodeLabelTable)
	{
		if(nullptr!=l.label)
		{
			int len=strlen(l.label);
			if(0==strncmp(str.c_str(),l.label,len) && (0==str[len] || '\\'==str[len] || 0x0d==str[len] || 0x0a==str[len]))
			{
				return l.code;
			}
		}
	}
	return 0;
}
std::string FM77AVKeyCodeToKeyLabel(int fm77avkey)
{
	for(auto &l : keyCodeLabelTable)
	{
		if(fm77avkey==l.code)
		{
			return l.label;
		}
	}
	return "?";
}




typedef struct KeyTranslationTableStruct
{
	unsigned char c;
	FM77AVKeyCombination comb;
} KeyTranslationTable;

static const KeyTranslationTable keyTranslationTableSource[]=
{
	{'\t',{AVKEY_TAB,           false,false,false}},
	{' ', {AVKEY_MID_SPACE,     false,false,false}},		// 32
	{'!', {AVKEY_1,             true,false,false}},
	{'\"',{AVKEY_DOUBLE_QUOTE,  false,false,false}},
	{'#', {AVKEY_3,             true,false,false}},
	{'$', {AVKEY_4,             true,false,false}},
	{'%', {AVKEY_5,             true,false,false}},
	{'&', {AVKEY_6,             true,false,false}},
	{'\'',{AVKEY_7,             true,false,false}},
	{'(', {AVKEY_8,             true,false,false}},
	{')', {AVKEY_9,             true,false,false}},
	{'*', {AVKEY_NUM_STAR,      false,false,false}},
	{'+', {AVKEY_NUM_PLUS,      false,false,false}},
	{',', {AVKEY_NUM_COMMA,     false,false,false}},
	{'-', {AVKEY_NUM_MINUS,     false,false,false}},
	{'.', {AVKEY_NUM_DOT,       false,false,false}},
	{'/', {AVKEY_NUM_SLASH,     false,false,false}},
	{'0', {AVKEY_NUM_0,         false,false,false}},		// 48
	{'1', {AVKEY_NUM_1,         false,false,false}},
	{'2', {AVKEY_NUM_2,         false,false,false}},
	{'3', {AVKEY_NUM_3,         false,false,false}},
	{'4', {AVKEY_NUM_4,         false,false,false}},
	{'5', {AVKEY_NUM_5,         false,false,false}},
	{'6', {AVKEY_NUM_6,         false,false,false}},
	{'7', {AVKEY_NUM_7,         false,false,false}},
	{'8', {AVKEY_NUM_8,         false,false,false}},
	{'9', {AVKEY_NUM_9,         false,false,false}},
	{':', {AVKEY_COLON,         false,false,false}},
	{';', {AVKEY_SEMICOLON,     false,false,false}},
{'<', {AVKEY_COMMA,true,false,false}},
	{'=', {AVKEY_NUM_EQUAL,     false,false,false}},
{'>', {AVKEY_DOT,true,false,false}},
	{'?', {AVKEY_SLASH,         true,false,false}},
	{'@', {AVKEY_AT,            false,false,false}},		//64
	{'A', {AVKEY_A,             true,false,false}},
	{'B', {AVKEY_B,             true,false,false}},
	{'C', {AVKEY_C,             true,false,false}},
	{'D', {AVKEY_D,             true,false,false}},
	{'E', {AVKEY_E,             true,false,false}},
	{'F', {AVKEY_F,             true,false,false}},
	{'G', {AVKEY_G,             true,false,false}},
	{'H', {AVKEY_H,             true,false,false}},
	{'I', {AVKEY_I,             true,false,false}},
	{'J', {AVKEY_J,             true,false,false}},
	{'K', {AVKEY_K,             true,false,false}},
	{'L', {AVKEY_L,             true,false,false}},
	{'M', {AVKEY_M,             true,false,false}},
	{'N', {AVKEY_N,             true,false,false}},
	{'O', {AVKEY_O,             true,false,false}},
	{'P', {AVKEY_P,             true,false,false}},			//80
	{'Q', {AVKEY_Q,             true,false,false}},
	{'R', {AVKEY_R,             true,false,false}},
	{'S', {AVKEY_S,             true,false,false}},
	{'T', {AVKEY_T,             true,false,false}},
	{'U', {AVKEY_U,             true,false,false}},
	{'V', {AVKEY_V,             true,false,false}},
	{'W', {AVKEY_W,             true,false,false}},
	{'X', {AVKEY_X,             true,false,false}},
	{'Y', {AVKEY_Y,             true,false,false}},
	{'Z', {AVKEY_Z,             true,false,false}},
	{'[', {AVKEY_LEFT_SQUARE_BRACKET,false,false,false}},
	{'\\',{AVKEY_YEN,           false,false,false}},
	{']', {AVKEY_RIGHT_SQUARE_BRACKET,false,false,false}},
	{'^', {AVKEY_HAT,           false,false,false}},
{'_', {AVKEY_DOUBLE_QUOTE,true,false,false}},
{'`', {AVKEY_AT,true,false,false}},  // 96
	{'a', {AVKEY_A,             false,false,false}},
	{'b', {AVKEY_B,             false,false,false}},
	{'c', {AVKEY_C,             false,false,false}},
	{'d', {AVKEY_D,             false,false,false}},
	{'e', {AVKEY_E,             false,false,false}},
	{'f', {AVKEY_F,             false,false,false}},
	{'g', {AVKEY_G,             false,false,false}},
	{'h', {AVKEY_H,             false,false,false}},
	{'i', {AVKEY_I,             false,false,false}},
	{'j', {AVKEY_J,             false,false,false}},
	{'k', {AVKEY_K,             false,false,false}},
	{'l', {AVKEY_L,             false,false,false}},
	{'m', {AVKEY_M,             false,false,false}},
	{'n', {AVKEY_N,             false,false,false}},
	{'o', {AVKEY_O,             false,false,false}},
	{'p', {AVKEY_P,             false,false,false}},
	{'q', {AVKEY_Q,             false,false,false}},
	{'r', {AVKEY_R,             false,false,false}},
	{'s', {AVKEY_S,             false,false,false}},
	{'t', {AVKEY_T,             false,false,false}},
	{'u', {AVKEY_U,             false,false,false}},
	{'v', {AVKEY_V,             false,false,false}},
	{'w', {AVKEY_W,             false,false,false}},
	{'x', {AVKEY_X,             false,false,false}},
	{'y', {AVKEY_Y,             false,false,false}},
	{'z', {AVKEY_Z,             false,false,false}},
{'{', {AVKEY_LEFT_SQUARE_BRACKET,true,false,false}},
	{'|', {AVKEY_YEN,           true,false,false}},
{'}', {AVKEY_RIGHT_SQUARE_BRACKET,true,false,false}},
	{'~', {AVKEY_HAT,           true,false,false}},


/*	{'', {AVKEY_,             false,false,false}},
	{'', {AVKEY_,             false,false,false}},
*/
};
static const KeyTranslationTable keyTranslationTableSourceAdditional[]= // Additional table for reverse mapping.
{
	{0x01,{AVKEY_A,             false,true,false}},
	{0x02,{AVKEY_B,             false,true,false}},
	{0x03,{AVKEY_C,             false,true,false}},
	{0x04,{AVKEY_D,             false,true,false}},
	{0x05,{AVKEY_E,             false,true,false}},
	{0x06,{AVKEY_F,             false,true,false}},
	{0x07,{AVKEY_G,             false,true,false}},
	{0x08,{AVKEY_H,             false,true,false}},
	{0x09,{AVKEY_I,             false,true,false}},
	{0x0A,{AVKEY_J,             false,true,false}},
	{0x0B,{AVKEY_K,             false,true,false}},
	{0x0C,{AVKEY_L,             false,true,false}},
	{0x0D,{AVKEY_M,             false,true,false}},
	{0x0E,{AVKEY_N,             false,true,false}},
	{0x0F,{AVKEY_O,             false,true,false}},
	{0x10,{AVKEY_P,             false,true,false}},
	{0x11,{AVKEY_Q,             false,true,false}},
	{0x12,{AVKEY_R,             false,true,false}},
	{0x13,{AVKEY_S,             false,true,false}},
	{0x14,{AVKEY_T,             false,true,false}},
	{0x15,{AVKEY_U,             false,true,false}},
	{0x16,{AVKEY_V,             false,true,false}},
	{0x17,{AVKEY_W,             false,true,false}},
	{0x18,{AVKEY_X,             false,true,false}},
	{0x19,{AVKEY_Y,             false,true,false}},
	{0x1A,{AVKEY_Z,             false,true,false}},

	{'0', {AVKEY_NUM_0,         true ,false,false}},
	{'1', {AVKEY_NUM_1,         true ,false,false}},
	{'2', {AVKEY_NUM_2,         true ,false,false}},
	{'3', {AVKEY_NUM_3,         true ,false,false}},
	{'4', {AVKEY_NUM_4,         true ,false,false}},
	{'5', {AVKEY_NUM_5,         true ,false,false}},
	{'6', {AVKEY_NUM_6,         true ,false,false}},
	{'7', {AVKEY_NUM_7,         true ,false,false}},
	{'8', {AVKEY_NUM_8,         true ,false,false}},
	{'9', {AVKEY_NUM_9,         true ,false,false}},

	{',', {AVKEY_COMMA,         false,false,false}},
	{'\"',{AVKEY_2,             true ,false,false}},
	{'*', {AVKEY_COLON,         true ,false,false}},
	{'+', {AVKEY_SEMICOLON,     true ,false,false}},
	{'-', {AVKEY_MINUS,         false,false,false}},
	{'=', {AVKEY_MINUS,         true ,false,false}},
	{'/', {AVKEY_SLASH,         false,false,false}},
	{'?', {AVKEY_SLASH,         true ,false,false}},
	{'0', {AVKEY_0,             false,false,false}},
	{'1', {AVKEY_1,             false,false,false}},
	{'2', {AVKEY_2,             false,false,false}},
	{'3', {AVKEY_3,             false,false,false}},
	{'4', {AVKEY_4,             false,false,false}},
	{'5', {AVKEY_5,             false,false,false}},
	{'6', {AVKEY_6,             false,false,false}},
	{'7', {AVKEY_7,             false,false,false}},
	{'8', {AVKEY_8,             false,false,false}},
	{'9', {AVKEY_9,             false,false,false}},

	{0x05,{AVKEY_EL,            false,false,false}},
	{0x05,{AVKEY_EL,            true ,false,false}},
	{0x0B,{AVKEY_HOME,          false,false,false}},
	{0x0B,{AVKEY_HOME,          true ,false,false}},
	{0x0C,{AVKEY_CLS,           false,false,false}},
	{0x0C,{AVKEY_CLS,           true ,false,false}},
	{0x11,{AVKEY_DUP,           false,false,false}},
	{0x11,{AVKEY_DUP,           true ,false,false}},
	{0x12,{AVKEY_INS,           false,false,false}},
	{0x12,{AVKEY_INS,           true ,false,false}},
	{0x7F,{AVKEY_DEL,           false,false,false}},
	{0x7F,{AVKEY_DEL,           true ,false,false}},
	{0x1B,{AVKEY_ESC,           false,false,false}},
	{0x1B,{AVKEY_ESC,           true ,false,false}},
	{0x1C,{AVKEY_RIGHT,         false,false,false}},
	{0x1D,{AVKEY_LEFT,          false,false,false}},
	{0x1E,{AVKEY_UP,            false,false,false}},
	{0x1F,{AVKEY_DOWN,          false,false,false}},
	{0x06,{AVKEY_RIGHT,         true ,false,false}},
	{0x02,{AVKEY_LEFT,          true ,false,false}},
	{0x19,{AVKEY_UP,            true ,false,false}},
	{0x1A,{AVKEY_DOWN,          true ,false,false}},
	{0x08,{AVKEY_BACKSPACE,     false,false,false}},
	{0x08,{AVKEY_BACKSPACE,     true ,false,false}},
	{0x0D,{AVKEY_RETURN,        false,false,false}},
	{0x0D,{AVKEY_RETURN,        true ,false,false}},
};

static const KeyTranslationTable keyTranslationTableSourceGraph[]= // Additional table for reverse mapping.
{
	{'\t',{AVKEY_TAB,           false,false,true}},
	{' ', {AVKEY_MID_SPACE,     false,false,true}},		// 32
	{0xF9,{AVKEY_1,             false,false,true}},
	{0xFA,{AVKEY_2,             false,false,true}},
	{0xFB,{AVKEY_3,             false,false,true}},
	{0xFC,{AVKEY_4,             false,false,true}},
	{0xF2,{AVKEY_5,             false,false,true}},
	{0xF3,{AVKEY_6,             false,false,true}},
	{0xF4,{AVKEY_7,             false,false,true}},
	{0xF5,{AVKEY_8,             false,false,true}},
	{0xF6,{AVKEY_9,             false,false,true}},
	{0xF7,{AVKEY_0,             false,false,true}},
	{0x8C,{AVKEY_MINUS,         false,false,true}},
	{0x8B,{AVKEY_HAT,           false,false,true}},
	{0xF1,{AVKEY_YEN,           false,false,true}},

	{0xFD,{AVKEY_Q,             false,false,true}},
	{0xF8,{AVKEY_W,             false,false,true}},
	{0xE4,{AVKEY_E,             false,false,true}},
	{0xE5,{AVKEY_R,             false,false,true}},
	{0x9C,{AVKEY_T,             false,false,true}},
	{0x9D,{AVKEY_Y,             false,false,true}},
	{0xF0,{AVKEY_U,             false,false,true}},
	{0xE8,{AVKEY_I,             false,false,true}},
	{0xE9,{AVKEY_O,             false,false,true}},
	{0x8D,{AVKEY_P,             false,false,true}},			//80
	{0x8A,{AVKEY_AT,            false,false,true}},		//64
	{0xED,{AVKEY_LEFT_SQUARE_BRACKET,false,false,true}},

	{0x95,{AVKEY_A,             false,false,true}},
	{0x96,{AVKEY_S,             false,false,true}},
	{0xE6,{AVKEY_D,             false,false,true}},
	{0xE7,{AVKEY_F,             false,false,true}},
	{0x9E,{AVKEY_G,             false,false,true}},
	{0x9F,{AVKEY_H,             false,false,true}},
	{0xEA,{AVKEY_J,             false,false,true}},
	{0xEB,{AVKEY_K,             false,false,true}},
	{0x8E,{AVKEY_L,             false,false,true}},
	{0x89,{AVKEY_SEMICOLON,     false,false,true}},
	{0x94,{AVKEY_COLON,         false,false,true}},
	{0xEC,{AVKEY_RIGHT_SQUARE_BRACKET,false,false,true}},

	{0x80,{AVKEY_Z,             false,false,true}},
	{0x81,{AVKEY_X,             false,false,true}},
	{0x82,{AVKEY_C,             false,false,true}},
	{0x83,{AVKEY_V,             false,false,true}},
	{0x84,{AVKEY_B,             false,false,true}},
	{0x85,{AVKEY_N,             false,false,true}},
	{0x86,{AVKEY_M,             false,false,true}},
	{0x87,{AVKEY_COMMA,         false,false,true}},
	{0x88,{AVKEY_DOT,           false,false,true}},
	{0x97,{AVKEY_SLASH,         false,false,true}},
	{0xE0,{AVKEY_DOUBLE_QUOTE,  false,false,true}},

	{0x98,{AVKEY_NUM_STAR,      false,false,true}},
	{0x99,{AVKEY_NUM_PLUS,      false,false,true}},
	{0xEE,{AVKEY_NUM_MINUS,     false,false,true}},
	{0x91,{AVKEY_NUM_SLASH,     false,false,true}},
	{0x9A,{AVKEY_NUM_1,         false,false,true}},
	{0x90,{AVKEY_NUM_2,         false,false,true}},
	{0x9B,{AVKEY_NUM_3,         false,false,true}},
	{0x93,{AVKEY_NUM_4,         false,false,true}},
	{0x8F,{AVKEY_NUM_5,         false,false,true}},
	{0x92,{AVKEY_NUM_6,         false,false,true}},
	{0xE1,{AVKEY_NUM_7,         false,false,true}},
	{0xE2,{AVKEY_NUM_8,         false,false,true}},
	{0xE3,{AVKEY_NUM_9,         false,false,true}},
	{0xEF,{AVKEY_NUM_EQUAL,     false,false,true}},
};

static bool ForwardTableMade=false;
static FM77AVKeyCombination keyForwardTranslationTable[256];

FM77AVKeyCombination FM77AVTranslateCharToCode(unsigned char c)
{
	if(true!=ForwardTableMade)
	{
		for(auto &k : keyForwardTranslationTable)
		{
			k.keyCode=AVKEY_NULL;
		}
		for(auto &tab : keyTranslationTableSource)
		{
			keyForwardTranslationTable[tab.c]=tab.comb;
		}
		ForwardTableMade=true;
	}

	if(0!=c)
	{
		printf("Translateing: %c\n",c);
	}

	return keyForwardTranslationTable[c];
}

static bool ReverseTableMade=false;
static unsigned char keyReverseTranslationTable[AVKEY_NUM_KEYCODE];
static unsigned char keyReverseTranslationTableShift[AVKEY_NUM_KEYCODE];
static unsigned char keyReverseTranslationTableCtrl[AVKEY_NUM_KEYCODE];
static unsigned char keyReverseTranslationTableGraph[AVKEY_NUM_KEYCODE];

static void RegisterKeyReverseMapping(struct KeyTranslationTableStruct x)
{
	if(true==x.comb.shift)
	{
		keyReverseTranslationTableShift[x.comb.keyCode]=x.c;
	}
	else if(true==x.comb.ctrl)
	{
		keyReverseTranslationTableCtrl[x.comb.keyCode]=x.c;
	}
	else if(true==x.comb.graph)
	{
		keyReverseTranslationTableGraph[x.comb.keyCode]=x.c;
	}
	else
	{
		keyReverseTranslationTable[x.comb.keyCode]=x.c;
	}
}

uint16_t FM77AVTranslateKeyCombinationToChar(FM77AVKeyCombination keyComb)
{
	if(true!=ReverseTableMade)
	{
		for(int k=0; k<AVKEY_NUM_KEYCODE; ++k)
		{
			keyReverseTranslationTable[k]=0;
			keyReverseTranslationTableShift[k]=0;
			keyReverseTranslationTableCtrl[k]=0;
			keyReverseTranslationTableGraph[k]=0;
		}
		for(int i=0; i<2; ++i)
		{
			for(auto x : keyTranslationTableSource)
			{
				RegisterKeyReverseMapping(x);
			}
			for(auto x : keyTranslationTableSourceAdditional)
			{
				RegisterKeyReverseMapping(x);
			}
			for(auto x: keyTranslationTableSourceGraph)
			{
				RegisterKeyReverseMapping(x);
			}
		}
		ReverseTableMade=true;
	}
	if(AVKEY_PF1<=keyComb.keyCode && keyComb.keyCode<=AVKEY_PF10)
	{
		return 0x100|(1+keyComb.keyCode-AVKEY_PF1);
	}
	else if(true==keyComb.ctrl)
	{
		return keyReverseTranslationTableCtrl[keyComb.keyCode];
	}
	else if(true==keyComb.shift)
	{
		return keyReverseTranslationTableShift[keyComb.keyCode];
	}
	else if(true==keyComb.graph)
	{
		return keyReverseTranslationTableGraph[keyComb.keyCode];
	}
	return keyReverseTranslationTable[keyComb.keyCode];
}

const char *FM77AVGetKeyPress30BitPattern(int fm77avkey)
{
	for(auto &i : bitPattern)
	{
		if(i.code==fm77avkey)
		{
			return i.label;
		}
	}
	return nullptr;
}

std::string FM77AVGetKeyRelease30BitPattern(int fm77avkey)
{
	std::string ptn;
	auto pressPtn=FM77AVGetKeyPress30BitPattern(fm77avkey);
	if(nullptr!=pressPtn)
	{
		/* Thanks to Mr. Kobayashi from Classic PC & Retro Game Club JAPAN
		   for calculation of the key-release code.
		*/

		auto firstByte=FM77AVGet1stByteIn30bitEncoding(pressPtn);
		firstByte-=0x60;
		firstByte&=255;

		char code30[31];
		for(int i=0; i<31; ++i)
		{
			code30[i]=pressPtn[i];
		}
		FM77AVPut1stByteIn30bitEncoding(code30,firstByte);
		ptn=code30;
	}
	return ptn;
}

std::string FM77AVMake30BitPattern(const FM77AVKeyEvent keyEvent)
{
	std::string str;

	if(true==keyEvent.pressEvent)
	{
		return FM77AVGetKeyPress30BitPattern(keyEvent.keyCode);
	}
	else
	{
		return FM77AVGetKeyRelease30BitPattern(keyEvent.keyCode);
	}
}

/* Repeat every 3rd of 4-byte groups.
   The raw pulse looks to be going 100us,125us,175us.  Thanks Mr. Kobayashi of Classic PC and Retro Gaming JAPAN!
   But, approximating it with 100us 100us 200us looks to work just fine.
*/
void FM77AV30bitTo40bit(char code40[41],const char code30[])
{
	int k=0;
	for(int i=0; i<30; i+=3)
	{
		code40[k++]=code30[i];
		code40[k++]=code30[i+1];
		code40[k++]=code30[i+2];
		code40[k++]=code30[i+2];
	}
	code40[40]=0;
	if(40!=k)
	{
		printf("Internal error!\n");
		printf("Did I count correctly?\n");
		exit(1);
	}
}

unsigned int FM77AVGet1stByteIn30bitEncoding(const char code30[])
{
	unsigned int sum=0;
	unsigned int bitShift=0x80;
	for(int i=4; i<12; ++i)
	{
		if('1'==code30[i])
		{
			sum+=bitShift;
		}
		bitShift>>=1;
	}
	return sum;
}

void FM77AVPut1stByteIn30bitEncoding(char code30[],unsigned int byteData)
{
	unsigned int bitShift=0x80;
	for(int i=4; i<12; ++i)
	{
		if(0!=(byteData&bitShift))
		{
			code30[i]='1';
		}
		else
		{
			code30[i]='0';
		}
		bitShift>>=1;
	}
}

