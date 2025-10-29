#ifndef DISPLAY_H_
#define DISPLAY_H_

void LCD_Init(void);
void LCD_String(char *str);
void LCD_String_xy(char row, char pos, char *str);
void LCD_Clear();

#endif /* DISPLAY_H_ */