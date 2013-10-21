/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 Copyright (c) 2010      Ricardo Quesada
 
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

#import "RootViewController.h"


@implementation RootViewController

@synthesize player;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    //[super viewDidLoad];
    [self enableGameCenter:NO];
}
 

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}

// For ios6.0 and higher, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations
{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


// Disable GameCenter options from view 
- (void)enableGameCenter:(BOOL)enableGameCenter 
{
	if(enableGameCenter)
	{
		NSLog(@"enable game center");
	}
	else
	{
		NSLog(@"disable game center");
	}
    //[showLeaderboardButton setEnabled:enableGameCenter];
}


// Example of how to bring up a specific leaderboard 
- (void)showLeaderboard:(NSNotification *)notification 
{
    NSString * leaderboard = [notification.userInfo valueForKey:@"leaderboard"];

    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil)
    {
       gameCenterController.gameCenterDelegate = self;
       gameCenterController.viewState = GKGameCenterViewControllerStateLeaderboards;
       gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeToday;
       gameCenterController.leaderboardCategory = leaderboard;
       [self presentViewController: gameCenterController animated: YES completion:nil];
    }
}

- (void)saveScore:(NSNotification *)notification
{   
    NSString * leaderboard = [notification.userInfo valueForKey:@"leaderboard"];
    
    NSDictionary* userInfo = notification.userInfo;
    
    int64_t score = [[userInfo objectForKey:@"score"] intValue];
    
    // Check if the device is running iOS 7.0 or later.
    NSString *reqSysVer = @"7.0";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL isIOS7 = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    GKScore * submitScore;
    
    if(isIOS7)
    {
    	submitScore = [[GKScore alloc] initWithLeaderboardIdentifier:leaderboard];
        NSLog(@"Submit score iOS7");
    }
    else
    {
        submitScore = [[GKScore alloc] initWithCategory:leaderboard];
        NSLog(@"Submit score iOS6");
    }
    [submitScore setValue:score]; 
    
    // New feature in iOS5 tells GameCenter which leaderboard is the default per user.
    // This can be used to show a user's favorite course/track associated leaderboard, or just show the latest score submitted.
    [submitScore setShouldSetDefaultLeaderboard:YES];
    
    // New feature in iOS5 allows you to set the context to which the score was sent. For instance this will set the context to be 
    //the count of the button press per run time. Information stored in context isn't accessable in standard GKLeaderboardViewController,
    //instead it's accessable from GKLeaderboard's loadScoresWithCompletionHandler:
    //[submitScore setContext:context++];
    
    [self.player submitScore:submitScore];
    [submitScore release];
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
}


- (void)dealloc {
    [player release];
    [super dealloc];
}


@end
