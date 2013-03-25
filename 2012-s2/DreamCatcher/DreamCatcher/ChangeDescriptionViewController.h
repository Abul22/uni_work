//
//  ChangeDescriptionViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 9/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ChangeDescriptionViewController;
@protocol ChangeDescriptionViewControllerDelegate <NSObject>

-(void)changeDescriptionViewController:(ChangeDescriptionViewController *)sender
                 getThenNewDescription:(NSString *)description;

@end
@interface ChangeDescriptionViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextView *descriptionTextView;
@property (strong,nonatomic)NSString *description;
@property(nonatomic,weak) id <ChangeDescriptionViewControllerDelegate>delegate;

@end
