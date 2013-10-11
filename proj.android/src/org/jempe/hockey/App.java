package org.jempe.hockey;

import me.kiip.sdk.Kiip;
import android.app.Application;
import android.util.Log;

public class App extends Application {
	@Override
	public void onCreate() {
	    super.onCreate();
	    Kiip kiip = Kiip.init(this, getString(R.string.kipp_key), getString(R.string.kiip_secret));
	    Kiip.setInstance(kiip);
	    Log.d("HOCKEY", "Initialize Kiip");
	}

}
