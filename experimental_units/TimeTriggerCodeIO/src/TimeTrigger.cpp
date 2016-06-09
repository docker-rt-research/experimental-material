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

/**
 * OpenDaVINCI - Tutorial.
 * Copyright (C) 2015 Christian Berger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>
#include <iomanip>


#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"


#include "../include/OpenCVCamera.h"

#include "../include/TimeTrigger.h"


using namespace std;
using namespace odtools::recorder;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore::base::module;

TimeTrigger::TimeTrigger(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TimeTrigger"),
    m_recorder(),
    m_camera()
    {}

TimeTrigger::~TimeTrigger() {}

void TimeTrigger::setUp() {


    // SETUP RECORDER
    stringstream recordingURL;
    recordingURL << "file://data/" << "proxy" << ".rec";
    // Size of memory segments.
    const uint32_t MEMORY_SEGMENT_SIZE = 2800000;
    // Number of memory segments.
    const uint32_t NUMBER_OF_SEGMENTS = 20;
    // Run recorder in asynchronous mode to allow real-time recording in background.
    const bool THREADING = false;
    // Dump shared images and shared data?
    const bool DUMP_SHARED_DATA = true;

    m_recorder = unique_ptr<Recorder>(new Recorder(recordingURL.str(), MEMORY_SEGMENT_SIZE, NUMBER_OF_SEGMENTS, THREADING, DUMP_SHARED_DATA));
    

    // Create the camera grabber.
    const string NAME = "usbcam";
    string TYPE = "webcam";
    std::transform(TYPE.begin(), TYPE.end(), TYPE.begin(), ::tolower);
    const uint32_t ID = 0;
    const uint32_t WIDTH = 1280;
    const uint32_t HEIGHT = 720;
    const uint32_t BPP = 3;

    m_camera = unique_ptr<Camera>(new OpenCVCamera(NAME, ID, WIDTH, HEIGHT, BPP));

    if (m_camera.get() == NULL) {
        cerr << "No valid camera type defined." << endl;
    }



    // Reset benchmark variables
    // within the RT object.

    piTimes   = 0;
    duration *= getFrequency();
    occupy = ((1000/getFrequency())*(occupy/100))*1000*1000;

    bigTimer = odcore::data::TimeStamp();

    odcore::data::TimeStamp::setupSerial("/dev/ttyS0", 115200);

    // Print out info before starting
    // execution of timeslices.
    if (verbose!=MODE3&&verbose!=QUIET) {
        cout << endl;
        cout << "Running at:                            "  << getFrequency()    << "hz" << endl;
        cout << "Occupation \% per slice:                " << occupy << "%" << endl << endl;
    }
}

void TimeTrigger::tearDown() {
    odcore::data::TimeStamp::writeMessageToSerial("end");
    // Print out results from run
    const char* measured = (measureByTime ? "Occupied " : "Limited pi decimals per slice to ");
    cout << endl << endl;;
    cout << "Measured by:    " << measured << (!measureByTime ? piLimit : occupy) << (!measureByTime ? " digits" : "\% of each timeslice with calculations") << endl;
    cout << "Ran for:                               " << odcore::data::TimeStamp().getSeconds()-bigTimer.getSeconds()  << " second(s)"           << endl;
    cout << "Total pi timeslice(s):                 " << piTimes                            << " calculation(s)"      << endl;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TimeTrigger::body() {
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        odcore::data::TimeStamp::writeNanoToSerial("2");
        odcore::data::TimeStamp start, end;

        if (m_camera.get() != NULL) {
            odcore::data::image::SharedImage si = m_camera->capture();
            odcore::data::TimeStamp::writeNanoToSerial("3");
            odcore::data::Container c(si);
            distribute(c);
            odcore::data::TimeStamp::writeNanoToSerial("4");
        }



        // Pi algorithm variable are
        // reset after each timeslice.
        long double tempPi;
        long double pi     = 4.0;
        int i              = 1;
        int j              = 3;
        float oDuration    = 0;

        while (true) {

            // Calculate pi
            tempPi = static_cast<double>(4)/j;
            if (i%2 != 0) {
                pi -= tempPi;
            } else if (i%2 == 0) {
                pi += tempPi;
            }
            i++;
            j+=2;


            // Occupy for a certain duration
            end = odcore::data::TimeStamp();
            oDuration = end.toNanoseconds()-start.toNanoseconds();
            if (oDuration >= occupy)
                break;
        }
        // Add to the timeslice counter
        piTimes++;

        odcore::data::TimeStamp::writeNanoToSerial("5");
        if (piTimes==duration)
            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
    }
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}


void TimeTrigger::distribute(odcore::data::Container c) {
    // Store data to recorder.
    if (m_recorder.get() != NULL) {
        // Time stamp data before storing.
        c.setReceivedTimeStamp(odcore::data::TimeStamp());
        m_recorder->store(c);
    }

    // Share data.
    getConference().send(c);
}




int32_t main(int32_t argc, char **argv) {
    TimeTrigger tte(argc, argv);

    // Setup the default value
    // for the flag variables.
    tte.occupy          = 80;
    tte.duration        = 10;
    tte.verbose         = TimeTrigger::QUIET;
    tte.measureByTime   = true;
    tte.piLimit         = 1000;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-p" || string(argv[args])=="--pi") {
            tte.measureByTime = false;
            istringstream buffer(string(argv[args+1]));
            buffer >> tte.piLimit;
        } else if (string(argv[args])=="-v" || string(argv[args])=="--verbose") {
            int tmpV;
            istringstream buffer(string(argv[args+1]));
            buffer >> tmpV;
            switch(tmpV) {
                case 1 : tte.verbose = TimeTrigger::MODE1; break;
                case 2 : tte.verbose = TimeTrigger::MODE2; break;
                case 3 : tte.verbose = TimeTrigger::MODE3; break;
                default:  tte.verbose = TimeTrigger::MODE1; break;
            }
        } else if (string(argv[args])=="-o" || string(argv[args])=="--occupy") {
            istringstream buffer(string(argv[args+1]));
            buffer >> tte.occupy;
        } else if (string(argv[args])=="-d" || string(argv[args])=="--duration") {
            istringstream buffer(string(argv[args+1]));
            buffer >> tte.duration;
        }
    }



    return tte.runModule();
}

