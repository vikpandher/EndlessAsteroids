/* ==========================================================================
   >File: Win32Main.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Main file for a game of 3D asteroids.
   ========================================================================== */

#pragma once
#include <windows.h>
#include "ErrorCodes.h"



///#include "MemoryManager.h"
// Global variable being used to track/allocate/free memory
///MemoryManager g_memoryManager;

// Overloading the default new and delete to track heap allocation
// ...
// Never mind I'm not.
// My memory manager is slow...
// ========================================================================== //
///void * operator new(size_t size)
///{
///    return g_memoryManager.allocate(size, INFO_USED);
///}
///void operator delete(void * p)
///{
///    g_memoryManager.free(p);
///}
// -------------------------------------------------------------------------- //



#include "Game.h"
// ^ includes "ScreenBuffer.h"



// Timer IDs
#define IDT_TIMER1 1


// function delarations
// -------------------------------------------------------------------------- //
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SizeWindowToAspectRatio(int edge, RECT *dragRect, float aspectRatio);
const char * GetErrorString(int value);


/* --------------------------------------------------------------------------
   NOTE:
   static global variables are translation unit specific.
   -------------------------------------------------------------------------- */

// Used to to indicate if the application is running
static bool s_running;

// The ScreenBuffer, holds bitmap info and memory
ScreenBuffer * g_screenBuffer;

// Fixing the dimensions helps with debugging alot because its easyer to 
// see individual pixels when you can stretch them out and make them big.
static const bool FIXED_DIMENSIONS = true;

// Fixed buffer dimensions
static const int BUFFER_WIDTH = 240; // 320; // 480;
static const int BUFFER_HEIGHT = 160; // 240; // 360;

// Window aspect ratio, width / height, determined after using AdjustWindowRectEx
// to find the window size for the desired client size
static float s_windowAspectRatio;



/* --------------------------------------------------------------------------
   NOTES:
   In the function headers, _In_ marks parameters that are passed into the
   function and_Out_ are parameters that are sent out from the fuction.
   
   Also for whatever reason windows functions are capitalized. I typically
   capitalize classes and leave functions lower case.
   -------------------------------------------------------------------------- */

// WinMain
// ========================================================================== //
// The user-provided entry point for a graphical Windows-based application.
// 
// @params
// * _In_ HINSTANCE hInstance, handle to the current instance of app
// * _In_ HINSTANCE hPrevInstance, handle to the previous instance of this 
//   application. It is always NULL, more details on msdn.
// * _In_ LPSTR lpCmdLine, command line for the application, excludes the
//   program name.
// * _In_ int nCmdShow, controls how the window is to be shown. Options are
// selected via (RClick on Icon->Properties->Shortcut->Run dropdown select)
//
// @return
// If the function succeeds, terminating when it receives a WM_QUIT message, it
// should return the exit value contained in that message's wParam parameter.
// If the function terminates before entering the message loop, it should return
// zero.
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{    
    // The window struct contains the attributes registered by the RegisterClass function.
    WNDCLASS wndClass = {};
    // Indicate that we need to redraw the whole window when rezised, horizontaly and verticaly
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = TEXT("3DMathStuff");

    // Register a window class for subsequent use in calls to the CreateWindow
    if (!RegisterClass(&wndClass))
    {
        // If this fails, GG
        return 0; // Maybe I'll handle errors one day
    }

    int initialWindowWidth, initialWindowHeight;

    // Not sure if I want fixed window/buffer dimensions
    if (FIXED_DIMENSIONS)
    {
        RECT adjustedClientRect = {};
        adjustedClientRect.right = BUFFER_WIDTH;
        adjustedClientRect.bottom = BUFFER_HEIGHT;
        
        // Calculates the required size of the window rectangle, based on the desired size of the client rectangle.
        AdjustWindowRectEx(
            &adjustedClientRect, // _Inout_ LPRECT lpRect, this gets modified
            WS_OVERLAPPEDWINDOW, // _In_    DWORD  dwStyle,
            0,                   // _In_    BOOL   bMenu,
            0);                  // _In_    DWORD  dwExStyle
        
        s_windowAspectRatio = (float) adjustedClientRect.right / adjustedClientRect.bottom;
        initialWindowWidth = adjustedClientRect.right * 2;
        initialWindowHeight = adjustedClientRect.bottom * 2;
    }
    else
    {
        initialWindowWidth = CW_USEDEFAULT;
        initialWindowHeight = CW_USEDEFAULT;
    }

    // Awww yeah, make dat window
    HWND hwnd = CreateWindowEx(
        0,                             // dwExStyle
        wndClass.lpszClassName,        // window class name
        TEXT(""),                      // window caption
        WS_OVERLAPPEDWINDOW,           // window style
        CW_USEDEFAULT,                 // initial x position
        CW_USEDEFAULT,                 // initial y position
        initialWindowWidth,            // initial x size
        initialWindowHeight,           // initial y size
        0,                             // parent window handle
        0,                             // window menu handle
        hInstance,                     // program instance handle
        0);                            // creation parameters

    if (!hwnd)
    {
        // GetLastError
        return 0; // Maybe I'll handle errors one day
    }
     
    // Set window style to WS_VISIBLE
    ShowWindow(hwnd, nCmdShow);

    // ---------------------------------------------------------------------- //

    if (!FIXED_DIMENSIONS)
    {
        // Retrieve the coodinates of the window's client area
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;
        // Initialize the backbuffer
        g_screenBuffer = new ScreenBuffer(clientWidth, clientHeight);
    }
    else
    {
        g_screenBuffer = new ScreenBuffer(BUFFER_WIDTH, BUFFER_HEIGHT);
    }

    // Will Contain message information from a thread's message queue.
    MSG message;

    // Flag for the main animation loop
    s_running = true;

    // ---------------------------------------------------------------------- //

    try
    {
        initialize();
    }
    catch (int x)
    {
        OutputDebugString("initialize() exception: ");
        OutputDebugString(GetErrorString(x));
        OutputDebugString("\n");
    }

    // Consistant Timing 
    LARGE_INTEGER startTime, currentTime, elapsedMicroseconds;

    LARGE_INTEGER frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startTime);

    // ---------------------------------------------------------------------- //
    // MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP  //
    // ---------------------------------------------------------------------- //

    while (s_running)
    {
        QueryPerformanceCounter(&currentTime);
        elapsedMicroseconds.QuadPart = currentTime.QuadPart - startTime.QuadPart;

        elapsedMicroseconds.QuadPart *= 1000000;
        elapsedMicroseconds.QuadPart /= frequency.QuadPart;
        // ups == updates per second
        if (elapsedMicroseconds.QuadPart >= 50000) // currently 20ups // 16667 == 60ups
        {
            try
            {
                update();
                startTime = currentTime;
            }
            catch (int x)
            {
                OutputDebugString("update() exception: ");
                OutputDebugString(GetErrorString(x));
                OutputDebugString("\n");
            }
        }
        
        
        // Go through the thread message queue.        
        // Unlike GetMessage, the PeekMessage function does not wait for a
        // message to be posted before returning. So it won't block
        // Also be sure the remove the message with the PM_REMOVE flag
        while (PeekMessage(&message, // lpMsg
            0, // hWnd
            0, // wMsgFilterMin
            0, // wMsgFilterMax
            PM_REMOVE)) // wRemoveMsg
        {
            // Translate virtual-key messages into character messages
            TranslateMessage(&message);

            // Dispatche a message to the window procedure, WindowProc
            DispatchMessage(&message);
        }

        try
        {
            draw();
            ///update();
        }
        catch (int x)
        {
            OutputDebugString("draw() exception: ");
            OutputDebugString(GetErrorString(x));
            OutputDebugString("\n");
        }
        
        // get the handle to a display device context, need it to draw
        HDC deviceContext = GetDC(hwnd);
        
        // Retrieve the coordinates of a window's client area, the area excluding
        // the title bar, toolbars, status bar, scroll bars.
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        
        g_screenBuffer->paintWindow(deviceContext, clientRect);
        
        // NEEED to remember to release the device context (else memory gets packed)
        ReleaseDC(hwnd, deviceContext);
    }

    // return the exit code
    return message.wParam;
} // WinMain


// WindowProc
// ========================================================================== //
// An application-defined function that processes messages sent to a window.
// The WNDPROC type defines a pointer to this callback function.
// 
// @params
// * HWND hwnd, handle to the window
// * UINT uMsg, the system defined message
// * WPARAM wParam, additional message info that depends on uMsg
// * LPARAM lParam, additional message info that depends on uMsg
// 
// @return
// The result of the message processing, depends on the message sent
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Usually if an application processes the message, it should return zero.
    LRESULT result = 0;

    switch (uMsg)
    {

    // Maybe pause if the user clicks out of the window?
    /// // Sent when a window belonging to a different application than the active
    /// // window is about to be activated. The message is sent to the application
    /// // whose window is being activated and to the application whose window is
    /// // being deactivated.
    /// case WM_ACTIVATEAPP:
    /// {
    ///     OutputDebugString("WM_ACTIVATEAPP\n");
    ///     break;
    /// }

    ///// Posted to the window with the keyboard focus when a WM_KEYDOWN message is
    ///// translated by the TranslateMessage function. The WM_CHAR message contains
    ///// the character code of the key that was pressed.
    ///case WM_CHAR:
    ///{
    ///    OutputDebugString("WM_CHAR: ");
    ///    char stringArray[2];
    ///    stringArray[0] = wParam;
    ///    stringArray[1] = '\0';
    ///    OutputDebugString(stringArray);
    ///    OutputDebugString("\n");
    ///    break;
    ///}

    // Sent as a signal that a window or an application should terminate.
    case WM_CLOSE:
    {
        ///OutputDebugString("WM_CLOSE\n");
        s_running = false;
        break;
    }

    // Sent when a window is being destroyed after the window is removed from
    // the screen. This message is sent to the parrent window before the child.
    case WM_DESTROY:
    {
        ///OutputDebugString("WM_DESTROY\n");
        s_running = false;
        KillTimer(hwnd, IDT_TIMER1);
        break;
    }

    // Posted to the window with the keyboard focus when a nonsystem key is 
    // pressed. A nonsystem key is a key that is pressed when the ALT key is
    // not pressed.
    case WM_KEYDOWN:
    {
        /// // Check if the key was up before. (key down message repeats while down)
        /// if ((lParam & (1 << 30)) == 0)
        keyDown(wParam);
        break;
    }

    // Posted to the window with the keyboard focus when a nonsystem key is
    // released. A nonsystem key is a key that is pressed when the ALT key is
    // not pressed, or a keyboard key that is pressed when a window has the
    // keyboard focus.
    case WM_KEYUP:
    {
        keyUp(wParam);
        break;
    }

    /// // Posted to a window when the cursor moves. wParam is a mask indicating
    /// // which keys are being held down. lParam contains two shorts with the x and
    /// // y coordinates. x is the low-order bits.
    /// case WM_MOUSEMOVE:
    /// {
    ///     if (wParam & MK_LBUTTON)
    ///     {
    ///         // left button pressed
    ///         short xPos = lParam >> sizeof(short);
    ///         short yPos = lParam;
    ///     }
    /// }

    // Sent when the system or another application makes a request to paint a 
    // portion of an application's window. The message is also sent when the
    // UpdateWindow or RedrawWindow function is called, or by the
    // DispatchMessage function when the application obtains a WM_PAINT message
    // by using the GetMessage or PeekMessage function.
    case WM_PAINT:
    {
        ///OutputDebugString("WM_PAINT\n");

        // Holds information for an application used to paint
        PAINTSTRUCT paintStruct;
         
        // Prep the specified window for painting (fills paintStruct) and get
        // the handle to a display device context for the specified window.
        HDC deviceContext = BeginPaint(hwnd, &paintStruct);
        
        // Retrieve the coordinates of a window's client area, the area excluding
        // the title bar, toolbars, status bar, scroll bars.
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        
        g_screenBuffer->paintWindow(deviceContext, clientRect);
        
        // End of painting in the specified window
        EndPaint(hwnd, &paintStruct);
        break;
    }

    // Sent to a window after its size has changed.
    case WM_SIZE:
    {
        ///OutputDebugString("WM_SIZE\n");
        
        if (!FIXED_DIMENSIONS)
        {
            // Retrieve the coodinates of the window's client area
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;
            // Resize the backbuffer
            if(g_screenBuffer) g_screenBuffer->resizeDIBSection(clientWidth, clientHeight);
        }
        break;
    }

    // Sent to a window that the user is resizing.
    // wParam gives the edge of the window that is being sized. 
    // lParam points to the drag rectangle.
    case WM_SIZING:
    {
        ///OutputDebugString("WM_SIZING\n");

        if (FIXED_DIMENSIONS)
        {
            RECT *dragRect = (RECT *)lParam;
            SizeWindowToAspectRatio(wParam, dragRect, s_windowAspectRatio);
        }
    }

    default:
    {
        // handle these messages with the default behaviour
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        break;
    }
    }

    return result;
} // WindowProc


// SizeWindowToAspectRatio
// ========================================================================== //
// This function takes a RECT structure and resizes it so its width / height
// is equal to some given aspectRatio. The resizing depends on which edge of
// the rectangle had been resized.
// This function is used durring a WM_SIZING.
//
// @params
// * int edge, the edge that had been resized durring the WM_SIZING event and
// potentially caused the rectangle to nolonger conform to the aspectRatio.
// * RECT *dragRect, the rectangle holding the dimentions to be resized
// * float aspectRatio, the aspect ratio (width / height)
void SizeWindowToAspectRatio(int edge, RECT *dragRect, float aspectRatio)
{
    int dragWidth = dragRect->right - dragRect->left;
    int dragHeight = dragRect->bottom - dragRect->top;

    switch (edge)
    {
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        // Resize the top and bottom of the window to maintain the ratio
        int newDragHeight = dragWidth / aspectRatio;
        int deltaHeight = dragHeight - newDragHeight;
        dragRect->top += deltaHeight / 2;
        dragRect->bottom -= deltaHeight / 2;
        break;
    }
    case WMSZ_TOP:
    case WMSZ_BOTTOM:
    {
        // Resize the left and right of the window to maintain the ratio
        int newDragWidth = dragHeight * aspectRatio;
        int deltaWidth = dragWidth - newDragWidth;
        dragRect->left += deltaWidth / 2;
        dragRect->right -= deltaWidth / 2;
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int newDragWidth = dragHeight * aspectRatio;
        int newDragHeight = dragWidth / aspectRatio;
        int deltaHeight = dragHeight - newDragHeight;
        int deltaWidth = dragWidth - newDragWidth;
        dragRect->top += deltaHeight / 2;
        dragRect->left += deltaWidth / 2;
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int newDragWidth = dragHeight * aspectRatio;
        int newDragHeight = dragWidth / aspectRatio;
        int deltaHeight = dragHeight - newDragHeight;
        int deltaWidth = dragWidth - newDragWidth;
        dragRect->top += deltaHeight / 2;
        dragRect->right -= deltaWidth / 2;
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int newDragWidth = dragHeight * aspectRatio;
        int newDragHeight = dragWidth / aspectRatio;
        int deltaHeight = dragHeight - newDragHeight;
        int deltaWidth = dragWidth - newDragWidth;
        dragRect->bottom -= deltaHeight / 2;
        dragRect->left += deltaWidth / 2;
        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        int newDragWidth = dragHeight * aspectRatio;
        int newDragHeight = dragWidth / aspectRatio;
        int deltaHeight = dragHeight - newDragHeight;
        int deltaWidth = dragWidth - newDragWidth;
        dragRect->bottom -= deltaHeight / 2;
        dragRect->right -= deltaWidth / 2;
        break;
    }
    }
} // SizeWindowToAspectRatio


// GetErrorString
// ========================================================================== //
// Debugging...
const char * GetErrorString(int value)
{
    switch (value)
    {
    case ERROR_OUTSIDE_ARRAY_BOUNDS:
    {
        return "ERROR_OUTSIDE_ARRAY_BOUNDS";
    }
    case ERROR_MEMORY_UNAVAILABLE:
    {
        return "ERROR_MEMORY_UNAVAILABLE";
    }
    case ERROR_OUTSIDE_BUFFER_BOUNDS:
    {
        return "ERROR_OUTSIDE_BUFFER_BOUNDS";
    }
    case ERROR_NEGATIVE_INPUT:
    {
        return "ERROR_NEGATIVE_INPUT";
    }
    case ERROR_INPUT_OUT_OF_BOUNDS:
    {
        return "ERROR_INPUT_OUT_OF_BOUNDS";
    }
    default:
    {
        return  "ERROR_???";
    }
    }
}


// Nolonger using SetTimer. It's not accurate.
/// /*
///  * NOTE !!!
///  * With the EXCEPTION of the WM_PAINT message, the WM_TIMER message, and
///  * the WM_QUIT message, the system always posts messages at the end of a
///  * message queue. This ensures that a window receives its input messages
///  * in the proper first in, first out (FIFO) sequence. The WM_PAINT message,
///  * the WM_TIMER message, and the WM_QUIT message, however, are kept in the
///  * queue and are forwarded to the window procedure only when the queue
///  * contains no other messages.
///  *
///  */
/// 
/// // Set the timer
/// UINT timerSetupSuccessfull = SetTimer(
///     hwnd,             // handle to main window 
///     IDT_TIMER1,            // timer identifier 
///     MILLISECONDS_PER_FRAME, // The time-out value, in milliseconds.
///     (TIMERPROC)TimerProc); // timer callback
/// 
/// if (!timerSetupSuccessfull)
/// {
///     return 0;
/// }
/// 
/// 
/// // TimerProc
/// // ========================================================================== //
/// // Function called when IDT_TIMER1 ticks down. When WM_TIMER message is
/// // processed.
/// // 
/// // @params
/// // * HWND hwnd, handle to the window
/// // * UINT uMsg, WM_TIMER message 
/// // * UINT idTimer, timer identifier
/// // * DWORD dwTime, current system time
/// VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idTimer, DWORD dwTime)
/// {
///     try
///     {
///         update();
///     }
///     catch (int x)
///     {
///         OutputDebugString("update() exception: ");
///         OutputDebugString(GetErrorString(x));
///         OutputDebugString("\n");
///     }
/// } // TimerProc