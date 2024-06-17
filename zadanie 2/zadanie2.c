#include <stdio.h>
#include <stdbool.h>

#define NMIN 10
#define NMAX 100
#define R_MAX 2147483647 

static long long unsigned int SEED = 0x1; 
void srnd(int seed){ 
    SEED = seed;
}

int rnd(){
    SEED = SEED * 16807 % R_MAX;
    return 1 + (int) SEED % 6;
}

int max(int r1, int r2){
    return r1>r2 ? r1 : r2;
}

int checkTunely(int tunely[][2], int dlzka_t){
    for(int i=0; i<dlzka_t; i++){
        for(int j=i+1; j<dlzka_t; j++){
            if((tunely[i][0]==tunely[j][0]) || (tunely[i][0]==tunely[j][1]) || (tunely[i][1]==tunely[j][0])){
                return 1;
            }
        }
    }
    return 0;
}

void vypisTunelov(int tunely[][2], int dlzka_t){
    if(dlzka_t==0) printf("TUNNELS:");
    else{
        printf("TUNNELS: ");
        for(int i=0; i<dlzka_t-1; i++){
            for(int j=0; j<dlzka_t-1; j++){
                if(tunely[j][0]>tunely[j+1][0]){
                    int t=tunely[j][0];
                    int t2=tunely[j][1];
                    tunely[j][0]=tunely[j+1][0];
                    tunely[j][1]=tunely[j+1][1];
                    tunely[j+1][0]=t;
                    tunely[j+1][1]=t2;
                }
            }
        }
        for(int i=0; i<dlzka_t; i++){
            printf("[%d,%d] ", tunely[i][0], tunely[i][1]);
        }
    }   
}

int exitTunel(int tunely[][2], int dlzka_t, int pozicia){
    for(int i=0; i<dlzka_t; i++){
        if(pozicia==tunely[i][0]){
            pozicia=tunely[i][1];
            return pozicia;
        }
    }
}

int enterTunel(int tunely[][2], int dlzka_t, int pozicia){
    for(int i=0; i<dlzka_t; i++){
        if(pozicia==tunely[i][0]){
            return 1;
        }
    }
    return 0;
}

int main(){
    int s, t, n;
    int enter, exit;
    scanf("%d %d %d", &s, &t, &n);
    int tunely[t][2];
    int draha[n];
    srnd(s);

    for(int i=0; i<n; i++){
        draha[i]=0;
    }

    if((s>0) && (t*2<=n/2) && (n>=NMIN && n<=NMAX)){
        for(int i=0; i<t; i++){
            scanf("%d %d", &enter, &exit);
            if(enter>0 && (exit<n-1) && (enter<n-1) && exit>0 && enter!=exit){
                tunely[i][0]=enter;
                tunely[i][1]=exit;
            }
            else{
                return 2;
            }
        }
        if(checkTunely(tunely, t)){
            return 2;
        }
    }
    else{
        return 1;
    }

    vypisTunelov(tunely, t);
    int round=1; char winner;
    int Player_pos = -1, Oponent_pos = -1;
    int Player_r1=0, Player_r2=0;
    int Oponent_r1=0, Oponent_r2=0;
    bool Player_turn=true, Oponent_turn=false;

    while(Player_pos<n && Oponent_pos<n){
        while(Player_turn){
            if(Player_pos==-1){
                Player_r1=rnd(); Player_r2=rnd();
                printf("\n[%d,1] [%d] [%d,%d] ", round, Player_pos, Player_r1, Player_r2);
                if(Player_r1+Player_r2>7){
                    Player_pos=Player_r1+Player_r2-7;
                    if(enterTunel(tunely, t, Player_pos)){
                        Player_pos=exitTunel(tunely,t,Player_pos);
                        if(Player_pos==Oponent_pos){
                            Oponent_pos = -1;
                            printf("[%d] T E", Player_pos);
                        }
                        else printf("[%d] T", Player_pos);
                    } 
                    else{           
                        if(Player_pos==Oponent_pos){
                            Oponent_pos = -1;
                            printf("[%d] E", Player_pos);
                        }
                        else printf("[%d]", Player_pos);
                    }
                    
                    draha[Player_pos] += 1;
                    Oponent_turn=true; Player_turn=false; break;
                }
                printf("[%d]", Player_pos);
                
            }
            else{
                Player_r1=rnd(); Player_r2=rnd();
                printf("\n[%d,1] [%d] [%d,%d] ", round, Player_pos, Player_r1, Player_r2);             
                if((Player_r1==6 && Player_r2==6) && Player_pos<Oponent_pos){
                    int temp=Player_pos;
                    Player_pos=Oponent_pos;
                    Oponent_pos=temp;
                    draha[Oponent_pos]+=1;
                    printf("[%d] S", Player_pos);
                }
                else if((Player_r1==1 && Player_r2==1) && Player_pos>Oponent_pos && Oponent_pos!=-1){
                    int temp=Player_pos;
                    Player_pos=Oponent_pos;
                    Oponent_pos=temp;
                    draha[Oponent_pos]+=1;
                    printf("[%d] S", Player_pos);
                }
                else{
                    Player_pos+=max(Player_r1, Player_r2);
                    if(enterTunel(tunely, t, Player_pos)){
                        Player_pos=exitTunel(tunely,t,Player_pos);
                        if(Player_pos==Oponent_pos){
                            Oponent_pos = -1;
                            printf("[%d] T E", Player_pos);
                        }
                        else printf("[%d] T", Player_pos);
                    } 
                    else if(Player_pos==Oponent_pos){
                        Oponent_pos = -1;
                        printf("[%d] E", Player_pos);
                    }  
                    else  printf("[%d]", Player_pos);
                }
                draha[Player_pos] += 1;
            }
            if(Player_pos>=n){
                winner='1';
                Oponent_turn=false;
                break;
            }
            Oponent_turn=true; Player_turn=false; 
        }
        round++;
        while(Oponent_turn){
            if(Oponent_pos==-1){
                Oponent_r1=rnd(); Oponent_r2=rnd();
                printf("\n[%d,2] [%d] [%d,%d] ", round, Oponent_pos, Oponent_r1, Oponent_r2);
                if(Oponent_r1+Oponent_r2>7){
                    Oponent_pos=Oponent_r1+Oponent_r2-7;
                    if(enterTunel(tunely, t, Oponent_pos)){
                        Oponent_pos=exitTunel(tunely,t,Oponent_pos);
                        if(Oponent_pos==Player_pos){
                            Player_pos = -1;
                            printf("[%d] T E", Oponent_pos);
                        }
                        else printf("[%d] T", Oponent_pos);
                    } 
                    else{
                        if(Oponent_pos==Player_pos){
                            Player_pos = -1;
                            printf("[%d] E", Oponent_pos);
                        }
                        else printf("[%d]", Oponent_pos);
                    }
                    draha[Oponent_pos] += 1;
                    Player_turn=true; Oponent_turn=false; break;
                }
                printf("[%d]", Oponent_pos);             
            }
            else{
                Oponent_r1=rnd(); Oponent_r2=rnd();
                printf("\n[%d,2] [%d] [%d,%d] ", round, Oponent_pos, Oponent_r1, Oponent_r2);
                if((Oponent_r1==6 && Oponent_r2==6) && Oponent_pos<Player_pos){
                        int temp=Oponent_pos;
                        Oponent_pos=Player_pos;
                        Player_pos=temp;
                        draha[Player_pos]+=1;
                        printf("[%d] S", Oponent_pos);
                }
                else if((Oponent_r1==1 && Oponent_r2==1) && Oponent_pos>Player_pos && Player_pos!=-1){
                        int temp=Oponent_pos;
                        Oponent_pos=Player_pos;
                        Player_pos=temp;
                        draha[Player_pos]+=1;
                        printf("[%d] S", Oponent_pos);
                }
                else{
                    Oponent_pos+=max(Oponent_r1, Oponent_r2);
                    if(enterTunel(tunely, t, Oponent_pos)){
                        Oponent_pos=exitTunel(tunely,t,Oponent_pos);
                        if(Oponent_pos==Player_pos){
                            Player_pos = -1;
                            printf("[%d] T E", Oponent_pos);
                        }
                        else printf("[%d] T", Oponent_pos);
                    }
                    else if(Oponent_pos==Player_pos){
                        Player_pos = -1;
                        printf("[%d] E", Oponent_pos);
                    }
                    else printf("[%d]", Oponent_pos);
                }   
                draha[Oponent_pos] += 1; 
            }
            if(Oponent_pos>=n){
                winner='2';
                Player_turn=false;
                break;
            }
            Player_turn=true; Oponent_turn=false;
        }
        round++;
    }
    printf("\nWINNER: %c\n", winner);
    printf("VISITS: ");
    for(int i=0; i<sizeof(draha)/sizeof(draha[0]); i++){
        printf("%d ", draha[i]);
    }
    return 0;
}
