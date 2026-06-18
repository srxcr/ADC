#include <pic18.h>

void delay(){
    int i, j;
    for (i = 0; i < 250; i++) {
        for (j = 0; j < 250; j++) {
        }
    }
}

void command(int cmd) {
    LATD = cmd;
    RC0 = 0;
    RC1 = 1;
    delay();
    RC1 = 0;
}

void data(int data) {
    LATD = data;
    RC0 = 1;
    RC1 = 1;
    delay();
    RC1 = 0;
}

void main(void) {
    char a, b, c, d;

    TRISD = 0x00;
    TRISA = 0xFF;
    TRISC = 0x00;

    ADCON0 = 0x01;
    ADCON1 = 0x00;

    delay();

    command(0x38);
    command(0x80);
    command(0x06);
    command(0x0E);
    command(0x01);

    GODONE = 1;

    while (GODONE == 1) {
        a = ADRESH;

        b = ((a / 100) + 48);
        data(b);

        c = (((a % 100) / 10) + 48);
        data(c);

        d = ((a % 10) + 48);
        data(d);

        delay();
        command(0x01);
    }

    return;
}
