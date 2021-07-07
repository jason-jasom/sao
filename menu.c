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

#define WIDTH     30
#define HEIGHT    20
#define OFFSET_X  5
#define OFFSET_Y  5

#define NUM_KEYS 7
#define REFRESH_RATE 10

void setMainmenu(Menu *m);

int timer(void);

int menu(void)
{
    int IsEnding = 0;
    int level=0;

    int k;
    char str[40] = {'\0'};

    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN, VK_SPACE};

    int cur_tick, last_tick;

    Menu mainmenu;

    Image* background;
    background = read_image("background.pixel", "background.color");



    /* �Ұ���L���� ��ӵ{�����u�n���@���N��F*/
    initializeKeyInput();
    cur_tick = last_tick = timer();

    setMainmenu(&mainmenu);

    show_image(background, 0, 0);
    saveScreen();


    /* �L�a�j��  �̭��]�t�F�o�ӵ{�����D�n�u�@
    �ӥB�C���j��|���_��̷s���e����ܨ�ù��W
    ���O���ʵe�@�� �C���j���s�@���e�� */
    while (!IsEnding) {



        /* �C�g�L REFRESH_RATE �� ticks �~�|��s�@���e�� */
        cur_tick = timer(); /* �C�� tick 0.01 �� */
        sprintf(str, "%10d", cur_tick/1000);

        if (cur_tick-last_tick > REFRESH_RATE) {

            last_tick = cur_tick;


           clearScreen();
           restoreScreen();

            putStringLarge(mainmenu.large_font, OFFSET_X, OFFSET_Y-1, str, 14);

            /* ����e�X�� */
            showMenu(&mainmenu);

            /* ���F�n���@�s�� putASCII2() ���ʧ@���ͮĪG
               �����n�I�s�@�� drawCmdWindow() �⤧�e�e���������e�@����ܨ�ù��W */
            drawCmdWindow();

        } /* end of if (cur_tick % REFRESH_RATE == 0 ... */

        for (k=0; k<NUM_KEYS; k++) {
            /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
            }
        }

        /* ��L���� �B�z���ۤ��񪺪��p */
        for (k=0; k<NUM_KEYS; k++) {
            /* ����q�쥻�Q���U�����A �ܦ���}�����A  �o�O���F�B�z���ۤ��񪺱��p */
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;

                switch (key_val[k]) {
                case VK_UP:
                    scrollMenu(&mainmenu, -1);
                    break;
                case VK_DOWN:
                    scrollMenu(&mainmenu, +1);
                    break;
                case VK_RETURN:
                    if (IsOnItem(&mainmenu, 3)) {
                        if (IsItemSelected(&mainmenu, 0)) {
                            PlaySound(TEXT("metalplate.wav"), NULL, SND_ASYNC|SND_FILENAME);
                            level=0;
                            IsEnding=1;
                        }
                        if (IsItemSelected(&mainmenu, 1)) {
                            PlaySound(TEXT("metalplate.wav"), NULL, SND_ASYNC|SND_FILENAME);
                            level=1;
                            IsEnding=1;
                        }
                        if (IsItemSelected(&mainmenu, 2)) {
                            PlaySound(TEXT("metalplate.wav"), NULL, SND_ASYNC|SND_FILENAME);
                            level=2;
                            IsEnding=1;
                        }
                    }
                    break;
                case VK_SPACE:
                    /* �i�H�ոլݩ��U�t�@�ؿ��Φ�
                        �P�ɯ঳�h�Ӷ��إi�H�Q���
                        toggleMenu(&mainmenu);
                    */
                    radioMenu(&mainmenu);
                    break;
                }

            }

        }

    } /* while (IsEnding) */
	clearScreen();
	return level;
}

int timer(void)
{
    return (clock()/(0.001*CLOCKS_PER_SEC));
}

void setMainmenu(Menu *m)
{
    int i;

    m->large_font = read_font("font.txt");
    m->x = 2;
    m->y = 2;
    m->fgcolor = 7;
    m->bgcolor = 15;
    m->num_options = 4;
    m->cursor = 0;
    for (i=0; i<m->num_options; i++) {
        m->state[i] = 0;
    }
    m->state[m->cursor] = m->state[m->cursor] | 1;  /* �ثe��ܪ����� */

    strcpy(m->text[0], "EASY");
    strcpy(m->text[1], "NORMAL");
    strcpy(m->text[2], "HARD");
    strcpy(m->text[3], "START");
    strcpy(m->alt_text[0], "EASY *");
    strcpy(m->alt_text[1], "NORMAL *");
    strcpy(m->alt_text[2], "HARD *");
    strcpy(m->alt_text[3], "START *");
}

