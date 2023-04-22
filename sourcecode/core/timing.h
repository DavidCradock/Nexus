#pragma once
#include "../precompiled_header.h"

namespace Nexus
{
    // Time stuff
    class Timing
    {
    public:

        // Constructor
        Timing();

        // Destructor
        ~Timing();

        // Call this to compute time difference between last call to this method.
        void update(void);

        // Returns time passed in seconds since last call to update() method.
        double getSecPast(void) { return deltaSec; }

        // Suspends execution of the calling thread for (at least) usec microseconds. The sleep may be lengthened slightly by any system activity or by the time spent processing the call or by the granularity of system timers. 
        void sleep(unsigned int uiMilliseconds);

        // Sets the rate at which the getFPSS() method's return value will be updated per second.
        // fSecondsPerUpdate - How much time in seconds before the FPSS value is updated.
        // For example, a value of 2.0 = update once every 2 seconds.
        inline void setStatFPSSrate(float fSecondsPerUpdate) { statsFPSSrate = fSecondsPerUpdate; if (statsFPSSrate <= 0) statsFPSSrate = 0.1f; }

        // Returns the currently set rate at which the getFPSS() method's return value will be updated per second.
        inline float getStatFPSSrate(void) { return statsFPSSrate; }

        // Returns computed Frames Per Second current value.
        inline double getStatFPS(void) { return statsFPS; }

        // Returns computed Frames Per Second smoothed value.
        inline double getStatFPSS(void) { return statsFPSS; }


    private:
        std::chrono::duration<double> timeDeltaSec;
        std::chrono::time_point<std::chrono::steady_clock> timePointOld, timePointNew;

        double deltaSec;        // Holds time delta since last call to update() method in seconds.

        // Resets all values to sane values.
        void reset(void);

        // Stuff for FPS
        double statsFPS;                 // Holds computed current frames per second value
        double statsFPSFrameTime;        // Used to compute FPS stuff
        unsigned int statsNumFrames;    // Used to compute FPS stuff        

        // Stuff for FPS smoothed
        double statsFPSS;                // Holds computed smoothed frames per second value
        float statsFPSSrate;             // Number of times per second in which is the delay until unpdating the value returned by getFPSS() method.
        double statsFPSSTimeCount;       // Used to compute FPSS stuff
        int statsFPSSNumCallsPerSec;     // Used to compute FPSS stuff
        double statsFPSSAccum;           // Used to compute FPSS stuff


    };
}