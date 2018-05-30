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

Send questions or comments to Jeff Boody at jeffboody@gmail.com

Source Branches
===============

The master branch contains upgraded source code for Android Studio, Gradle
and CMake. This code base was derived from the legacy branch but tweaked
to fit the new directory structure and build requirements. The legacy branch
contains older source code for Ant and ndk build. For the legacy branch you
may have the best luck with the last official commit.

a750aa2 update version for 20150227

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
