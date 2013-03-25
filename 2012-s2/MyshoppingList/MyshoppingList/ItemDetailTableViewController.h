//
//  ItemDetailTableViewController.h
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ItemDetailTableViewController;

@protocol ItemDetailTableViewControllerDelegate <NSObject>

-(void)itemDetailTableViewController:(ItemDetailTableViewController*)sender
                 gotTheNewItemDetail:(NSMutableDictionary*)item
                gotTheIndexOfTheItem:(NSIndexPath*)indexPath;

@end

@interface ItemDetailTableViewController : UITableViewController
// the properties in the item detail
@property (weak, nonatomic) IBOutlet UITextField *nameTextField;
@property (weak, nonatomic) IBOutlet UILabel *quantityLable;
@property (weak, nonatomic) IBOutlet UITextField *descriptionTextField;
// the action to hide keybord and change the value buy the stepper
- (IBAction)TexFieldHideKeybord:(id)sender;
- (IBAction)changeQuantity:(UIStepper*)sender;
// the properties need to delegate
@property (nonatomic,strong)NSMutableDictionary *item;
@property (nonatomic,strong)NSString *name;
@property (nonatomic,strong)NSString *description;
@property (nonatomic,strong)NSNumber *quantity;
@property (nonatomic,strong)NSIndexPath *indexPath;
@property(nonatomic,weak) id <ItemDetailTableViewControllerDelegate>delegate;
@end
