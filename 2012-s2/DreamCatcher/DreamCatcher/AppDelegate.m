//
//  AppDelegate.m
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "AppDelegate.h"
#import "SCUI.h"
@implementation AppDelegate

@synthesize dreamDatabase = _dreamDatabase;
// init the sound cloud id
+ (void)initialize;
{
    [SCSoundCloud  setClientID:@"ed1fd95913db40b1c2e0b694161e0840"
                        secret:@"ff0e7ba8b3c36067149d88d04b72fb05"
                   redirectURL:[NSURL URLWithString:@"dreamcatcher://oauth2"]];
}

- (void)setDreamDatabase:(UIManagedDocument *)dreamDatabase{
    // set the dream database 
    if (_dreamDatabase != dreamDatabase) {
        _dreamDatabase = dreamDatabase;
    }
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // create the database if is not there 
    if (!self.dreamDatabase) {
        NSURL *url = [[[NSFileManager defaultManager]URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask]lastObject];
        url = [url URLByAppendingPathComponent:@"Default Dream database"];
        self.dreamDatabase = [[UIManagedDocument alloc]initWithFileURL:url];
    }
    
    return YES;
}
// allow the controll view and use the database
-(BOOL)useDocumnet{
    
    __block BOOL finalSuccess = NO;
    
    if (![[NSFileManager defaultManager]fileExistsAtPath:[self.dreamDatabase.fileURL path]]) {
        [self.dreamDatabase saveToURL:self.dreamDatabase.fileURL forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success){
            finalSuccess = success;
        }];

    }else if(self.dreamDatabase.documentState == UIDocumentStateClosed){
        [self.dreamDatabase openWithCompletionHandler:^(BOOL success){
            finalSuccess = success;
        }];
    }else if (self.dreamDatabase.documentState == UIDocumentStateNormal){
        finalSuccess = YES;
    }
    return finalSuccess;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.


}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
