//
//  Dream.h
//  DreamCatcher
//
//  Created by Chao Ruan on 15/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface Dream : NSManagedObject

@property (nonatomic, retain) NSString * descriotion;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSDate * time;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSString * url;
@property (nonatomic, retain) NSString * uploadURL;

@end
