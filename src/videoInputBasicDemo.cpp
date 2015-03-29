// videoInputBasicDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <shlwapi.h>


triangleApp TAPP;
	 

void GLFWCALL keyfun( int key, int action )
{
    if( action != GLFW_PRESS )
    {
        return;
    }

    switch( key )
    {
	    case GLFW_KEY_ESC:
        	printf( "ESC => quit program\n" );
        	break;    	
       
    }

	TAPP.keyDown(key);

    fflush( stdout );
}

void GLFWCALL mousewheelfun( int position ) {
	printf( "mousewheelfun(%d)\n",position );
    fflush( stdout );
	TAPP.mousewheelRotated(position);
}

void GLFWCALL windowsizefun( int width, int height ) {
	printf( "windowsizefun(%i, %i)\n",width,height );
    fflush( stdout );
}


int main(int argc, _TCHAR* argv[])
{


    int     width, height, running, frames;
    double  t, t0, fps;
    char    titlestr[ 200 ];
	int p=0;
	
	printf("BLUntrl written 2009, 2010 by Sven Killig <sven@killig.de>\n");
	printf("BLUntrl.exe [ VfWDeviceNo | MonitorNo (-1=all) Brightness (0-100%) ]\n");
//printf("argc=%i\n", argc);
	if(argc==3) {
		TAPP.setBrightness(atoi((char*)argv[1]),atoi((char*)argv[2]));
		return 0;
	} else if(argc==2) {
		// all doesn't work?!
		//printf("argv[1]='%s'\n", argv[1]);
		//p=StrToInt(argv[1]);
		/*if(StrToIntEx(argv[1], STIF_DEFAULT, &p)) printf("StrToIntEx=true\n");
		else printf("StrToIntEx=false\n");*/
		p=atoi((char*)argv[1]);
		//printf("p=%i\n", p);
	}

    // Initialise GLFW
    glfwInit();

    // Open OpenGL window    
    if (!glfwOpenWindow(160,120,    // Open window
    24, 24, 24,                                // Red, green, and blue bits for color buffer
    24,                                        // Bits for alpha buffer
    24,                                        // Bits for depth buffer (Z-buffer)
    24,                                        // Bits for stencil buffer
    GLFW_WINDOW)){
        glfwTerminate();
        return 0;
    }
    
    glfwSetKeyCallback( keyfun );
    glfwSetMouseWheelCallback( mousewheelfun );
    glfwSetWindowSizeCallback( windowsizefun );
    TAPP.init(p);
    glfwEnable( GLFW_STICKY_KEYS );
glfwDisable( GLFW_AUTO_POLL_EVENTS );
    // Disable vertical sync (on cards that support it)
    glfwSwapInterval( 1 );

    // Main loop
    running = GL_TRUE;
    frames = 0;
    t0 = glfwGetTime();
	int i=0;
    while( running ){
i++;
        // Get time and mouse position
        t = glfwGetTime();
        //glfwGetMousePos( &x, &y );
        
		if(TAPP.idle() && !glfwGetWindowParam(GLFW_ICONIFIED)) {
			frames ++;
			 
			
			// Get window size (may be different than the requested size)
			glfwGetWindowSize( &width, &height );
			height = height > 0 ? height : 1;
			// Set viewport
			glViewport( 0, 0, width, height );
			// Clear color buffer
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear( GL_COLOR_BUFFER_BIT );

			TAPP.draw();			
			// Swap buffers
			glfwSwapBuffers();

        // Calculate and display FPS (frames per second)
        //if( (t-t0) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);
            sprintf( titlestr, "BLUntrl (%.1f FPS); %i", fps, i );
            glfwSetWindowTitle( titlestr );
            t0 = t;
            frames = 0;
        }

		} else Sleep(1000/30);
glfwPollEvents();
		
        // Check if the ESC key was pressed or the window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) /*&&
                  glfwGetWindowParam( GLFW_OPENED )*/;
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
