//
//  ViewController.h
//  EHentaiDownloader
//
//  Created by 胡峰鹤 on 16/8/12.
//  Copyright © 2016年 HUFH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <IOKit/pwr_mgt/IOPMLib.h>
#import "EHD.h"

@interface ViewController : NSViewController
@property (unsafe_unretained) IBOutlet NSTextFieldCell *titleEG;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *titleJP;
@property (unsafe_unretained) IBOutlet NSTextView *messOut;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *urlInput;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *usrNameIn;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *usrPasswIn;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *pageCount;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *ImageStartDownload;
@property (unsafe_unretained) IBOutlet NSTextFieldCell *totalPageUrl;
@property (strong) IBOutlet NSButtonCell *downloadSymbol;
@property (strong) IBOutlet NSButtonCell *SettingDownloadOrder;
- (IBAction)ChangeDownloadOrder:(id)sender;
@property (strong) IBOutlet NSTextFieldCell *date;
@property (strong) NSColor *signalDownload;
@property (nonatomic, strong) NSTimer *Timer;
@property (nonatomic, strong) NSString *homeDirectory;
- (IBAction)Login:(id)sender;
- (IBAction)Adder:(id)sender;
- (IBAction)DownLoadstart:(id)sender;
- (IBAction)cleanWindow:(id)sender;
- (IBAction)cleanUrl:(id)sender;
- (IBAction)SetStartImageNum:(id)sender;
- (IBAction)AddWholePage:(id)sender;
@end
