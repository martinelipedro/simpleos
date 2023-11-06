#include "keyboard.h"

#include <stdint.h>
#include "vga.h"
#include "isr.h"
#include "util.h"

static void keyboard_callback(int_registers_T regs);

void init_keyboard(void)
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}

void print_letter(uint8_t scancode)
{
    switch (scancode)
    {
    case 0x0:
        vga_puts("ERROR");
        break;
    case 0x1:
        vga_puts("ESC");
        break;
    case 0x2:
        vga_puts("1");
        break;
    case 0x3:
        vga_puts("2");
        break;
    case 0x4:
        vga_puts("3");
        break;
    case 0x5:
        vga_puts("4");
        break;
    case 0x6:
        vga_puts("5");
        break;
    case 0x7:
        vga_puts("6");
        break;
    case 0x8:
        vga_puts("7");
        break;
    case 0x9:
        vga_puts("8");
        break;
    case 0x0A:
        vga_puts("9");
        break;
    case 0x0B:
        vga_puts("0");
        break;
    case 0x0C:
        vga_puts("-");
        break;
    case 0x0D:
        vga_puts("+");
        break;
    case 0x0E:
        vga_puts("Backspace");
        break;
    case 0x0F:
        vga_puts("Tab");
        break;
    case 0x10:
        vga_puts("Q");
        break;
    case 0x11:
        vga_puts("W");
        break;
    case 0x12:
        vga_puts("E");
        break;
    case 0x13:
        vga_puts("R");
        break;
    case 0x14:
        vga_puts("T");
        break;
    case 0x15:
        vga_puts("Y");
        break;
    case 0x16:
        vga_puts("U");
        break;
    case 0x17:
        vga_puts("I");
        break;
    case 0x18:
        vga_puts("O");
        break;
    case 0x19:
        vga_puts("P");
        break;
    case 0x1A:
        vga_puts("[");
        break;
    case 0x1B:
        vga_puts("]");
        break;
    case 0x1C:
        vga_puts("ENTER");
        break;
    case 0x1D:
        vga_puts("LCtrl");
        break;
    case 0x1E:
        vga_puts("A");
        break;
    case 0x1F:
        vga_puts("S");
        break;
    case 0x20:
        vga_puts("D");
        break;
    case 0x21:
        vga_puts("F");
        break;
    case 0x22:
        vga_puts("G");
        break;
    case 0x23:
        vga_puts("H");
        break;
    case 0x24:
        vga_puts("J");
        break;
    case 0x25:
        vga_puts("K");
        break;
    case 0x26:
        vga_puts("L");
        break;
    case 0x27:
        vga_puts(";");
        break;
    case 0x28:
        vga_puts("'");
        break;
    case 0x29:
        vga_puts("`");
        break;
    case 0x2A:
        vga_puts("LShift");
        break;
    case 0x2B:
        vga_puts("\\");
        break;
    case 0x2C:
        vga_puts("Z");
        break;
    case 0x2D:
        vga_puts("X");
        break;
    case 0x2E:
        vga_puts("C");
        break;
    case 0x2F:
        vga_puts("V");
        break;
    case 0x30:
        vga_puts("B");
        break;
    case 0x31:
        vga_puts("N");
        break;
    case 0x32:
        vga_puts("M");
        break;
    case 0x33:
        vga_puts(",");
        break;
    case 0x34:
        vga_puts(".");
        break;
    case 0x35:
        vga_puts("/");
        break;
    case 0x36:
        vga_puts("Rshift");
        break;
    case 0x37:
        vga_puts("Keypad *");
        break;
    case 0x38:
        vga_puts("LAlt");
        break;
    case 0x39:
        vga_puts("Spc");
        break;
    default:
        break;
    }
}

static void keyboard_callback(int_registers_T regs)
{
    uint8_t scancode = inb(0x60);
    print_letter(scancode);
}