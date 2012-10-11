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

#ifndef FGRAB_H
  #define FGRAB_H

#include <alcommon/almodule.h>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <iostream>


namespace AL {
  // This is a forward declaration of AL:ALBroker which
  // avoids including <alcommon/albroker.h> in this header
  class ALBroker;
}


/**
 * This class inherits AL::ALModule. This allows it to bind methods
 * and be run as a remote executable within NAOqi
 */
class FGrab : public AL::ALModule {
public:
  FGrab(boost::shared_ptr<AL::ALBroker> broker,
        const std::string &name);

  ~FGrab();

  /**
   * Overloading ALModule::init().
   * This is called right after the module has been loaded.
   */
  virtual void init();

  /**
   * Overloading ALModule::exit().
   * This is called to unload the module.
   */
  virtual void exit();
  
  // Writes a raw picture from the active camera to the file
  void grab_to_file(const std::string &file);

  // Returns a buffer containing a raw YUV422 picture. This buffer is
  // not to be freed manually. Call release() for cleanup.
  unsigned char * grab();

  int getImageSize();


  // Release the buffer, necessary before calling grab again
  void release();

  // Changes the camera being captuerd. The parameter corresponds to
  // Aldebaran's camera selector codes
  void switchCamera(int camera);
  
  /* no copy
  FGrab & operator=(const FGrab&) = delete;
  FGrab(const FGrab&) = delete;
  FGrab() = default;
  */
private:
  struct Impl;
  boost::shared_ptr<Impl> impl;
};


class FGrabError : public std::exception {
public:
  FGrabError(const char* message);
  FGrabError(const std::string message);
  virtual const char* what();
private:
  const char* msg;
};


#endif // FGRAB_H
