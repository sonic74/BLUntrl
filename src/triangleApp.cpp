#include "triangleApp.h"

#include <Dshow.h>

//The first device we want to open
int dev;
double tOld;


BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor DC
  LPRECT lprcMonitor, // monitor intersection rectangle
  LPARAM dwData       // data
);


//empty constructor
triangleApp::triangleApp() {
	for(int i=0; i<sizeof(m_maxVal)/sizeof(int); i++) m_maxVal[i]=-1;
	m_brightness=-1;
}

void triangleApp::init(int dev_){

	dev=dev_;

	//optional static function to list devices
	//for silent listDevices use listDevices(true);
	int numDevices = videoInput::listDevices();

	//uncomment for silent setup
	//videoInput::setVerbose(false);
videoInput::setVerbose(true);

	//we allocate our openGL texture objects
	//we give them a ma size of 2048 by 2048 pixels
//	IT  = new imageTexture(2048,2048, GL_RGB);    	
//	IT2 = new imageTexture(2048,2048, GL_RGB);  

	//by default we use a callback method
	//this updates whenever a new frame
	//arrives if you are only ocassionally grabbing frames
	//you might want to set this to false as the callback caches the last
	//frame for performance reasons. 
//	VI.setUseCallback(true); // default: true

	//try and setup device with id 0 and id 1
	//if only one device is found the second 
	//setupDevice should return false

	//if you want to capture at a different frame rate (default is 30) 
	//specify it here, you are not guaranteed to get this fps though.
//VI.setIdealFramerate(dev, 60);
VI.setIdealFramerate(dev, 1);

	//we can specifiy the dimensions we want to capture at
	//if those sizes are not possible VI will look for the next nearest matching size	
//	VI.setupDevice(dev,   320, 240, VI_COMPOSITE); 
VI.setupDevice(dev, 3, 3); 
	//VI.setupDevice(dev+1, 640, 480, VI_COMPOSITE);	
IT=new imageTexture(VI.getWidth(dev),VI.getHeight(dev), GL_RGB);    	
	
	//once the device is setup you can try and
	//set the format - this is useful if your device
	//doesn't remember what format you set it to
	//VI.setFormat(dev, VI_NTSC_M);					//optional set the format

	//we allocate our buffer based on the number
	//of pixels in each frame - this will be width * height * 3
	frame = new unsigned char[VI.getSize(dev)];
VI.setVideoSettingFilter(dev, VI.propColorEnable, 0, VideoProcAmp_Flags_Manual);
}










///////////////////////////////////////////////////////////////////////
// Name: AllocateMonitorHandles
// Description: Get handles to the physical monitors.
///////////////////////////////////////////////////////////////////////

BOOL triangleApp::AllocateMonitorHandles()
{
    // Enumerate the display monitors. The callback will do the work.
    return EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
}


///////////////////////////////////////////////////////////////////////
// Name: MonitorEnumProc
// Description: Callback for EnumDisplayMonitors function.
//
// The callback data is a pointer to the MainDialog class.
///////////////////////////////////////////////////////////////////////

BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor DC
  LPRECT lprcMonitor, // monitor intersection rectangle
  LPARAM dwData       // data
)
{
    TRACE((L"MonitorEnumProc"));
printf("MonitorEnumProc(0x%X, 0x%X, ...)\n", hMonitor, hdcMonitor);

	triangleApp *pDialog = (triangleApp*)dwData;
    BOOL bContinue = FALSE;

    if (pDialog)
    {
        bContinue = pDialog->OnEnumMonitor(hMonitor);
    }
    return bContinue;
}


///////////////////////////////////////////////////////////////////////
// Name: OnEnumMonitor
// Description: Callback for EnumDisplayMonitors function.
//
// This function is where we get the physical monitor handles.
///////////////////////////////////////////////////////////////////////

BOOL triangleApp::OnEnumMonitor(HMONITOR hMonitor)
{
    TRACE((L"OnEnumMonitor: Handle = 0x%X", hMonitor));
printf("OnEnumMonitor: Handle = 0x%X\n", hMonitor);

    BOOL bSuccess = FALSE;
    DWORD cMonitors = 0;
    PHYSICAL_MONITOR *pPhysicalMonitors = NULL;

    // Free any monitor handles that we allocated previously.
    FreeMonitorHandles();

    // Get the number of physical monitors.
    bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(
        hMonitor, 
        &cMonitors
        );

    TRACE_IF_FAILED(L"GetNumberOfPhysicalMonitorsFromHMONITOR", bSuccess);
	printf("GetNumberOfPhysicalMonitorsFromHMONITOR: cMonitors=%i; bSuccess=%i\n", cMonitors, bSuccess);

    // Allocate the array of PHYSICAL_MONITOR structs.
    if (bSuccess)
    {
        pPhysicalMonitors = new PHYSICAL_MONITOR[cMonitors];

        if (pPhysicalMonitors == NULL)
        {
            bSuccess = FALSE;
        }
    }

    // Get the array of monior handles.
    if (bSuccess)
    {
        bSuccess = GetPhysicalMonitorsFromHMONITOR( //!
            hMonitor, cMonitors, pPhysicalMonitors);

        TRACE_IF_FAILED(L"GetPhysicalMonitorsFromHMONITOR", bSuccess);
    }
printf("GetPhysicalMonitorsFromHMONITOR: bSuccess=%i\n", bSuccess);

	if (bSuccess)
    {
        m_pPhysicalMonitors = pPhysicalMonitors;
        m_NumPhysicalMonitors = cMonitors;
    }

    // Add the display string to the combo box.
    // Also add the index of the array as user data. 
    if (bSuccess)
    {
        for (DWORD i = 0; i < cMonitors; i++)
        //for (DWORD i = cMonitors-1; i >= 0; i--)
        {
            TRACE((L"Physical monitor: %s (handle = 0x%X)", 
                pPhysicalMonitors[i].szPhysicalMonitorDescription,
                pPhysicalMonitors[i].hPhysicalMonitor
                ));
wprintf(L"Physical monitor %i: '%s' (handle = 0x%X)\n", i, pPhysicalMonitors[i].szPhysicalMonitorDescription, pPhysicalMonitors[i].hPhysicalMonitor);


			DWORD minVal = 0, maxVal = 0, current = 0;
			if(m_maxVal[m_noMonitor]==-1) {
				bSuccess = GetMonitorBrightness(pPhysicalMonitors[i].hPhysicalMonitor, &minVal, &current, &maxVal);
if(!bSuccess) {
	printf("Catalyst 11.2 override!!!!!!!!!!!!!!!!!!!!\n");
	if(m_noMonitor==0) maxVal=255; else maxVal=100;
	bSuccess=true;
}
				printf("Brightness: %d (%d - %d) -> %d\n", current, minVal, maxVal, bSuccess);
				m_maxVal[m_noMonitor]=maxVal;
			}
			if(m_maxVal[m_noMonitor]>0 && (m_targetMonitor==-1 || m_targetMonitor==m_noMonitor) ) {
				SetMonitorBrightness(pPhysicalMonitors[i].hPhysicalMonitor, m_brightness*m_maxVal[m_noMonitor]/100);
				printf("\nSetMonitorBrightness(%d)\n", m_brightness*m_maxVal[m_noMonitor]/100);
			}
//SetMonitorBrightness(pPhysicalMonitors[i].hPhysicalMonitor, m_brightness);
			m_noMonitor++;
			/*bSuccess = GetMonitorBrightness(pPhysicalMonitors[i].hPhysicalMonitor, &minVal, &current, &maxVal);
			printf("Brightness: %d (%d - %d) -> %d; ", current, minVal, maxVal, bSuccess);*/


/*            bSuccess = comboMonitors.AddString(
                pPhysicalMonitors[i].szPhysicalMonitorDescription,  // string 
                (DWORD_PTR)i    // user data = array index
                );*/

            if (!bSuccess)
            {
                break;
            }
        }
    }

/*    // Select the first one in the list.
    if (bSuccess)
    {
        comboMonitors.Select(0);
    }*/

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////
// Name: FreeMonitorHandles
//
// Description: Release the array of monitor handles.
///////////////////////////////////////////////////////////////////////

BOOL triangleApp::FreeMonitorHandles()
{
    BOOL bSuccess = TRUE;

    if (m_NumPhysicalMonitors > 0)
    {
//        assert(m_pPhysicalMonitors != NULL);

        bSuccess = DestroyPhysicalMonitors(m_NumPhysicalMonitors, m_pPhysicalMonitors);
    }

    SAFE_ARRAY_DELETE(m_pPhysicalMonitors);

    return bSuccess;
}











void triangleApp::setBrightness(int monitor, int brightness) {
	if(brightness>100) brightness=100;
	else if(brightness<0) brightness=0;
	if(brightness!=m_brightness) {
		m_brightness=brightness;
		m_noMonitor=0;
		m_targetMonitor=monitor;
		AllocateMonitorHandles();
	}
}



bool triangleApp::idle(){
	//check to see if we have got a new frame
	if( VI.isFrameNew(dev) )	
	{
		//we get the pixels by passing in out buffer which gets filled
double t0=glfwGetTime();
if(t0-tOld>.5) {
		VI.getPixels(dev,frame, true);

		int count=0;
		
		m_avg=0;
		count=0;
		for(int y=0; y<VI.getHeight(dev)/*/2*/; y++) {
			for(int x=0; x<VI.getWidth(dev); x++) {
				//for(int i=0;i<VI.getSize(dev);i++) {
				//for(int i=0;i<VI.getSize(dev)/2;i++) {
					m_avg+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+0]; count++;
					m_avg+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+1]; count++;
					m_avg+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+2]; count++;
					//m_avg+=frame[i];
			}
		}
		//m_avg/=VI.getSize(dev);
		m_avg/=count;
		printf("avg=%i; ", m_avg);

		/*m_avg2=0;
		count=0;
		for(int y=VI.getHeight(dev)/2; y<VI.getHeight(dev); y++) {
			for(int x=VI.getWidth(dev)/3;x<VI.getWidth(dev)*2/3;x++) {
				//for(int i=VI.getSize(dev)/2;i<VI.getSize(dev);i++) {
					m_avg2+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+0]; count++;
					m_avg2+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+1]; count++;
					m_avg2+=frame[(VI.getHeight(dev)-y-1)*VI.getWidth(dev)*3+x*3+2]; count++;
					//m_avg2+=frame[i];
			}
		}
		m_avg2/=count;
		printf("avg2=%i\n", m_avg2);
		if(m_avg2<90) m_avg=0;*/

//		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
		//BOOL bSuccess = TRUE;
		//SetMonitorBrightness(m_hMonitor, avg/2);
		/*m_brightness=100*avg/255+glfwGetMouseWheel();
		m_noMonitor=0;
		AllocateMonitorHandles();*/
		setBrightness(-1, 100*m_avg/255+m_shift);

		//we then load them into our texture
		IT->loadImageData(frame, VI.getWidth(dev), VI.getHeight(dev),GL_RGB);
tOld=t0;
		return true;
}
	}
	return false;
	
	//check to see if we have got a new frame
	/*if( VI.isFrameNew(dev+1) )						
	{	
		//here we are directly return the pixels into our texture
		//use VI.getWidth getHeight etc so that you don't get a crash
   		IT2->loadImageData(VI.getPixels(dev+1, true), VI.getWidth(dev+1), 	VI.getHeight(dev+1), GL_RGB); 
	}*/
}


void triangleApp::draw(){
  
	setupScreen();
	IT->renderTexture(0, 0, VI.getWidth(dev), VI.getHeight(dev));
	//IT2->renderTexture(VI.getWidth(dev), 0, VI.getWidth(dev+1), VI.getHeight(dev+1));
}

void triangleApp::keyDown  (char c){

	//some options hooked up to key commands
	if(c=='S')VI.showSettingsWindow(dev);
	//if(c=='D')VI.showSettingsWindow(dev+1);

	if(c=='R')VI.restartDevice(dev);
	//if(c=='T')VI.restartDevice(dev+1);

	if(c == '1')VI.setVideoSettingCameraPct(0, VI.propExposure, 0.1, 2);
	if(c == '2')VI.setVideoSettingCameraPct(0, VI.propExposure, 0.9, 2);	
	
	if(c == '5')VI.setVideoSettingFilterPct(0, VI.propWhiteBalance, 0.12, 2);
	if(c == '6')VI.setVideoSettingFilterPct(0, VI.propWhiteBalance, 0.88, 2);

	if(c=='Q') VI.stopDevice(dev);
}

void triangleApp::mousewheelRotated(int position) {
	int diff=position-m_oldPosition;
	m_shift+=diff;
	if(m_shift>100) m_shift=100;
	else if(m_shift<-100) m_shift=-100;
	printf("\nm_brightness=%d; m_shift=%d\n", m_brightness, m_shift);
	/*m_brightness+=diff;
	AllocateMonitorHandles();*/
	setBrightness(-1, 100*m_avg/255+m_shift);
	m_oldPosition=position;
}

void triangleApp::mouseMove( float x, float y ){
}

void triangleApp::mouseDrag( float x, float y ){
}

void triangleApp::mouseDown( float x, float y, int button ){
}

void triangleApp::mouseUp  ( float x, float y, int button ){
}
