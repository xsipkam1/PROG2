#include <stdio.h>

int vypocet_stavu_motora(double vsoft, double h, double de, double v){
    if(h<=de && v<vsoft){     // mal som povodne h<de kvoli tomu mam 9 bodov ><
        return 1;
    }
    return 0;
}

void vypis(int s, double h, double t, double v){
    printf("s=%d h=%08.3lf t=%.3lf v=%.3lf\n", s, h, t, v);
}

double vypocet_novej_vysky(double a, double h, double v, double dt){
    double hnew=h+v*dt+((a*dt*dt)/2);
    return hnew;
}

double vypocet_novej_rychlosti(double v, double a, double dt){
    double vnew=v+a*dt;
    return vnew;
}

double vypocet_de(double A, double g, double H){
    double df = ((A-g)*H)/A; 
    double de = H-df;
    return de;
}

double vypocet_a(int s, double g, double A){
    double a;
    if(s==1){
        a=A-g; // ak je motor zapnuty
    }
    else{
        a=-g;  // ak je motor vypnuty
    }
    return a;
}

int main(){
    double m,T,H,g,vsoft,dt;
    scanf("%lf %lf %lf %lf %lf %lf", &m, &T, &H, &g, &vsoft, &dt);
    printf("m=%.3lf\n", m);
    printf("T=%.3lf\n", T);
    printf("H=%.3lf\n", H);
    printf("g=%.3lf\n", g);
    printf("vsoft=%.3lf\n", vsoft);
    printf("dt=%.3lf\n", dt);

    const double A=T/m;
    double de=vypocet_de(A, g, H);
    printf("de=%.3lf\n", de);

    double v=0; //pociatocna rychlost sa vzdy rovna nule - casom sa bude aktualizovat na aktualnu rychlost
    double t=0; //uplynuty cas simulacie - bude sa priebezne aktualizovat (pripocitavat k tomu dt)  
    int s=0; //stav motora inicializovany na nulu
    double h=H; //aktualna vyska sa na zaciatku rovna pociatocnej vyske
    double a;
    int total=0; //celkovy pocet riadkov kedy bol motor zapnuty
    int max=0; //nepretrzity pocet riadkov kedy bol motor zapnuty
    int dlzka=0;

    while(h>0){
        s = vypocet_stavu_motora(vsoft, h, de, v);
        if(s==1){                  //zistuje sa total a max
            total+=1;
            dlzka+=1;
            if(dlzka>max){
                max=dlzka;
            }
        } 
        if(s!=1){
            if(dlzka>max){
                max=dlzka;
            }
            dlzka=0;
        }
        vypis(s, h, t, v);
        a=vypocet_a(s, g, A);
        h=vypocet_novej_vysky(a, h, v, dt);
        v=vypocet_novej_rychlosti(v, a, dt);
        t+=dt;
    }

    printf("---Landed---\n");
    printf("h    =%08.3lf\n", h);
    printf("t    =%8.3lf\n", t);
    printf("v    =%8.3lf\n", v);
    printf("total=%4d\n", total);
    printf("max  =%4d\n", max);

    return 0;
}
