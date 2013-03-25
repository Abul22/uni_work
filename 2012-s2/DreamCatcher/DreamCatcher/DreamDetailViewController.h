//
//  DreamDetailViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 1/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "SCUI.h"
#import "Dream+Get.h"
#import "ZenPlayerButton/ZenPlayerButton.h"
#import "SVSegmentedControl/SVSegmentedControl.h"
#import "ChangeDescriptionViewController.h"
#import "ChangeNameViewController.h"
#import "AwesomeMenu/AwesomeMenu.h"
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>
#import "SVProgressHUD/SVProgressHUD.h"
#import "MAConfirmButton/MAConfirmButton.h"
#import <Social/Social.h>
#import "MBProgressHUD.h"


@class DreamDetailViewController;

@protocol DreamDetailViewControllerDelegate <NSObject>

-(void)dreamDetailViewController:(DreamDetailViewController *)sender
               getNewDreamDetail:(Dream *)dream
            gotTheIndexOfTheDream:(NSIndexPath*)indexPath;

@end

@interface DreamDetailViewController : UIViewController<MFMailComposeViewControllerDelegate>{
    AVAudioPlayer *player;
}
@property (nonatomic, retain) ZenPlayerButton* zenPlayerButton;
@property (nonatomic, retain) MAConfirmButton *uploadButton;
@property (nonatomic, retain) AwesomeMenu *menu;
@property (weak, nonatomic) IBOutlet UILabel *nameLable;
@property (weak, nonatomic) IBOutlet UILabel *descriptionLable;
@property (weak, nonatomic) IBOutlet UILabel *timeLable;
@property (strong,nonatomic)Dream *dream;
@property (weak,nonatomic)NSString *type;
@property (weak,nonatomic)NSString *uploadURL;
@property (strong,nonatomic)NSURL *urlOfDream;
@property (nonatomic,strong)NSIndexPath *indexPath;
@property(nonatomic,weak) id <DreamDetailViewControllerDelegate>delegate;
- (void) zenPlayerButtonDidTouchUpInside:(id)sender;

@end
