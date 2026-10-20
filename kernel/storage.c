#include "storage.h"
#include <stddef.h>
#define ATA_DATA 0
#define ATA_ERROR 1
#define ATA_SECCOUNT 2
#define ATA_LBA0 3
#define ATA_LBA1 4
#define ATA_LBA2 5
#define ATA_DRIVE 6
#define ATA_STATUS 7
#define ATA_CMD 7
#define ATA_SR_BSY 0x80
#define ATA_SR_DRQ 0x08
#define ATA_SR_ERR 0x01
#define ATA_CMD_IDENTIFY 0xEC
#define ATA_CMD_READ 0x20
static inline void outb(uint16_t p,uint8_t v){__asm__ volatile("outb %0, %1"::"a"(v),"Nd"(p));}
static inline uint8_t inb(uint16_t p){uint8_t v;__asm__ volatile("inb %1, %0":"=a"(v):"Nd"(p));return v;}
static inline uint16_t inw(uint16_t p){uint16_t v;__asm__ volatile("inw %1, %0":"=a"(v):"Nd"(p));return v;}
static void delay400(uint16_t base){(void)inb(base+ATA_STATUS);(void)inb(base+ATA_STATUS);(void)inb(base+ATA_STATUS);(void)inb(base+ATA_STATUS);}
static int wait_ready(uint16_t base,int need_drq){
 for(uint32_t i=0;i<1000000u;i++){
  uint8_t s=inb(base+ATA_STATUS);
  if(s==0xFFu||s==0u)return 0;
  if(s&ATA_SR_ERR)return 0;
  if(!(s&ATA_SR_BSY)&&(!need_drq||(s&ATA_SR_DRQ)))return 1;
 }
 return 0;
}
static int read_sector0(uint16_t base,uint8_t drive,uint16_t *words){
 outb(base+ATA_DRIVE,(uint8_t)(0xE0u|(drive<<4)));
 delay400(base);
 outb(base+ATA_SECCOUNT,1);outb(base+ATA_LBA0,0);outb(base+ATA_LBA1,0);outb(base+ATA_LBA2,0);
 outb(base+ATA_CMD,ATA_CMD_READ);
 if(!wait_ready(base,1))return 0;
 for(unsigned i=0;i<256;i++)words[i]=inw(base+ATA_DATA);
 return 1;
}
static int probe(uint16_t base,uint8_t drive,uint16_t *identify){
 outb(base+ATA_DRIVE,(uint8_t)(0xA0u|(drive<<4)));
 delay400(base);
 outb(base+ATA_SECCOUNT,0);outb(base+ATA_LBA0,0);outb(base+ATA_LBA1,0);outb(base+ATA_LBA2,0);
 outb(base+ATA_CMD,ATA_CMD_IDENTIFY);
 uint8_t s=inb(base+ATA_STATUS);
 if(s==0u||s==0xFFu)return 0;
 if(!wait_ready(base,1))return 0;
 for(unsigned i=0;i<256;i++)identify[i]=inw(base+ATA_DATA);
 /* Reject ATAPI devices: this first driver is deliberately ATA disk only. */
 if((identify[0]&0x8000u)!=0)return 0;
 return 1;
}
static int has_mbr_signature(const uint16_t *sector){return sector[255]==0xAA55u;}
static uint8_t count_mbr_partitions(const uint16_t *sector){
 const uint8_t *b=(const uint8_t*)sector;
 uint8_t n=0;
 for(unsigned i=0;i<4;i++){
  const uint8_t *p=b+446+i*16;
  uint32_t size=(uint32_t)p[12]|((uint32_t)p[13]<<8)|((uint32_t)p[14]<<16)|((uint32_t)p[15]<<24);
  if(p[4]!=0&&size!=0)n++;
 }
 return n;
}
uint8_t storage_scan(uint8_t *partition_count){
 static const uint16_t bases[2]={0x1F0,0x170};
 uint16_t identify[256],sector[256];
 uint8_t disks=0,parts=0;
 for(unsigned c=0;c<2;c++)for(uint8_t d=0;d<2;d++){
  if(!probe(bases[c],d,identify))continue;
  disks++;
  if(read_sector0(bases[c],d,sector)&&has_mbr_signature(sector)){
   uint8_t n=count_mbr_partitions(sector);
   if((uint16_t)parts+n>255u)parts=255u;else parts=(uint8_t)(parts+n);
  }
 }
 if(partition_count)*partition_count=parts;
 return disks;
}
