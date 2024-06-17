//POZNÁMKA: na stabilné triedene bol použitý algoritmus "bubble sort"

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#define BUFFER_SIZE 255

void sortDistances(int pocet_skladov, double arr[][2]){
    for(int i=0; i<pocet_skladov-1; i++){
        for(int j=0; j<pocet_skladov-1; j++){
            if(arr[j][0]>arr[j+1][0]){
                int tmp=arr[j][0];
                int tmp2=arr[j][1];
                arr[j][0]=arr[j+1][0];
                arr[j][1]=arr[j+1][1];
                arr[j+1][0]=tmp;
                arr[j+1][1]=tmp2;
            }          
        }
    }
}

int checkChybnySklad(int max, int indexy[], int x){
    for(int i=0; i<=max-1; i++){
        if(indexy[i]==x){
            return 1;
        }
    }
    return 0;
}

int isNumber(char s[]){
    for (int i = 0; s[i] != '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

int hasNumber(char s[]){
    for (int i = 0; s[i] != '\0'; i++){
        if(isdigit(s[i]))
              return 1;
    }
    return 0;
}

void replace(WAREHOUSE *db, int i, int k){
    char temp[MAX_NAME+1];
    int temp_cena;
    strcpy(temp, db[i].items[k].name);
    strcpy(db[i].items[k].name, db[i].items[k+1].name);
    strcpy(db[i].items[k+1].name, temp);
    temp_cena=db[i].items[k].price;                            
    db[i].items[k].price=db[i].items[k+1].price;
    db[i].items[k+1].price=temp_cena;
}

int main(int argc, char *argv[]){
    bool prepinac_w=false, prepinac_n=false, prepinac_e=false, prepinac_a=false,prepinac_d=false;
    char *name,*n_string="", *e_string="";
    double lat=0, lon=0;
    int opt;
    char* optstring = ":w:n:e:ad";

    while((opt = getopt(argc, argv, optstring)) != -1){
        switch (opt){
            case 'w':
                name=optarg;
                prepinac_w = true;
                break;
            case 'n':
                lat=strtod(optarg, &n_string);
                prepinac_n = true;
            	break;
            case 'e':
                lon=strtod(optarg, &e_string);
                prepinac_e = true;
            	break;
            case 'a':
                prepinac_a = true;
            	break;
            case 'd':
                prepinac_d = true;
            	break;
            default: 
                return 0;
        }
    }

    FILE *f = fopen(WAREHOUSE_DB_FILE, "r");
    char buffer[BUFFER_SIZE];
    int pocet_skladov=0;
    while(fgets(buffer, BUFFER_SIZE, f)) pocet_skladov++;
    rewind(f);
    WAREHOUSE *db=malloc(sizeof(WAREHOUSE)*pocet_skladov);
    //WAREHOUSE db[pocet_skladov];
    
    int file_error_sklady[pocet_skladov];
    int index_file_error_skladu=0;
    int format_error_sklady[pocet_skladov];
    int index_format_error_skladu=0;
    int capacity_error_sklady[pocet_skladov];
    int index_capacity_error_skladu=0;  

    int index=0;
    while(fscanf(f, "%s %lf %lf %d", db[index].name, &db[index].gps.lat, &db[index].gps.lon, &db[index].capacity) != EOF){
        printf("%s %.3lf %.3lf %d\n", db[index].name, db[index].gps.lat, db[index].gps.lon, db[index].capacity);
        char items_file_path[150];
        strcpy(items_file_path, ITEMS_FOLDER); 
        strcat(items_file_path, PATH_SEPARATOR);   
        char file_name[MAX_NAME+1];
        strcpy(file_name, db[index].name);
        strcat(file_name, ".txt");
        strcat(items_file_path, file_name);
        
        FILE *f2 = fopen(items_file_path, "r");
        if(f2==NULL){
            file_error_sklady[index_file_error_skladu]=index;
            index_file_error_skladu++;
            if(!prepinac_w && !prepinac_n && !prepinac_e){
                fprintf(stderr, "FILE_ERROR %s.txt\n", db[index].name);
            } 
        }
        else{
            char buffer2[BUFFER_SIZE];
            int pocet_riadkov2=0;
            char string[MAX_NAME+1]; int price=0;
            while(fgets(buffer2, BUFFER_SIZE, f2) != NULL){
                pocet_riadkov2++;
                if(sscanf(buffer2, "%s %d", string, &price)!=2 || price<1 || isNumber(string) || hasNumber(string)){
                    format_error_sklady[index_format_error_skladu]=index;
                    index_format_error_skladu++;
                    if(!prepinac_w && !prepinac_n && !prepinac_e){
                        fprintf(stderr, "FORMAT_ERROR %s.txt\n", db[index].name);
                    }    
                }
            } 
            rewind(f2);
            db[index].n=pocet_riadkov2;
            if(db[index].n>db[index].capacity){
                capacity_error_sklady[index_capacity_error_skladu]=index;
                index_capacity_error_skladu++;
                if(!prepinac_w && !prepinac_n && !prepinac_e){
                    fprintf(stderr, "CAPACITY_ERROR %s.txt\n", db[index].name);
                }      
            }
            db[index].items=malloc(sizeof(ITEM)*pocet_riadkov2);
            int index2=0;
            while(fscanf(f2, "%s %d", db[index].items[index2].name, &db[index].items[index2].price) != EOF){
                index2++;
            }
        } 
        if(f2!=NULL){
            fclose(f2);
        }
        index++;
    }
    fclose(f);

    //prepinac -w (optional -a / -d)
    if(prepinac_w){
        int index_skladu=0;
        for(int i=0; i<pocet_skladov; i++){
            if(strcmp(db[i].name, name)==0){
                index_skladu=i;
                if(checkChybnySklad(index_file_error_skladu, file_error_sklady, index_skladu)){
                    fprintf(stderr, "FILE_ERROR %s.txt\n", db[index_skladu].name);
                }
                else if(checkChybnySklad(index_format_error_skladu, format_error_sklady, index_skladu)){
                    fprintf(stderr, "FORMAT_ERROR %s.txt\n", db[index_skladu].name);
                }
                else if(checkChybnySklad(index_capacity_error_skladu, capacity_error_sklady, index_skladu)){
                    fprintf(stderr, "CAPACITY_ERROR %s.txt\n", db[index_skladu].name);
                }
                else{
                    printf("%s %.3lf %.3lf %d :\n", db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].capacity);
                    for(int j=0; j<db[i].n-1; j++){
                        for(int k=0; k<db[i].n-1; k++){
                            if((!prepinac_a && !prepinac_d && strcmp(db[i].items[k].name, db[i].items[k+1].name)>0)
                            ||(prepinac_a && (db[i].items[k].price>db[i].items[k+1].price))
                            ||(prepinac_d && (db[i].items[k].price<db[i].items[k+1].price))){
                                replace(db, i, k);
                            }
                        }
                    }
                    for(int l=0, poc=1; l<db[index_skladu].n; l++, poc++){
                        printf("%d. %s %d\n", poc, db[index_skladu].items[l].name, db[index_skladu].items[l].price);
                    }
                }
                break;          
            }
        }
    }

   //prepinace -n -e (optional -a / -d)
    else if(prepinac_n && prepinac_e){
        GPS user_input={lat, lon};
        double vzdialenosti[pocet_skladov][2];
        for(int i=0; i<pocet_skladov; i++){
            double vzdialenost=distance(user_input, db[i].gps);
            vzdialenosti[i][0]=vzdialenost;
            vzdialenosti[i][1]=i;
        }
        sortDistances(pocet_skladov, vzdialenosti);
        int najblizsi_sklad = vzdialenosti[0][1];

        if(checkChybnySklad(index_file_error_skladu, file_error_sklady, najblizsi_sklad)){
            fprintf(stderr, "FILE_ERROR %s.txt\n", db[najblizsi_sklad].name);
        }
        else if(checkChybnySklad(index_format_error_skladu, format_error_sklady, najblizsi_sklad)){
            fprintf(stderr, "FORMAT_ERROR %s.txt\n", db[najblizsi_sklad].name);
        }
        else if(checkChybnySklad(index_capacity_error_skladu, capacity_error_sklady, najblizsi_sklad)){
            fprintf(stderr, "CAPACITY_ERROR %s.txt\n", db[najblizsi_sklad].name);
        }
        else{
            printf("%s %.3lf %.3lf %d :\n", db[najblizsi_sklad].name, db[najblizsi_sklad].gps.lat, db[najblizsi_sklad].gps.lon, db[najblizsi_sklad].capacity);
            for(int j=0; j<db[najblizsi_sklad].n-1; j++){
                for(int k=0; k<db[najblizsi_sklad].n-1; k++){
                    if((!prepinac_a && !prepinac_d && strcmp(db[najblizsi_sklad].items[k].name, db[najblizsi_sklad].items[k+1].name)>0)
                    ||(prepinac_a && (db[najblizsi_sklad].items[k].price>db[najblizsi_sklad].items[k+1].price))
                    ||(prepinac_d && (db[najblizsi_sklad].items[k].price<db[najblizsi_sklad].items[k+1].price))){
                        replace(db, najblizsi_sklad, k);
                    }
                }
            }
            
            for(int i=0, poc=1; i<db[najblizsi_sklad].n; i++, poc++){
                printf("%d. %s %d\n", poc, db[najblizsi_sklad].items[i].name, db[najblizsi_sklad].items[i].price);
            }
        }
    }

    else{
        for(int i=0; i<pocet_skladov; i++){
            if(!checkChybnySklad(index_file_error_skladu, file_error_sklady, i) && !checkChybnySklad(index_format_error_skladu, format_error_sklady, i) && !checkChybnySklad(index_capacity_error_skladu, capacity_error_sklady, i)){
                printf("%s %.3lf %.3lf %d :\n", db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].capacity);
                for(int j=0; j<db[i].n-1; j++){
                    for(int k=0; k<db[i].n-1; k++){
                        if((!prepinac_a && !prepinac_d && strcmp(db[i].items[k].name, db[i].items[k+1].name)>0)
                        ||(prepinac_a && (db[i].items[k].price>db[i].items[k+1].price))
                        ||(prepinac_d && (db[i].items[k].price<db[i].items[k+1].price))){
                            replace(db, i, k);
                        }
                    }
                }
                for(int l=0, poc=1; l<db[i].n; l++, poc++){
                    printf("%d. %s %d\n", poc, db[i].items[l].name, db[i].items[l].price);
                }
            }
        }
    }   

    return 0;
}
