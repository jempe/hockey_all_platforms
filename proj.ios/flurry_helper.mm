//
//  flurry_helper.m
//  Hockey
//
//  Created by user9227 on 9/27/13.
//
//

#import "flurry_helper.h"
#import "Flurry.h"

inline NSString* CStrToNSString(const char* string) {
    return [NSString stringWithCString:string encoding:NSUTF8StringEncoding];
}

void flurry_helper::logEvent(const char* event_name){
    [Flurry logEvent:CStrToNSString(event_name)];
}