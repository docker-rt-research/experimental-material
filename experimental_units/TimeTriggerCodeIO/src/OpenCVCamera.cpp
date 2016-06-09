/**
 * proxy - Sample application to encapsulate HW/SW interfacing with embedded systems.
 * Copyright (C) 2012 - 2015 Christian Berger
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "../include/OpenCVCamera.h"


OpenCVCamera::OpenCVCamera(const string &name, const uint32_t &id, const uint32_t &width, const uint32_t &height, const uint32_t &bpp) :
    Camera(name, id, width, height, bpp),
    m_capture(NULL),
    m_image(new cv::Mat) {

    m_capture.reset(new cv::VideoCapture(id));
    if (m_capture) {
        m_capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
        m_capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }
    else {
        cerr << "proxy: Could not open camera '" << name << "' with ID: " << id << endl;
    }
}

OpenCVCamera::~OpenCVCamera() {
    if (m_capture) {
        m_capture->release();
        m_capture = nullptr;
    }
}

bool OpenCVCamera::isValid() const {
    return (m_capture != NULL);
}

bool OpenCVCamera::captureFrame() {
    bool retVal = false;
    if (m_capture != nullptr) {
        if (m_capture->read(*m_image)) {
            retVal = true;
        }
    }
    return retVal;
}

bool OpenCVCamera::copyImageTo(char *dest, const uint32_t &size) {
    bool retVal = false;

    if ( (dest != NULL) && (size > 0) && (m_image != NULL) ) {
        ::memcpy(dest, &m_image->data, size);

        // cv::imshow("Window title", *m_image);
        // cv::waitKey(10);

        retVal = true;
    }

    return retVal;
}

