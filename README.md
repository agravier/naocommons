naocommons
==========

Naocommons is a software platform to program the NAO robot. It aims at
compelementing Aldebaran's API by providing 

* low-latency configurable data streams from the robot and 

* flexible means to process these streams and build an application on
  top of them.

Ultimately, the naocommons framework will allow you to program in the
environment and language of your choice and reuse code that can only
runs different platforms. For that, 

## Components (Please note that most of these don't exist yet)

The **naoserver** is a live interface to your robot. You can communicate
with the server, either over the network or as direct a plug-in: 

* determine what services are avalaible, from server-mediated access
  to NAO actuators and sensory streams to high-level services provided
  by other clients and plug-ins

* communicate directly to a service by using its API

* make your own services available to other clients.

The **naoclient** runs on NAO and handles requests from naoserver. It
streams sensor data to the naoserver to make it available for your
programming needs.

The **client framework** contains libraries and code samples in a
variety of languages to help you start programming your NAO control
application using clients. This is the most flexible framework for you
to write your NAO application:

* you may run a client on the same machine as naoserver or on a
  different machine, communicating with the server over the
  network. Communication with the server uses JSON RPC.

* you may write your client program in any language. JSON RPC is a
  simple protocol with implementations in numerous languages.

* the drawback of the client framework is a higher latency.

The **plug-in framework** lets your write your control program as a
naoserver plug-in. 

* you can only run it on the same computer as the naoserver. 

* it is most conveniently written in a language with access to C
  types.

* the advantage is a lower latency.

## Dependencies

Aldebaran tools (naoqi, libraries, C++ SDK)

NAO v4

## Quickstart

This program is developed on a 32 bit Linux machine for a NAO v4
(Atom), so YMMV with other configurations.

Assuming that you have set up your development environment as advised
by Aldebaran in the official NAO documentation, and named your NAO
build chain "nao-atom", then you should be able to chechout this
project in your qibuild workspace, add it and run

    qibuild configure -c nao-atom && qibuild make -c nao-atom && qibuild deploy -c nao-atom nao@192.168.1.11:naoqi

where 192.168.1.11 is the IP of your NAO.


Now, ssh to your NAO. On NAO, add the following to ~/naoqi/preferences/autoload.ini, under
the [user] section:

    /home/nao/naoqi/lib/naoqi/libnaoclient.so

restart NAOqi:

    nao restart

try it:

    start_naoclient

(For now, it just grabs a frame from the active camera to the file
~/startstream.yuv. You can copy it to your computer and use,
for instance, pyuv to look at it: 1280x960, YUV 4:2:2, interleaved.)

You can check the log in /var/log/naoqi.log. To have a look at the
verbose log, you can stop naoqi

    nao stop

and start it with "verbose" output:

    naoqi -v

to see if the RTSP server works, put a raw h264 file in your current
directort and call 

    ~/naoqi/bin/testOnDemandRTSPServer 

You should be able to connect to your stream from another computer
using VLC.

## Documentation

For API doc, see the generated documentation when there is one, or the
source code comments.

For design docs and future development plans, see the relevant wiki pages.

The Naocommons wiki also contains various resources from and by researchers
who use Aldebaran's Nao. Anyone can edit the wiki and contribute. I
rearrange the pages when necessary.

## Attribution and licensing

### Names

NAO and NAOqi are trademarks of [Aldebaran
Robotics](http://aldebaran-robotics.com).

The LiveMedia framework is developed by [Live Networks,
Inc.](http://live555.com) and distributed under the LGPL.

x264 is a free software library developed by the [x264
team](http://www.videolan.org/developers/x264.html) distributed under
the GPL.

The naocommons wiki contents are the property of their respective
creators.

### Licensing

The naoclient, naoserver and core naoserver plug-ins and clients are
distributed under the General Public License, version 3 (LGPL
v3). This lets you to program your own naoserver plug-ins and clients
privately without having to redistribute the source. However, if your
application that makes use of naocommons is distributed, you must
release the entire source code under the GPL. ** Do not ask me to
change the license, it is not possible because I make use of x264,
another GPL-licensed piece of software.** Although it is
dual-licensed, I did not purchase the right to commercially distribute
it.
