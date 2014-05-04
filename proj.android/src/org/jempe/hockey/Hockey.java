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
import android.widget.Toast;

import com.amazon.device.ads.Ad;
import com.amazon.device.ads.AdError;
import com.amazon.device.ads.AdProperties;
import com.amazon.device.ads.AdRegistration;
import com.amazon.device.ads.DefaultAdListener;
import com.amazon.device.ads.InterstitialAd;
import com.amazon.insights.AmazonInsights;
import com.amazon.insights.Event;
import com.amazon.insights.EventClient;
import com.amazon.insights.InsightsCredentials;
import com.revmob.RevMob;
import com.revmob.RevMobTestingMode;

public class Hockey extends Cocos2dxActivity {

	private static final String TAG = "Hockey Activity ";

	private InterstitialAd interstitialAd;
	private RevMob revmob;

	// analytics
	private AmazonInsights insights;

	private static Boolean mTestAds = true;

	private static Context mContext;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mContext = this;

		// Create a credentials object by using the values from the Amazon
		// developer oortal Analytics site.
		// The Identifier's Public Key acts as the application key.
		InsightsCredentials credentials = AmazonInsights.newCredentials(
				getString(R.string.amazoninsights_app_key),
				getString(R.string.amazoninsights_private_key));

		// Initialize a new instance of AmazonInsights specifically for your
		// Android application.
		// The AmazonInsights library requires the Android context in order to
		// access
		// Android services (i.e., SharedPrefs, etc.).
		insights = AmazonInsights.newInstance(credentials,
				getApplicationContext());

		revmob = RevMob.start(this); // RevMob App ID configured in the
										// AndroidManifest.xml file

		// AmazonMobileAds Registration
		AdRegistration.setAppKey(getString(R.string.amazon_app_key));

		// Create the interstitial.
		this.interstitialAd = new InterstitialAd(this);

		// Set the listener to use the callbacks below.
		this.interstitialAd.setListener(new MyCustomAdListener());

		// Load the interstitial.
		this.interstitialAd.loadAd();

		if (mTestAds) {
			// Amazon set Test Mode
			AdRegistration.enableTesting(true);
			AdRegistration.enableLogging(true);

			// Revmob set Test Mode
			revmob.setTestingMode(RevMobTestingMode.WITH_ADS); // with this
																// line, RevMob
																// will always
																// deliver a
																// sample ad
			// revmob.setTestingMode(RevMobTestingMode.WITHOUT_ADS); // with
			// this line, RevMob will not delivery ads

			Toast toast = Toast.makeText(this, "Ads in Testing Mode",
					Toast.LENGTH_LONG);
			toast.show();
		}
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
				// Hockey.this.interstitialAd.showAd();
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
	}

	@Override
	public void onResume() {
		super.onResume();
		revmob = RevMob.start(this);
		this.insights.getSessionClient().resumeSession();
	}

	@Override
	public void onPause() {
		super.onPause();
		// Notify the AmazonInsights SDK that a session pause happened in this
		// Android activity.
		// Be sure to include this in every activity's onPause.
		this.insights.getSessionClient().pauseSession();

		// Call this to submit events to the server.
		// It is recommended to call this in every activity's onPause method.
		this.insights.getEventClient().submitEvents();
	}

	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

	private native void pauseGame();

	static {
		System.loadLibrary("game");
	}

	private void showAmazonAd() {
		this.interstitialAd.showAd();
	}

	private void showRevmobAd() {
		revmob.showFullscreen(this);
	}

	private static void showInterstitial(final String event_name) {
		Log.d(TAG, "show Interstitial");
		((Activity) mContext).runOnUiThread(new Runnable() {

			public void run() {
				// ((Hockey) mContext).showAmazonAd();
				// ((Hockey) mContext).showRevmobAd();
			}
		});
	}

	private void log_analytics_event(String event_name) {
		// Get the event client from insights instance
		EventClient eventClient = insights.getEventClient();

		Event event = eventClient.createEvent(event_name);

		// Record the level completion event.
		eventClient.recordEvent(event);

	}

	private static void analytics_event(final String event_name) {
		Log.d(TAG, "analytics event:" + event_name);
		((Activity) mContext).runOnUiThread(new Runnable() {

			public void run() {
				((Hockey) mContext).log_analytics_event(event_name);
			}
		});
	}
}
