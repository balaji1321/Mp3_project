#pragma once
#include "types.h"

#define E_MAX_SIZE 100


typedef struct{
    char *mp3_edit;
    FILE *fptr_mp3_edit;
    FILE *fptr_temp_file;

    /*args info*/
    char *tag_arg;
    char *edit_arg;

    /*size*/
    uint title_size;
    uint artist_size;
    uint Album_size;
    uint year_size;
    uint Music_size;
    uint Comment_size;
    
}EditInfo;

/*For edit*/
Status edit_info(EditInfo *editInf);

/*open file*/
Status open_edit_file(EditInfo *editInf);

/*check id3*/
Status edit_check_id3(EditInfo *editInf);

/*check version*/
Status edit_check_version(EditInfo *editInf);

/*check edit tag*/
Status edit_check_tag(EditInfo *editInf);

/*convert little endian to big endian*/
void e_little_to_big(char *ptr,uint n);

/*validations*/
Status e_validations(char *argv[],EditInfo *editInf);