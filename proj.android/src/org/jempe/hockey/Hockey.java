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

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

import com.amazon.device.ads.AdLayout;
import com.amazon.device.ads.AdRegistration;
import com.amazon.device.ads.AdSize;
import com.amazon.device.ads.AdTargetingOptions;
import com.flurry.android.FlurryAgent;
import com.revmob.cocos2dx.RevMobWrapper;

public class Hockey extends Cocos2dxActivity{
    
	private static final String TAG = "Hockey Activity ";
	
    private ViewGroup adViewContainer; // View group to which the ad view will be added
    private static AdLayout nextAdView; // A placeholder for the next ad so we can keep the current ad visible while the next ad loads
	
    private static Context mContext;

	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		mContext = this;
		
		RevMobWrapper.setActivity(this); 
		
		// AmazonMobileAds Registration
		AdRegistration.setAppKey(getString(R.string.amazon_app_key));
		AdRegistration.enableTesting(true);
		AdRegistration.enableLogging(true);
		
		//LayoutParams adParams = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT,
	    //        		LayoutParams.WRAP_CONTENT, Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);
		//LinearLayout subLayout = new LinearLayout(this);
		//subLayout.setBackgroundColor(Color.TRANSPARENT);
		//addContentView(subLayout, adParams);
		//adViewContainer = subLayout;
		
		//LoadAmazonAd();
	}
	

	@Override
	public void onDestroy()
	{
		super.onDestroy();
        if (nextAdView != null)
            this.nextAdView.destroy();
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
		
		FlurryAgent.onStartSession(this, getString(R.string.flurry_api_key));
	}
	
	@Override
	protected void onStop()
	{
		super.onStop();	
		FlurryAgent.onEndSession(this);
	}
    
    private native void pauseGame();
	
    static {
         System.loadLibrary("game");
    }
    
    static void flurry_event(final String event_name)
    {
    	FlurryAgent.logEvent(event_name);
    }
    
    
    /**
     * Loads a new ad. Keeps the current ad visible while the next ad loads.
     */
    private static void LoadAmazonAd(final String event_name) {
    	
        if (nextAdView == null) { // Create and configure a new ad if the next ad doesn't currently exist
            nextAdView = new AdLayout((Activity) mContext, AdSize.SIZE_300x250);
            //LayoutParams layoutParams = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT,
            //		LayoutParams.WRAP_CONTENT, Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);
    	    
      
            // Note: The above implementation is for an auto-sized ad in an AdLayout of width MATCH_PARENT and
            // height WRAP_CONTENT. If you instead want to give the ad a fixed size, you will need to factor in
            // the phone's scale when setting up the AdLayout dimensions. See the example below for 320x50 dpi:
            //    nextAdView = new AdLayout(this, AdSize.SIZE_320x50);
                float scale = mContext.getApplicationContext().getResources().getDisplayMetrics().density;
                LayoutParams layoutParams = new FrameLayout.LayoutParams((int) (320 * scale),
            	          (int) (250 * scale), Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);
            
            //nextAdView.setLayoutParams(layoutParams);
            
            ((Activity) mContext).addContentView(nextAdView, layoutParams);
            // Register our ad handler that will receive call-backs for state changes during the ad life cycle        
            //nextAdView.setListener((AdListener) this);
        }
                
        // Load the ad with the appropriate ad options.
        AdTargetingOptions adOptions = new AdTargetingOptions();
        nextAdView.loadAd(adOptions);
    }
}
