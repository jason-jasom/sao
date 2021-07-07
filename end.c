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

void end(int a){
	Font *large_font=read_font("font.txt");
	clearScreen();
	initializeKeyInput();
	Image *win=read_image("win.pixel","win.color");
	Image *lose=read_image("lose.pixel","lose.color");
	char ch;
	int i=1;
	while(i){
		putStringLarge(large_font,100,20,"esc to end",1);
		if(a==0){
			putStringLarge(large_font,1,1,"GAME OVER",1);
			show_image(lose,10,10);
		}
		else if(a==1){
			putStringLarge(large_font,1,1,"Congratulations!!",1);
			show_image(win,10,10);
		}
		if(waitForKeyDown(0.5)){
			ch=getKeyEventASCII();
			switch(ch){
				case VK_ESCAPE:
					i=0;
					break;
			}
		}
		drawCmdWindow();
		Sleep(300);
	}
	clearScreen();
}
