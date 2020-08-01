MintIRCd
========

A basic IRC daemon based from miniircd which includes new features like 
standalone services.

NOTE: Basic protocol for the IRC backside between clients are as 
follows. IRC protocol between the IRC services are not, as it is more of 
a modification for the IRC daemon.

Plans
-----
* Server linking
* Channel Operator

Additional features
-------------------
* Services (Standalone)
* Fixed bugs within timeout

Compiling and starting
----------------------
First off, you need to compile the services.

    # cd services
    # make

Then you can start the server along with the services using start_ircd.
Make sure you edit the variables in the start script!

    # cd ..
    # ./start_ircd

Changelog
---------
* v0.4.1b
	-Bugfix: SOCKBUFIN value too low. Cause high CPU when too long of a string was parsed.
* v0.4.1
	-Added standalone C service linking to python daemon
	-Added IP cloaking for registered users
	-Cleaned up code positioning

Licensing
---------
MintIRCd is released, as like miniIRCd, under the GNU General 
Public License version 2 or later.

Developers
----------

* 2003-2014, Joel Rosdahl (jeol@rosdahl.net). Original developer.
* 2014, Chris Dorman (cddo@riseup.net)

Contributors
------------
Contributors to the previous MiniIRCd include
* Alex Wright
* Leandro Lucarella
* Martin Maney
* Matt Behrens
* Ron Fritz
