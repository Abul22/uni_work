//
//  ShoppingListTableViewController.h
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ShoppingListTableViewController : UITableViewController

// the "add" buttion, set this outlet for hide it self, so the pointer will be strong
@property (strong, nonatomic) IBOutlet UIBarButtonItem *addButton;
@end
