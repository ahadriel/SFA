#include <stddef.h>
#include <stdint.h>

#define W 80
#define H 25
#define VGA ((volatile uint16_t*)0xB8000)

static size_t row, col;
static uint8_t colour = 0x0F;
static inline void outb(uint16_t p, uint8_t v) { __asm__ volatile ("outb %0, %1" : : "a"(v), "Nd"(p)); }
static inline uint8_t inb(uint16_t p) { uint8_t v; __asm__ volatile ("inb %1, %0" : "=a"(v) : "Nd"(p)); return v; }

static void cell(char c, size_t x, size_t y) { VGA[y * W + x] = ((uint16_t)colour << 8) | (uint8_t)c; }
static void clear(void) {
    row = col = 0;
    for (size_t y = 0; y < H; ++y) for (size_t x = 0; x < W; ++x) cell(' ', x, y);
}
static void scroll(void) {
    for (size_t y = 1; y < H; ++y) for (size_t x = 0; x < W; ++x) VGA[(y - 1) * W + x] = VGA[y * W + x];
    for (size_t x = 0; x < W; ++x) cell(' ', x, H - 1);
    row = H - 1;
}
static void putc(char c) {
    if (c == '\n') { col = 0; if (++row >= H) scroll(); return; }
    if (c == '\r') { col = 0; return; }
    if (c == '\b') { if (col) { --col; cell(' ', col, row); } return; }
    cell(c, col, row);
    if (++col >= W) { col = 0; if (++row >= H) scroll(); }
}
static void print(const char *s) { while (*s) putc(*s++); }
static int eq(const char *a, const char *b) { size_t i=0; while (a[i] && b[i] && a[i]==b[i]) ++i; return a[i]==b[i]; }

static const char keys[128] = {
 [2]='1',[3]='2',[4]='3',[5]='4',[6]='5',[7]='6',[8]='7',[9]='8',[10]='9',[11]='0',
 [14]='\b',[15]='\t',[16]='q',[17]='w',[18]='e',[19]='r',[20]='t',[21]='y',[22]='u',[23]='i',[24]='o',[25]='p',
 [28]='\n',[30]='a',[31]='s',[32]='d',[33]='f',[34]='g',[35]='h',[36]='j',[37]='k',[38]='l',
 [44]='z',[45]='x',[46]='c',[47]='v',[48]='b',[49]='n',[50]='m',[57]=' '
};
static int shift;
static char read_key(void) {
    for (;;) {
        if (!(inb(0x64) & 1)) continue;
        uint8_t sc = inb(0x60);
        if (sc == 0x2A || sc == 0x36) { shift=1; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift=0; continue; }
        if (sc & 0x80) continue;
        char c=keys[sc];
        if (shift && c>='a' && c<='z') c=(char)(c-'a'+'A');
        if (shift && c>='1' && c<='9') c="!@#$%^&*("[c-'1'];
        if (shift && c=='0') c=')';
        return c;
    }
}
static char line[128];
static void help(void) {
    print("Commands: help, clear, about, status, os list, reboot, halt\n");
    print("Disk writes are disabled in this early prototype.\n");
}
static void command(void) {
    if (eq(line,"help")) help();
    else if (eq(line,"clear")) clear();
    else if (eq(line,"about")) {
        print("HYBRID OS - a freestanding kernel prototype\n");
        print("No Debian userspace. CLI first; GUI comes later.\n");
    } else if (eq(line,"status")) {
        print("Kernel: running | Memory manager: pending\n");
        print("Filesystem: pending | GUI: planned after CLI\n");
    } else if (eq(line,"os list")) print("OS registry is a future milestone; no disks are modified.\n");
    else if (eq(line,"reboot")) { print("Rebooting...\n"); outb(0x64,0xFE); for (;;) __asm__ volatile ("hlt"); }
    else if (eq(line,"halt")) { print("System halted. Reset the VM to continue.\n"); for (;;) __asm__ volatile ("cli; hlt"); }
    else if (line[0]) print("Unknown command. Type help.\n");
}
void kernel_main(void) {
    clear();
    colour=0x0B; print("HYBRID OS\n");
    colour=0x0F;
    print("Your machine. Your operating systems. Your own kernel.\n");
    print("CLI milestone 0.1 | safe prototype\n");
    help();
    print("\nhybrid> ");
    size_t n=0;
    for (;;) {
        char c=read_key();
        if (c=='\t') continue;
        if (c=='\n') {
            putc('\n'); line[n]=0; command(); n=0; print("\nhybrid> ");
        } else if (c=='\b') { if(n) { --n; putc('\b'); } }
        else if(c>=32 && c<=126 && n<sizeof(line)-1) { line[n++]=c; putc(c); }
    }
}
