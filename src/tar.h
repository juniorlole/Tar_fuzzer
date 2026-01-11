/* tar Header Block, from POSIX 1003.1-1990.  */

/* POSIX header.  */
#define BLOCKSIZE 512
#define MAX_FILENAME_LEN 100
struct tar_t
{
  char name[MAX_FILENAME_LEN];   // File name
  char mode[8];                  // File mode
  char uid[8];                   // User ID
  char gid[8];                   // Group ID
  char size[12];                 // File size
  char mtime[12];                // Modified time
  char chksum[8];                // Checksum
  char typeflag;                 // File type
  char linkname[MAX_FILENAME_LEN];// Linked file name
  char magic[6];                 // Magic
  char version[2];               // Version
  char uname[32];                // User name
  char gname[32];                // Group name
  char devmajor[8];              // Device major number
  char devminor[8];              // Device minor number
  char prefix[155];              // Prefix
  char padding[12];              // Padding
};

typedef struct {
    char name[50];
} Name;

#define TMAGIC   "ustar"        /* ustar and a null */
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2
/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

#define XHDTYPE  'x'            /* Extended header referring to the
                                   next file in the archive */
#define XGLTYPE  'g'            /* Global extended header */

/* Bits used in the mode field, values in octal.  */
#define TSUID    04000          /* set UID on execution */
#define TSGID    02000          /* set GID on execution */
#define TSVTX    01000          /* reserved */
                                /* file permissions */
#define TUREAD   00400          /* read by owner */
#define TUWRITE  00200          /* write by owner */
#define TUEXEC   00100          /* execute/search by owner */
#define TGREAD   00040          /* read by group */
#define TGWRITE  00020          /* write by group */
#define TGEXEC   00010          /* execute/search by group */
#define TOREAD   00004          /* read by other */
#define TOWRITE  00002          /* write by other */
#define TOEXEC   00001          /* execute/search by other */
struct tar_t basicheader();
Name generatenamefile(int i, int success);
void flaw_test(char* field,unsigned int size, int i);
void debug(struct tar_t header);
void write_and_test(int i, struct tar_t header);
