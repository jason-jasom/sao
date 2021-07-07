#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include "audio.h" 
#include "font.h" 
#include "image.h" 
#include "kb_input2.h" 
#include "console_draw3.h" 

void startanimation(void){
	initializeKeyInput();
	char ch;
	int i=1;
	Font *large_font=read_font("font.txt");
	int flag=0;
	Image *sao=read_image("sao.pixel","sao.color");
	Image *boss=read_image("boss.pixel","boss.color");
	while(i){ 
		clearScreen();
		putStringLarge(large_font,1,1,"SWORD ART ONLINE",1);
		putString(100,20,"enter to start",1,7);
		flag=(flag+1)%2;
		if(flag==0){
			show_image(sao,10,10);
		}
		else{
			show_image(boss,10,10);
		}
		drawCmdWindow();
		if(waitForKeyDown(0.5)){
			ch=getKeyEventASCII();
			
			switch(ch){
				case VK_RETURN:
					i=0;
					break;
			}
		}
		Sleep(600);
	}
	destroy_image(sao);
	destroy_image(boss);
	clearScreen();
}
