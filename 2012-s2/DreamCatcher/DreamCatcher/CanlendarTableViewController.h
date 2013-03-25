//
//  CanlendarTableViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 16/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "Dream+Get.h"
#import "DreamTableViewController.h"
#import <TapkuLibrary/TapkuLibrary.h>
#import "NewDreamDetailViewController.h"



@interface CanlendarTableViewController : TKCalendarMonthTableViewController<NewDreamDetailViewControllerDelegate>
@property (nonatomic,strong) UIManagedDocument *dreamDatabase;
@property (nonatomic,strong)NSArray *dreamArray;
@property (nonatomic,strong)NSMutableDictionary *dataDictionary;
@property (nonatomic,strong)NSMutableArray *dataArray;


@end
