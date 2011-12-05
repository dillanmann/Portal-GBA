//================
//    HEADERS
//================
#include <stdint.h>
#include <stdlib.h>
#include "gba.h"

//================
//   Function: main()
//      Notes: Main entry point into rom
//================
int main()
{
//===============
//	Prototypes:
void DisplayPortals(int bluey, int orangey);
void DisplayPlayer(int&xp, int&yp);
void PortalCollision(int&xp, int&yp,int bluey,int orangey);
void PortalMovement(int&orangey, int&bluey);
void ShootPortal(int xp, int yp, int&bluey, int&orangey);
//===============
	//REG_DISPCNT (defined in gba.h) is the main display register that controls
	//the mode of operation which backgrounds are enabled and which buffer is
	//currently being drawn by the GPU.
	//this sets the screen mode to mode three and enables background 2
	//background 2 is required for all bitmap modes(3-5)
	REG_DISPCNT = MODE4 | BG2_ENABLE;

	// set colours
	SetPaletteBG(1,RGB(16,16,16)); // grey background
	SetPaletteBG(2,RGB(0,14,31)); // blue portal
	SetPaletteBG(3,RGB(0,16,31)); // blue shading
	SetPaletteBG(4,RGB(0,18,31)); // blue shading
	SetPaletteBG(5,RGB(0,20,31)); // blue shading
	SetPaletteBG(6,RGB(30,16,0)); // orange portal
	SetPaletteBG(7,RGB(30,18,0)); // orange shading
	SetPaletteBG(8,RGB(30,20,0)); // orange shading
	SetPaletteBG(9,RGB(30,22,0)); // orange shading
	SetPaletteBG(10,RGB(31,31,31)); // white player
	ClearScreen8(1);
		
	
	// initialise variables
	int x=120,y=70,bluetopy=5,orangetopy=110;

	// Main game loop	
	while (true){
		FlipBuffers();
		ClearScreen8(1);
		DisplayPortals(bluetopy,orangetopy);
		DisplayPlayer(x,y);
		PortalCollision(x,y,bluetopy,orangetopy);
		//PortalMovement(orangetopy,bluetopy);
		ShootPortal(x,y,bluetopy,orangetopy);
		WaitVSync();
	}
		
	return 0;
}

void DisplayPortals(int bluey, int orangey){	// Displays portal at specified co-ordinates.
	int i=0;
	// draw portals
	// blue
	for(i=0;i<11;i++){
		PlotPixel8(0,bluey,2);
		bluey++;
	}
	// orange
	for(i=0;i<11;i++){
		PlotPixel8(239,orangey,6);
		orangey++;
	}
}

void DisplayPlayer(int&xp, int&yp){ // Works out player location based on button presses and displays player in current location.
	if((REG_P1 & KEY_LEFT) ==0)
		xp--;
	if((REG_P1 & KEY_RIGHT) ==0)
		xp++;
	if((REG_P1 & KEY_UP) ==0)
		yp--;
	if((REG_P1 & KEY_DOWN) ==0)
		yp++;
		
	// movement outwith screen size prevention
	if (xp>SCREEN_WIDTH)
		xp=239;
	else if (xp<0)
		xp=0;
	if (yp>SCREEN_HEIGHT)
		yp=159;
	else if (yp<0)
		yp=0;
		
	PlotPixel8(xp,yp,10);
}

void PortalCollision(int&xp, int&yp,int bluey,int orangey){ // Computes whether the player is in a portal and moves them
	int i=0;
	for(i=0;i<11;i++){
		if ((xp == 0) && (yp == bluey)){
			xp=238;yp=orangey+5;
			break;
		}
		bluey++;
	}
	
	for(i=0;i<11;i++){
		if((xp==239) && (yp == orangey)){
			xp=1;yp=bluey-5;
			break;
		}
		orangey++;
	}
}

void ShootPortal(int xp, int yp, int&bluey, int&orangey){
	int xport=xp,yport=yp;
	if ((REG_P1 & KEY_R) ==0){
		xport=xp+1;
		for(int i=0;i<=(SCREEN_WIDTH-xp);i++){
				PlotPixel8(xp,yp,10);
				PlotPixel8(xport,yport,((rand()%4)+6));
				xport++;
				if(i>((SCREEN_WIDTH-xp)*(0.25))){
					PlotPixel8(xport+1,yport+1,((rand()%4)+6));
					PlotPixel8(xport+1,yport-1,((rand()%4)+6));
				}
				if(i>((SCREEN_WIDTH-xp)*(0.5))){
					PlotPixel8(xport+1,yport+1,((rand()%4)+6));
					PlotPixel8(xport+1,yport-1,((rand()%4)+6));
					PlotPixel8(xport+2,yport+2,((rand()%4)+6));
					PlotPixel8(xport+2,yport-2,((rand()%4)+6));
				}
				if(i>((SCREEN_WIDTH-xp)*(0.75))){
					PlotPixel8(xport+1,yport+1,((rand()%4)+6));
					PlotPixel8(xport+1,yport-1,((rand()%4)+6));
					PlotPixel8(xport+2,yport+2,((rand()%4)+6));
					PlotPixel8(xport+2,yport-2,((rand()%4)+6));
					PlotPixel8(xport+3,yport+3,((rand()%4)+6));
					PlotPixel8(xport+3,yport-3,((rand()%4)+6));
				}
				FlipBuffers();
				WaitVSync();
				ClearScreen8(1);
		}
		orangey = yport-5;
	}
	
	else if((REG_P1 & KEY_L) ==0){
		xport=xp-1;
		for(int i=0;i<(xp);i++){
			PlotPixel8(xp,yp,10);
			PlotPixel8(xport,yport,((rand()%2+3)));
			xport--;
				if(i>((xp)*(0.25))){
					PlotPixel8(xport-1,yport+1,((rand()%2)+3));
					PlotPixel8(xport-1,yport-1,((rand()%2)+3));
				}
				if(i>((xp)*(0.5))){
					PlotPixel8(xport-1,yport+1,((rand()%2)+3));
					PlotPixel8(xport-1,yport-1,((rand()%2)+3));
					PlotPixel8(xport-2,yport+2,((rand()%2)+3));
					PlotPixel8(xport-2,yport-2,((rand()%2)+3));
				}
				if(i>((xp)*(0.75))){
					PlotPixel8(xport-1,yport+1,((rand()%2)+3));
					PlotPixel8(xport-1,yport-1,((rand()%2)+3));
					PlotPixel8(xport-2,yport+2,((rand()%2)+3));
					PlotPixel8(xport-2,yport-2,((rand()%2)+3));
					PlotPixel8(xport-3,yport+3,((rand()%2)+3));
					PlotPixel8(xport-3,yport-3,((rand()%2)+3));
				}
			FlipBuffers();
			WaitVSync();
			ClearScreen8(1);	
		}
		bluey = yport-5;
	}

}

void PortalMovement(int&orangey, int&bluey){
	if ((REG_P1 & KEY_R) == 0){
		orangey = rand()%150+10;
		}
	if ((REG_P1 & KEY_L) == 0){
		bluey = rand()%150+10;
	}
}
