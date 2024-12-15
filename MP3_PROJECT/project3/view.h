#pragma once
#include "types.h"

#define MAX_SIZE 100

typedef struct{
    /*for view*/
    char *mp3_view;
    FILE *fptr_mp3_view;

    /*tags*/
    char Title[MAX_SIZE];
    char Artist_name[MAX_SIZE];
    char Album_name[MAX_SIZE];
    char Year[MAX_SIZE];
    char Music[MAX_SIZE];
    char Comment[MAX_SIZE];

    /**/

}ViewInfo;


/*check operation type*/
OperationType check_operation_type(char *argv);

/*read and validations*/
Status validations(char *argv[],ViewInfo *viewInf);

/*to view*/
Status view_info(ViewInfo *viewInf);

/*open files*/
Status open_files(ViewInfo *viewInf);

/*check mp3view*/
Status check_id3(ViewInfo *viewInf);

/*check version*/
Status check_version(ViewInfo *viewInf);

/*view the data*/
Status view_data(ViewInfo *viewInf);

/*little endian to big endian*/
void little_to_big(char *ptr,uint n);
