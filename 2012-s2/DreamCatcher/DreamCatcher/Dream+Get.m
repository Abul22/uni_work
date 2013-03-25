//
//  Dream+Get.m
//  DreamCatcher
//
//  Created by Chao Ruan on 29/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "Dream+Get.h"

@implementation Dream (Get)

+(Dream *)dreamWithInfo:(NSMutableDictionary *)dreamInfo
 inManagedObjectContext:(NSManagedObjectContext *)context{
    Dream *dream = nil;
    dream = [NSEntityDescription insertNewObjectForEntityForName:@"Dream" inManagedObjectContext:context];
    
    dream.name = [dreamInfo objectForKey:DREAM_NAME];
    dream.type = [dreamInfo objectForKey:DREAM_TYPE];
    dream.descriotion = [dreamInfo objectForKey:DREAM_DESCRIOTION];
    dream.url = [dreamInfo objectForKey:DREAM_URL];
    dream.time = [dreamInfo objectForKey:DREAM_TIME];
    dream.uploadURL = [dreamInfo objectForKey:DREAM_UPLOADURL];
    return dream;
}

@end
