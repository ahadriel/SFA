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
static int read_sector(uint16_t base,uint8_t drive,uint32_t lba,uint16_t *words){
 if(lba>0x0FFFFFFFu)return 0;
 outb(base+ATA_DRIVE,(uint8_t)(0xE0u|(drive<<4)|((lba>>24)&0x0Fu)));
 delay400(base);
 outb(base+ATA_SECCOUNT,1);outb(base+ATA_LBA0,(uint8_t)lba);outb(base+ATA_LBA1,(uint8_t)(lba>>8));outb(base+ATA_LBA2,(uint8_t)(lba>>16));
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

static uint32_t le32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint8_t count_partitions(uint16_t base,uint8_t drive,const uint16_t *mbr){
 const uint8_t *m=(const uint8_t*)mbr;
 int protective=0;
 for(unsigned i=0;i<4;i++)if(m[446+i*16+4]==0xEEu)protective=1;
 if(!protective)return count_mbr_partitions(mbr);
 uint16_t header[256],entries[256];
 if(!read_sector(base,drive,1,header))return count_mbr_partitions(mbr);
 const uint8_t *h=(const uint8_t*)header;
 static const char sig[8]={'E','F','I',' ','P','A','R','T'};
 for(unsigned i=0;i<8;i++)if(h[i]!=(uint8_t)sig[i])return count_mbr_partitions(mbr);
 uint32_t entry_lba=le32(h+72),entry_count=le32(h+80),entry_size=le32(h+84);
 if(!entry_lba||entry_lba>0x0FFFFFFFu||entry_size<128u||entry_size>512u||512u%entry_size)return 0;
 if(entry_count>128u)entry_count=128u;
 uint32_t per_sector=512u/entry_size,found=0;
 for(uint32_t i=0;i<entry_count;){
  if(!read_sector(base,drive,entry_lba+i/per_sector,entries))break;
  const uint8_t *e=(const uint8_t*)entries;
  for(uint32_t j=0;j<per_sector&&i<entry_count;j++,i++){
   const uint8_t *guid=e+j*entry_size;int nonzero=0;
   for(unsigned g=0;g<16;g++)if(guid[g]){nonzero=1;break;}
   if(nonzero)found++;
  }
 }
 return (uint8_t)found;
}

uint8_t storage_scan(uint8_t *partition_count){
 static const uint16_t bases[2]={0x1F0,0x170};
 uint16_t identify[256],sector[256];
 uint8_t disks=0,parts=0;
 for(unsigned c=0;c<2;c++)for(uint8_t d=0;d<2;d++){
  if(!probe(bases[c],d,identify))continue;
  disks++;
  if(read_sector(bases[c],d,0,sector)&&has_mbr_signature(sector)){
   uint8_t n=count_partitions(bases[c],d,sector);
   if((uint16_t)parts+n>255u)parts=255u;else parts=(uint8_t)(parts+n);
  }
 }
 if(partition_count)*partition_count=parts;
 return disks;
}
