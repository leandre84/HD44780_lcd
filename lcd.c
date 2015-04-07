/*
 * lcd.c
 * Drive a 16x2 HD44780 display in 4bit addressing mode using wiringPi
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <lcd.h>

#define LCD_RS 3
#define LCD_E 2
#define LCD_D0 0
#define LCD_D1 7 
#define LCD_D2 9 
#define LCD_D3 8 

void usage(char *progname, FILE *stream);

int main(int argc, char **argv) {

    int optopt = -1;
    char *token = NULL;
    unsigned int value = 0;
    char *strtolep = NULL;
    volatile unsigned int i = 0;

    int lcd_handle = -1;
    char *lcd_line1 = NULL;
    char *lcd_line2 = NULL;

    /* default values for lcdInit */
    unsigned int lcd_rs = LCD_RS;
    unsigned int lcd_e = LCD_E;
    unsigned int lcd_d0 = LCD_D0;
    unsigned int lcd_d1 = LCD_D1;
    unsigned int lcd_d2 = LCD_D2;
    unsigned int lcd_d3 = LCD_D3;

    while ((optopt = getopt(argc, argv, "i:1:2:h")) != -1) {
        switch (optopt) {
            case 'i':
                token = strtok(optarg, " ");
                while (token != NULL) {
                    i++;
                    value = strtoul(token, &strtolep, 10);
                    if (strlen(strtolep) > 0) {
                        fprintf(stderr, "%s: non numeric argument found\n", argv[0]);
                        usage(argv[0], stderr);
                    }
                    if (i == 1) {
                        lcd_rs = value;
                    } else if (i == 2) {
                        lcd_e = value;
                    } else if (i == 3) {
                        lcd_d0 = value;
                    } else if (i == 4) {
                        lcd_d1 = value;
                    } else if (i == 5) {
                        lcd_d2 = value;
                    } else if (i == 6) {
                        lcd_d3 = value;
                    } else {
                        usage(argv[0], stderr);
                    }
                    token = strtok(NULL, " ");
                }
                break;
            case '1':
                lcd_line1 = optarg;
                break;
            case '2':
                lcd_line2 = optarg;
                break;
            case 'h':
                usage(argv[0], stdout);
                return 0;
            default:
                usage(argv[0], stderr);
                return 1;
        }
    }

    if (lcd_line1 == NULL && lcd_line2 == NULL) {
        fprintf(stderr, "%s: either -1 or -2 must be given\n", argv[0]);
        usage(argv[0], stderr);
        return 2;
    }

    wiringPiSetup();

    /* rows, cols, shift register bits, rs, strb(=e), d0, d1, ... */
    lcd_handle = lcdInit(2, 16, 4, lcd_rs, lcd_e, lcd_d0, lcd_d1, lcd_d2, lcd_d3, 0, 0, 0, 0);

    if (lcd_handle < 0) {
        fprintf(stderr, "%s: lcdInit failed\n", argv[0]);
        return 3;
    }

    if (lcd_line1 != NULL) {
        lcdPosition(lcd_handle, 0, 0);
        lcdPuts(lcd_handle, lcd_line1);
    }

    if (lcd_line2 != NULL) {
        lcdPosition(lcd_handle, 0, 1);
        lcdPuts(lcd_handle, lcd_line2);
    }

    return 0;
}

void usage(char *progname, FILE *stream) {
    fprintf(stream, "%s: usage: \n", progname);
    fprintf(stream, "%s:    -i [a b c d e f]: initialize using wiringPi pin# for RS, E, DATA0-DATA3. Default: \"%d %d %d %d %d %d\"\n",
            progname, LCD_RS, LCD_E, LCD_D0, LCD_D1, LCD_D2, LCD_D3);
    fprintf(stream, "%s:    -1 [text]: display 16 chars of text in line 1\n", progname);
    fprintf(stream, "%s:    -2 [text]: display 16 chars of text in line 2\n", progname);
    fprintf(stream, "%s:    -h: display this usage help\n", progname);
}
