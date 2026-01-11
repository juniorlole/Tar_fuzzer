#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include "tar.h"


static const unsigned ALL_MODE[] ={TSUID,TSGID,TSVTX,TUREAD,TUWRITE,TUEXEC,TGREAD,TGWRITE,TGEXEC,TOREAD,TOWRITE,TOEXEC};
static const int NUM_MODES = sizeof(ALL_MODE)/sizeof(ALL_MODE[0]);
static struct tar_t header;
static int test_numb=0;
//static struct tar_t header;// pour eviter que les processus accedent sur un meme fichier
char* extractor;



// Calculates the checksum for the given tar header
unsigned int calculate_checksum(struct tar_t* entry){
    // use spaces for the checksum bytes while calculating the checksum
    memset(entry->chksum, ' ', 8);
    // sum of entire metadata
    unsigned int check = 0;
    unsigned char* raw = (unsigned char*) entry;
    for(int i = 0; i < BLOCKSIZE; i++){
        check += raw[i];
    }
    return check;
}

/*RETURN A BASIC SIMPLE HEADER THAT IS SUPPOSED TO NOT CRASH THE PROGRAM*/
struct tar_t basicheader(){
    struct tar_t h;
    memset(&h, 0, sizeof(struct tar_t));
    sprintf(h.name, "file");
    sprintf(h.mode, "%07o", 0644);
    sprintf(h.uid, "%07o", 1001);
    sprintf(h.gid, "%07o", 1002);
    sprintf(h.size, "%011o", 12345);
    sprintf(h.mtime, "%011o", (unsigned long) time(NULL));
    h.typeflag = '0';
    strncpy(h.magic, "ustar", sizeof(h.magic));
    strncpy(h.version, "00", sizeof(h.version));
    strncpy(h.uname, "lolo", sizeof(h.uname));
    strncpy(h.gname, "jiji", sizeof(h.gname));
    memset(h.devmajor, 's', sizeof(h.devmajor));
    memset(h.devminor, 's', sizeof(h.devminor));
    memset(h.prefix, 's', sizeof(h.prefix));
    memset(h.padding, 0, sizeof(h.padding));
    // Calculate the checksum for the header
    unsigned int checksum = calculate_checksum(&h);
    sprintf(h.chksum, "%06o", checksum);
    return h;
}

/*
 *GENERATE THE NAME FOR THE TESTED FILE, OR THE SUCCESS FILE
 *IT DEPENDS ON THE FIELD TESTED AND EACH FIELD ARE AFFILIATED 
 *TO A NUMBER EX: NAME-->1,MODE-->2,UID-->3,....
 IF success = 0 THIS MEAN GENERATING A NAME FILE FOR A TEST THAT WE RE MAKING
 ELSE WE'RE GENERATING A NEW NAME FOR A FILE THAT CRASHED THE EXTRACTOR
*/
Name generatenamefile(int i, int success) {
  Name result;
  if (success ==0){
  switch (i) {
    case 1:
      sprintf(result.name, "archive_%d_name_test.tar", test_numb);
      break;
    case 2:
      sprintf(result.name, "archive_%d_mode_test.tar", test_numb);
      break;
    case 3:
      sprintf(result.name, "archive_%d_uid_test.tar", test_numb);
      break;
    case 4:
      sprintf(result.name, "archive_%d_gid_test.tar", test_numb);
      break;
    case 5:
      sprintf(result.name, "archive_%d_size_test.tar", test_numb);
      break;
    case 6:
      sprintf(result.name, "archive_%d_mtime_test.tar", test_numb);
      break;
    case 7:
      sprintf(result.name, "archive_%d_chksum_test.tar", test_numb);
      break;
    case 8:
      sprintf(result.name, "archive_%d_linkname_test.tar", test_numb);
      break;
    case 9:
      sprintf(result.name, "archive_%d_magic_test.tar", test_numb);
      break;
    case 10:
      sprintf(result.name, "archive_%d_version_test.tar", test_numb);
      break;
    case 11:
      sprintf(result.name, "archive_%d_uname_test.tar", test_numb);
      break;
    case 12:
      sprintf(result.name, "archive_%d_gname_test.tar", test_numb);
      break;
    case 13:
      sprintf(result.name, "archive_%d_devmajor_test.tar", test_numb);
      break;
    case 14:
      sprintf(result.name, "archive_%d_devminor_test.tar", test_numb);
      break;
    case 15:
      sprintf(result.name, "archive_%d_prefix_test.tar", test_numb);
      break;
    case 16:
      sprintf(result.name, "archive_%d_padding_test.tar", test_numb);
      break;
    case 17:
      sprintf(result.name, "archive_%d_typeflag_test.tar", test_numb);
      break;
  }
   }
  else{
  switch (i) {
    case 0:
      sprintf(result.name,"success_special%d.tar", test_numb);
    case 1:
      sprintf(result.name, "success_%d_name_test.tar", test_numb);
      break;
    case 2:
      sprintf(result.name, "success_%d_mode_test.tar", test_numb);
      break;
    case 3:
      sprintf(result.name, "success_%d_uid_test.tar", test_numb);
      break;
    case 4:
      sprintf(result.name, "success_%d_gid_test.tar", test_numb);
      break;
    case 5:
      sprintf(result.name, "success_%d_size_test.tar", test_numb);
      break;
    case 6:
      sprintf(result.name, "success_%d_mtime_test.tar", test_numb);
      break;
    case 7:
      sprintf(result.name, "success_%d_chksum_test.tar", test_numb);
      break;
    case 8:
      sprintf(result.name, "success_%d_linkname_test.tar", test_numb);
      break;
    case 9:
      sprintf(result.name, "success_%d_magic_test.tar", test_numb);
      break;
    case 10:
      sprintf(result.name, "success_%d_version_test.tar", test_numb);
      break;
    case 11:
      sprintf(result.name, "success_%d_uname_test.tar", test_numb);
      break;
    case 12:
      sprintf(result.name, "success_%d_gname_test.tar", test_numb);
      break;
    case 13:
      sprintf(result.name, "success_%d_devmajor_test.tar", test_numb);
      break;
    case 14:
      sprintf(result.name, "success_%d_devminor_test.tar", test_numb);
      break;
    case 15:
      sprintf(result.name, "success_%d_prefix_test.tar", test_numb);
      break;
    case 16:
      sprintf(result.name, "success_%d_padding_test.tar", test_numb);
      break;
    case 17:
      sprintf(result.name, "success_%d_typeflag_test.tar", test_numb);
      break;
   }
  }
  return result;
}
/*RECEIVE THE NAME OF THE ARCHIVE TO TEST AND TEST IT WITH THE EXTRACTOR PASSED AS PARAMETER WHEN YOU COMPILE THE PROJECT*/
int Test(char ch[50], int o){
    printf("TEST %s",ch);
    int rv = 0;
    char cmd[51];
    char *result = NULL;
    strncpy(cmd, extractor, 25);
    cmd[26] = '\0';
    strncat(cmd, " ", 25);
    strncat(cmd, ch, 25);
    char buf[33];
    memset(buf, 0, sizeof(buf));
    FILE *fp;
    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }
    int null = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL) {//WE GO TROUGH ALL THE OUTPUT THAT CAN BE PRINTED TO CHECK IF WE CRASHED EXTRACTOR
        if (buf != NULL){
            null = 1;
        }
        result = strstr(buf,"*** The program has crashed ***"); //HERE WE VERIFY IF THE OUTPUT CONTAINS THIS SPECIFIC MESSAGE THAT WE'RE TRYING TO GET
        if(result != NULL){
            printf(result);
            Name newname;
            int z = 1;
            newname = generatenamefile(o, z); //we generate a new name for the file that crashed the extractor
            rename(ch, newname.name);
            break;
        }
    }
    if( null == 1 && result == NULL){
        printf("Not the crash message\n");
        goto finally;
    }
    else if (null == 0){
      printf("No output\n");
      goto finally;        
    }
    else{
        goto finally;
    }
    finally:
    if(pclose(fp) == -1) {
      printf("Command not found\n");
      rv = -1;
    }
    return rv;
}

void generate_combinations() {//it is used to create all the combinaison of all the mode that we can pass to the mod field and make every archive for a header for each combination created and we have to test them
    unsigned mode;
    for (int i = 0; i < (1 << NUM_MODES); i++) {
        mode = 0;
        for (int j = 0; j < NUM_MODES; j++) {
            if (i & (1 << j)) {
                mode |= ALL_MODE[j];
            }
        }
        header = basicheader();
        sprintf(header.mode, "%07o", mode);
        write_and_test(3,header);
    }
}


/*THIS SECTION REPRESENT ALL THE TEST FOR EACH FIELD, 
 *EACH TEST REPRESENT A TEST OF AN ARCHIVE TAR THAT 
 *CONTAINS A TAR BASIC TARHEADER WITH THE MODIFIED FIELD THAT WE USE TO CRASH THE EXTRACTOR
*/
void tName(){
    header = basicheader();
    char* field = header.name;
    unsigned int size = sizeof(field);
    flaw_test(field,size,1);
}

void tMode(){
    header = basicheader();
    char* field = header.mode;
    unsigned int size = sizeof(field);
    flaw_test(field, size,2);
    generate_combinations();
    //combinations();
}

void tUid(){
    header = basicheader();
    char* field = header.uid;
    unsigned int size = sizeof(field);
    flaw_test(field,size,3); 
}
void tGid(){
    header = basicheader();
    char* field = header.gid;
    unsigned int size = sizeof(field);
    flaw_test(field,size,4);
}

void tSize(){
    printf("1ere fois");
    header = basicheader();
    char* field = header.size;
    unsigned int size = sizeof(field);
    flaw_test(field,size,5);
}

void tMtime(){
    header = basicheader();
    char* field = header.mtime;
    unsigned int size = sizeof(field);
    flaw_test(field,size,6);
}



void tLinkname(){
    header = basicheader();
    char* field = header.linkname;
    unsigned int size = sizeof(field);
    flaw_test(field,size,8);
}


void tMagic(){
    header = basicheader();
    char* field = header.magic;
    unsigned int size = sizeof(field);
    flaw_test(field,size,9);
}
void tVersion(){
    header = basicheader();
    char* field = header.version;
    unsigned int size = sizeof(field);
    flaw_test(field,size,10);
}

void tUname(){
    header = basicheader();
    char* field = header.uname;
    unsigned int size = sizeof(field);
    flaw_test(field,size,11);
}
void tGname(){
    header = basicheader();
    char* field = header.gname;
    unsigned int size = sizeof(field);
    flaw_test(field,size,12);
}

void tDevmajor(){
    header = basicheader();
    char* field = header.devmajor;
    unsigned int size = sizeof(field);
    flaw_test(field,size,13);
}

void tDevminor(){
    header = basicheader();
    char* field = header.devminor;
    unsigned int size = sizeof(field);
    flaw_test(field,size,14);
}

void tPrefix(){
    header = basicheader();
    char* field = header.prefix;
    unsigned int size = sizeof(field);
    flaw_test(field,size,15);
}

void tPadding(){
    header = basicheader();
    char* field = header.padding;
    unsigned int size = sizeof(field);
    flaw_test(field,size,16);
}

void tTypeflag(){
    header = basicheader();
    char field = header.typeflag;
          //forbiddenchar
    char forbidden_char[] = {'*', '\\', '/', '"', '?', '=','\0','<','>','.','|',','};
    for (int i = 0;i < sizeof(forbidden_char);i++){
        field= forbidden_char[i];
        write_and_test(17,header);
    }
  char WEIRD_CHARS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 26, 27,  30, 31,127, 128, 130,159,160, 18, 255};
  for (int i = 0;i < sizeof(WEIRD_CHARS);i++){
      field = WEIRD_CHARS[i]; 
      write_and_test(17,header);
  }
}
  
/*
 *IT REPRESENT THE tar that we are writing the header to a tar file and the test of that file
 *
*/

void write_and_test(int i, struct tar_t h)
{
    test_numb = test_numb + 1;
    Name na;
    char *ptr;
    na = generatenamefile(i,0); //WE GENERATE THE NAME OF THE FILE
    //debug(header);
    FILE *f = fopen(na.name, "wb");
    fwrite(&h, sizeof(header), 1, f);
    struct tar_t end;
    memset(&end, 0, sizeof(end));
    fwrite(&end, sizeof(end), 1, f);
    fclose(f);
    Test(na.name, i);
}




/*
 *REPRESENT ALL THE TEST THAT WE ARE GOING TO DO ON A FIELD SO FOR A FIELD WE ARE GOING TO TEST IF A FORMAT used to modify the field WILL HELP TO CRASH THE EXTRACTOR PROGRAM
 */
void flaw_test(char* field,unsigned int size, int i){
  //empty format
  strncpy(field, "", size);
  write_and_test(i,header);
  
  //not_numeric format
  strncpy(field, "shishosho", size);
  write_and_test(i,header);

  //big format
  memset(field, '7', size - 1);
  field[size - 1] = 0;
  write_and_test(i,header);

  //not_octal format
  memset(field, '9', size - 1);
  field[size - 1] = 0;
  write_and_test(i,header);
  
  //do end of char 
  memset(field, '4', size);
  write_and_test(i,header);

  //middle_null_termination format
  memset(field, 0, size);
  memset(field, '2', size / 2);
  write_and_test(i,header);

  //0 and middle null termination format
  memset(field, 0, size);
  memset(field,'0', size / 2);
  write_and_test(i,header);

  //not ascii format
  strncpy(field, "\x90\x10", size);
  write_and_test(i,header);

  //all 0 format
  memset(field, '0', size - 1);
  field[size - 1] = 0;
  write_and_test(i,header);
  
  //all null but end_0
  memset(field, 0, size - 1);
  field[size - 1] = '0';
  write_and_test(i,header);
  
  //forbiddenchar in the field format
  char forbidden_char[] = {'*', '\\', '/', '"', '?', '=','\0','<','>','.','|',','};
  for (int o = 0;o < sizeof(forbidden_char);o++){
      memset(field, 'o', size - 1);
      field[size - 1] = '\0';
      field[0] = forbidden_char[o];
      write_and_test(i,header);
      
  }
  //weird char in the field format
  char WEIRD_CHARS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 127, 128, 130, 200, 255};
  for (int o = 0;o < sizeof(WEIRD_CHARS);o++){
      memset(field, 'o', size - 1);
      field[size - 1] = '\0';
      field[0] = WEIRD_CHARS[o];
      field[size - 2] = WEIRD_CHARS[o]; 
      write_and_test(i,header);
  }
}


//USE FOR A SPECIAL TAR THAT WE CREATED, SO WE WILL ONLY WRITE AN ATTRIBUTE OF THE TAR_T HEADER ON A ARCHIVE AND TEST IF IT CRASH THE TAR EXTRACTOR,
//AND AGAIN EACH NUMBER IS ASSIGNED TO AN ATTRIBUTE OF THE TR_T HEADER THAT WE WILL RETURN
//AND WE ARE GOING WITH THE HELP OF A POINTER MOVE FROM ATTRIBUTE TO ATTRIBUTE OF THE HEADER TO GET THE CORRESPONDING ATTRIBUTE WITH THE MEANING OF THE NUMBER PASSED IN PARAMETER
char* chgpointer(struct tar_t* h, int i){
    char* ptr;
    switch (i) {
        case 0:
            return h->name;
        case 1:
            ptr = h->name+sizeof(h->name);
            return ptr;
        case 2:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode);
            return ptr;
        case 3:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid);
            return ptr;
        case 4:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid);
            return ptr;
        case 5:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size);
            return ptr;
        case 6:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime);
            return ptr;
        case 7:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum);
            return ptr;
        case 8:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag);
            return ptr;
        case 9:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname);
            return ptr;
        case 10:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic);
            return ptr;
        case 11:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version);
            return ptr;
        case 12:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version)+sizeof(h->uname);
            return ptr;
        case 13:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version)+sizeof(h->uname)+sizeof(h->gname);
            return ptr;
        case 14:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version)+sizeof(h->uname)+sizeof(h->gname)+sizeof(h->devmajor);
            return ptr;
        case 15:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version)+sizeof(h->uname)+sizeof(h->gname)+sizeof(h->devmajor)+sizeof(h->devminor);
            return ptr;
        case 16:
            ptr = h->name+sizeof(h->name)+sizeof(h->mode)+sizeof(h->uid)+sizeof(h->gid)+sizeof(h->size)+sizeof(h->mtime)+sizeof(h->chksum)+sizeof(h->typeflag)+sizeof(h->linkname)+sizeof(h->magic)+sizeof(h->version)+sizeof(h->uname)+sizeof(h->gname)+sizeof(h->devmajor)+sizeof(h->devminor)+sizeof(h->prefix);
            return ptr;
        default:
            return h->name;       
    }
}
//USED TO WRITE A THE CHOOSED ATTRIBUTE IN THE TAR AND TEST THAT SPECIAL TAR
void specialwritetest(int i, char* h)
{
    test_numb = test_numb + 1;
    //debug(header);
    FILE *f = fopen("special.tar", "wb");
    fwrite(h, sizeof(h), 1, f);
    struct tar_t end;
    memset(&end, 0, sizeof(end));
    fwrite(&end, sizeof(end), 1, f);
    fclose(f);
    char name[50] = "special.tar";
    Test(name, 0);
}
//THIS METHODE WRITE AN SIMPLE TAR_T HEADER AND WE CHOOSED RANDOWMLY THE ATTRIBUTE TO WROTE TO THE TAR ARCHIVE AND TEST IT
void specialtar()
{
    struct tar_t file1;
    memset(&file1, 0, sizeof(struct tar_t)); // initialize to zero
    strncpy(file1.name, "file1", sizeof(file1.name));
    strncpy(file1.mode, "0000644", sizeof(file1.mode));
    strncpy(file1.uid, "0000010", sizeof(file1.uid));
    strncpy(file1.gid, "0000010", sizeof(file1.gid));
    strncpy(file1.size, "0000006", sizeof(file1.size));
    strncpy(file1.mtime, "00000000000", sizeof(file1.mtime));
    file1.typeflag = '0';
    strncpy(file1.magic, "ustar", sizeof(file1.magic));
    strncpy(file1.version, "00", sizeof(file1.version));
    strncpy(file1.uname, "username", sizeof(file1.uname));
    strncpy(file1.gname, "groupname", sizeof(file1.gname));
    unsigned int cheeck = calculate_checksum(&file1);
    sprintf(file1.chksum, "%06o", cheeck);
    // Write the header and file contents to the output tar file
    srand(time(0));
    int r = rand()% 17;
    char* ch = chgpointer(&file1, r);
    specialwritetest(0, ch);
}



int main(int argc, char* argv[])
{
  if (argc < 2){
      fprintf(stderr, "TAR EXTRACTOR MISSING!!!");
      return -1;
  }
  extractor = argv[1];
  tName();
  tMode();
  tUid();
  tGid();
  tSize();
  tMtime();
  //tChcksum();
  tTypeflag();
  tLinkname();
  tMagic();
  tVersion();
  tUname();
  tGname();
  tDevmajor();
  tDevminor();
  tPrefix();
  tPadding();
  specialtar();
  return 0;
}
