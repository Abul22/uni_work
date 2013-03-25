//
//  NewDreamDetailViewController.h
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
#import "AwesomeMenu/AwesomeMenu.h"
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>
#import "SVProgressHUD/SVProgressHUD.h"
#import "MAConfirmButton/MAConfirmButton.h"
#import <Social/Social.h>
#import "MBProgressHUD.h"
#import <TapkuLibrary/TapkuLibrary.h>
// Judge the iPhone 5
#define iPhone5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) : NO)



@class NewDreamDetailViewController;
@protocol NewDreamDetailViewControllerDelegate <NSObject>
@optional
-(void)NewDreamDetailViewController:(NewDreamDetailViewController *)sender
               getNewDreamDetail:(Dream *)dream
           gotTheIndexOfTheDream:(NSIndexPath*)indexPath;
@end

@interface NewDreamDetailViewController : UITableViewController<MFMailComposeViewControllerDelegate>{
    AVAudioPlayer *player;
}
@property (nonatomic, retain) ZenPlayerButton* zenPlayerButton;
@property (nonatomic, retain) MAConfirmButton *uploadButton;
@property (nonatomic, retain) AwesomeMenu *menu;
@property (strong,nonatomic)Dream *dream;
@property (strong,nonatomic)NSArray *cells;
@property (strong,nonatomic)TKLabelTextFieldCell *NameCell;
@property (strong,nonatomic)TKLabelTextViewCell *descriptionCell;

@property (weak,nonatomic)NSString *type;
@property (weak,nonatomic)NSString *uploadURL;
@property (strong,nonatomic)NSURL *urlOfDream;
@property (nonatomic,strong)NSIndexPath *indexPath;
@property(nonatomic,weak) id <NewDreamDetailViewControllerDelegate>delegate;
- (void) zenPlayerButtonDidTouchUpInside:(id)sender;

@end
