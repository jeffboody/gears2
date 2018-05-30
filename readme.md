About
=====

Gears for Android is a heavily modified port of the famous "gears" demo to Android.

The Gears demo is an open source project intended to help developers learn how to create
OpenGL ES programs on Android. The Gears demo was originally written by Brian Paul as
part of the Mesa3D project. My implementation includes variations for Java/OpenGL ES 1.x,
Java/C/OpenGL ES 1.x and Java/C/OpenGL ES 2.0. I have also added several features not
found in the original implementation including touch screen support, VBOs and an
on-screen FPS counter.

The FPS (frames-per-second) counter is often used as a benchmark metric for graphics
programs. On Android the frame rate is limited by v-sync (typically 60 FPS) which is
the fastest rate that a display can refresh the screen. Since Gears is capable of
rendering much faster than v-sync on most devices it provides limited benchmarking
value.

See http://www.jeffboody.net/gears4android.php for more information.

Send questions or comments to Jeff Boody at jeffboody@gmail.com

Source Branches
===============

The master branch contains upgraded source code for Android Studio, Gradle
and CMake. This code base was derived from the legacy branch but tweaked
to fit the new directory structure and build requirements. The legacy branch
contains older source code for Ant and ndk build. For the legacy branch you
may have the best luck with the last official commit.

a750aa2 update version for 20150227

Source Code
===========

	# Source code is hosted on github at https://github.com/jeffboody/gears2
	# To clone the git repository enter the following commands:
	git clone git://github.com/jeffboody/gears2.git
	cd gears2
	git submodule update --init

Installing JDK on Ubuntu
========================

1. Download the "Java SE Development Kit"

	jdk-xxxx-linux-x64.tar.gz

2. Extract the jdk to /opt

	<replace the xxxx as needed>
	sudo mv jdk-xxxx-linux-x64.tar.gz /opt
	sudo cd /opt
	sudo tar -xzf jdk-xxxx-linux-x64.tar.gz
	sudo rm jdk-xxxx-linux-x64.tar.gz
	sudo ln -s jdkXXXX jdk

3. Update alternatives

	# install jdk
	sudo update-alternatives --install /usr/bin/jar jar /opt/jdk/bin/jar 100
	sudo update-alternatives --install /usr/bin/jarsigner jarsigner /opt/jdk/bin/jarsigner 100
	sudo update-alternatives --install /usr/bin/java java /opt/jdk/bin/java 100
	sudo update-alternatives --install /usr/bin/javac javac /opt/jdk/bin/javac 100
	sudo update-alternatives --install /usr/bin/javadoc javadoc /opt/jdk/bin/javadoc 100

	# update defaults (if needed)
	sudo update-alternatives --config jar
	sudo update-alternatives --config jarsigner
	sudo update-alternatives --config java
	sudo update-alternatives --config javac
	sudo update-alternatives --config javadoc

4. Update dependencies for 32-bit libraries needed by aapt

	sudo apt-get install gcc-multilib lib32z1 lib32stdc++6

Building and Installing
=======================

1. Install the Android(TM) SDK and NDK available from http://developer.android.com/
2. Initialize environment variables

	<edit profile>
	source profile

3. Build project

	./build-native.sh
	./build-java.sh

4. Install apk

	./install.sh

Uninstalling
============

On the device/emulator, navigate to Settings, Applications, Manage applications

Or uninstall via adb

	./uninstall.sh

License
=======

	Copyright (c) 2009-2011 Jeff Boody

	Gears for Android is a heavily modified port of the famous "gears" demo.
	As such, it is a derived work subject to the license requirements (below)
	of the original work.

	Copyright (c) 1999-2001  Brian Paul   All Rights Reserved.

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

White Rabbit Font
=================

About
-----

	Font: White Rabbit
	Created By: Matthew Welch
	E-Mail: matt@squaregear.net
	Web Address: http://www.squaregear.net/fonts/

	My fonts are all free.  You can use them for personal or commercial projects,
	and I ask for no money.  I would, however, love to hear from you.  If you use
	my fonts for something please e-mail me letting me know how you used it.  Send
	me a copy if you can or let me know where I can find your work. You are under
	no obligation to do this, I just like to see how my fonts get used.

	A license.txt file should have been included with this font, explaining the
	license under which it is made available. You can also read it at:

	http://www.squaregear.net/fonts/license.shtml

	About the font:

	White Rabbit is reminiscent of the characters displayed on all those
	old low-res terminal screens. Smoothed out and cleaned up for 1999,
	this is the font to use for all your computing applications.

License
-------

	Copyright (C) 1999 by Matthew Welch

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this font software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
