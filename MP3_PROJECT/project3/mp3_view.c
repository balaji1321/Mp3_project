#include<stdio.h>
#include<string.h>
#include "types.h"
#include "view.h"

/*read and validations*/
Status validations(char *argv[],ViewInfo *viewInf){
    char str[10];
    if(strchr(argv[2],'.')==NULL){
        /*error msg for command line arguments*/
        printf(".................................................................................\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("To view plese pass like: ./a.out -v mp3filename\n");
        printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("..................................................................................\n");
        return e_failure;
    }
    strcpy(str,strchr(argv[2],'.'));
    if(strcmp(str,".mp3")){
        /*error msg for command line arguments*/
        printf(".................................................................................\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("To view plese pass like: ./a.out -v mp3filename\n");
        printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("..................................................................................\n");
        return e_failure;
    }
    viewInf->mp3_view=argv[2];
    return e_success;
}

Status view_info(ViewInfo *viewInf){
    /*open files*/
    if(open_files(viewInf) == e_failure){
        return e_failure;
    }

    /*check id3*/
    if(check_id3(viewInf) == e_failure){
        return e_failure;
    }

    /*check version*/
    if(check_version(viewInf) == e_failure){
        return e_failure;
    }

    /*view the data*/
    if(view_data(viewInf) == e_failure){
        return e_failure;
    }
}

Status open_files(ViewInfo *viewInf){
    /*open mp3 file*/
    viewInf->fptr_mp3_view=fopen(viewInf->mp3_view,"r");
    /*if no existing file*/
    if(viewInf->fptr_mp3_view == NULL){
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", viewInf->mp3_view);

    	return e_failure;
    }
    return e_success;

}

Status check_id3(ViewInfo *viewInf){
    /*extrct id3 from mp3 file*/
    rewind(viewInf->fptr_mp3_view);
    char buffer[3];
    /*read 3 bytes from the mp3file*/
    fread(buffer,3,1,viewInf->fptr_mp3_view);
    /*check ID3 tag is there or not*/
    if(strcmp(buffer,"ID3")){
        printf("ID3 Tag not matched");
        return e_failure;
    }
    return e_success;
}

Status check_version(ViewInfo *viewInf){
    short int version;
    /*read 2 bytes from the mp3 file*/
    fread(&version,2,1,viewInf->fptr_mp3_view);
    if(version==3){
        fseek(viewInf->fptr_mp3_view,10,SEEK_SET);
        return e_success;
    }
    return e_failure;
}

Status view_data(ViewInfo *viewInf){
    char buffer[4];
    /*read 4 bytes and store into buffer*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"TIT2")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Title,size-1,1,viewInf->fptr_mp3_view);
        printf("Title     :   %s\n",viewInf->Title);
    }
    /*read 4 bytes for artist tag*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"TPE1")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Artist_name,size-1,1,viewInf->fptr_mp3_view);
        printf("Artist    :   %s\n",viewInf->Artist_name);
    }

    /*read 4bytes for album tag*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"TALB")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Album_name,size-1,1,viewInf->fptr_mp3_view);
        printf("ALBUM     :   %s\n",viewInf->Album_name);
    }

    /*read 4 bytes for year tag*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"TYER")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Year,(size-1),1,viewInf->fptr_mp3_view);
        printf("Year      :   %s\n",viewInf->Year);
    }

    /*read 4 bytes for Music tag*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"TCON")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Music,(size-1),1,viewInf->fptr_mp3_view);
        printf("Music     :   %s\n",viewInf->Music);
    }

    /*read 4 bytes for comments tag*/
    fread(buffer,4,1,viewInf->fptr_mp3_view);
    if(!strcmp(buffer,"COMM")){
        int size;
        /*read tag size from the mp3file*/
        fread(&size,4,1,viewInf->fptr_mp3_view);
        little_to_big((char*)&size,sizeof(int));
        fseek(viewInf->fptr_mp3_view,3,SEEK_CUR);
        fread(viewInf->Comment,(size-1),1,viewInf->fptr_mp3_view);
        if(viewInf->Comment[3]==0){
            viewInf->Comment[3]='.';
        }
        printf("Comments  :   %s\n",viewInf->Comment);
    }
    return e_success;

}

void little_to_big(char *ptr,uint n){
    for(int i=0;i<n/2;i++){
        char temp=ptr[i];
        ptr[i]=ptr[n-i-1];
        ptr[n-i-1]=temp;
    }
}

