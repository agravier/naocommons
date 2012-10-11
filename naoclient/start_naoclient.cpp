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


#include <alcommon/alproxy.h>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int pport = 9559;
  std::string pip = "127.0.0.1";

  // check the number of arguments
  if (argc != 1 && argc != 3 && argc != 5) {
    std::cerr << "Wrong number of arguments!" << std::endl;
    std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
    exit(2);
  }

  // if there is only one argument it should be IP or PORT
  if (argc == 3) {
    if (std::string(argv[1]) == "--pip")
      pip = argv[2];
    else if (std::string(argv[1]) == "--pport")
      pport = atoi(argv[2]);
    else {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }

  // Sepcified IP or PORT for the connection
  if (argc == 5) {
    if (std::string(argv[1]) == "--pport"
        && std::string(argv[3]) == "--pip") {
      pport = atoi(argv[2]);
      pip = argv[4];
    } else if (std::string(argv[3]) == "--pport"
             && std::string(argv[1]) == "--pip") {
      pport = atoi(argv[4]);
      pip = argv[2];
    } else {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }

  /**
   * Create a proxy to a module in NAOqi process so that we can call
   * the bind method of this module
   * AL::ALProxy proxy(<module_name>, <robot_IP>, <robot_port>);
   */
  boost::shared_ptr<AL::ALProxy> proxy =
      boost::shared_ptr<AL::ALProxy>(new AL::ALProxy("NaoClient", pip, pport));

  /**
   * If the bind methode is a void return
   * you can call bind methode using callVoid
   * proxy.callVoid(<bind_method>, <parameter>, ...)
   */
  // proxy->callVoid("grab_to_file", std::string("/home/nao/raw_pic_2.yuv"));
  proxy->callVoid("startStream");
  proxy->callVoid("stopStream");

  /**
   * Otherwise you can use template call methode
   * type res = proxy.call<type>(<bind_methode>, <parameter>, ...);
   */
  bool res = proxy->call<bool>("ping");

  return 0;
}
