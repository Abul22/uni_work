//
//  ItemManager.m
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "ItemManager.h"
@interface ItemManager()
@end

@implementation ItemManager

@synthesize itmesArray = _itmesArray;

// set the items array
-(NSMutableArray*)itmesArray{
    if(_itmesArray ==nil)
        _itmesArray = [[NSMutableArray alloc]init];
    return _itmesArray;
}


-(NSMutableArray *)addNewItemtoItemArray:(NSMutableDictionary *)item{
    [self.itmesArray addObject:item];
    return self.itmesArray;
}

-(NSMutableDictionary *)setItemDetail:(NSString *)name :(NSString *)descriotion :(NSNumber *)quantity{
    NSMutableDictionary *item = [[NSMutableDictionary alloc]init];
    //if name descripotion and quantity are nil set the defalut value
    if(!name) name = @"";
    if(!descriotion) descriotion = @"";
    if(!quantity) quantity = [NSNumber numberWithInt:0];

    // set the detail fo an item
    [item setObject:name forKey:ITEM_NAME];
    [item setObject:descriotion forKey:ITEM_DESCRIOTION];
    [item setObject:quantity forKey:ITEM_QUANTITY];
    return item;
}

@end
