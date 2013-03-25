//
//  Dream+Get.h
//  DreamCatcher
//
//  Created by Chao Ruan on 29/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "Dream.h"

#define DREAM_NAME @"name"
#define DREAM_DESCRIOTION @"descriotion"
#define DREAM_TYPE @"type"
#define DREAM_URL @"url"
#define DREAM_TIME @"time"
#define DREAM_UPLOADURL @"uploadURL"




@interface Dream (Get)
+(Dream *)dreamWithInfo:(NSMutableDictionary *)dreamInfo
 inManagedObjectContext:(NSManagedObjectContext *)context;


@end
