//
//  RecorderViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 27/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "ZenPlayerButton/ZenPlayerButton.h"
#import <AudioToolbox/AudioToolbox.h>
#import "HZActivityIndicator/HZActivityIndicatorView.h"
#import "SVProgressHUD/SVProgressHUD.h"
#import "AppDelegate.h"
#import "Dream+Get.h"
#import <TapkuLibrary/TapkuLibrary.h>
#define iPhone5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) : NO)


@interface RecorderViewController : UIViewController<AVAudioPlayerDelegate,AVAudioRecorderDelegate>
{
    NSFileManager *manager;
    AVAudioPlayer *player;
    AVAudioRecorder *recorder;
    SystemSoundID soundID;
    HZActivityIndicatorView *activityIndicator;
    NSURL *dreamFileURL;
    NSURL *tempFileURL;

}
@property (nonatomic,strong) UIManagedDocument *dreamDatabase;

@property (nonatomic, retain) ZenPlayerButton* zenPlayerButton;
@property (weak, nonatomic) IBOutlet UIButton *recordButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *saveButton;
- (IBAction)recording:(id)sender;
- (IBAction)saveDream:(id)sender;
- (void) zenPlayerButtonDidTouchUpInside:(id)sender;


@end
