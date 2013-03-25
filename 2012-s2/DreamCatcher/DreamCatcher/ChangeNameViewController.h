//
//  ChangeNameViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 9/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ChangeNameViewController;

@protocol ChangeNameViewControllerDelgate <NSObject>

-(void)changeNameViewController:(ChangeNameViewController *)sender
                  getTheNewName:(NSString *)name;

@end

@interface ChangeNameViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextField *nameTestField;
@property (strong,nonatomic)NSString *name;
@property(nonatomic,weak) id <ChangeNameViewControllerDelgate>delegate;

@end
