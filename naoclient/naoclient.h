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

#ifndef NAOCLIENT_H
  #define NAOCLIENT_H

#include <alcommon/almodule.h>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <iostream>
#include <netinet/in.h>


namespace AL {
  // This is a forward declaration of AL:ALBroker which
  // avoids including <alcommon/albroker.h> in this header
  class ALBroker;
}


static const std::string DEST_IP = "dest_ip",
  DEST_PORT = "dest_port",
  RATE = "rate",
  VID_RES = "vid_res",
  CAMERA = "camera";


class NaoClient : public AL::ALModule {
public:
  NaoClient(boost::shared_ptr<AL::ALBroker> broker,
            const std::string &name);

  ~NaoClient();

  virtual void init(), exit();

  /** The configure method can be called anytime. the dest_ip field
   * should be set before starting streaming. The following fields are
   * available for setup:
   * 
   * dest_ip: The destination IP of the stream, decimal dot-separated.
   * 
   * dest_port: The destination port, between 1024 and 65535. Defaults
   * to 2224.
   * 
   * rate: The sampling rate (framerate for video feeds), defaults to
   * 10 fps.
   * 
   * vid_res: The video mode, integer as encoded by Aldebaran
   * (0 = kQQVGA, 1 = kQVGA, 2 = kVGA, 3 = k4VGA, etc)
   * 
   * camera: Camera select, as an integer defined by Aldebaran
   * (kTop=0, kBottom=1)
   */
  void configure(const std::string &field, const std::string &value);
  
  void startStream();

  void stopStream();

private:
  struct Impl;
  boost::shared_ptr<Impl> impl;
};


class NaoClientError : public std::exception {
public:
  NaoClientError(const char* message);
  NaoClientError(const std::string message);
  virtual const char* what();
private:
  const char* msg;
};


#endif // NAOCLIENT_H
