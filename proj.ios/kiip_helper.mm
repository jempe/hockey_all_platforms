//
//  flurry_helper.m
//  Hockey
//
//  Created by user9227 on 9/27/13.
//
//

#import "kiip_helper.h"
#import <KiipSDK/KiipSDK.h>

inline NSString* CStrToNSString(const char* string) {
    return [NSString stringWithCString:string encoding:NSUTF8StringEncoding];
}

void kiip_helper::saveMoment(const char* moment_name){
    [[Kiip sharedInstance] saveMoment:CStrToNSString(moment_name) withCompletionHandler:nil];

}
