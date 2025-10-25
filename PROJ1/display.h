/*
 * display.h
 *
 * Created: 13/10/2025 14:06:15
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
// Define as o número dos pinos de E e RS na porta F

// void limpa_display ();
// void inicializa_display();
// void escreve_dado_char_lcd (char dado);
// void escreve_string_lcd(char *texto);

void LCD_Init(void);
void LCD_String(char *str);
void LCD_String_xy(char row, char pos, char *str);
void LCD_Clear();

#endif /* DISPLAY_H_ */