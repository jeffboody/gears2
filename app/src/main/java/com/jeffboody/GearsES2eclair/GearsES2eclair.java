/*
 * Copyright (c) 2009-2010 Jeff Boody
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

import android.util.Log;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.content.Intent;
import java.lang.Math;
import java.lang.Exception;
import com.jeffboody.a3d.A3DSurfaceView;
import com.jeffboody.a3d.A3DNativeRenderer;
import com.jeffboody.a3d.A3DResource;

// Enable traceview (see also onResume and onPause)
// See: http://developer.android.com/guide/developing/tools/traceview.html
// Add to Android.manifest: <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
// import android.os.Debug;

public class GearsES2eclair extends Activity
{
	private static final String TAG = "GearsES2eclair";

	private A3DNativeRenderer Renderer;
	private A3DSurfaceView    Surface;

	private static final int MENU_ABOUT  = 0;

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		A3DResource r = new A3DResource(this, R.raw.timestamp);
		r.Add(R.raw.resource, "resource.pak");

		// Make window fullscreen
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		                     WindowManager.LayoutParams.FLAG_FULLSCREEN); 

		Renderer = new A3DNativeRenderer(this);
		Surface  = new A3DSurfaceView(Renderer, r, this);
		setContentView(Surface);
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		// Start tracing to "/sdcard/gears.trace"
		// To analyze performance enable this code (see also onPause)
		// then use the Traceview tool to open the trace file
		// Debug.startMethodTracing("/sdcard/gears");

		Surface.ResumeRenderer();
	}

	@Override
	protected void onPause()
	{
		Surface.PauseRenderer();

		// To analyze performance enable this code (see also onResume)
		// then use the Traceview tool to open the trace file
		// Debug.stopMethodTracing();

		super.onPause();
	}

	@Override
	protected void onDestroy()
	{
		Surface.StopRenderer();
		Surface = null;
		Renderer = null;
        super.onDestroy();
	}

	static
	{
		System.loadLibrary("GearsES2eclair");
	}
}
