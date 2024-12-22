/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "ps2_core.h"
#include "spi_core.h"
#include "math.h"


volatile uint8_t click = 0;
uint8_t hit = 0;
uint8_t miss = 0;

/**********************RAT GAME*******************************/
float get_mag(SpiCore *spi_p);
void display_number(OsdCore &osd, int col, int row, uint8_t number);

void osd_and_mouse_check(OsdCore *osd_p, Ps2Core *ps2_p) {
   int lb, rb, x, y;

   osd_p->set_color(0x0f0, 0x001); // dark gray/green
   osd_p->bypass(0);
   osd_p->clr_screen();
   const char text[] = "You are in a lab where rats are tested!";
   const char text2[]= "Some of them have started to act up >:C";
   const char text3[] ="Get ready to exterminate!";
   const char text4[]= "Click anywhere to BEGIN!";

   for (int i = 0; i < 39; i++) {
       osd_p->wr_char(18 + i, 12, text[i]);
       sleep_ms(50);
   }
   for (int i = 0; i < 39; i++) {
	   osd_p->wr_char(18 + i, 13, text2[i]);
       sleep_ms(50);
   }
   for (int i = 0; i < 25; i++) {
	   osd_p->wr_char(25 + i, 14, text3[i]);
       sleep_ms(50);
   }
   for (int i = 0; i < 24; i++) {
	   osd_p->wr_char(26 + i, 15, text4[i]);
       sleep_ms(50);
   }

   while(lb != 1 || rb != 1){
   ps2_p->get_mouse_activity(&lb, &rb, &x, &y);
   if (lb == 1 || rb == 1) {
   		click++;
   		return;
   	}
   }
   sleep_ms(1);
}


void rat_and_hammer_and_target_check(SpriteCore *rat_p, SpiCore *spi_p ,SpriteCore *hammer_p, SpriteCore *target_p, OsdCore &osd) {
    int x_rat, y_rat;
    int x_hammer,y_hammer;
    int x_target,y_target;

    x_rat = 620;
    y_rat = 200;
    y_hammer = 195;
    x_hammer = 95;
    y_target = 215;
    x_target = 100;

    rat_p->bypass(0);
    target_p->bypass(0);
    target_p->move_xy(x_target, y_target);

    for (int i = 0; i < 156; i++) {
    	float mag = get_mag(spi_p);
        rat_p->move_xy(x_rat, y_rat);
        x_rat = x_rat - 4;
        if(mag < 1){
        	hammer_p -> bypass(0);
            hammer_p->move_xy(x_hammer, y_hammer);
            //PERFECT SCORE
            if (x_rat >= 99 && x_rat <= 101){
               	rat_p->bypass(1);
               	hit = hit + 10;
               	display_number(osd, 15, 0, hit);

               	break;
            }
            //AWESOME
            else if (x_rat >=98 && x_rat <= 102){
               	rat_p->bypass(1);
               	hit = hit + 8;
               	display_number(osd, 15, 0, hit);

               	break;
            }
            //GOOD
            else if (x_rat >=97 && x_rat <= 103){
               	 rat_p->bypass(1);
               	 hit = hit + 6;
               	 display_number(osd, 15, 0, hit);

               	break;
            }
            //ITS OK
            else if (x_rat >=96 && x_rat <= 104){
               	 rat_p->bypass(1);
               	 hit = hit + 4;
               	 display_number(osd, 15, 0, hit);

               	break;
            }
            //GET BETTER
            else if (x_rat >=95 && x_rat <= 105){
               	 rat_p->bypass(1);
               	 hit = hit + 2;
               	 display_number(osd, 15, 0, hit);

               	break;
            }
            else if(x_rat > 105 && x_rat < 560){
               	 miss++;
               	 display_number(osd, 15, 1, miss);
            }
         }
         else{
        	 hammer_p -> bypass(1);
         }
       sleep_ms(50);
    }
    sleep_ms(100);
}

float get_mag(SpiCore *spi_p) {
	const uint8_t RD_CMD = 0x0b;
	const uint8_t DATA_REG = 0x08;
	const float raw_max = 127.0 / 2.0;

	    int8_t xraw, yraw, zraw;
	    float x, y, z;
	    float mag;

	    spi_p->set_freq(400000);
	    spi_p->set_mode(0, 0);
	    spi_p->assert_ss(0);
	    spi_p->transfer(RD_CMD);
	    spi_p->transfer(DATA_REG);

	    xraw = spi_p->transfer(0x00);
	    yraw = spi_p->transfer(0x00);
	    zraw = spi_p->transfer(0x00);
	    spi_p->deassert_ss(0);
	    x = (float)xraw / raw_max;
	    y = (float)yraw / raw_max;
	    z = (float)zraw / raw_max;

	    mag = fabs(x) + fabs(y) + fabs(z);
	    sleep_ms(1);
	    uart.disp(mag);
	    uart.disp("\n\r");
	    return mag;
}

void display_number(OsdCore &osd, int col, int row, uint8_t number) {
    char digits[3];

    int i = 0;
    do {
        digits[i++] = (number % 10) + 48;
        number /= 10;
    } while (number > 0);

    for (int j = i - 1; j >= 0; j--) {
    	osd.wr_char(col++, row, digits[j]);
    }
}

// external core instantiation
FrameCore frame(FRAME_BASE);
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
SpriteCore target(get_sprite_addr(BRIDGE_BASE, V3_USER3), 1024);
SpriteCore rat(get_sprite_addr(BRIDGE_BASE, V4_USER4), 1024);
SpriteCore hammer(get_sprite_addr(BRIDGE_BASE, V5_USER5), 1024);
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));

int main() {
	int id = -1;
	while (id != 2) {
		id = ps2.init();
	}

	rat.bypass(1);
	hammer.bypass(1);
	target.bypass(1);
	frame.clr_screen(0x1A);


	while(click == 0){
	osd_and_mouse_check(&osd, &ps2);
	}

	if(click > 0) {

		const char text[] = "Number of Hits:";
		const char text2[]= "Number of Miss:";


		osd.set_color(0x0f0, 0x1A);
		osd.bypass(0);
		osd.clr_screen();

		for (int i = 0; i < 15; i++) {
		 		osd.wr_char(i, 0, text[i]);
		}
		for (int i = 0; i < 15; i++) {
		 		osd.wr_char(i, 1, text2[i]);
		}

		while (1) {
		 rat_and_hammer_and_target_check(&rat, &spi, &hammer, &target, osd);
	}
   } // while
} //main
