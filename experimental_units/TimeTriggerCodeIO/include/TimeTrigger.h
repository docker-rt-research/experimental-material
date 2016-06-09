/**
 * ODVpi - Benchmark code extending OpenDaVinci 
 * (https://github.com/se-research/OpenDaVINCI)
 * Copyright (C) 2016 Philip Masek, Magnus Thulin
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


/*
 * Execution example:
 *     Occupy scenario (80%):    
 *     ./timetrigger --cid=111 --freq=10 --realtime=49 --verbose --occupy 80
 *     
 *     Pi limit scenario (1000 digits per timeslice):
 *     ./timetrigger --cid=111 --freq=10 --realtime=49 --verbose --pi 1000
 *     
 */

#include <memory>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include "opendavinci/odcore/data/Container.h"

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opendavinci/odtools/recorder/Recorder.h"
#include "Camera.h"


class TimeTrigger : public odcore::base::module::TimeTriggeredConferenceClientModule {
    public:
        enum {
            NANOSECOND  = 1,
            MICROSECOND = 1000 * NANOSECOND,    // 1000 nanoseconds are one microsecond.
            MILLISECOND = 1000 * MICROSECOND,   // 1000 microseconds are one millisecond.
            SECOND      = 1000 * MILLISECOND,   // 1000 milliseconds are one second.
            
            // first digit is runtime in seconds for exection
            RUNTIME     = 5 * MICROSECOND * MICROSECOND, 
            TIMESLICE   = 10,                   // ms/timeslice
            OCCUPY      = 80                    // percent of timeslice to execute
        };

    // Benchmark Variables
    public:
        float piDigits;
        float piDuration;
        int piTimes;
        int duration;
        int * dataStorage;
        odcore::data::TimeStamp bigTimer;

    // Flag Variables
    public:
        int piLimit;
        float occupy;
        bool measureByTime;
        enum VERBOSE_MODE {
            QUIET,
            MODE1,
            MODE2,
            MODE3
        };

        VERBOSE_MODE verbose;

    private:
        /**
         * "Forbidden" copy constructor. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the copy constructor.
         *
         * @param obj Reference to an object of this class.
         */
        TimeTrigger(const TimeTrigger &/*obj*/);

        /**
         * "Forbidden" assignment operator. Goal: The compiler should warn
         * already at compile time for unwanted bugs caused by any misuse
         * of the assignment operator.
         *
         * @param obj Reference to an object of this class.
         * @return Reference to this instance.
         */
        TimeTrigger& operator=(const TimeTrigger &/*obj*/);
        

        std::unique_ptr<odtools::recorder::Recorder> m_recorder;
        std::unique_ptr<Camera> m_camera;
        void distribute(odcore::data::Container c);


    public:
        /**
         * Constructor.
         *
         * @param argc Number of command line arguments.
         * @param argv Command line arguments.
         */
        TimeTrigger(const int32_t &argc, char **argv);

        virtual ~TimeTrigger();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();

        virtual void tearDown();

};


