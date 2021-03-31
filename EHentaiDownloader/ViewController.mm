//
//  ViewController.m
//  EHentaiDownloader
//
//  Created by 胡峰鹤 on 16/8/12.
//  Copyright © 2016年 HUFH. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupCounter];
    
    _homeDirectory = NSHomeDirectory();
    string command = [self.homeDirectory cStringUsingEncoding:	NSUTF8StringEncoding];
    
    if (whetherDownloadByOrder){
        string temp = "下载方式：按序号下载";
        self.SettingDownloadOrder.title = [NSString stringWithUTF8String:temp.c_str()];
    }
    else {
        string temp = "下载方式：按源文件名下载";
        self.SettingDownloadOrder.title = [NSString stringWithUTF8String:temp.c_str()];
    }
    
//    documentPath = GetProgramDir() + "/EHentaiDownloader.app/Contents/Resources/Caches";
//    messOutputText << documentPath << endl;
    //resetNumofImageHaveDown();
    picturePath = command + "/Pictures";
    documentPath = command + "/Documents/EHD";
//    documentPath = command + "/Documents/EHD";
    command = "mkdir "+ documentPath;
    system(command.c_str());
    
    string temp0 = documentPath + "/out.txt";
    remove(temp0.c_str());
    string temp1 = documentPath + "/out2.txt";
    remove(temp1.c_str());
    string temp2 = documentPath + "/out3.txt";
    remove(temp2.c_str());
    string temp3 = documentPath + "/limit.txt";
    remove(temp3.c_str());
    //messOutputText << command << endl;
    // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    self.messOut.string = [NSString stringWithUTF8String:messOutputText.str().c_str()];
    // Update the view, if already loaded.
}

- (void)setupCounter{
    self.Timer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(count:) userInfo:nil repeats:YES];
}

NSThread *thread_d;
string JPName;
string EGName;
- (void)count:(NSTimer *)Timer{
    NSDate *now = [NSDate date];
    NSDateFormatter *df = [[NSDateFormatter alloc] init];
    df.timeZone = [NSTimeZone systemTimeZone];
    df.dateFormat = @"yyyy-MM-dd HH:mm:ss";
    self.date.title =  [df stringFromDate:now];
    //self.date.title = [[NSDate date] description];
    self.messOut.string = [self.messOut.string stringByAppendingString:[NSString stringWithUTF8String:messOutputText.str().c_str()]];
    messOutputText.str("");
    if (thread_d != nil) {
        string temp = "正在下载";
        self.downloadSymbol.title = [NSString stringWithUTF8String:temp.c_str()];
    }
    else {
        string temp = "下载";
        self.downloadSymbol.title = [NSString stringWithUTF8String:temp.c_str()];
    }
    self.titleEG.title = [NSString stringWithUTF8String:EGName.c_str()];
    self.titleJP.title = [NSString stringWithUTF8String:JPName.c_str()];
}

- (IBAction)Login:(id)sender {
    userName = [self.usrNameIn.title cStringUsingEncoding:	NSUTF8StringEncoding];
    userPass = [self.usrPasswIn.title cStringUsingEncoding:	NSUTF8StringEncoding];
    findCookie();
    self.usrPasswIn.title = @"";
    self.usrNameIn.title = @"";
}

stack<string> Url;
stack<string> pageStr;

-(void)DownLoading{
    //CFStringRef reasonForActivity= CFSTR("EHentai is Downloading");
    
    //IOPMAssertionID assertionID;
    //IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoIdleSleep,
                                                   //kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    //组织系统休眠的初始化程序
    //if (success == kIOReturnSuccess)
    //{
        //  Add the work you need to do without
        //  the system sleeping here.
    
//    string url = [self.urlInput.title cStringUsingEncoding:	NSUTF8StringEncoding];
//    string pageString = [self.pageCount.title cStringUsingEncoding:	NSUTF8StringEncoding];
        while (!Url.empty()) {
            
            string temp1 = documentPath + "/out2.txt";
            remove(temp1.c_str());
            string temp2 = documentPath + "/out3.txt";
            remove(temp2.c_str());
            
            fp = NULL;
            file = "";
            totalUrl = "";
            filePath = "";
            pictureFilePath = "";
            identNumber = 0;
            retry = 0;
            
            string url = Url.top();
            string pageString = pageStr.top();
            Url.pop();
            pageStr.pop();
            
            //messOutputText << documentPath << endl;
            //messOutputText << picturePath << endl;
            identNumber = 0;
            bool urlGetIden = GetUrl(url, documentPath + "/out2.txt");
            identNumber = 1;
            pair<string,string> nameTotal = analysisTitle();
            
            int page = 0;
            if (pageString != "") {
                page = atoi(pageString.c_str());
            }
            else {
                page = analysisPage();
            }
            //cout<<page;
            //messOutputText << totalUrl << endl;
            
            if (nameTotal.first == "" && nameTotal.second == ""){
                messOutputText << "路径错误" << endl;
                messOutputText << "完成" << endl;
                resetNumofImageHaveDown();
                continue;
            }
            EGName = nameTotal.first;
            JPName = nameTotal.second;
            
            readUrl(urlGetIden);
            totalUrl.clear();
            for (int i = 1; i < page; i++) {
                identNumber = 0;
                //remove("/Users/HuFH/Documents/Testbench/Testbench/out2.txt");
                string temp1 = documentPath + "/out2.txt";
                remove(temp1.c_str());
                string temp2 = documentPath + "/out3.txt";
                remove(temp2.c_str());
                string nextUrl = "";
                if (url[url.size() - 1] >= '0' && url[url.size() - 1] <= '9'){
                    nextUrl = getStartPage(url, i);
                }
                else{
                    stringstream temp;
                    temp << i;
                    string num;
                    temp >> num;
                    nextUrl = url + "?p=" + num;
                }
                messOutputText << nextUrl << endl;
                messOutputText << "nextPage" << endl;
                bool urlGetIden = GetUrl(nextUrl, documentPath + "/out2.txt");
                identNumber = 1;
                readUrl(urlGetIden);
                totalUrl.clear();
            }
            messOutputText << "完成" << endl;
            resetNumofImageHaveDown();
            pictureFilePath = "";
        }
        [thread_d cancel];
        thread_d = nil;
        
        //success = IOPMAssertionRelease(assertionID);
        //  The system will be able to sleep again.
    //}
}

- (IBAction)AddWholePage:(id)sender {
    string temp1 = documentPath + "/out4.txt";
    remove(temp1.c_str());
    
    string url = [self.totalPageUrl.title cStringUsingEncoding:NSUTF8StringEncoding];
    bool urlGetIden = GetUrl(url, documentPath + "/out4.txt");
    if (urlGetIden != false) {
        vector<string> read = analysisTotalPage();
        for (int i = 0; i < read.size(); i++) {
            Url.push(read[i]);
            pageStr.push("");
        }
    }
    remove(temp1.c_str());
    self.totalPageUrl.title = @"";
}

- (IBAction)DownLoadstart:(id)sender {
    if (thread_d != nil) {
        [thread_d cancel];
        thread_d = nil;
        string temp = "下载";
        
        resetNumofImageHaveDown();
        self.downloadSymbol.title = [NSString stringWithUTF8String:temp.c_str()];
        messOutputText.str("");
        self.messOut.string = @"";
        string temp0 = documentPath + "/out.txt";
        remove(temp0.c_str());
        string temp1 = documentPath + "/out2.txt";
        remove(temp1.c_str());
        string temp2 = documentPath + "/out3.txt";
        remove(temp2.c_str());
        string temp3 = documentPath + "/limit.txt";
        remove(temp3.c_str());
        _homeDirectory = NSHomeDirectory();
        
        fp = NULL;
        file = "";
        totalUrl = "";
        filePath = "";
        identNumber = 0;
        retry = 0;
    }
    else{
        //resetNumofImageHaveDown();
        
        string temp1 = documentPath + "/out2.txt";
        remove(temp1.c_str());
        string temp2 = documentPath + "/out3.txt";
        remove(temp2.c_str());
        _homeDirectory = NSHomeDirectory();

        fp = NULL;
        file = "";
        totalUrl = "";
        filePath = "";
        identNumber = 0;
        retry = 0;
        
        messOutputText.str("");
        self.messOut.string = @"";
        
        string url = [self.urlInput.title cStringUsingEncoding:	NSUTF8StringEncoding];
        string pageString = [self.pageCount.title cStringUsingEncoding:	NSUTF8StringEncoding];
        if (url != ""){
            Url.push(url);
            pageStr.push(pageString);
            self.urlInput.title = @"";
            self.pageCount.title = @"";
        }
        thread_d=[[NSThread alloc]initWithTarget:self selector:@selector(DownLoading) object:nil];
        [thread_d start];
    }
}

- (IBAction)Adder:(id)sender {
    string url = [self.urlInput.title cStringUsingEncoding:	NSUTF8StringEncoding];
    string pageString = [self.pageCount.title cStringUsingEncoding:	NSUTF8StringEncoding];
    if (url != ""){
        Url.push(url);
        pageStr.push(pageString);
        self.urlInput.title = @"";
        self.pageCount.title = @"";
    }
}

- (IBAction)cleanWindow:(id)sender {
    messOutputText.str("");
    self.messOut.string = @"";
}

- (IBAction)cleanUrl:(id)sender {
    self.urlInput.title = @"";
    self.pageCount.title = @"";
}

- (IBAction)ChangeDownloadOrder:(id)sender {
    if(changeDownloadBehave()){
        string temp = "下载方式：按序号下载";
        self.SettingDownloadOrder.title = [NSString stringWithUTF8String:temp.c_str()];
    }
    else {
        string temp = "下载方式：按源文件名下载";
        self.SettingDownloadOrder.title = [NSString stringWithUTF8String:temp.c_str()];
    }
}

- (IBAction)SetStartImageNum:(id)sender {
    string imageStartNumber = [self.ImageStartDownload.title cStringUsingEncoding:	NSUTF8StringEncoding];
    int number = atoi(imageStartNumber.c_str());
    resetNumofImageHaveDown(number);
}

@end
