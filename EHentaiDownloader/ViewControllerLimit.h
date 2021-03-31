//
//  ViewController.h
//  EHentaiDownloader
//
//  Created by 胡峰鹤 on 16/8/12.
//  Copyright © 2016年 HUFH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EHDLimit.h"

@interface ViewControllerLimit : NSViewController
@property (unsafe_unretained) IBOutlet NSTextFieldCell *limitCount;
@property (nonatomic, strong) NSString *homeDirectoryLimit;
@end