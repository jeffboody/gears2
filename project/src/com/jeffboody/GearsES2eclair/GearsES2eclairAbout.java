/*
 * Copyright (c) 2009-2011 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

package com.jeffboody.GearsES2eclair;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;

public class GearsES2eclairAbout extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		WebView wv = new WebView(this);
		String text = "<html><body bgcolor=\"#37547c\" text=\"#FFFFFF\" link=\"#a08f6b\" vlink=\"#a08f6b\">" +
		              "<h2>About Gears for Android(TM)</h2>" +
		              "<p>" +
		              "	Gears for Android(TM) is a heavily modified port of the infamous \"gears\" demo to Android." +
		              "</p>" +
		              "<p>" +
		              "	The Gears demo is an open source project intended to help developers learn how to create " +
		              "	OpenGL ES programs on Android. The Gears demo was originally written by Brian Paul as " +
		              "	part of the Mesa3D project. My implementation includes variations for Java/OpenGL ES 1.x, " +
		              "	Java/C/OpenGL ES 1.x and Java/C/OpenGL ES 2.0. I have also added several features not " +
		              "	found in the original implementation including touch screen support, VBOs and an " +
		              "	on-screen FPS counter." +
		              "</p>" +
		              "<p>" +
		              "	The FPS (frames-per-second) counter is often used as a benchmark metric for graphics " +
		              "	programs. On Android the frame rate is limited by v-sync (typically 60 FPS) which is " +
		              "	the fastest rate that a display can refresh the screen. Since Gears is capable of " +
		              "	rendering much faster than v-sync on most devices it provides limited benchmarking " +
		              "	value." +
		              "</p>" +
		              "<h2>Source Code</h2>" +
		              "<p>" +
		              "	Source code is available at the <a href=\"http://www.jeffboody.net/gears4android.php\">Gears for Android(TM)</a> website" +
		              "</p>" +
		              "<h2>Release Notes</h2>" +
		              "<p>20110501" +
		              "	<ul>" +
		              "		<li>For Android 2.0 (eclair)</li>" +
		              "		<li>Fix lighting bug observed when zooming</li>" +
		              "		<li>Replaced simclist library</li>" +
		              "		<li>Compile in thumb mode</li>" +
		              "		<li>Simplified a3d GL wrapper</li>" +
		              "		<li>Updated Gears icon (again)</li>" +
		              "		<li>Uploaded source to github</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20101218" +
		              "	<ul>" +
		              "		<li>For Android 2.0 (eclair)</li>" +
		              "		<li>Workaround for rendering corruption on Nexus S and Galaxy S</li>" +
		              "		<li>Updated Gears icon</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20101113" +
		              "	<ul>" +
		              "		<li>For Android 2.0 (eclair)</li>" +
		              "		<li>Reimplemented in OpenGL ES 2.0</li>" +
		              "		<li>Based on 20100905 release</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20100905" +
		              "	<ul>" +
		              "		<li>For Android 2.0 (eclair)</li>" +
		              "		<li>Reimplemented in C with NDK</li>" +
		              "		<li>Included better FPS counter font</li>" +
		              "		<li>Added GLES performance analysis logging</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20100211" +
		              "	<ul>" +
		              "		<li>For Android 2.0 (eclair)</li>" +
		              "		<li>Multitouch zoom support</li>" +
		              "		<li>Fullscreen support</li>" +
		              "		<li>Faster orientation changes</li>" +
		              "		<li>Render gears with VBOs</li>" +
		              "		<li>Disable blending when not required</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20091229" +
		              "	<ul>" +
		              "		<li>For Android 1.6 or Android 2.0 (donut or eclair)</li>" +
		              "		<li>Fixed EGL config selection for Droid</li>" +
		              "		<li>I would like to thank David Van de Ven for help debugging/testing on the Droid</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20091010" +
		              "	<ul>" +
		              "		<li>For Android 1.6 (donut)</li>" +
		              "		<li>Touchscreen performance patched by Greg Copeland with additional suggestions by Alexander Mikhnin</li>" +
		              "		<li>Rewrote texture loader</li>" +
		              "		<li>Changed data passed into gl*Pointer functions to be allocateDirect</li>" +
		              "		<li>Changed various data to be nativeOrder</li>" +
		              "		<li>Added directions for running Traceview</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20090621" +
		              "	<ul>" +
		              "		<li>For Android 1.5 (cupcake)</li>" +
		              "		<li>Fixed performance bug (allocating variables on heap causing gc to run)</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20090620" +
		              "	<ul>" +
		              "		<li>For Android 1.5 (cupcake)</li>" +
		              "		<li>Added on-screen fps counter at 1 second interval</li>" +
		              "		<li>Removed logcat fps counter</li>" +
		              "		<li>Fixed a bug handling EGL_CONTEXT_LOST event</li>" +
		              "		<li>Updated About screen to use WebView</li>" +
		              "		<li>Updated Gears icon</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<p>20090502" +
		              "	<ul>" +
		              "		<li>For Android 1.1 (pre-cupcake)</li>" +
		              "		<li>3D hardware accelerated rendering is implemented using OpenGL ES 1.0</li>" +
		              "		<li>Demo may be run on either an Android device such as the T-Mobile G1 or on the emulator that is included with the Android SDK</li>" +
		              "		<li>Available from the Android market</li>" +
		              "		<li>Touchscreen may be used to rotate camera</li>" +
		              "		<li>fps is written to logcat</li>" +
		              "		<li>Achieves 60 fps on the T-Mobile G1 (performance is limited by display, not GPU)</li>" +
		              "		<li>Performance may drop while using the touchscreen (known problem with the Android SDK)</li>" +
		              "	</ul>" +
		              "</p>" +
		              "<h2>Feedback</h2>" +
		              "<p>" +
		              "	Send questions or comments to Jeff Boody at jeffboody@gmail.com" +
		              "</p>" +
		              "<h2>License</h2>" +
		              "<p>" +
		              "	Copyright (c) 2009-2011 Jeff Boody<br/>" +
		              "	Gears for Android(TM) is a heavily modified port of the infamous \"gears\" demo to " +
		              "	Android/Java/GLES 1.0. As such, it is a derived work subject to the license " +
		              "	requirements (below) of the original work.<br/>" +
		              "	<br/>" +
		              "	Copyright (c) 1999-2001  Brian Paul   All Rights Reserved.<br/>" +
		              "	<br/>" +
		              "	Permission is hereby granted, free of charge, to any person obtaining a " +
		              "	copy of this software and associated documentation files (the \"Software\"), " +
		              "	to deal in the Software without restriction, including without limitation " +
		              "	the rights to use, copy, modify, merge, publish, distribute, sublicense, " +
		              "	and/or sell copies of the Software, and to permit persons to whom the " +
		              "	Software is furnished to do so, subject to the following conditions:<br/>" +
		              "	<br/>" +
		              "	The above copyright notice and this permission notice shall be included " +
		              "	in all copies or substantial portions of the Software.<br/>" +
		              "	<br/>" +
		              "	THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR " +
		              "	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, " +
		              "	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE " +
		              "	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER " +
		              "	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, " +
		              "	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN " +
		              "	THE SOFTWARE." +
		              "</p>" +
		              "</body></html>";
		wv.loadData(text, "text/html", "utf-8");
		setContentView(wv);
	}
}
