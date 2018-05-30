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
import android.content.Context;
import android.content.Intent;
import android.os.Message;
import android.os.Handler;
import android.os.Handler.Callback;
import android.os.SystemClock;
import android.net.Uri;
import android.view.KeyEvent;
import android.media.AudioManager;
import java.lang.Math;
import java.lang.Exception;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import com.jeffboody.a3d.A3DSurfaceView;
import com.jeffboody.a3d.A3DNativeRenderer;
import com.jeffboody.a3d.A3DResource;

// Enable traceview (see also onResume and onPause)
// See: http://developer.android.com/guide/developing/tools/traceview.html
// Add to Android.manifest: <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
// import android.os.Debug;

public class GearsES2eclair
extends Activity
implements Handler.Callback
{
	private static final String TAG = "GearsES2eclair";

	private A3DNativeRenderer Renderer;
	private A3DSurfaceView    Surface;

	// native commands
	private static final int GEARS_CMD_PLAY_CLICK = 1;
	private static final int GEARS_CMD_EXIT       = 2;
	private static final int GEARS_CMD_LOADURL    = 3;

	private static LinkedList<Integer> mCmdQueue = new LinkedList<Integer>();
	private static Lock                mCmdLock  = new ReentrantLock();

	// "singleton" used for callbacks
	// handler is used to trigger commands on UI thread
	private static Handler mHandler = null;
	private static String  mURL     = "";

	// key events
	private static final int GEARS_KEY_ENTER     = 0x00D;
	private static final int GEARS_KEY_ESCAPE    = 0x01B;
	private static final int GEARS_KEY_BACKSPACE = 0x008;
	private static final int GEARS_KEY_DELETE    = 0x07F;
	private static final int GEARS_KEY_UP        = 0x100;
	private static final int GEARS_KEY_DOWN      = 0x101;
	private static final int GEARS_KEY_LEFT      = 0x102;
	private static final int GEARS_KEY_RIGHT     = 0x103;
	private static final int GEARS_KEY_HOME      = 0x104;
	private static final int GEARS_KEY_END       = 0x105;
	private static final int GEARS_KEY_PGUP      = 0x106;
	private static final int GEARS_KEY_PGDOWN    = 0x107;
	private static final int GEARS_KEY_INSERT    = 0x108;

	private static double getTimestamp(double t0)
	{
		// convert "uptime" timestamp to UTC timestamp
		double now = (double) System.currentTimeMillis();
		double t1  = (double) SystemClock.uptimeMillis();
		return (now + t0 - t1)/1000.0;
	}

	/*
	 * Native interface
	 */

	private native void NativeKeyDown(int keycode, int meta, double ts);
	private native void NativeKeyUp(int keycode, int meta, double ts);

	/*
	 * Command Queue - A queue is needed to ensure commands
	 * are not lost between the rendering thread and the
	 * main thread when pausing the app.
	 */

	private void DrainCommandQueue(boolean handler)
	{
		mCmdLock.lock();
		try
		{
			while(mCmdQueue.size() > 0)
			{
				int cmd = mCmdQueue.remove();
				if(cmd == GEARS_CMD_PLAY_CLICK)
				{
					if(handler)
					{
						AudioManager am = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
						am.playSoundEffect(AudioManager.FX_KEY_CLICK, 0.5F);
					}
				}
				else if(cmd == GEARS_CMD_LOADURL)
				{
					try
					{
						Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(mURL));
						startActivity(intent);
					}
					catch(Exception e)
					{
						// ignore
					}
				}
				else if(cmd == GEARS_CMD_EXIT)
				{
					if(handler)
					{
						finish();
					}
				}
				else
				{
					Log.w(TAG, "unknown cmd=" + cmd);
				}
			}
		}
		catch(Exception e)
		{
			Log.e(TAG, "exception: " + e);
		}
		mCmdLock.unlock();
	}

	/*
	 * Callback interface
	 */

	private static void CallbackCmd(int cmd, String msg)
	{
		try
		{
			if(cmd == GEARS_CMD_LOADURL)
			{
				mURL = msg;
			}

			mCmdLock.lock();
			mHandler.sendMessage(Message.obtain(mHandler, cmd));
			mCmdQueue.add(cmd);
			mCmdLock.unlock();
		}
		catch(Exception e)
		{
			// ignore
		}
	}

	/*
	 * Handler.Callback interface
	 */

	public boolean handleMessage(Message msg)
	{
		DrainCommandQueue(true);
		return true;
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		mHandler = new Handler(this);

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
		mHandler = null;
        super.onDestroy();
	}

	private static int getKey(int keycode, KeyEvent event)
	{
		int ascii = event.getUnicodeChar(0);
		if(keycode == KeyEvent.KEYCODE_ENTER)
		{
			return GEARS_KEY_ENTER;
		}
		else if(keycode == KeyEvent.KEYCODE_ESCAPE)
		{
			return GEARS_KEY_ESCAPE;
		}
		else if(keycode == KeyEvent.KEYCODE_DEL)
		{
			return GEARS_KEY_BACKSPACE;
		}
		else if(keycode == KeyEvent.KEYCODE_FORWARD_DEL)
		{
			return GEARS_KEY_DELETE;
		}
		else if(keycode == KeyEvent.KEYCODE_DPAD_UP)
		{
			return GEARS_KEY_UP;
		}
		else if(keycode == KeyEvent.KEYCODE_DPAD_DOWN)
		{
			return GEARS_KEY_DOWN;
		}
		else if(keycode == KeyEvent.KEYCODE_DPAD_LEFT)
		{
			return GEARS_KEY_LEFT;
		}
		else if(keycode == KeyEvent.KEYCODE_DPAD_RIGHT)
		{
			return GEARS_KEY_RIGHT;
		}
		else if(keycode == KeyEvent.KEYCODE_MOVE_HOME)
		{
			return GEARS_KEY_HOME;
		}
		else if(keycode == KeyEvent.KEYCODE_MOVE_END)
		{
			return GEARS_KEY_END;
		}
		else if(keycode == KeyEvent.KEYCODE_PAGE_UP)
		{
			return GEARS_KEY_PGUP;
		}
		else if(keycode == KeyEvent.KEYCODE_PAGE_DOWN)
		{
			return GEARS_KEY_PGDOWN;
		}
		else if(keycode == KeyEvent.KEYCODE_BACK)
		{
			return GEARS_KEY_ESCAPE;
		}
		else if((ascii > 0) && (ascii < 128))
		{
			return ascii;
		}
		return 0;
	}

	@Override
	public boolean onKeyDown(int keycode, KeyEvent event)
	{
		int    ascii = getKey(keycode, event);
		int    meta  = event.getMetaState();
		double ts    = getTimestamp(event.getEventTime());

		if(ascii > 0)
		{
			NativeKeyDown(ascii, meta, ts);
		}

		return true;
	}

	@Override
	public boolean onKeyUp(int keycode, KeyEvent event)
	{
		int    ascii = getKey(keycode, event);
		int    meta  = event.getMetaState();
		double ts    = getTimestamp(event.getEventTime());

		if(ascii > 0)
		{
			NativeKeyUp(ascii, meta, ts);
		}

		return true;
	}

	static
	{
		System.loadLibrary("GearsES2eclair");
	}
}
