#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

int getIndex(char nazov_skladu[]){
    for(int i=0; i<DB_NUM; i++){
        if(strcmp(db[i].name, nazov_skladu)==0){
            return i;
        }
    }
    return 0;
}

int checkItem(int index, char nazov_tovaru[]){
    for(int j=0; j<db[index].n; j++){
        if(strcmp(db[index].items[j].name, nazov_tovaru)==0){
            return 1;
        }
    }
    return 0;
}

void sklado_centricky_vypis(int index){
    printf("%s %.3lf %.3lf %d :\n", db[index].name, db[index].gps.lat, db[index].gps.lon, db[index].n);
    for(int j=0, poc=1; j<db[index].n; j++){
        printf("%d. %s %d\n", poc, db[index].items[j].name, db[index].items[j].price);
        poc++;
    }
}

void tovaro_centricky_vypis(int index){
    static int poc=1;
    for(int j=0; j<db[index].n; j++){
        printf("%d. %s %d : %s %.3lf %.3lf %d\n", 
            poc, db[index].items[j].name, db[index].items[j].price, db[index].name, db[index].gps.lat, db[index].gps.lon, db[index].n);
        poc++;
    }
}

void sortDistances(double arr[][2]){
    for(int i=0; i<DB_NUM-1; i++){
        for(int j=0; j<DB_NUM-1; j++){
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

void p_t(bool prepinac_W, int index_skladu, char *item_name2){
    static int poc=1;
    if(prepinac_W){
        printf("%s %.3lf %.3lf %d :\n", db[index_skladu].name, db[index_skladu].gps.lat, db[index_skladu].gps.lon, db[index_skladu].n);
        poc = 1;
    } 
    for(int i=0; i<db[index_skladu].n; i++){
        if(strcmp(db[index_skladu].items[i].name, item_name2)==0){
            if(prepinac_W) printf("%d. %s %d\n", poc, item_name2, db[index_skladu].items[i].price);
            else{
                printf("%d. %s %d : %s %.3lf %.3lf %d\n", 
                    poc, item_name2, db[index_skladu].items[i].price, db[index_skladu].name, db[index_skladu].gps.lat, db[index_skladu].gps.lon, db[index_skladu].n);
            }              
            poc++;
        }
    }
}

void p_p(bool prepinac_W, int index_skladu, int max){
    static int poc=1;
    if(prepinac_W){
        printf("%s %.3lf %.3lf %d :\n", db[index_skladu].name, db[index_skladu].gps.lat, db[index_skladu].gps.lon, db[index_skladu].n);
        poc=1;
    } 
    for(int j=0; j<db[index_skladu].n; j++){
        if(db[index_skladu].items[j].price<=max){
            if(prepinac_W) printf("%d. %s %d\n", poc, db[index_skladu].items[j].name, db[index_skladu].items[j].price);
            else{
                printf("%d. %s %d : %s %.3lf %.3lf %d\n", 
                    poc, db[index_skladu].items[j].name, db[index_skladu].items[j].price, db[index_skladu].name, db[index_skladu].gps.lat, db[index_skladu].gps.lon, db[index_skladu].n);
            }       
            poc++;
        }   
    } 
}

int main(int argc, char *argv[]){
    bool prepinac_w=false, prepinac_i=false, prepinac_n=false, prepinac_e=false, prepinac_t=false, prepinac_p=false,prepinac_W=false;
    char *name, *item_name, *item_name2, *n_string="", *e_string="";
    double lat=0, lon=0;
    int max; int opt;
    char* optstring = ":w:i:n:e:t:p:W";

    while((opt = getopt(argc, argv, optstring)) != -1){
        switch (opt){
            case 'w':
                name=optarg;
                prepinac_w = true;
                break;
            case 'i':
                item_name=optarg;
                prepinac_i = true;
                break;
            case 'n':
                lat=strtod(optarg, &n_string);
                prepinac_n = true;
            	break;
            case 'e':
                lon=strtod(optarg, &e_string);
                prepinac_e = true;
            	break;
            case 't':
                item_name2=optarg;
                prepinac_t = true;           
            	break;
            case 'p':
                sscanf(optarg, "%d", &max); 
                prepinac_p = true;        
            	break;
            case 'W':
                prepinac_W = true;        
            	break;
            case ':':
                //printf("returnula sa 2");
                return 2;
            default: 
                //printf("returnula sa 1");
                return 1;
        }
    }
    
    if((prepinac_n && !prepinac_e) || (!prepinac_n && prepinac_e)){
        //printf("returnula sa 3");
        return 3;
    }
    
    if((lat<(-90) || lat>90) || (lon<(-180) || lon>180) || strlen(n_string) != 0 || strlen(e_string) != 0){
        //printf("returnula sa 4");
        return 4;
    }
    
    if(prepinac_w){
        int index_skladu=getIndex(name);
        if(index_skladu!=0){
            if(prepinac_t){
                p_t(prepinac_W, index_skladu, item_name2);
            }
            else if(prepinac_p){
                p_p(prepinac_W, index_skladu, max);
            }
            else{
                if(prepinac_W) sklado_centricky_vypis(index_skladu);
                else tovaro_centricky_vypis(index_skladu);
            }           
        }
    }
    else if(prepinac_i){
        for(int i=0; i<DB_NUM; i++){
            if(checkItem(i, item_name)){
                if(prepinac_p){
                    p_p(prepinac_W, i, max);
                }
                else if(prepinac_t){
                    p_t(prepinac_W, i, item_name2);
                }
                else{
                    if(prepinac_W) sklado_centricky_vypis(i);
                    else tovaro_centricky_vypis(i);
                }              
            }
        }
    }
    else if(prepinac_n && prepinac_e){
        GPS user_input={lat,lon};
        double vzdialenosti[DB_NUM][2];
        
        for(int i=0; i<DB_NUM; i++){
            double vzdialenost=distance(user_input, db[i].gps);
            vzdialenosti[i][0]=vzdialenost;
            vzdialenosti[i][1]=i;
        }
        sortDistances(vzdialenosti);
        int najblizsi_sklad = vzdialenosti[0][1];
        if(prepinac_t){
            p_t(prepinac_W, najblizsi_sklad, item_name2);
        }
        else if(prepinac_p){
            p_p(prepinac_W, najblizsi_sklad, max);
        }
        else{
            if(prepinac_W) sklado_centricky_vypis(najblizsi_sklad);
            else tovaro_centricky_vypis(najblizsi_sklad);
        }       
    }
    //len prepinac -t (optional -W)
    if(prepinac_t && !prepinac_w && !prepinac_n && !prepinac_e && !prepinac_i){
        int poc=1;
        for(int i=0; i<DB_NUM; i++){
            if(prepinac_W){
                printf("%s %.3lf %.3lf %d :\n", db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                poc=1;
            } 
            for(int j=0; j<db[i].n; j++){
                if(strcmp(db[i].items[j].name, item_name2)==0){
                    if(prepinac_W) printf("%d. %s %d\n", poc, item_name2, db[i].items[j].price);
                    else printf("%d. %s %d : %s %.3lf %.3lf %d\n", poc, item_name2, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                    poc++;
                }
            }
        }
    } 
    //len prepinac -p (optional -W)
    else if(prepinac_p && !prepinac_w && !prepinac_n && !prepinac_e && !prepinac_i){
        int poc=1;
        for(int i=0; i<DB_NUM; i++){
            if(prepinac_W){
                printf("%s %.3lf %.3lf %d :\n", db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                poc=1;
            } 
            for(int j=0; j<db[i].n; j++){
                if(db[i].items[j].price<=max){
                    if(prepinac_W) printf("%d. %s %d\n", poc, db[i].items[j].name, db[i].items[j].price);
                    else{
                        printf("%d. %s %d : %s %.3lf %.3lf %d\n", 
                            poc, db[i].items[j].name, db[i].items[j].price, db[i].name, db[i].gps.lat, db[i].gps.lon, db[i].n);
                    }             
                    poc++;
                }
            }
        }
    }
    if(!prepinac_w && !prepinac_i && !prepinac_n && !prepinac_e && !prepinac_t && !prepinac_p){
        for(int i=0; i<DB_NUM; i++){
            if(prepinac_W) sklado_centricky_vypis(i);
            else tovaro_centricky_vypis(i);
        }
    }
    return 0;
}
