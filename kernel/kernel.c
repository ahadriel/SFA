#include <stddef.h>
#include <stdint.h>
#define VGA ((volatile uint16_t*)0xB8000)
#define BLACK 0x080D16u
#define SURFACE 0x101A27u
#define PANEL 0x142232u
#define BORDER 0x26394Au
#define WHITE 0xEAF3F7u
#define MUTED 0x8297A8u
#define CYAN 0x47E0C0u
#define BLUE 0x48A9FFu
#define GOLD 0xE8BE68u
static uint32_t *fb;
static uint32_t pitch, sw, sh;
static uint8_t rp, gp, bp;
static int gfx, page, console_mode, shift;
static char line[64], output[88]="Welcome to Hybrid OS. Your control center is ready.";
static size_t len;
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0, %1"::"a"(v),"Nd"(p));}
static inline uint8_t inb(uint16_t p){uint8_t v;__asm__ volatile("inb %1, %0":"=a"(v):"Nd"(p));return v;}
static const uint8_t font[36][7]={
{14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},{30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},{6,8,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,2,12},
{14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},{28,18,17,17,17,18,28},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},{14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},{7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},{17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},{30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},{15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},{17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},{17,17,10,4,4,4,4},{31,1,2,4,8,16,31}};
static int gi(char c){if(c>='0'&&c<='9')return c-'0';if(c>='A'&&c<='Z')return c-'A'+10;if(c>='a'&&c<='z')return c-'a'+10;return -1;}
static uint32_t cv(uint32_t c){return (((c>>16)&255)<<rp)|(((c>>8)&255)<<gp)|((c&255)<<bp);}
static void px(int x,int y,uint32_t c){if(gfx&&x>=0&&y>=0&&(uint32_t)x<sw&&(uint32_t)y<sh)fb[(uint32_t)y*(pitch/4)+(uint32_t)x]=cv(c);}
static void box(int x,int y,int w,int h,uint32_t c){if(!gfx)return;if(x<0){w+=x;x=0;}if(y<0){h+=y;y=0;}if(x+w>(int)sw)w=(int)sw-x;if(y+h>(int)sh)h=(int)sh-y;for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++)px(xx,yy,c);}
static void chr(int x,int y,char c,uint32_t col,int z){int n=gi(c);if(n>=0)for(int r=0;r<7;r++)for(int k=0;k<5;k++)if(font[n][r]&(1u<<(4-k)))box(x+k*z,y+r*z,z,z,col);else{}else if(c=='-')box(x,y+6*z,5*z,z,col);else if(c=='.')box(x+2*z,y+6*z,z,z,col);else if(c==':'){box(x+2*z,y+2*z,z,z,col);box(x+2*z,y+6*z,z,z,col);}}
static void txt(int x,int y,const char*s,uint32_t c,int z){while(*s&&x<(int)sw-6*z){chr(x,y,*s++,c,z);x+=6*z;}}
static void edge(int x,int y,int w,int h,uint32_t c){box(x,y,w,1,c);box(x,y+h-1,w,1,c);box(x,y,1,h,c);box(x+w-1,y,1,h,c);}
static void card(int x,int y,int w,int h,const char*t,const char*v,const char*d,uint32_t c){box(x,y,w,h,PANEL);edge(x,y,w,h,BORDER);box(x,y,4,h,c);txt(x+16,y+14,t,MUTED,1);txt(x+16,y+36,v,WHITE,2);txt(x+16,y+h-20,d,c,1);}
static void fallback(void){for(int y=0;y<25;y++)for(int x=0;x<80;x++)VGA[y*80+x]=(uint16_t)(' '|(0x00<<8));for(int x=0;x<80;x++)VGA[x]=(uint16_t)(' '|(0x30<<8));const char*a="HYBRID OS - GRAPHICAL CONTROL CENTER";for(int i=0;a[i];i++)VGA[2+i]=(uint16_t)(a[i]|(0x0f<<8));const char*b="Framebuffer unavailable. Text fallback active.";for(int i=0;b[i];i++)VGA[160+i]=(uint16_t)(b[i]|(0x0b<<8));}
static void draw(void){
 if(!gfx){fallback();return;}
 box(0,0,sw,sh,BLACK);box(0,0,sw,54,SURFACE);box(0,52,sw,2,CYAN);
 txt(28,19,"HYBRID",CYAN,2);txt(150,23,"SYSTEM CONTROL CENTER",WHITE,1);txt((int)sw-180,23,"KERNEL 0.2",MUTED,1);
 box(0,54,174,sh-94,SURFACE);txt(22,82,"WORKSPACE",MUTED,1);
 const char*nav[]={"1 HOME","2 SYSTEMS","3 TOOLS","4 ABOUT"};
 for(int i=0;i<4;i++){int y=112+i*48;if(page==i&&!console_mode)box(12,y-8,150,36,0x203A4B);box(12,y-8,3,36,page==i&&!console_mode?CYAN:SURFACE);txt(28,y,nav[i],page==i&&!console_mode?CYAN:WHITE,1);}
 box(18,sh-88,138,1,BORDER);txt(22,sh-70,"SAFE MODE",CYAN,1);txt(22,sh-48,"DISK WRITES OFF",MUTED,1);
 int x=204;txt(x,82,page==0?"OVERVIEW":page==1?"OPERATING SYSTEMS":page==2?"TOOLS AND ACTIONS":"ABOUT HYBRID",WHITE,2);
 txt(x,112,page==0?"YOUR MACHINE. YOUR OPERATING SYSTEMS. YOUR CHOICE.":page==1?"A SAFE HOME FOR YOUR INSTALLED SYSTEMS.":page==2?"KERNEL UTILITIES AVAILABLE IN THIS BUILD.":"AN INDEPENDENT OPERATING SYSTEM IN PROGRESS.",MUTED,1);
 if(page==0){card(x,150,174,112,"KERNEL","RUNNING","MULTIBOOT / BIOS",CYAN);card(x+190,150,174,112,"ARCHITECTURE","X86 32","FREESTANDING",BLUE);card(x+380,150,174,112,"GRAPHICS","ACTIVE","FRAMEBUFFER",CYAN);card(x,282,264,118,"SYSTEM REGISTRY","NOT READY","STORAGE PENDING",GOLD);card(x+280,282,258,118,"SAFETY STATUS","READ ONLY","NO DISK WRITES",CYAN);box(x,420,538,94,SURFACE);edge(x,420,538,94,BORDER);txt(x+18,438,"QUICK START",CYAN,1);txt(x+18,462,"PRESS 2 FOR SYSTEMS, 3 FOR TOOLS, C FOR CONSOLE",WHITE,1);txt(x+18,488,"THIS BUILD DOES NOT SCAN OR CHANGE YOUR DISKS",MUTED,1);}
 else if(page==1){card(x,150,538,106,"OS REGISTRY","NOT IMPLEMENTED","NO DISKS SCANNED OR MODIFIED",GOLD);box(x,276,538,104,SURFACE);edge(x,276,538,104,BORDER);txt(x+18,296,"PLANNED CAPABILITIES",CYAN,1);txt(x+18,322,"DETECT INSTALLED SYSTEMS",WHITE,1);txt(x+18,346,"REGISTER AND REMOVE BOOT ENTRIES",WHITE,1);txt(x+18,362,"READ-ONLY PARTITION INVENTORY FIRST",MUTED,1);}
 else if(page==2){card(x,150,260,110,"CONSOLE","AVAILABLE","PRESS C TO OPEN",CYAN);card(x+278,150,260,110,"REBOOT","AVAILABLE","PRESS R TO RESTART",BLUE);box(x,282,538,116,SURFACE);edge(x,282,538,116,BORDER);txt(x+18,302,"CURRENT COMMANDS",CYAN,1);txt(x+18,328,"HELP STATUS ABOUT VERSION",WHITE,1);txt(x+18,352,"THEME OS LIST CLEAR HALT",WHITE,1);txt(x+18,376,"COMMANDS DO NOT PERFORM DISK OPERATIONS",MUTED,1);}
 else{box(x,150,538,238,SURFACE);edge(x,150,538,238,BORDER);txt(x+22,174,"HYBRID OS",CYAN,2);txt(x+22,220,"INDEPENDENT FREESTANDING KERNEL",WHITE,1);txt(x+22,248,"EONIS-INSPIRED DARK CONTROL CENTER",WHITE,1);txt(x+22,290,"CURRENT MILESTONE: GRAPHICS + KEYBOARD",MUTED,1);txt(x+22,320,"NOT A LINUX LIVE SYSTEM",MUTED,1);txt(x+22,350,"BUILT FOR VIRTUAL MACHINE EXPERIMENTS",MUTED,1);}
 box(174,sh-40,sw-174,40,SURFACE);box(174,sh-40,sw-174,1,BORDER);txt(196,sh-26,console_mode?"CONSOLE ACTIVE - ESC RETURNS TO DESKTOP":"1-4 NAVIGATE   C CONSOLE   R REBOOT   H HOME",MUTED,1);
 if(console_mode){box(204,410,538,120,0x0B1420);edge(204,410,538,120,CYAN);txt(222,428,"HYBRID CONSOLE",CYAN,1);txt(222,454,output,WHITE,1);txt(222,480,"HYBRID>",CYAN,1);txt(294,480,line,WHITE,1);box(294+(int)len*6,478,5,10,CYAN);txt(222,506,"TYPE HELP FOR COMMANDS",MUTED,1);}
}
static int eq(const char*a,const char*b){size_t i=0;while(a[i]&&b[i]&&a[i]==b[i])i++;return a[i]==b[i];}
static void msg(const char*s){size_t i=0;while(s[i]&&i<sizeof(output)-1){output[i]=s[i];i++;}output[i]=0;}
static void reboot(void){outb(0x64,0xFE);for(;;)__asm__ volatile("cli; hlt");}
static void command(void){for(size_t i=0;line[i];i++)if(line[i]>='A'&&line[i]<='Z')line[i]=(char)(line[i]-'A'+'a');
 if(eq(line,"help"))msg("Commands: help status about version theme os list clear reboot halt");
 else if(eq(line,"clear")||eq(line,"cls"))msg("Console cleared. Ready.");
 else if(eq(line,"about"))msg("Hybrid OS: freestanding kernel with framebuffer desktop.");
 else if(eq(line,"version")||eq(line,"ver"))msg("Hybrid OS 0.2 | x86 32-bit | Multiboot v1");
 else if(eq(line,"theme"))msg("Midnight dark theme with cyan and seafoam accents.");
 else if(eq(line,"status"))msg("Kernel running. Framebuffer active. Storage pending.");
 else if(eq(line,"os list"))msg("OS registry is not implemented; no disks were changed.");
 else if(eq(line,"reboot"))reboot();
 else if(eq(line,"halt")){msg("CPU halted. Reset the virtual machine.");draw();for(;;)__asm__ volatile("cli; hlt");}
 else if(line[0])msg("Unknown command. Type help.");else msg("Ready.");
 len=0;line[0]=0;draw();}
static const char keys[128]={[1]=27,[2]='1',[3]='2',[4]='3',[5]='4',[6]='5',[7]='6',[8]='7',[9]='8',[10]='9',[11]='0',[14]='\b',[15]='\t',[16]='q',[17]='w',[18]='e',[19]='r',[20]='t',[21]='y',[22]='u',[23]='i',[24]='o',[25]='p',[28]='\n',[30]='a',[31]='s',[32]='d',[33]='f',[34]='g',[35]='h',[36]='j',[37]='k',[38]='l',[44]='z',[45]='x',[46]='c',[47]='v',[48]='b',[49]='n',[50]='m',[57]=' '};
static char key(void){for(;;){if(!(inb(0x64)&1))continue;uint8_t s=inb(0x60);if(s==0x2A||s==0x36){shift=1;continue;}if(s==0xAA||s==0xB6){shift=0;continue;}if(s&0x80)continue;char c=keys[s];if(shift&&c>='a'&&c<='z')c=(char)(c-'a'+'A');if(shift&&c>='1'&&c<='9')c="!@#$%^&*("[c-'1'];if(shift&&c=='0')c=')';return c;}}
void kernel_main(uint32_t magic,uint32_t mbi_addr){
 if(magic==0x2BADB002u&&mbi_addr){const uint8_t*m=(const uint8_t*)(uintptr_t)mbi_addr;uint32_t flags=*(const uint32_t*)(const void*)m;
 if((flags&(1u<<12))&&*(const uint32_t*)(const void*)(m+92)==0){uint64_t a=*(const uint64_t*)(const void*)(m+88);pitch=*(const uint32_t*)(const void*)(m+96);sw=*(const uint32_t*)(const void*)(m+100);sh=*(const uint32_t*)(const void*)(m+104);uint8_t bpp=m[108],type=m[109];
 if(a&&bpp==32&&type==1&&sw>=640&&sh>=480&&pitch>=sw*4){fb=(uint32_t*)(uintptr_t)(uint32_t)a;rp=m[110];gp=m[112];bp=m[114];if(rp<24&&gp<24&&bp<24)gfx=1;}}}
 draw();for(;;){char c=key();if(c==27){console_mode=0;len=0;line[0]=0;draw();continue;}if(!console_mode){if(c>='1'&&c<='4'){page=c-'1';draw();}else if(c=='h'||c=='H'){page=0;draw();}else if(c=='c'||c=='C'){console_mode=1;len=0;line[0]=0;draw();}else if(c=='r'||c=='R')reboot();continue;}
 if(c=='\t') {
  continue;
 }
 if(c=='\n') {
  line[len]=0;
  command();
 } else if(c=='\b') {
  if(len) {
   line[--len]=0;
   draw();
  }
 } else if(c>=32 && c<=126 && len<sizeof(line)-1) {
  line[len++]=c;
  line[len]=0;
  draw();
 }
 }
}
}
