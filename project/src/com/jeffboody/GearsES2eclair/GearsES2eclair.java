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

	// touch state
	private final int INIT_STATE   = 0;
	private final int ROTATE_STATE = 1;
	private final int ZOOM_STATE   = 2;
	private int   State = INIT_STATE;
	private float X1    = 0.0F;
	private float Y1    = 0.0F;
	private float X2    = 0.0F;
	private float Y2    = 0.0F;
	private float S     = 0.0F;

	// Native interface
	private native void NativeRotate(float dx, float dy);
	private native void NativeScale(float ds);
	private native void NativeRoll(float roll);

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		A3DResource r = new A3DResource(this, R.raw.timestamp);
		r.Add(R.raw.whitrabt, "whitrabt.tex.gz");

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

    @Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// 1) Place icons in the drawable directory
		// 2) Rebuild to populate the R datastructure
		menu.add(0, MENU_ABOUT,  0, "About").setIcon(R.drawable.about);
		return true;
	}

    @Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		// Intents must be placed in the AndroidManifest.xml file ...
		int id = item.getItemId();
		if(id == MENU_ABOUT) startActivity(new Intent(this, GearsES2eclairAbout.class));
		else return false;

		return true;
	}


	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int action = event.getAction();
		int count  = event.getPointerCount();

		if(action == MotionEvent.ACTION_UP)
		{
			// Do nothing
			State = INIT_STATE;
		}
		else if(count == 1)
		{
			if(action == MotionEvent.ACTION_DOWN)
			{
				X1 = event.getX();
				Y1 = event.getY();
				State = ROTATE_STATE;
			}
			else if((action == MotionEvent.ACTION_MOVE) && (State == ROTATE_STATE))
			{
				float dx = event.getX() - X1;
				float dy = event.getY() - Y1;
				NativeRotate(dx, dy);
				X1 = event.getX();
				Y1 = event.getY();
			}
		}
		else if(count == 2)
		{
			if((action == MotionEvent.ACTION_DOWN) ||
			   (action == MotionEvent.ACTION_POINTER_1_DOWN) ||
			   (action == MotionEvent.ACTION_POINTER_2_DOWN))
			{
				try
				{
					// some unknown device throws an exception here
					X1 = event.getX(event.findPointerIndex(0));
					Y1 = event.getY(event.findPointerIndex(0));
					X2 = event.getX(event.findPointerIndex(1));
					Y2 = event.getY(event.findPointerIndex(1));
				}
				catch(Exception e)
				{
					// fail silently
					return false;
				}
				double dx = Math.abs((double) (X2 - X1));
				double dy = Math.abs((double) (Y2 - Y1));
				S = (float) Math.sqrt((dx * dx) + (dy * dy));
				State = ZOOM_STATE;
			}
			else if((action == MotionEvent.ACTION_MOVE) && (State == ZOOM_STATE))
			{
				float x1 = 0.0F;
				float y1 = 0.0F;
				float x2 = 0.0F;
				float y2 = 0.0F;
				try
				{
					// some unknown device throws an exception here
					x1 = event.getX(event.findPointerIndex(0));
					y1 = event.getY(event.findPointerIndex(0));
					x2 = event.getX(event.findPointerIndex(1));
					y2 = event.getY(event.findPointerIndex(1));
				}
				catch(Exception e)
				{
					// fail silently
					return false;
				}
				double dx = Math.abs((double) (x2 - x1));
				double dy = Math.abs((double) (y2 - y1));
				float s = (float) Math.sqrt((dx * dx) + (dy * dy));
				NativeScale(S - s);

				// a.b = mag_a*mag_b*cos(roll)
				// direction or roll is determined by axb
				double ax = x2 - x1;
				double ay = y2 - y1;
				double bx = X2 - X1;
				double by = Y2 - Y1;
				double mag_a = Math.sqrt(ax*ax + ay*ay);
				double mag_b = Math.sqrt(bx*bx + by*by);
				if((mag_a >= 5.0) && (mag_b >= 5.0))
				{
					double axb  = (ax*by - ay*bx);
					double ab   = (ax*bx + ay*by);
					float  roll = (float) ((180.0/Math.PI)*Math.acos(ab/(mag_a*mag_b)));
					if(Math.abs((double) roll) >= 0.5)
					{
						NativeRoll((axb > 0.0) ? roll : -roll);
					}
				}

				X1 = x1;
				Y1 = y1;
				X2 = x2;
				Y2 = y2;
				S = s;
			}
		}

		return true;
	}

	static
	{
		System.loadLibrary("a3d");
		System.loadLibrary("GearsES2eclair");
	}
}
