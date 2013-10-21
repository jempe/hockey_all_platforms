#import "game_center.h"
#import "RootViewController.h"

inline NSString* CStrToNSString(const char* string) 
{
    return [NSString stringWithCString:string encoding:NSUTF8StringEncoding];
}

void game_center::showLeaderBoard(const char* leaderboard)
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"showLeaderBoard" object:nil userInfo:[NSDictionary dictionaryWithObject:CStrToNSString(leaderboard) forKey:@"leaderboard"]];
}

void game_center::saveScore(const char* leaderboard, long int score)
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"saveScore" object:nil userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
                                                                                                 CStrToNSString(leaderboard), @"leaderboard",
                                                                                                 [NSNumber numberWithInt:score], @"score",
                                                                                                 nil
                                                                                                 ]];
}
