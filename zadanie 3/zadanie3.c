#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

void replace(int j, int dlzka_slova, char string[], char papa[], bool prepinac_r, bool prepinac_R){
    if(dlzka_slova <= strlen(papa)){
        for(int k=j-dlzka_slova, x=0; k<j; k++, x++){
            if(prepinac_R){
                if(islower((int)string[k])) string[k]=tolower(papa[x]);
                else if(isupper((int)string[k])) string[k]=toupper(papa[x]);
                else string[k]=papa[x];
            }
            else string[k]=papa[x];
        }
    }
    else{
        for(int k=j-dlzka_slova, x=0; x<strlen(papa); k++, x++){
            if(prepinac_R){
                if(islower((int)string[k])) string[k]=tolower(papa[x]);
                else if(isupper((int)string[k])) string[k]=toupper(papa[x]);
                else string[k]=papa[x];              
            }
            else string[k]=papa[x];
        }
    } 
}

void prepinac_r_R(char string[], char *argumenty[], char papa[], bool prepinac_r, bool prepinac_R, int velkost_argumentov){
    int dlzka_slova=0, j=0, poc=0;
    for(int i=0; i<strlen(string); i++){
        if(!(ispunct(string[i])) && !(isspace(string[i]))){
            dlzka_slova++;
            if(string[i+1]=='\0'){
                j++;
                if(dlzka_slova!=0){
                    for(int h=0; h<velkost_argumentov; h++){     
                        for(int k=j-dlzka_slova, x=0; x<strlen(argumenty[h]); k++, x++){
                            if(prepinac_r && string[k]==argumenty[h][x]) poc++;                           
                            else if(prepinac_R && tolower(string[k])==tolower(argumenty[h][x])) poc++;
                        }
                        if(poc==strlen(argumenty[h])){
                            replace(j, dlzka_slova, string, papa, prepinac_r, prepinac_R);
                        }
                        poc=0;
                    }
                    dlzka_slova=0;
                }
            }
        }
        else{
            if(dlzka_slova!=0){
                for(int h=0; h<velkost_argumentov; h++){    
                    for(int k=j-dlzka_slova, x=0; x<strlen(argumenty[h]); k++, x++){
                        if(prepinac_r && string[k]==argumenty[h][x]) poc++;                           
                        else if(prepinac_R && tolower(string[k])==tolower(argumenty[h][x])) poc++;
                    }
                    if(poc==strlen(argumenty[h])){
                        replace(j, dlzka_slova, string, papa, prepinac_r, prepinac_R);
                    }
                    poc=0;
                }
                dlzka_slova=0;
            }
                        
        }
        j++;
    }
    string[j]='\0';
    printf("%s\n", string);
}
    
int main(int argc, char *argv[]){
    bool prepinac_d = false, prepinac_p = false, prepinac_u = false, prepinac_l = false, prepinac_r = false, prepinac_R = false;
	int opt; char *argumenty[]={""}; char *papa;
	char* optstring = ":dpulr:R:";

    while((opt = getopt(argc, argv, optstring)) != -1){
        switch (opt){
            case 'd':
                prepinac_d = true;
                break;
            case 'p':
                prepinac_p = true;
                break;
            case 'u':
                prepinac_u = true;
            	break;
            case 'l':
                prepinac_l = true;
            	break;
            case 'r':
                papa=optarg;
                prepinac_r = true;           
            	break;
            case 'R':
                papa=optarg;
                prepinac_R = true;        
            	break;
            case ':':
                return 2;
            default: /* '?' alebo ':' */
                return 1;
        }
    }
    int velkost_argumentov = 0, velkost_optind=0;
    if(optind < argc){
    	for(int i=optind; i<argc; i++){
            velkost_optind++;
    	}
    }
    else{
        velkost_argumentov++;
    }
    char string[1002];
     //ziadne prepinace (+l / +u)
    if(!prepinac_d && !prepinac_p){
        while(fgets(string, 1002, stdin) != NULL){
            if(string[0]=='\n') break;
            else{
                string[strlen(string)-1] = '\0'; 
                int j=0;
                for(int i=0; i<strlen(string); i++){
                    if(!(isdigit(string[i])) && !(ispunct(string[i]))){
                        if(prepinac_l) string[j]=tolower(string[i]);
                        else if(prepinac_u) string[j]=toupper(string[i]);
                        else string[j]=string[i];
                        j++;
                    }
                }
                string[j]='\0';
                if(prepinac_r || prepinac_R){
                    if(velkost_argumentov==1){
                        prepinac_r_R(string, argumenty, papa, prepinac_r, prepinac_R, 1);
                    }
                    else prepinac_r_R(string, &argv[optind], papa, prepinac_r, prepinac_R, velkost_optind);
                }
                else{
                    printf("%s\n", string);
                } 
            }       
        }
    }
    //len prepinac d (+l / +u)
    else if(prepinac_d && !prepinac_p){
        while(fgets(string, 1002, stdin) != NULL){
            if(string[0]=='\n') break;
            else{
                string[strlen(string)-1] = '\0'; 
                int j=0;
                for(int i=0; i<strlen(string); i++){
                    if(!(ispunct(string[i]))){
                        if(prepinac_l) string[j]=tolower(string[i]);
                        else if(prepinac_u) string[j]=toupper(string[i]);
                        else string[j]=string[i];
                        j++;
                    }
                }
                string[j]='\0';
                if(prepinac_r || prepinac_R){
                    if(velkost_argumentov==1){
                        prepinac_r_R(string, argumenty, papa, prepinac_r, prepinac_R, 1);
                    }
                    else prepinac_r_R(string, &argv[optind], papa, prepinac_r, prepinac_R, velkost_optind);
                }
                else{
                    printf("%s\n", string);
                }
            }
        }
    }
    //len prepinac p (+l / +u)
    else if(!prepinac_d && prepinac_p){
        while(fgets(string, 1002, stdin) != NULL){
            if(string[0]=='\n') break;
            else{
                string[strlen(string)-1] = '\0';
                int j=0; 
                for(int i=0; i<strlen(string); i++){
                    if(!(isdigit(string[i]))){
                        if(prepinac_l) string[j]=tolower(string[i]);
                        else if(prepinac_u) string[j]=toupper(string[i]);
                        else string[j]=string[i];
                        j++;
                    }
                }
                string[j]='\0';
                if(prepinac_r || prepinac_R){
                    if(velkost_argumentov==1){
                        prepinac_r_R(string, argumenty, papa, prepinac_r, prepinac_R, 1);
                    }
                    else prepinac_r_R(string, &argv[optind], papa, prepinac_r, prepinac_R, velkost_optind);
                }
                else{
                    printf("%s\n", string);
                }
            }
        }
    }
    //prepinace p d (+l / +u)
    else if(prepinac_d && prepinac_p){
        while(fgets(string, 1002, stdin) != NULL){
            if(string[0]=='\n') break;
            else{
                string[strlen(string)-1] = '\0';
                int j=0;
                for(int i=0; i<strlen(string); i++){
                    if(prepinac_l) string[j]=tolower(string[i]);
                    else if(prepinac_u) string[j]=toupper(string[i]);
                    else string[j]=string[i];
                    j++;
                }
                string[j]='\0';
                if(prepinac_r || prepinac_R){
                    if(velkost_argumentov==1){
                        prepinac_r_R(string, argumenty, papa, prepinac_r, prepinac_R, 1);
                    }
                    else prepinac_r_R(string, &argv[optind], papa, prepinac_r, prepinac_R, velkost_optind);
                }
                else{
                    printf("%s\n", string);
                }               
            }
        }
    }

	return 0;
}
