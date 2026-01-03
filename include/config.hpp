#pragma once

/// CONFIG ///

// Window / Display settings
#define RESOLUTION 720 
#define WINNAME "Zenjin"
// #define TARGETFPS 60 // Limit the framerate to this target. If framerate is not limited, GPU usage will be very high
//#define VSYNC // Attempt to automatically set the target framerate to the current monitors refresh rate. May not work with multiple monitors, see below option.
#define MONITOR 0 // if you have multiple monitors, you might have to change this number to match the one you wish to use. Your main monitor SHOULD be 0.

#define DRAWFPS // Optionally draw FPS to the screen.

// Game world settings
#define MAPWIDTH 50
#define MAPHEIGHT 25

// #define DEBUG // enable player noclip, print current world and pixel coords to console
