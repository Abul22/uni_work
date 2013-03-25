//
//  DreamTableViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "Dream+Get.h"
#import "NewDreamDetailViewController.h"
#import "CoreDataTableViewController.h"

@interface DreamTableViewController : CoreDataTableViewController<NSFetchedResultsControllerDelegate,NewDreamDetailViewControllerDelegate>{
    NSFileManager *manager;
}

@property (nonatomic,strong) UIManagedDocument *dreamDatabase;
@end
