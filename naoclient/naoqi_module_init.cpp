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


// Module library initialization code

#ifndef _WIN32
# include <signal.h>
#endif

#include <boost/shared_ptr.hpp>

#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>
#include "naoclient.h"

// export the entry point
#ifdef _WIN32
  #define ALCALL __declspec(dllexport)
#else
  #define ALCALL
#endif

extern "C"
{
  ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
  {
    // init broker with the main broker instance
    // from the parent executable
    AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
    AL::ALBrokerManager::getInstance()->addBroker(pBroker);
    // create module instances
//    AL::ALModule::createModule<FGrab>(pBroker, "FGrab");
    AL::ALModule::createModule<NaoClient>(pBroker, "NaoClient");
    return 0;
  }

  ALCALL int _closeModule(  )
  {
    return 0;
  }
}

