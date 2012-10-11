// Copyright 2012 Alexandre Gravier (al.gravier@gmail.com)
// This file is part of naoclient.

// naoclient is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// naoclient is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with naoclient. If not, see <http://www.gnu.org/licenses/>.

#include "fgrab.h"
#include "naoclient.h"
#include <alvision/alvisiondefinitions.h> // Aldebaran constants
#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <exception>
#include <iostream>
#include <qi/log.hpp>

/*********************
 *     NaoClient     *
 *********************/


NaoClientError::NaoClientError(const char* message) : msg(message) {}


NaoClientError::NaoClientError(const std::string message) : msg(message.c_str()) {}


const char* NaoClientError::what() {
  return msg;
}


typedef struct NaoClientConf {
  int framerate, resolution, camera;
  struct sockaddr_in sock_addr;
} NaoClientConf;


struct NaoClient::Impl {
  std::string nameId;
  boost::shared_ptr<FGrab> fgrabber;
  NaoClient &module;
  NaoClientConf conf;

  Impl(NaoClient &mod) : nameId("NaoClient_M"), module(mod) {
    try {
      qiLogVerbose("NaoClient") << "Constructing NaoClient implementation" << std::endl;
      // Make a local frame grabber module
      fgrabber = boost::shared_ptr<FGrab>
          (new FGrab(mod.getParentBroker(), "FGrab_for_naoclient"));
      fgrabber->init();
      // Make a H264 encoder thread
      // encoder = boost::shared_ptr<X264Encoder>(new X264Encoder());
      // 
      // Make a RTP server
      
    } catch (FGrabError& e) {
      qiLogError("NaoClient") << "Error while creating an FGrab module instance: "
                          << e.what() << std::endl;
      throw NaoClientError("Could not get a new FGrab module.");
    }
    if (!fgrabber) {
      qiLogError("NaoClient") << "Failed to get a proxy of FGrab" << std::endl;
      throw NaoClientError("Could not get a proxy to the FGrab module");
    }
  }

  ~Impl() {
    try {
      if(fgrabber) {
        fgrabber->release();
      }
      fgrabber.reset(); // explicit shared_ptr ref count decr
    } catch(std::exception& e) {
      qiLogError("NaoClient: Error during Impl desctruction: ") <<  e.what() << std::endl;
    }
  }
};


NaoClient::NaoClient(boost::shared_ptr<AL::ALBroker> broker,
                     const std::string &name)
    : AL::ALModule(broker, name) {
  setModuleDescription("Set a stream of sensor data over the network as a RTP stream over "
                       "UDP. Those can include video feeds.");

  functionName("configure", getName(),
               "Configures the parameters of the stream. The configure method can be "
               "called anytime. the dest_ip field should be set before starting "
               "streaming. The following fields are available for setup:\n\ndest_ip: "
               "The destination IP of the stream, decimal dot-separated.\n\ndest_port: "
               "The destination port, between 1024 and 65535. Defaults to "
               "2224.\n\nrate: The sampling rate (framerate for video feeds), defaults "
               "to 10 fps.\n\nvid_res: The video mode, integer as encoded by Aldebaran "
               "(0 = kQQVGA, 1 = kQVGA, 2 = kVGA, 3 = k4VGA, etc)\n\ncamera: Camera "
               "select, as an integer defined by Aldebaran (kTop=0, kBottom=1)");
  addParam("field", "A string indicating what is to be configured.");
  addParam("value", "A string with the configuration data.");
  BIND_METHOD(NaoClient::configure);

  functionName("startStream", getName(), "Start streaming sensory data. The configure "
               "method must have been called.");
  BIND_METHOD(NaoClient::startStream);

  functionName("stopStream", getName(), "Stop streaming the data. The configuration "
               "is retained.");
  BIND_METHOD(NaoClient::stopStream);
}


NaoClient::~NaoClient() {}


void NaoClient::init() { // auto called after construction
  try {
    impl = boost::shared_ptr<Impl>(new Impl(*this));
  } catch (std::exception& e) {
    qiLogError("NaoClient") << "Failed to initialize NaoClient module: " << e.what() << std::endl;
    exit();
  }
  qiLogVerbose("NaoClient") << "Successful initialization." << std::endl;
}


void NaoClient::exit() {}


// The configure method can be called anytime, but must have been
// called before startStream.
void NaoClient::configure(const std::string &field, const std::string &value) {
  char *end;
  long l;
  {
    if (field == DEST_IP) { // IPv4 for now
      struct sockaddr_in sa;
      if (inet_pton(AF_INET, value.c_str(), &(impl->conf.sock_addr)) != 1)
        throw NaoClientError("Configuration failed: not a valid IPv4 address :"
                             + field + ".");
    } else if (field == DEST_PORT) {
      l = strtol(value.c_str(), &end, 10);
      if ((errno == ERANGE && l == LONG_MAX) || l > 65535 || l < 1024 || value == ""
          || *end != '\0') {
        throw NaoClientError("Configuration failed: invalid port number: " + field + 
                             " (it should be between 1024 and 65535).");
      }
      impl->conf.sock_addr.sin_port = static_cast<short>(l);
    } else if (field == RATE) {
      l = strtol(value.c_str(), &end, 10);
      if ((errno == ERANGE && l == LONG_MAX) || l < 1 || l > 100 || value == ""
          || *end != '\0') {
        throw NaoClientError("Configuration failed: invalid frame rate: " + field + 
                             " (it should be a reasonable positive integer).");
      }
      impl->conf.framerate = l;
    } else if (field == VID_RES) {
      l = strtol(value.c_str(), &end, 10);
      if ((errno == ERANGE && l == LONG_MAX) || l < INT_MIN || l > INT_MAX || value == ""
          || *end != '\0') {
        throw NaoClientError("Configuration failed: invalid resolution code: " + field + 
                             " (it should be an integer).");
      }
      try {
        impl->fgrabber->switchCamera(l);
      } catch (std::exception e) {
        throw NaoClientError("Configuration failed: invalid resolution code: " + field + 
                             " (it should be one of Aldebaran's existing reolution codes).");
      }
      impl->conf.resolution = l;
    } else if (field == CAMERA) {
      l = strtol(value.c_str(), &end, 10);
      if ((errno == ERANGE && l == LONG_MAX) || l < INT_MIN || l > INT_MAX ||  value == ""
          || *end != '\0') {
        throw NaoClientError("Configuration failed: invalid camera number: " + field + 
                             " (it should be an integer defining a camera as "
                             "in Aldebaran's documentation).");
      }
      try {
        impl->fgrabber->switchCamera(l);
      } catch (std::exception e) {
        throw NaoClientError("Configuration failed could not switch to camera " + field + 
                             " (it should be one of Aldebaran's existing reolution codes).");
      }
      impl->conf.camera = l;
    } else {
      throw NaoClientError("Configuration failed: unknown field name \"" + field + "\".");
    }
  }
}

void NaoClient::startStream() {
  qiLogVerbose("NaoClient") << "Streaming started." << std::endl;
  impl->fgrabber->grab_to_file("/home/nao/startstream.yuv");
}


void NaoClient::stopStream() {
  qiLogVerbose("NaoClient") << "Streaming stopped." << std::endl;
}


