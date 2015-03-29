#ifndef _TRIANGLE_APP

#include <stdio.h>
#include <GL/glfw.h>
#include "simpleApp.h"
#include "imageTexture.h"
#include <videoInput.h>

#include <HighLevelMonitorConfigurationAPI.h>
#include "trace.h"

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x)  { if (x) { delete [] x; x = NULL; } }
#endif
#define TRACE_IF_FAILED(FnName, bSuccess) if (!bSuccess) { TRACE((L"%s FAILED (error %x)", FnName, GetLastError())); }


class triangleApp : public simpleApp{
	
	public:
		triangleApp();
		virtual bool idle();
		virtual void init(int dev_);
		virtual void draw();
		virtual void keyDown  (char c);
		virtual void mousewheelRotated(int position);
		virtual void mouseMove( float x, float y );
		virtual void mouseDrag( float x, float y );
		virtual void mouseDown( float x, float y, int button );
		virtual void mouseUp  ( float x, float y, int button );
		
	    BOOL OnEnumMonitor(HMONITOR hMonitor);
		void setBrightness(int monitor, int brightness);
		int m_brightness,m_noMonitor,m_targetMonitor,m_oldPosition,m_shift,m_avg,m_avg2;
		int m_maxVal[10];

		imageTexture * IT;
//		imageTexture * IT2;
		videoInput     VI;

		unsigned char * frame;
		unsigned char * frame2;

	private:
		BOOL AllocateMonitorHandles();
		BOOL FreeMonitorHandles();
		DWORD m_NumPhysicalMonitors;  // Number of physical monitors.
		PHYSICAL_MONITOR *m_pPhysicalMonitors;   // Array of physical monitor structures.

};

#endif	// _triangle_APP