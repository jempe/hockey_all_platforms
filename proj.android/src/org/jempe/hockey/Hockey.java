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

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Random;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.AbstractHttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.text.format.Time;
import android.util.Log;
import android.view.View;
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
import com.chartboost.sdk.CBPreferences;
import com.chartboost.sdk.Chartboost;
import com.revmob.RevMob;
import com.revmob.RevMobTestingMode;

@SuppressLint("NewApi")
public class Hockey extends Cocos2dxActivity {

	private static final String TAG = "Hockey Activity ";
	private static final String PREFS_NAME = "JempeHockeyPrefs";

	private InterstitialAd interstitialAd;
	private RevMob revmob;
	private Chartboost cb;

	// analytics
	private AmazonInsights insights;

	private static Boolean mTestAds = true;

	private static Context mContext;

	private static SharedPreferences mSettings;

	private static final String CHARTBOOST = "chartboost";
	private static final String AMAZON = "amazon";
	private static final String REVMOB = "revmob";

	private static String defaultAdsDistribution = AMAZON + "," + REVMOB + ","
			+ CHARTBOOST;
	private static final String adsDistributionSettings = "ads_distribution";

	private static final String adsDistributionBaseURL = "http://webservices.jempe.org/ads_distribution/";
	private static final String adsDistributionParams = "org.jempe.hockey/slideme.json";
	private static final String HOST = "webservices.jempe.org";

	private static String AdsDistribution = "";
	private static String[] AdsDistributionList = null;
	
	private static final String EVENTSUFFIX = " Slide Me";

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		hideSystemUI();

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

		// Revmob registration
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

		// Configure Chartboost
		this.cb = Chartboost.sharedChartboost();
		String appId = getString(R.string.chartboost_android_app_id);
		String appSignature = getString(R.string.chartboost_android_signature);
		this.cb.onCreate(this, appId, appSignature, null);
		CBPreferences.getInstance().setImpressionsUseActivities(true);

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

		mSettings = getSharedPreferences(PREFS_NAME, 0);
		processAdsDistribution();

		if (isConnected()) {
			new WebServiceTask(this).execute();
		}

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			// check window changes for immersive mode
			View decorView = getWindow().getDecorView();
			decorView
					.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener() {
						@Override
						public void onSystemUiVisibilityChange(int visibility) {

							Log.d(TAG, "window change");

							// Note that system bars will only be "visible" if
							// none of the
							// LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags
							// are set.
							if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
								// TODO: The system bars are visible. Make any
								// desired
								// adjustments to your UI, such as showing the
								// action bar or
								// other navigational controls.
								hideSystemUI();
							} else {
								// TODO: The system bars are NOT visible. Make
								// any desired
								// adjustments to your UI, such as hiding the
								// action bar or
								// other navigational controls.
								Log.d(TAG, "window items hidden");
							}
						}
					});
		}
	}

	private void hideSystemUI() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			Log.i(TAG, "Immersive mode.");

			int uiOptions = this.getWindow().getDecorView()
					.getSystemUiVisibility();

			boolean isImmersiveModeEnabled = ((uiOptions | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY) == uiOptions);

			if (!isImmersiveModeEnabled) {

				Log.d(TAG, "Enabling Immersive mode");
			} else {
				Log.d(TAG, "Immersive mode already enabled");
			}

			this.getWindow()
					.getDecorView()
					.setSystemUiVisibility(
							View.SYSTEM_UI_FLAG_LAYOUT_STABLE
									| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
									| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
									| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
									| View.SYSTEM_UI_FLAG_FULLSCREEN
									| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
		}
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) {
			Log.d(TAG, "get focus");
			hideSystemUI();
		}
	}

	private static void immersiveMode() {
		((Activity) mContext).runOnUiThread(new Runnable() {

			public void run() {
				((Hockey) mContext).hideSystemUI();
			}
		});
	}

	private void processAdsDistribution() {
		AdsDistribution = mSettings.getString(adsDistributionSettings,
				defaultAdsDistribution);
		AdsDistributionList = AdsDistribution.split(",");

		Log.d(TAG, "ads distribution: " + AdsDistribution);
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

		this.cb.onDestroy(this);
	}

	@Override
	public void onResume() {
		super.onResume();
		hideSystemUI();
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

		this.cb.onStart(this);

		// Notify the beginning of a user session. Must not be dependent on user
		// actions or any prior network requests.
		this.cb.startSession();
	}

	@Override
	protected void onStop() {
		super.onStop();

		this.cb.onStop(this);
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

	private void showChartboostAd() {
		// Show an interstitial
		this.cb.showInterstitial();
	}

	private static void showInterstitial(final String event_name) {
		Log.d(TAG, "show Interstitial");

		((Activity) mContext).runOnUiThread(new Runnable() {

			public void run() {

				Time t = new Time();
				t.setToNow();

				Random r = new Random(t.toMillis(false));

				int selectedNumber = r.nextInt(AdsDistributionList.length);

				String selectedNetwork = AdsDistributionList[selectedNumber];

				if (selectedNetwork.equals(AMAZON)) {
					Log.d(TAG, "show Amazon Interstitial");
					((Hockey) mContext).showAmazonAd();
				} else if (selectedNetwork.equals(CHARTBOOST)) {
					Log.d(TAG, "show Chartboost Interstitial");
					((Hockey) mContext).showChartboostAd();
				} else {
					Log.d(TAG, "show Revmob Interstitial");
					((Hockey) mContext).showRevmobAd();

				}
			}
		});
	}

	private Boolean isConnected() {

		ConnectivityManager connMgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeInfo = connMgr.getActiveNetworkInfo();
		if (activeInfo != null && activeInfo.isConnected()) {
			Boolean wifiConnected = activeInfo.getType() == ConnectivityManager.TYPE_WIFI;
			Boolean mobileConnected = activeInfo.getType() == ConnectivityManager.TYPE_MOBILE;
			if (wifiConnected) {
				return true;
			} else if (mobileConnected) {
				return true;
			}
		} else {
			return false;
		}
		return false;
	}

	private class WebServiceTask extends AsyncTask {
		private Context mContext;

		public WebServiceTask(Context context) {
			mContext = context;
		}

		protected void onPostExecute() {

		}

		@Override
		public Object doInBackground(Object... params) {
			try {

				StringBuilder builder = new StringBuilder();
				HttpClient client = new DefaultHttpClient();

				((AbstractHttpClient) client).getCredentialsProvider()
						.setCredentials(
								new AuthScope(HOST, AuthScope.ANY_PORT),
								new UsernamePasswordCredentials(
										getString(R.string.jempe_username),
										getString(R.string.jempe_password)));

				HttpGet httpGet = new HttpGet(adsDistributionBaseURL
						+ adsDistributionParams);

				HttpResponse response = client.execute(httpGet);
				StatusLine statusLine = response.getStatusLine();
				int statusCode = statusLine.getStatusCode();

				if (statusCode == 200) {
					HttpEntity entity = response.getEntity();
					InputStream content = entity.getContent();
					BufferedReader reader = new BufferedReader(
							new InputStreamReader(content));
					String line;
					while ((line = reader.readLine()) != null) {
						builder.append(line);
					}
				} else {
					Log.e(TAG, "Failed to contact web service");
				}

				String webServiceResponse = builder.toString();

				JSONObject jsonObject = new JSONObject(webServiceResponse);

				if (jsonObject.getInt("success") == 1) {
					int amazonWeight = jsonObject.getInt(AMAZON);
					int revmobWeight = jsonObject.getInt(REVMOB);
					int chartboostWeight = jsonObject.getInt(CHARTBOOST);

					if ((amazonWeight + revmobWeight + chartboostWeight) > 0) {
						String newAdsDistribution = "";

						for (int i = 0; i < amazonWeight; i++) {
							if (newAdsDistribution != "") {
								newAdsDistribution += ",";
							}

							newAdsDistribution += AMAZON;
						}

						for (int i = 0; i < chartboostWeight; i++) {
							if (newAdsDistribution != "") {
								newAdsDistribution += ",";
							}

							newAdsDistribution += CHARTBOOST;
						}

						for (int i = 0; i < revmobWeight; i++) {
							if (newAdsDistribution != "") {
								newAdsDistribution += ",";
							}

							newAdsDistribution += REVMOB;
						}

						SharedPreferences.Editor editor = mSettings.edit();
						editor.putString(adsDistributionSettings,
								newAdsDistribution);
						editor.commit();

						processAdsDistribution();
					} else {
						Log.e(TAG, "ads distribution must not be empty");
					}
				} else {
					Log.e(TAG, "web service status issue");
				}

				Log.d(TAG, "webService response: " + webServiceResponse);

			} catch (Exception ex) {
				Log.e(TAG, "Web Service Error" + ex);
			}
			return null;
		}
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
				((Hockey) mContext).log_analytics_event(event_name + EVENTSUFFIX);
			}
		});
	}
}
