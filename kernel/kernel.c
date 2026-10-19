#include <stddef.h>
#include <stdint.h>

#define W 80
#define H 25
#define VGA ((volatile uint16_t*)0xB8000)

/* Hybrid's first dark UI: black canvas, cyan/seafoam accents, restrained chrome. */
#define C_BG       0x00
#define C_TEXT     0x0F
#define C_ACCENT   0x0B
#define C_MUTED    0x08
#define C_PANEL    0x07
#define C_SELECTED 0x03

static volatile uint16_t *const vga = VGA;
static char line[64];
static size_t line_len;
static char output[72] = "Kernel initialized. Type help to explore the prototype.";
static int shift;

static inline void outb(uint16_t p, uint8_t v) { __asm__ volatile ("outb %0, %1" : : "a"(v), "Nd"(p)); }
static inline uint8_t inb(uint16_t p) { uint8_t v; __asm__ volatile ("inb %1, %0" : "=a"(v) : "Nd"(p)); return v; }

static uint16_t make_cell(char ch, uint8_t color) { return ((uint16_t)color << 8) | (uint8_t)ch; }

static void put_at(size_t x, size_t y, char ch, uint8_t color) {
    if (x < W && y < H) vga[y * W + x] = make_cell(ch, color);
}

static void text_at(size_t x, size_t y, const char *s, uint8_t color) {
    while (*s && x < W) put_at(x++, y, *s++, color);
}

static void fill_row(size_t y, char ch, uint8_t color) {
    for (size_t x = 0; x < W; ++x) put_at(x, y, ch, color);
}

static void hline(size_t x, size_t y, size_t width, char ch, uint8_t color) {
    for (size_t i = 0; i < width && x + i < W; ++i) put_at(x + i, y, ch, color);
}

static int eq(const char *a, const char *b) {
    size_t i = 0;
    while (a[i] && b[i] && a[i] == b[i]) ++i;
    return a[i] == b[i];
}

static void panel(size_t x, size_t y, size_t w, size_t h, const char *title) {
    put_at(x, y, '+', C_MUTED);
    put_at(x + w - 1, y, '+', C_MUTED);
    put_at(x, y + h - 1, '+', C_MUTED);
    put_at(x + w - 1, y + h - 1, '+', C_MUTED);
    hline(x + 1, y, w - 2, '-', C_MUTED);
    hline(x + 1, y + h - 1, w - 2, '-', C_MUTED);
    for (size_t r = y + 1; r < y + h - 1; ++r) {
        put_at(x, r, '|', C_MUTED);
        put_at(x + w - 1, r, '|', C_MUTED);
        hline(x + 1, r, w - 2, ' ', C_BG);
    }
    text_at(x + 2, y, title, C_ACCENT);
}

static void draw_screen(void) {
    for (size_t y = 0; y < H; ++y) fill_row(y, ' ', C_BG);

    fill_row(0, ' ', C_SELECTED);
    text_at(2, 0, "HYBRID", C_TEXT);
    text_at(10, 0, "SYSTEM CONTROL CENTER", C_TEXT);
    text_at(59, 0, "KERNEL // 0.1", C_TEXT);
    hline(0, 1, W, '=', C_ACCENT);

    text_at(2, 2, "YOUR MACHINE. YOUR OPERATING SYSTEMS. YOUR CHOICE.", C_TEXT);
    text_at(2, 3, "Independent x86 kernel prototype  /  DARK INTERFACE", C_MUTED);

    panel(1, 5, 25, 6, "SYSTEM");
    text_at(3, 7, "Kernel", C_MUTED);
    text_at(12, 7, "RUNNING", C_ACCENT);
    text_at(3, 8, "Architecture", C_MUTED);
    text_at(16, 8, "x86 / 32-bit", C_TEXT);
    text_at(3, 9, "Boot mode", C_MUTED);
    text_at(14, 9, "Legacy BIOS", C_TEXT);

    panel(27, 5, 25, 6, "COMPONENTS");
    text_at(29, 7, "Memory manager", C_MUTED);
    text_at(29, 8, "Filesystem", C_MUTED);
    text_at(29, 9, "Graphics desktop", C_MUTED);
    text_at(44, 7, "PLANNED", C_ACCENT);
    text_at(44, 8, "PLANNED", C_ACCENT);
    text_at(44, 9, "PLANNED", C_ACCENT);

    panel(53, 5, 26, 6, "SAFETY");
    text_at(55, 7, "Disk writes", C_MUTED);
    text_at(68, 7, "DISABLED", C_ACCENT);
    text_at(55, 8, "OS registry", C_MUTED);
    text_at(68, 8, "PLANNED", C_ACCENT);
    text_at(55, 9, "Boot manager", C_MUTED);
    text_at(68, 9, "PLANNED", C_ACCENT);

    text_at(2, 12, "QUICK COMMANDS", C_ACCENT);
    text_at(2, 13, "help", C_TEXT);
    text_at(10, 13, "Available commands", C_MUTED);
    text_at(2, 14, "status", C_TEXT);
    text_at(10, 14, "Kernel and subsystem status", C_MUTED);
    text_at(2, 15, "os list", C_TEXT);
    text_at(10, 15, "Future OS registry placeholder", C_MUTED);
    text_at(2, 16, "about", C_TEXT);
    text_at(10, 16, "Project information", C_MUTED);

    panel(1, 18, 78, 4, "CONSOLE");
    text_at(3, 19, output, C_TEXT);
    text_at(3, 20, "Read-only prototype: no disks are scanned, mounted, or modified.", C_MUTED);

    fill_row(22, ' ', C_SELECTED);
    text_at(2, 22, "hybrid>", C_TEXT);
    text_at(10, 22, line, C_TEXT);
    hline(10 + line_len, 22, 1, '_', C_ACCENT);
    text_at(2, 23, "ENTER run  |  BACKSPACE edit  |  type help for commands", C_MUTED);
    hline(0, 24, W, '-', C_MUTED);
    text_at(2, 24, "HYBRID OS  |  EARLY PROTOTYPE  |  DARK MODE", C_ACCENT);
}

static const char keys[128] = {
    [2]='1',[3]='2',[4]='3',[5]='4',[6]='5',[7]='6',[8]='7',[9]='8',[10]='9',[11]='0',
    [14]='\b',[15]='\t',[16]='q',[17]='w',[18]='e',[19]='r',[20]='t',[21]='y',[22]='u',[23]='i',[24]='o',[25]='p',
    [28]='\n',[30]='a',[31]='s',[32]='d',[33]='f',[34]='g',[35]='h',[36]='j',[37]='k',[38]='l',
    [44]='z',[45]='x',[46]='c',[47]='v',[48]='b',[49]='n',[50]='m',[57]=' '
};

static char read_key(void) {
    for (;;) {
        if (!(inb(0x64) & 1)) continue;
        uint8_t sc = inb(0x60);
        if (sc == 0x2A || sc == 0x36) { shift = 1; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift = 0; continue; }
        if (sc & 0x80) continue;
        char c = keys[sc];
        if (shift && c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        if (shift && c >= '1' && c <= '9') c = "!@#$%^&*("[c - '1'];
        if (shift && c == '0') c = ')';
        return c;
    }
}

static void set_output(const char *s) {
    size_t i = 0;
    while (s[i] && i < sizeof(output) - 1) { output[i] = s[i]; ++i; }
    output[i] = 0;
}

static void run_command(void) {
    /* Shell commands are case-insensitive, even though the UI preserves input case. */
    for (size_t i = 0; line[i]; ++i) {
        if (line[i] >= 'A' && line[i] <= 'Z') line[i] = (char)(line[i] - 'A' + 'a');
    }

    if (eq(line, "help")) set_output("Commands: help, clear/cls, about, version/ver, theme, status, os list, reboot, halt");
    else if (eq(line, "clear") || eq(line, "cls")) set_output("");
    else if (eq(line, "about")) set_output("Hybrid OS: independent freestanding C/ASM kernel; GRUB Multiboot v1.");
    else if (eq(line, "version") || eq(line, "ver")) set_output("Hybrid OS kernel prototype 0.1 | x86 32-bit | BIOS/Multiboot");
    else if (eq(line, "theme")) set_output("Theme: Midnight dark | accent: cyan/seafoam | renderer: VGA text mode");
    else if (eq(line, "status")) set_output("Kernel: running | memory, filesystem, network and GUI: pending.");
    else if (eq(line, "os list")) set_output("OS registry is planned. No disks are scanned or modified by this command.");
    else if (eq(line, "reboot")) {
        set_output("Requesting reboot...");
        draw_screen();
        outb(0x64, 0xFE);
        for (;;) __asm__ volatile ("cli; hlt");
    } else if (eq(line, "halt")) {
        set_output("System halted. Reset the virtual machine to continue.");
        draw_screen();
        for (;;) __asm__ volatile ("cli; hlt");
    } else if (line[0]) set_output("Unknown command. Type help to see available commands.");
    else set_output("Ready. Type help to explore the Hybrid prototype.");
    line_len = 0;
    line[0] = 0;
    draw_screen();
}

void kernel_main(void) {
    line_len = 0;
    line[0] = 0;
    draw_screen();
    for (;;) {
        char c = read_key();
        if (c == '\t') continue;
        if (c == '\n') {
            line[line_len] = 0;
            run_command();
        } else if (c == '\b') {
            if (line_len) { line[--line_len] = 0; draw_screen(); }
        } else if (c >= 32 && c <= 126 && line_len < sizeof(line) - 1) {
            line[line_len++] = c;
            line[line_len] = 0;
            draw_screen();
        }
    }
}
