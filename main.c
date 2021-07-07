#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include "audio.h" 
#include "font.h" 
#include "image.h" 
#include "kb_input2.h" 
#include "console_draw3.h" 
#include "framework.h"

#define MAPSIZE 22
#define OFFSET_X  2
#define OFFSET_Y  2
#define NUM_KEYS 9
#define REFRESH_DURATION 5


//外部指令
extern void startanimation(void);
extern int menu(void);
extern int level;
extern void end(int);

//內部指令 
typedef struct character{
	int x;
	int y;
	int color;
	int direction;
	int HP;
	int atk;
	int energy;
	char c;
}Character;

typedef struct enemy{
	int x;
	int y;
	int color;
	int direction;
	int HP;
	int atk;
	int tick;
	int duration;
	char c;
}Enemy;

Audio audio[2];

Enemy* setenemy(int);
Character* setkirito(void);
void setmap(int,Character*,Enemy*);

void showmap(int,Character*,Enemy*);
void showkiritolife(Character*);
void showenemylife(Enemy*);

void kiritohurt(Character*,Enemy*);
void kiritoattack(Character*,Enemy*);
void vertical(Character*,Enemy*);
void starburst_stream(Character*,Enemy*);
void black(Character*);
void increaseenergy(Character*);

void moveboss(Character*,Enemy*);
void bosshurt(Enemy*,int);
void bossattack(Character*,Enemy*);


int check1(Character*,Enemy*);
int check2(Character*,Enemy*);

char map[MAPSIZE][MAPSIZE];
char dic[4]={'^','<','V','>'};

int main(void){
	startanimation();
	
	int ending;
	int isend=0;
	char logo[] = "SWORD ART ONLINE";
	int i,k;
	
	Character *kirito;
	Enemy *boss;
	
	/* 用來計時 */
    clock_t startc;
    int game_tick = 0;
    
    openAudioFile("DR220A_BD.wav", &audio[0]);
	openAudioFile("DR220A_RIM.wav", &audio[1]);
    
    
    /* 變數 vk 用來記錄使用者按下的鍵 */
    int vk;
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_S, VK_V, VK_SPACE};
    
    kirito=setkirito();
    boss=setenemy(menu());
    startc = clock();
    
    initializeKeyInput();
    
	while(!isend){
		
		if ((double)(clock()-startc) > 0.01*CLOCKS_PER_SEC) {
            game_tick++;
            startc = clock();
        }
    
        if (game_tick > REFRESH_DURATION) {
            game_tick = 0;
            clearScreen();

            putString(OFFSET_X, OFFSET_Y, logo, 14, 3);
            
            setmap(MAPSIZE,kirito,boss);
            showmap(MAPSIZE,kirito,boss);
            showkiritolife(kirito);
			showenemylife(boss);

            drawCmdWindow();  /* update window immediately */
        }
        
        moveboss(kirito,boss);
        
        for (k=0; k<NUM_KEYS; k++) {
            /* 按鍵從原本被按下的狀態 變成放開的狀態  這是為了處理按著不放的情況 */
            
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
				black(kirito);
                switch (key_val[k]) {
                case VK_UP:
                    if(kirito->x-1!=0){
                    	kirito->x--;
                    	kirito->direction=0;
                    	kirito->c=dic[kirito->direction];
					}
                    break;
                case VK_DOWN:
                    if(kirito->x+1!=MAPSIZE-1){
                    	kirito->x++;
                    	kirito->direction=2;
                    	kirito->c=dic[kirito->direction];
					}
                    break;
                case VK_LEFT:
                    if(kirito->y-1!=0){
                    	kirito->y--;
                    	kirito->direction=1;
                    	kirito->c=dic[kirito->direction];
					}
                    break;
                case VK_RIGHT:
                    if(kirito->y+1!=MAPSIZE-1){
                    	kirito->y++;
                    	kirito->direction=3;
                    	kirito->c=dic[kirito->direction];
					}
                    break;
                case VK_SPACE:
                    kiritoattack(kirito,boss);
                    increaseenergy(kirito);
                    break;
                case VK_V:
                    vertical(kirito,boss);
                    break;
                case VK_S:
                    starburst_stream(kirito,boss);
                    break;
                }
                

            }
            
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }

        }
        
        if(check1(kirito,boss)==1){
        	isend=1;
		}
	}
	
	clearScreen();
    ending=check2(kirito,boss);
	
	end(ending);
	return 0;
}

Character* setkirito(void){
	Character *k;
	
	k = (Character*) malloc( sizeof(Character) );
	
	k->x=20;
	k->y=20;
	k->color=0;
	k->HP=100;
	k->atk=1000;
	k->energy=0;
	k->direction=0;
	k->c=dic[k->direction];
	return k;
}

Enemy* setenemy(int a){
	Enemy*g;
	g = (Enemy*) malloc( sizeof(Enemy) );
	g->color=1;
	g->x=1;
	g->y=1;
	if(a==0){
		g->HP=20000;
		g->atk=5;
		g->duration=250000;
	}
	else if(a==1){
		g->HP=50000;
		g->atk=10;
		g->duration=200000;
	}
	else if(a==2){
		g->HP=100000;
		g->atk=20;
		g->duration=150000;
	}
	g->c='g';
	g->tick=0;
	return g;
}

void setmap(int a,Character*k,Enemy*g){
	int i,j;
	for(i=0;i<a;i++){
		for(j=0;j<a;j++){
			if(i==0||j==0||i==a-1||j==a-1){
				map[i][j]='#';
			}
			else{
				map[i][j]='.';
			}
		}
	}
}

void showmap(int a,Character*k,Enemy*g){
	int i,j;
	for(i=0;i<a;i++){
		for(j=0;j<a;j++){
			if(i==k->x&&j==k->y){
				putASCII2(j + OFFSET_X, i + OFFSET_Y+2, k->c , k->color, 7);
			}
			else if(i==g->x&&j==g->y){
				putASCII2(j + OFFSET_X, i + OFFSET_Y+2, g->c , g->color, 7);
			}
			else{
				putASCII2(j + OFFSET_X, i + OFFSET_Y+2, map[i][j] , 9, 7);
			}
		}
	}
}

void showkiritolife(Character*k){
	char a[]="kirito's life:";
	char b[]="energy:";
	char c[3];
	char d[3];
	sprintf(c,"%d",k->HP);
	sprintf(d,"%d",k->energy);
	putString( OFFSET_X+25,  OFFSET_Y, a, 0, 7);
	putString( OFFSET_X+39,  OFFSET_Y, c, 0, 7);
	putString( OFFSET_X+25,  OFFSET_Y+1, b, 0, 7);
	putString( OFFSET_X+39,  OFFSET_Y+1, d, 0, 7);
}

void showenemylife(Enemy*g){
	char a[]="boss's life:";
	char c[3];
	sprintf(c,"%d",g->HP);
	putString( OFFSET_X+25,  OFFSET_Y+3, a , 1, 7);
	putString( OFFSET_X+39,  OFFSET_Y+3, c, 1, 7);
}

void kiritohurt(Character*k,Enemy*g){
	k->HP=k->HP-g->atk;
	k->color=12;
	playAudio(&audio[0]);
}

void kiritoattack(Character*k,Enemy*g){
	int i,j;
	if(k->direction==0){//up
		if(k->x-1==g->x&&k->y==g->y){
			bosshurt(g,k->atk);
		}
	}
	else if(k->direction==1){//left
		if(k->x==g->x&&k->y-1==g->y){
			bosshurt(g,k->atk);
		}
	}
	else if(k->direction==2){//down
		if(k->x+1==g->x&&k->y==g->y){
			bosshurt(g,k->atk);
		}
	}
	else if(k->direction==3){//right
		if(k->x==g->x&&k->y+1==g->y){
			bosshurt(g,k->atk);
		}
	}
	if(k->x==g->x&&k->y==g->y){
		bosshurt(g,k->atk);
	}
}

void vertical(Character*k,Enemy*g){
	int i,j;
	if(k->energy>=25){
		if(k->direction==0){//up
			if(k->x-1==g->x&&k->y==g->y){
				bosshurt(g,k->atk*4);
			}
		}
		else if(k->direction==1){//left
			if(k->x==g->x&&k->y-1==g->y){
				bosshurt(g,k->atk*4);
			}
		}
		else if(k->direction==2){//down
			if(k->x+1==g->x&&k->y==g->y){
				bosshurt(g,k->atk*4);
			}
		}
		else if(k->direction==3){//right
			if(k->x==g->x&&k->y+1==g->y){
				bosshurt(g,k->atk*4);
			}
		}
		if(k->x==g->x&&k->y==g->y){
			bosshurt(g,k->atk*4);
		}
		k->energy=k->energy-25;
	}
}

void starburst_stream(Character*k,Enemy*g){
	int i,j;
	if(k->energy>=100){
		if(k->direction==0){//up
			if(k->x-1==g->x&&k->y==g->y){
				bosshurt(g,k->atk*16);
			}
		}
		else if(k->direction==1){//left
			if(k->x==g->x&&k->y-1==g->y){
				bosshurt(g,k->atk*16);
			}
		}
		else if(k->direction==2){//down
			if(k->x+1==g->x&&k->y==g->y){
				bosshurt(g,k->atk*16);
			}
		}
		else if(k->direction==3){//right
			if(k->x==g->x&&k->y+1==g->y){
				bosshurt(g,k->atk*16);
			}
		}
		if(k->x==g->x&&k->y==g->y){
			bosshurt(g,k->atk*16);
		}
		k->energy=k->energy-100;
	}
}

void black(Character*k){
	
	k->color=0;

}

void increaseenergy(Character*k){
	if(k->energy<100){
		k->energy=k->energy+10;
	}
	if(k->energy>100){
		k->energy=100;
	}
}

void moveboss(Character*k,Enemy*g){
	if(g->tick!=g->duration){
		g->tick++;
	}
	else{
		g->color=1;
		g->direction=rand()%5;
		if(g->direction==0){
			if(g->x-1!=0){
				g->x--;
			}
		}
		else if(g->direction==1){
			if(g->y-1!=0){
				g->y--;
			}
		}
		else if(g->direction==2){
			if(g->x+1!=MAPSIZE-1){
				g->x++;
			}
		}
		else if(g->direction==3){
			if(g->y+1!=MAPSIZE-1){
				g->y++;
			}
		}
		else if(g->direction==4){
			bossattack(k,g);
		}
		g->tick=0;
	}
}

void bosshurt(Enemy*g,int a){
	g->HP=g->HP-a;
	g->color=12;
	playAudio(&audio[1]);
}

void bossattack(Character*k,Enemy*g){
	g->color=9;
	if((k->x)>=(g->x-1)&&(k->x)<=(g->x+1)&&(k->y)>=(g->y-1)&&(k->y)<=(g->y+1)){
		kiritohurt(k,g);
	}
}

int check1(Character*k,Enemy*g){
	if(g->HP<=0||k->HP<=0){
		return 1;
	}
	else{
		return 0;
	}
}

int check2(Character*k,Enemy*g){
	if(g->HP<=0){
		return 1;
	}
	else if(k->HP<=0){
		return 0;
	}
}
