//
//  AlremViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#define iPhone5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) : NO)


@interface AlremViewController : UIViewController

@property (nonatomic, strong) IBOutlet UIDatePicker *datePicker;
- (IBAction)setTheAlremTime:(id)sender;
@property (weak, nonatomic) IBOutlet UISwitch *timeSwitch;
@property (weak, nonatomic) IBOutlet UILabel *timeLable;
@property (weak, nonatomic) IBOutlet UILabel *glowLable;


@end
