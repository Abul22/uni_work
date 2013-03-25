//
//  DreamManager.h
//  DreamCatcher
//
//  Created by Chao Ruan on 29/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <Foundation/Foundation.h>
#define DREAM_NAME @"name"
#define DREAM_DESCRIOTION @"descriotion"
#define DREAM_TYPE @"type"
#define DREAM_URL @"url"

@interface DreamManager : NSObject
@property (nonatomic,strong)NSMutableArray *dreamsArray;
-(NSMutableArray *)addNewDreamtoItemArray:(NSMutableDictionary *)dream;
-(NSMutableDictionary *)setDreamDetail:(NSString*)name
                                      :(NSString*)descriotion
                                      :(NSString*)type
                                      :(NSString*)url;


@end
