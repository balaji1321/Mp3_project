#include<stdio.h>
#include<string.h>
#include "view.h"
#include "types.h"
#include "edit.h"

int main(int argc,char *argv[]){
    ViewInfo viewInf;
    EditInfo editInf;

    /*check no of arguments passed or not*/
    if(argc>2){ 
        /*check operator type */
        if(check_operation_type(argv[1]) == e_view){
            if(validations(argv,&viewInf) == e_success){
                /*to view*/
            printf("......................Select to view.........................\n");
            /*call view fuction*/
            view_info(&viewInf);
            }
           
        }
        else if(check_operation_type(argv[1]) == e_edit){
            /*to edit*/
            if(argc>4){
                printf(".......................Select to edit.........................\n");
                
                editInf.tag_arg=argv[2];
                editInf.edit_arg=argv[3];
                
                if(e_validations(argv,&editInf) == e_success){
                    /*call edit function*/
                    
                    edit_info(&editInf);
                }
            }
            else{
                /*error msg for command line arguments*/
                printf(".................................................................................\n");
                printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
                printf("To view plese pass like: ./a.out -v mp3filename\n");
                printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
                printf("To get help pass like: ./a.out --help\n");
                printf("..................................................................................\n");
            }

        }

        /*help menu*/
        else if(check_operation_type(argv[1]) == e_help){
            printf("........................HELP MENU.........................\n");
            printf("1. -v -> to view mp3 file contents\n");
            printf("2. -e -> to edit mp3 file contents\n");
            printf("     2.1 -t -> to edit song title\n");
            printf("     2.2 -a -> to edit artist name\n");
            printf("     2.3 -A -> to edit album name\n");
            printf("     2.4 -y -> to edit year\n");
            printf("     2.5 -m -> to edit content\n");
            printf("     2.6 -c -> to edit comment\n");
            printf(".............................................................\n");
        }
        else{
            /*error msg for command line arguments*/
            printf(".................................................................................\n");
            printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
            printf("To view plese pass like: ./a.out -v mp3filename\n");
            printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
            printf("To get help pass like: ./a.out --help\n");
            printf("..................................................................................\n");
        }
    }
    else{
        /*error msg for command line arguments*/
        printf(".................................................................................\n");
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("To view plese pass like: ./a.out -v mp3filename\n");
        printf("To edit plese pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like: ./a.out --help\n");
        printf("..................................................................................\n");
    }
    return 0;
}


/*check operation type*/
OperationType check_operation_type(char *argv){
    if(!strcmp(argv,"-v")){
        return e_view;
    }
    else if(!strcmp(argv,"-e")){
       return e_edit;
    }
    else if(!strcmp(argv,"--help")){
       return e_help;
    }
    else{
       return e_unsupported;
    }
}