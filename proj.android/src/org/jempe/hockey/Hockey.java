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
import android.view.ViewGroup;

import com.amazon.device.ads.Ad;
import com.amazon.device.ads.AdError;
import com.amazon.device.ads.AdLayout;
import com.amazon.device.ads.AdProperties;
import com.amazon.device.ads.AdRegistration;
import com.amazon.device.ads.DefaultAdListener;
import com.amazon.device.ads.InterstitialAd;
import com.flurry.android.FlurryAgent;
import com.revmob.cocos2dx.RevMobWrapper;

public class Hockey extends Cocos2dxActivity {

	private static final String TAG = "Hockey Activity ";

	private ViewGroup adViewContainer; // View group to which the ad view will

	private InterstitialAd interstitialAd;
	// be added
	private static AdLayout nextAdView; // A placeholder for the next ad so we
										// can keep the current ad visible while
										// the next ad loads

	private static Context mContext;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mContext = this;

		RevMobWrapper.setActivity(this);

		// AmazonMobileAds Registration
		AdRegistration.setAppKey(getString(R.string.amazon_app_key));
		AdRegistration.enableTesting(true);
		AdRegistration.enableLogging(true);

		// Create the interstitial.
		this.interstitialAd = new InterstitialAd(this);

		// Set the listener to use the callbacks below.
		this.interstitialAd.setListener(new MyCustomAdListener());

		// Load the interstitial.
		this.interstitialAd.loadAd();
	}

	// AdLister for amazon ads
	class MyCustomAdListener extends DefaultAdListener {
		@Override
		public void onAdLoaded(Ad ad, AdProperties adProperties) {
			if (ad == Hockey.this.interstitialAd) {
				// Show the interstitial ad to the app's user.
				// Note: While this implementation results in the ad being shown
				// immediately after it has finished loading, for smoothest user
				// experience you will generally want the ad already loaded
				// before it’s time to show it. You can thus instead set a flag
				// here to indicate the ad is ready to show and then wait until
				// the best time to display the ad before calling showAd().
				//Hockey.this.interstitialAd.showAd();
				//LoadAmazonAd("test");
				Log.d(TAG, "Amazon Interstitial loaded");
			}
		}

		@Override
		public void onAdFailedToLoad(Ad ad, AdError error) {
			// Call backup ad network.
		}

		@Override
		public void onAdDismissed(Ad ad) {
			// Start the level once the interstitial has disappeared.
			// startNextLevel();
			Hockey.this.interstitialAd.loadAd();
		}
	}

	@Override
	public void onDestroy() {
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
	protected void onStart() {
		super.onStart();

		FlurryAgent.onStartSession(this, getString(R.string.flurry_api_key));
	}

	@Override
	protected void onStop() {
		super.onStop();
		FlurryAgent.onEndSession(this);
	}

	private native void pauseGame();

	static {
		System.loadLibrary("game");
	}

	static void flurry_event(final String event_name) {
		FlurryAgent.logEvent(event_name);
	}
	
	private void showAmazonAd()
	{
		this.interstitialAd.showAd();
	}

	/**
	 * Loads a new ad. Keeps the current ad visible while the next ad loads.
	 */
	private static void LoadAmazonAd(final String event_name) {
		Log.d(TAG, "load Amazon Ad");
		((Activity) mContext).runOnUiThread(new Runnable() {

			public void run() {
				((Hockey) mContext).showAmazonAd();
			}
		});
	}
}
