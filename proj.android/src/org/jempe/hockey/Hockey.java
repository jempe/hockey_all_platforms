/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.jempe.hockey;

import me.kiip.sdk.Kiip;
import me.kiip.sdk.KiipFragmentCompat;
import me.kiip.sdk.Poptart;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.util.Log;

import com.flurry.android.FlurryAgent;

public class Hockey extends Cocos2dxActivity{
	
    private final static String KIIP_TAG = "kiip_fragment_tag";

    private static KiipFragmentCompat mKiipFragment;
    
	private static final String TAG = "Hockey Activity";
	

	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		// Change libcocos2dx Activity class to FragmentActivity need to add android-support-v4.jar
		// add KiipSDK.jar to the libs folder
		
        // Create or re-use KiipFragment.
        if (savedInstanceState != null) {
            mKiipFragment = (KiipFragmentCompat) getSupportFragmentManager().findFragmentByTag(KIIP_TAG);
        } else {
            mKiipFragment = new KiipFragmentCompat();
            getSupportFragmentManager().beginTransaction().add(mKiipFragment, KIIP_TAG).commit();
        }
	}
	
	@Override
	public void onResume() {
		super.onResume();
	}
	
	@Override
	public void onPause() {
	    super.onPause();
	}
	
	@Override
	protected void onStart()
	{
		super.onStart();
        Kiip.getInstance().startSession(new Kiip.Callback() {
            @Override
            public void onFailed(Kiip kiip, Exception exception) {
                // handle failure
            }

            @Override
            public void onFinished(Kiip kiip, Poptart poptart) {
                onPoptart(poptart);
            }
        });
		
		FlurryAgent.onStartSession(this, getString(R.string.flurry_api_key));
	}
	
	@Override
	protected void onStop()
	{
		super.onStop();	
		FlurryAgent.onEndSession(this);
        Kiip.getInstance().endSession(new Kiip.Callback() {
            @Override
            public void onFailed(Kiip kiip, Exception exception) {
                // handle failure
            }

            @Override
            public void onFinished(Kiip kiip, Poptart poptart) {
                onPoptart(poptart);
            }
        });
	}
	
    static void onPoptart(Poptart poptart) {
        mKiipFragment.showPoptart(poptart);
    }
    
    private native void pauseGame();
	
    static {
         System.loadLibrary("game");
    }
    
    static void flurry_event(final String event_name)
    {
    	FlurryAgent.logEvent(event_name);
    }
  
    static void kiip_moment(final String moment_name)
    {
       	Kiip.getInstance().saveMoment(moment_name, new Kiip.Callback() {
    		@Override
    		public void onFinished(Kiip kiip, Poptart reward) 
    		{
    			onPoptart(reward);
    			Log.d("kiip", "kiip reward");
    		}

    		@Override
    		public void onFailed(Kiip kiip, Exception exception) {
    			// handle failure
    		}
    	});
    }
}
