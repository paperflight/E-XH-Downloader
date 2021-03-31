//
//  ViewController.m
//  EHentaiDownloader
//
//  Created by 胡峰鹤 on 16/8/12.
//  Copyright © 2016年 HUFH. All rights reserved.
//

#import "ViewControllerLimit.h"

@implementation ViewControllerLimit

- (void)viewDidLoad {
    [super viewDidLoad];
    _homeDirectoryLimit = NSHomeDirectory();
    string command = [self.homeDirectoryLimit cStringUsingEncoding:	NSUTF8StringEncoding];
    documentPathLimit = command + "/Documents/EHD";
    string temp3 = documentPathLimit + "/limit.txt";
    remove(temp3.c_str());
    string LimitPath = documentPathLimit + "/limit.txt";
    remove(LimitPath.c_str());
    if(!GetUrlHomepage(homepagePath, LimitPath)){
        self.limitCount.title = @"GFW!!!";
    }
    string limitGet = getLimit();
    self.limitCount.title = [NSString stringWithUTF8String:limitGet.c_str()];
    // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
