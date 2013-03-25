//
//  ItemManager.h
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <Foundation/Foundation.h>

// the key of item's name descriotion and quantity
#define ITEM_NAME @"name"
#define ITEM_DESCRIOTION @"descriotion"
#define ITEM_QUANTITY @"quantity"

@interface ItemManager : NSObject
// the array to save all the items 
@property (nonatomic,strong)NSMutableArray *itmesArray;
// push a item into the array
-(NSMutableArray *)addNewItemtoItemArray:(NSMutableDictionary *)item;
// the detail of one item
-(NSMutableDictionary * )setItemDetail:(NSString*)name
                                      :(NSString*)descriotion
                                      :(NSNumber*)quantity;
@end
