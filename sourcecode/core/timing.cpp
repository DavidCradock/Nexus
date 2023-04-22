#include "precompiled_header.h"
#include "timing.h"

namespace Nexus
{

    Timing::Timing()
    {
        reset();
    }


    Timing::~Timing()
    {
    }

    void Timing::update(void)
    {
        timePointNew = std::chrono::steady_clock::now();
        timeDeltaSec = timePointNew - timePointOld;
        timePointOld = std::chrono::steady_clock::now();

        deltaSec = timeDeltaSec.count();

        // Compute FPS
        ++statsNumFrames;
        statsFPSFrameTime += deltaSec * 1000.0;
        if (statsFPSFrameTime > 0.0)
        {
            statsFPS = statsNumFrames * (1000.0 / statsFPSFrameTime);
            
            statsNumFrames = 0;
            statsFPSFrameTime = 0.0;
        }

        // Compute FPS smoothed
        statsFPSSTimeCount += deltaSec;// *1000.0;
        ++statsFPSSNumCallsPerSec;
        statsFPSSAccum += statsFPS;
        if (statsFPSSTimeCount > statsFPSSrate)
        {
            if (statsFPSSNumCallsPerSec < 1)
                statsFPSSNumCallsPerSec = 1;
            statsFPSS = statsFPSSAccum / statsFPSSNumCallsPerSec;
            statsFPSSTimeCount = 0;
            statsFPSSNumCallsPerSec = 0;
            statsFPSSAccum = 0;
        }
    }

    void Timing::sleep(unsigned int uiMilliseconds)
    {
        Sleep(uiMilliseconds);
    }

    void Timing::reset(void)
    {
        timePointNew = std::chrono::steady_clock::now();
        timePointOld = std::chrono::steady_clock::now();
        timeDeltaSec = timePointNew - timePointOld;
        deltaSec = timeDeltaSec.count();

        // Stuff for FPS
        statsFPS = 1.0;                  // Holds computed current frames per second value
        statsNumFrames = 0;             // Used to compute FPS stuff        
        statsFPSFrameTime = 0;           // Used to compute FPS stuff
        
        // Stuff for FPS smoothed
        statsFPSS = 1.0;             // Holds computed smoothed frames per second value
        statsFPSSrate = 1.0f;        // Number of times per second in which is the delay until unpdating the value returned by getFPSS() method.
        statsFPSSTimeCount = 0;      // Used to compute FPSS stuff
        statsFPSSNumCallsPerSec = 0; // Used to compute FPSS stuff
        statsFPSSAccum = 0;          // Used to compute FPSS stuff


    }

}   // namespace DCL