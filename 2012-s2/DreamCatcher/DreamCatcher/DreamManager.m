//
//  DreamManager.m
//  DreamCatcher
//
//  Created by Chao Ruan on 29/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "DreamManager.h"
@interface DreamManager()
@end

@implementation DreamManager
@synthesize dreamsArray = _dreamsArray;

-(NSMutableArray *)dreamsArray{
    if (_dreamsArray == nil) 
        _dreamsArray = [[NSMutableArray alloc]init];
    
    return _dreamsArray;
    
}


-(NSMutableArray *)addNewDreamtoItemArray:(NSMutableDictionary *)dream{
    [self.dreamsArray addObject:dream];
    return self.dreamsArray;
    
}

-(NSMutableDictionary *)setDreamDetail:(NSString*)name
                                      :(NSString*)descriotion
                                      :(NSString*)type
                                      :(NSString*)url{
    NSMutableDictionary *dream = [[NSMutableDictionary alloc]init];
    if(!name) name =@"";
    if(!descriotion) descriotion =@"";
    if(!type) type =@"";
    if(!url) url =@"";
    [dream setObject:name forKey:DREAM_NAME];
    [dream setObject:type forKey:DREAM_TYPE];
    [dream setObject:descriotion forKey:DREAM_DESCRIOTION];
    [dream setObject:url forKey:DREAM_URL];
    return dream;
    
}

@end
