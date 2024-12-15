#include<stdio.h>
#include<string.h>
#include "types.h"
#include "edit.h"

Status e_validations(char *argv[],EditInfo *editInf){
    /*check mp3 file passed or not*/
    char str[10];
    /*check . passed or not*/
    if(strchr(argv[4],'.')==NULL){
        printf(".................................................................................\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("To view plese pass like: ./a.out -v mp3filename\n");
        printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("..................................................................................\n");
        return e_failure;
    }
    
    strcpy(str,strchr(argv[4],'.'));
    /*check .mp3 extn or not*/
    if(strcmp(str,".mp3")){
        printf(".................................................................................\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("To view plese pass like: ./a.out -v mp3filename\n");
        printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("..................................................................................\n");
        return e_failure;
    }
    editInf->mp3_edit=argv[4];
    return e_success;
}

Status edit_info(EditInfo *editInf){
    /*open files*/
    if(open_edit_file(editInf)==e_failure){
        return e_failure;
    }

    /*check id3*/
    if(edit_check_id3(editInf) == e_failure){
        return e_failure;
    }

    /*check version*/
    if(edit_check_version(editInf) == e_failure){
        return e_failure;
    }
    
    /*check edit tag*/
    if(edit_check_tag(editInf) == e_failure){
        return e_failure;
    }


}

Status open_edit_file(EditInfo *editInf){
    /*open mp3 file*/
    editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"r");
    /*if file is not exist*/
    if(editInf->fptr_mp3_edit==NULL){
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", editInf->mp3_edit);

    	return e_failure;
    }
    /*open temparory file*/
    editInf->fptr_temp_file=fopen("temp.mp3","r+");
    /*if it not there print error*/
    if(editInf->fptr_temp_file == NULL){
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", editInf->mp3_edit);

    	return e_failure;
    }
    return e_success;
}

Status edit_check_id3(EditInfo *editInf){
    char buffer[3];
    rewind(editInf->fptr_mp3_edit);
    /*read 3 bytes from */
    fread(buffer,3,1,editInf->fptr_mp3_edit);
    /*check id3 or not*/
    if(strcmp(buffer,"ID3")){
        
        return e_failure;
       
    }
    return e_success;
}


Status edit_check_version(EditInfo *editInf){
    short version;
    /*read two bytes from mp3 file for version*/
    fread(&version,2,1,editInf->fptr_mp3_edit);
    /*if version is equal to 3*/
    if(version==3){
        rewind(editInf->fptr_mp3_edit);
        char temp[10];
        /*read 10 bytes from src mp3 file*/
        fread(temp,10,1,editInf->fptr_mp3_edit);
        /*write 10 bytes from dest mp3 file*/
        fwrite(temp,10,1,editInf->fptr_temp_file);
        return e_success;
    }
    return e_failure;
}


Status edit_check_tag(EditInfo *editInf){
    /*check for edit*/
    fseek(editInf->fptr_mp3_edit,10,SEEK_SET);
    /*read 4 bytes for tag*/
    char buffer[4];
    fread(buffer,4,1,editInf->fptr_mp3_edit);
    int temp_title;
    int flag=0;
    if(!strcmp(buffer,"TIT2")){
        
        /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->title_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_title=editInf->title_size;
        e_little_to_big((char*)&editInf->title_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
            if(!strcmp(editInf->tag_arg,"-t")){
                flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE TITLE.....................\n");
            printf("Title        :   %s\n",editInf->edit_arg);
            printf("...........TITLE CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->title_size-1,SEEK_CUR);
            editInf->title_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_title,4,1,editInf->fptr_temp_file);
        char buffer2[editInf->title_size+2];
        //read 3 bytes of flag
        fread(buffer2,editInf->title_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer2,editInf->title_size+2,1,editInf->fptr_temp_file);
    }


    /*change artist name*/
     fread(buffer,4,1,editInf->fptr_mp3_edit);
     int temp_artist;
    if(!strcmp(buffer,"TPE1")){
        /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->artist_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_artist=editInf->artist_size;
        e_little_to_big((char*)&editInf->artist_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
        if(!strcmp(editInf->tag_arg,"-a")){
            flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE ARTIST.....................\n");
            printf("ARTIST        :   %s\n",editInf->edit_arg);
            printf("...........ARTIST CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->artist_size-1,SEEK_CUR);
            editInf->artist_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_artist,4,1,editInf->fptr_temp_file);
        char buffer3[editInf->artist_size+2];
        //read 3 bytes of flag
        fread(buffer3,editInf->artist_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer3,editInf->artist_size+2,1,editInf->fptr_temp_file);
    }
    

    /*change Album*/
    fread(buffer,4,1,editInf->fptr_mp3_edit);
    int temp_Album;
    if(!strcmp(buffer,"TALB")){
         /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->Album_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_Album=editInf->Album_size;
        e_little_to_big((char*)&editInf->Album_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
        if(!strcmp(editInf->tag_arg,"-A")){
            flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE ALBUM.....................\n");
            printf("ALBUM        :   %s\n",editInf->edit_arg);
            printf("...........ALBUM CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->Album_size-1,SEEK_CUR);
            editInf->Album_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_Album,4,1,editInf->fptr_temp_file);
        char buffer4[editInf->Album_size+2];
        //read 3 bytes of flag
        fread(buffer4,editInf->Album_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer4,editInf->Album_size+2,1,editInf->fptr_temp_file);
    }
    
    


    /*change Year*/
    fread(buffer,4,1,editInf->fptr_mp3_edit);
    int temp_year;
    if(!strcmp(buffer,"TYER")){
        /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->year_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_year=editInf->year_size;
        e_little_to_big((char*)&editInf->year_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
        if(!strcmp(editInf->tag_arg,"-y")){
            flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE YEAR.....................\n");
            printf("YEAR        :   %s\n",editInf->edit_arg);
            printf("...........YEAR CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->year_size-1,SEEK_CUR);
            editInf->year_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_year,4,1,editInf->fptr_temp_file);
        char buffer5[editInf->year_size+2];
        //read 3 bytes of flag
        fread(buffer5,editInf->year_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer5,editInf->year_size+2,1,editInf->fptr_temp_file);
            
    }
    

    
    /*change Music*/
    fread(buffer,4,1,editInf->fptr_mp3_edit);
    int temp_music;
    if(!strcmp(buffer,"TCON")){
        /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->Music_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_music=editInf->Music_size;
        e_little_to_big((char*)&editInf->Music_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
        if(!strcmp(editInf->tag_arg,"-m")){
            flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE MUSIC.....................\n");
            printf("MUSIC        :   %s\n",editInf->edit_arg);
            printf("...........MUSIC CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->Music_size-1,SEEK_CUR);
            editInf->Music_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_music,4,1,editInf->fptr_temp_file);
        char buffer6[editInf->Music_size+2];
        //read 3 bytes of flag
        fread(buffer6,editInf->Music_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer6,editInf->Music_size+2,1,editInf->fptr_temp_file);  
    }
    
    

    /*change Comments*/
    fread(buffer,4,1,editInf->fptr_mp3_edit);
    int temp_comm;
    if(!strcmp(buffer,"COMM")){
         /*copy tag*/
        fwrite(buffer,4,1,editInf->fptr_temp_file);
        /*read 4 bytes size from mp3 file*/
        fread(&editInf->Comment_size,4,1,editInf->fptr_mp3_edit);
        /*convert size little endian to big endian*/
        temp_comm=editInf->Comment_size;
        e_little_to_big((char*)&editInf->Comment_size,sizeof(int));
        /*skip flag*/
        //fseek(editInf->fptr_mp3_edit,3,SEEK_CUR);
        if(!strcmp(editInf->tag_arg,"-c")){
            flag=1;
            /*copy size into temp file*/
            int size=strlen(editInf->edit_arg)+1;
            /*convert little to big*/
            int temp=size;      

            e_little_to_big((char*)&size,sizeof(int));
            /*store size into temp file*/
            fwrite(&size,4,1,editInf->fptr_temp_file);
            /*add three bytes of 0*/
            //fseek(editInf->fptr_temp_file,3,SEEK_CUR);
            char buffer1[3];
            //read 3 bytes of flag
            fread(buffer1,3,1,editInf->fptr_mp3_edit);
            /*write into temp file*/
            fwrite(buffer1,3,1,editInf->fptr_temp_file);
            /*write tha data into mp3 file*/
            fwrite(editInf->edit_arg,strlen(editInf->edit_arg),1,editInf->fptr_temp_file);
            printf("...........CHANGE THE COMMENTS.....................\n");
            printf("COMMENTS        :   %s\n",editInf->edit_arg);
            printf("...........COMMENTS CHANGED SUCCESSFULLY...........\n");
            fseek(editInf->fptr_mp3_edit,editInf->Comment_size-1,SEEK_CUR);
            editInf->Comment_size=size;
            /*copy remaining data*/
            char ch;//declare a character
            while(fread(&ch,1,1,editInf->fptr_mp3_edit)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_temp_file);//copy to file
            }
            fclose(editInf->fptr_mp3_edit);
            fclose(editInf->fptr_temp_file);
            editInf->fptr_mp3_edit=fopen(editInf->mp3_edit,"w");
            editInf->fptr_temp_file=fopen("temp.mp3","r");
            while(fread(&ch,1,1,editInf->fptr_temp_file)>0){
            //get to ch
                fwrite(&ch,1,1,editInf->fptr_mp3_edit);//copy to file
            }
            return e_failure;
        }
        /*write 4 bytes of size into temp file*/
        fwrite(&temp_comm,4,1,editInf->fptr_temp_file);
        char buffer7[editInf->Comment_size+2];
        //read 3 bytes of flag
        fread(buffer7,editInf->Comment_size+2,1,editInf->fptr_mp3_edit);
        /*write into temp file*/
        fwrite(buffer7,editInf->Comment_size+2,1,editInf->fptr_temp_file); 
    }
    if(flag==0){
         /*error msg for command line arguments*/
            printf(".................................................................................\n");
            printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
            printf("To view plese pass like: ./a.out -v mp3filename\n");
            printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
            printf("To get help pass like: ./a.out --help\n");
            printf("..................................................................................\n");
            return e_failure;
    }


}

void e_little_to_big(char *ptr,uint n){
    /*convert little endian to big*/
    for(int i=0;i<n/2;i++){
        char temp=ptr[i];
        ptr[i]=ptr[n-i-1];
        ptr[n-i-1]=temp;
    }
}