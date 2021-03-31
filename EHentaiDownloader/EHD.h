//
//  main.cpp
//  Testbench
//
//  Created by 胡峰鹤 on 16/8/3.
//  Copyright © 2016年 HUFH. All rights reserved.
//

#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/mprintf.h>
#include <curl/multi.h>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <stack>
using namespace std;

//string documentPath = "./EHentaiDownloader.app/Contents/Resources/Caches";
//string documentPath =  "/Users/HuFH/Desktop/EHD";
string documentPath = "";
string picturePath = "";
string pictureFilePath = "";
string userName = "";
string userPass = "";
FILE *fp = NULL;
string file = "";
string totalUrl = "";
string filePath = "";
stringstream messOutputText;
//string result = "";
int identNumber = 0;
int retry = 0;
int countofImageHaveDownload = 0;
static bool whetherDownloadByOrder = false;
size_t write_callback( void *ptr, size_t size, size_t nmemb, void *stream ){
    long int written = size * nmemb;
    //FILE *fp = (FILE *)stream;
    if ( access( (char*)stream, 0 ) == -1 )
    {
        fp = fopen((char*)stream , "wb" );
        file = file + (char*)ptr;
        if (identNumber == 0) {
            totalUrl = totalUrl + (char*)ptr;
            //ss << (char*)ptr;
        }
    }
//    else
//    {
//        fp = fopen((char*)stream, "ab" );
//    }
    if (fp)
    {
        written = fwrite( ptr, size, nmemb, fp );
        //file = file + (char*)ptr;
        if (identNumber == 0) {
            totalUrl = totalUrl + (char*)ptr;
            //ss << (char*)ptr;
        }
    }
    return written;
}

string GetProgramDir(){
    char *buffer;
    //也可以将buffer作为输出参数
    if((buffer = getcwd(NULL, 0)) == NULL){
        cout << "getcwd error";
    }
    else{
        free(buffer);
    }
    return (string)buffer;
}

void resetNumofImageHaveDown(){
    countofImageHaveDownload = 0;
}

void resetNumofImageHaveDown(int haveDown){
    countofImageHaveDownload = haveDown;
}

bool changeDownloadBehave(){
    whetherDownloadByOrder = !whetherDownloadByOrder;
    return whetherDownloadByOrder;
}

void findCookie(){
    string cookiePath = documentPath + "/cookie.txt";
    string outPath = documentPath + "/out.txt";
    CURL *curl;
    struct curl_slist *headers = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl){
        //初始化cookie引擎
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION, 1L);
        
        //http请求头
//        headers = curl_slist_append(headers,"Host:forums.e-hentai.org");
//        headers = curl_slist_append(headers,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
//        headers = curl_slist_append(headers,"Accept-Encoding: gzip, deflate");
//        headers = curl_slist_append(headers,"Accept-Language: zh-cn");
//        headers = curl_slist_append(headers,"Content-Type: application/x-www-form-urlencoded");
//        headers = curl_slist_append(headers,"Origin: https://forums.e-hentai.org");
//        headers = curl_slist_append(headers,"Content-Length: 162");
//        headers = curl_slist_append(headers,"Connection: keep-alive");
        headers = curl_slist_append(headers,"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/601.7.7 (KHTML, like Gecko) Version/9.1.2 Safari/601.7.7");
//        headers = curl_slist_append(headers,"Referer: https://forums.e-hentai.org/index.php?act=Login&CODE=01&CookieDate=1");
//        headers = curl_slist_append(headers,"DNT: 1");
//        headers = curl_slist_append(headers,"Cookie: ipb_anonlogin=-1; ipb_coppa=0; ipb_member_id=0; ipb_pass_hash=0; ipb_session_id=5be3ec7ba45e6ca4fbab55fd4825aa6e");
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://forums.e-hentai.org/index.php?");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");        //把服务器发过来的cookie保存到cookie.txt
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath.c_str());
        
        //发送http请求头
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION, true);
        
        string url = "act=Login&CODE=01&s=5be3ec7ba45e6ca4fbab55fd4825aa6e&referer=act%3DLogin%26CODE%3D01%26CookieDate%3D1&CookieDate=1&UserName="+userName+"&PassWord="+userPass+"&submit=Log+In";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,url.c_str());
        
        string content;
        //设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outPath.c_str());
        
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE,"/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiePath.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath.c_str());
        //执行http请求
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        headers = NULL;
        
    }
    curl_global_cleanup();
}

bool GetUrl(string url, string savepath){
    string cookiePath = documentPath + "/cookie.txt";
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    
    curl = curl_easy_init();
    if ( curl ) {
        //http请求头
        headers = curl_slist_append(headers,"Host: exhentai.org");
        headers = curl_slist_append(headers,"Accept-Language: zh-cn");
        headers = curl_slist_append(headers,"Connection: keep-alive");
        //headers = curl_slist_append(headers,"Set-Cookie:uconfig=uh_y-lt_m-rc_0-tl_r-cats_0-xns_0-ts_m-tr_2-prn_y-dm_l-ar_0-rx_0-ry_0-ms_n-mt_n-cs_a-fs_p-to_a-pn_0-sc_0-ru_rrggb-xr_a-sa_y-oi_n-qb_n-tf_n-hh_-hp_-hk_-xl_; expires=Wed, 09-Aug-2017 07:57:32 GMT; Max-Age=31536000; path=/; domain=.exhentai.org");
        headers = curl_slist_append(headers,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        headers = curl_slist_append(headers,"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/601.7.7 (KHTML, like Gecko) Version/9.1.2 Safari/601.7.7");
        headers = curl_slist_append(headers,"DNT: 1");
        //headers = curl_slist_append(headers,"Accept-Encoding: gzip, deflate");
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,true);
        
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiePath.c_str());
        //curl_easy_setopt(curl, CURLOPT_COOKIELIST, "/Users/HuFH/Documents/Testbench/Testbench/cookie1.txt");
        //curl_easy_setopt(curl, CURLOPT_POST, 0);
        
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
        
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath.c_str());
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,true);
        //指定回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        //这个变量可作为接收或传递数据的作用
        curl_easy_setopt( curl, CURLOPT_WRITEDATA, savepath.c_str());
        res = curl_easy_perform( curl );
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiePath.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath.c_str());
        if (res == CURLE_OK){
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            fclose(fp);
            return true;
        }
    }
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    if (retry == 4) {
        retry = 0;
        return false;
    }
    retry++;
    GetUrl(url, savepath);
    return false;
}

bool download(string path,string url){
    //cout << url << endl;
    //cout << path << endl;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    ifstream fin(path.c_str());
    if(fin) return true;
    fin.close();
    if(curl != NULL)
    {
        //headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate");
        //headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/601.7.7 (KHTML, like Gecko) Version/9.1.2 Safari/601.7.7");
        //headers = curl_slist_append(headers, "Connection: Keep-Alive");
        
        //下载文件
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/HuFH/Documents/Testbench/Testbench/cookie1.txt");
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
        
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, path.c_str());
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        
        res = curl_easy_perform(curl);
        if(res == CURLE_OK)
        {
            //success
            fclose(fp);
            curl_slist_free_all(headers);
            return true;
        }
        
        curl_slist_free_all(headers);
        return false;
    }
    else
    {
        return false;
    }
}

string getStartPage(string url, int addNum){
    string urlNeedNum = "";
    string pageNum = "";
    bool sign = true;
    for(int i = int(url.size()) - 1; i >= 0; i--){
        if(url[i] == '=') sign = false;
        if(sign) pageNum = url[i] + pageNum;
        if(!sign) urlNeedNum = url[i] + urlNeedNum;
    }
    stringstream temp;
    temp << pageNum;
    int pageNumInt = 0;
    temp >> pageNumInt;
    pageNumInt = pageNumInt + addNum;
    stringstream temp1;
    pageNum = "";
    temp1 << pageNumInt;
    pageNum = temp1.str();
    string result = urlNeedNum + pageNum;
    return result;
}

string getName(string path){
    string name = "";
    if(whetherDownloadByOrder){
        for (int i = (int)path.size() - 1; i > 0; i--){
            if (path[i] != '|' && path[i] != '/' && path[i] != '*' && path[i] != '.') {
                name = path[i] + name;
            }
            else break;
        }
        stringstream temp;
        temp << countofImageHaveDownload;
        string number;
        temp >> number;
        name = number + "." + name;
    }
    else{
        for (int i = (int)path.size() - 1; i > 0; i--){
            if (path[i] != '|' && path[i] != '/' && path[i] != '*') {
                name = path[i] + name;
            }
            else break;
        }
    }
    return name;
}

string deleIden(string url){
    string result = "";
    for (int i = 0; i < url.size(); i++) {
        if (url[i] != '\'') {
            result = result + url[i];
        }
    }
    return result;
}

string getBackupUrl(int identify){
    string urlGet;
    const regex pattern("'([0-9]+\\-[0-9]{1,9})'");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    smatch resultUrl;
    while (regex_search(file, resultUrl, pattern)) {
        urlGet = resultUrl[0];
        file = resultUrl.suffix();
        break;
    }
    string out = "";
    string temp = deleIden(urlGet);
    if (identify > 0) {
        out = "&nl=" + temp;
    }
    else out = "?nl=" + temp;
    
    return out;
}

bool getIPandDownloadSecond(string urlPath, int level){
    //for (int i = 0; i < urlList.size(); i++) {
    if (level > 3) {
        return false;
    }
    //remove("/Users/HuFH/Documents/Testbench/Testbench/out3.txt");
    string tempPath = documentPath + "/out3.txt";
    remove(tempPath.c_str());
    
    //bool judge = GetUrl(urlPath.c_str(),"/Users/HuFH/Documents/Testbench/Testbench/out3.txt");
    file.clear();
    bool judge = GetUrl(urlPath.c_str(),documentPath + "/out3.txt");
    if(!judge) {
        messOutputText << "miss" << endl;
        return false;
    }
    
    string urlGet;
    const regex pattern("(http[s]{0,1}|ftp)://+([a-zA-Z\\.]*)hath\\.network+([:0-9]*)/h/+([0-9a-z-]*)/keystamp=([0-9a-z\\-]*);fileindex=([0-9]*);xres=([0-9a-zA-Z/._]*)");
    //pattern("(http[s]{0,1})://([0-9]{1,5})+\\.([0-9]{1,5})+\\.([0-9]{1,5})+\\.([0-9]{1,5})+(:[0-9]{1,5})?(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>;]*)?");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    smatch resultUrl;
    vector<string> urlDown;
    while (regex_search(file, resultUrl, pattern)) {
        urlGet = resultUrl[0];
        file = resultUrl.suffix();
        //download("/Users/HuFH/Documents/Testbench/Testbench/" + getName(urlGet), urlGet);
        //cout << getBackupUrl(1);
        //cout << endl << urlPath;
        //cout << urlGet << endl;
        string newPath = urlPath + getBackupUrl(level);
        if (level > 10) return false;
        //cout << endl << newPath;
        //string filePath = "/Users/HuFH/Documents/Testbench/Testbench/" + getName(urlGet);
        string filePathR = getName(urlGet);
        messOutputText << "downloading from backup path..." << filePathR << endl;
        filePathR = pictureFilePath + "/" + getName(urlGet);
        //getIPandDownloadSecond(newPath, level + 1);
        if(!download(filePathR.c_str(), urlGet)){
            remove(filePathR.c_str());
            messOutputText << "miss again " << getName(urlGet) << endl;
            if(!download(filePathR.c_str(), urlGet)){
                remove(filePathR.c_str());
                messOutputText << "miss again" << getName(urlGet) << endl;
                return getIPandDownloadSecond(urlPath + getBackupUrl(level), level + 1);
            }
            return true;
        }
        //cout << urlGet << endl;
        return true;
    }
    messOutputText << "miss, backup path" << endl;
    return getIPandDownloadSecond(urlPath + getBackupUrl(level), level + 1);
}

bool getIPandDownload(string urlPath, int level){
    //for (int i = 0; i < urlList.size(); i++) {
    if (level > 2) {
        return false;
    }
    //remove("/Users/HuFH/Documents/Testbench/Testbench/out3.txt");
    string tempPath = documentPath + "/out3.txt";
    remove(tempPath.c_str());
    
    //bool judge = GetUrl(urlPath.c_str(),"/Users/HuFH/Documents/Testbench/Testbench/out3.txt");
    bool judge = GetUrl(urlPath.c_str(),documentPath + "/out3.txt");
    if(!judge) {
        messOutputText << "miss" << endl;
        if(!GetUrl(urlPath.c_str(),documentPath + "/out3.txt")){
            messOutputText << "url Fail" << endl;
            return false;
        }
    }
    
//    string path = "/Users/HuFH/Documents/Testbench/Testbench/out3.txt";
//    fstream input(path.c_str(), ios::in);
//    stringstream stream;
//    string result = "";
//    string temp = "";
//    while (getline(input, temp)) {
//        result = result + temp;
//    }
    
    string urlGet;
    const regex pattern("(http[s]{0,1}|ftp)://+([a-zA-Z\\.]*)hath\\.network+([:0-9]*)/h/+([0-9a-z-]*)/keystamp=([0-9a-z\\-]*);fileindex=([0-9]*);xres=([0-9a-zA-Z/._]*)");
    //pattern("(http[s]{0,1}|ftp)://[0-9\\.\\-]+(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>;]*)?");
    //pattern("(http[s]{0,1})://([0-9]{1,5})+\\.([0-9]{1,5})+\\.([0-9]{1,5})+\\.([0-9]{1,5})+:([0-9]{1,5})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>;]*)?");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    vector<string> urlDown;
    const std::sregex_token_iterator resultUrl;  //需要注意一下这里
    
    std::sregex_token_iterator i(file.begin(),file.end(), pattern);
    file.clear();
    remove(tempPath.c_str());
    if (i != resultUrl) {
        urlGet = *i;
        //std::cout << urlGet << std::endl;
        string newPath = urlPath + getBackupUrl(level);
        //cout << endl << newPath;
        //string filePath = "/Users/HuFH/Documents/Testbench/Testbench/" + getName(urlGet);
        string filePathR = getName(urlGet);
        //messOutputText << "downloading..." << filePathR << endl;
        //messOutputText << "downloading...from" << urlGet << endl;
        filePathR = pictureFilePath + "/" + getName(urlGet);
        //cout << "downloading..." << filePathR << endl;
        //getIPandDownloadSecond(newPath, level + 1);
        if(!download(filePathR.c_str(), urlGet)){
            remove(filePathR.c_str());
            messOutputText << "miss, retry " << getName(urlGet) << endl;
            if(!download(filePathR.c_str(), urlGet)){
                remove(filePathR.c_str());
                messOutputText << "miss, backup path " << getName(urlGet) << endl;
                return getIPandDownloadSecond(newPath, level + 1);
            }
            else return true;
        }
        else return true;
    }
        //cout << urlGet << endl;
//    while (regex_search(file, resultUrl, pattern)) {
//        urlGet = resultUrl[0];
//        file = resultUrl.suffix();
//        //download("/Users/HuFH/Documents/Testbench/Testbench/" + getName(urlGet), urlGet);
//        //cout << getBackupUrl(1);
//        //cout << endl << urlPath;
//        string newPath = urlPath + getBackupUrl(level);
//        //cout << endl << newPath;
//        //string filePath = "/Users/HuFH/Documents/Testbench/Testbench/" + getName(urlGet);
//        string filePath = getName(urlGet);
//        cout << "downloading..." << filePath << endl;
//        //getIPandDownloadSecond(newPath, level + 1);
//        if(!download(filePath.c_str(), urlGet)){
//            remove(filePath.c_str());
//            cout << "miss, retry" << endl;
//            if(!download(filePath.c_str(), urlGet)){
//                remove(filePath.c_str());
//                cout << "miss, backup path" << endl;
//                getIPandDownloadSecond(urlPath + getBackupUrl(level), level + 1);
//            }
//        }
//        //cout << urlGet << endl;
//    }
    messOutputText << "miss, backup path" << endl;
    return getIPandDownloadSecond(urlPath + getBackupUrl(level), level + 1);
    //}/Users/HuFH/Documents/Testbench/Testbench/0002.jpg
}

string deleSymbol(string input){
    string output = "";
    bool iden = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '<') {
            iden = false;
        }
        if (iden) {
            if (input[i] == ':') {
                output = output + '_';
            }
            else if (input[i] == '?') {
                output = output + '_';
            }
            else if (input[i] == '|') {
                output = output + '-';
            }
            else if (input[i] == '"') {
                output = output + '-';
            }
            else if (input[i] == '/') {
                output = output + '-';
            }
            else if (input[i] == '(') {
                output = output + '[';
            }
            else if (input[i] == ')') {
                output = output + ']';
            }
            else if (input[i] == ';') {
                output = output + '_';
            }
            else if (input[i] == '&') {
                if (input[i+1] == '#' && input[i+2] == '0' && input[i+3] == '3'&& input[i+4] == '9'){
                    output = output + '\'';
                    i = i + 5;
                };
            }
            else output = output + input[i];
        }
        if (input[i] == '>') {
            iden = true;
        }
    }
    return output;
}

string generatePath(string input){
    string output = "";
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            output = output + '\\' + input[i];
        }
        else if (input[i] == '\'') {
            output = output + "\\\'";
        }
        else output = output + input[i];
    }
    return output;
}

pair<string,string> analysisTitle(){
    //cout << "read" << endl;
    string path = documentPath + "/out2.txt";
    fstream input(path.c_str(), ios::in);
    stringstream stream;
    
    pair<string,string> output;
    string result = "";
    string temp = "";
    
    while (getline(input, temp)) {
        result = result + temp;
    }
    
    const regex pattern("<h1 id=\"gn\">([^<]*?)</h1>");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    const std::sregex_token_iterator end;  //需要注意一下这里
    
    filePath = "";
    for (std::sregex_token_iterator i(result.begin(),result.end(), pattern); i != end; i++){
        string temp = *i;
        output.first = deleSymbol(temp);
        filePath = deleSymbol(temp) + "/";
        temp = "mkdir "+ picturePath + "/" + generatePath(deleSymbol(temp));
        pictureFilePath = picturePath + "/" + filePath;
        system(temp.c_str());
        //messOutputText << picturePath << endl;
    }
    
    const regex patternJP("<h1 id=\"gj\">([^<]*?)</h1>");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    
    for (std::sregex_token_iterator i(result.begin(),result.end(), patternJP); i != end; i++){
        string temp = *i;
        output.second = deleSymbol(temp);
        fstream tmp(pictureFilePath +"/" + deleSymbol(temp) +".txt", ios::out);
        if (tmp.fail()) {
            messOutputText << "JP name cannot generate" << endl;
            messOutputText << filePath + deleSymbol(temp) +".txt";
            output.first = "";
            output.second = "";
            return output;
        }
        //messOutputText << filePath + deleSymbol(temp) +".txt";
        tmp.close();
    }
    return output;
}

vector<string> analysisTotalPage(){
    vector<string> read;
    vector<string> readName;
    string path = documentPath + "/out4.txt";
    fstream input(path.c_str(), ios::in);
    stringstream stream;
    
    string result = "";
    string temp = "";
    
    while (getline(input, temp)) {
        result = result + temp;
    }
    
    const regex patternX("hide_image_pane\\(([0-9]{6,12})\\)\"\\>.*?\\</a\\>");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    const std::sregex_token_iterator endX;  //需要注意一下这里
    
    for (std::sregex_token_iterator i(result.begin(),result.end(), patternX); i != endX; i++){
        string temp = *i;
        readName.push_back(temp);
    }
    
    const regex pattern("(http[s]{0,1}|ftp)://+([a-z]{8,8})+\\.([a-z]{3,3})(/g/)([0-9]{6,12})(/[a-z0-9]{6,12})(/)");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    const std::sregex_token_iterator end;  //需要注意一下这里
    
    for (std::sregex_token_iterator i(result.begin(),result.end(), pattern); i != end; i++){
        string temp = *i;
        read.push_back(temp);
    }
    
    vector<string> resultPage;
    for (int i = 0; i < read.size(); i++){
        if (i >= readName.size()){
            break;
        }
        if ((readName[i].find("Spanish")==string::npos && readName[i].find("spanish")==string::npos && readName[i].find("Russian")==string::npos && readName[i].find("russian")==string::npos && readName[i].find("French")==string::npos && readName[i].find("french")==string::npos && readName[i].find("Korean")==string::npos && readName[i].find("korean")==string::npos &&
             readName[i].find("Vietnamese Tiếng Việt")==string::npos && readName[i].find("Portuguese")==string::npos &&
             readName[i].find("Italian")==string::npos && readName[i].find("italian")==string::npos &&
             readName[i].find("Thai ภาษาไทย")==string::npos)) {
            resultPage.push_back(read[i]);
            cout << read[i] << endl;
        }
//        cout <<readName[i].find("Spanish") <<" " << readName[i].find("spanish") <<" " << readName[i].find("Russian") <<" " << readName[i].find("russian") << " " << readName[i].find("French") << " " <<readName[i].find("french") <<" " << readName[i].find("Thai ภาษาไทย") << endl;
    }
    return resultPage;
}

int maxPageAnalysis(int page, string url){
    int result = 0;
    for(int i = (int)url.size()-1; i >= 0; i--){
        if (url[i] == '=' || url[i] == '/') break;
        else {
            result = (url[i]-'0') + result*10;
        }
    }
    if (result > page) return result;
    else return page;
}

int analysisPage(){
    //cout << "read" << endl;
    string path = documentPath + "/out2.txt";
    fstream input(path.c_str(), ios::in);
    stringstream stream;
    
    int page = 1;
    string result = "";
    string temp = "";
    
    while (getline(input, temp)) {
        result = result + temp;
    }
    
    const regex pattern("(http[s]{0,1}|ftp)://+([a-z]{8,8})+\\.([a-z]{3,3})(/g/)([0-9]{6,12})(/[a-z0-9]{6,12})(/[?]p=)([0-9]{1,4})");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    const std::sregex_token_iterator end;  //需要注意一下这里
    
    long int size = 0;
    filePath = "";
    std::sregex_token_iterator i(result.begin(),result.end(), pattern);
    for (;i != end; i++) {
        string temp = *i;
        size = temp.size();
        page = maxPageAnalysis(page, temp);
    }
    page++;
    messOutputText << "画册共有"<< page << "页" << endl;
    return page;
}

void readUrl(bool identi){
    identNumber = 1;
    if (identi) {
        messOutputText << "画册分析成功" << endl;
    }
    //cout << "read" << endl;
    //string path = "/Users/HuFH/Documents/Testbench/Testbench/out2.txt";
    string path = documentPath + "/out2.txt";
    fstream input(path.c_str(), ios::in);
    stringstream stream;
    
//    string result = "";
//    while (getline(input, result)){
//        stream << result;
//    }
//    
//    vector<string> urlList;
//    while (stream >> result){
//        if (result.find("http")) {
//            urlList.push_back(result);
//        }
//    }
//    cout << urlList[0] << endl;
//    cout << urlList[1] << endl;
//    cout << urlList[2] << endl;
//    return 0;
    
//    
    string result = "";
    string temp = "";

    while (getline(input, temp)) {
        result = result + temp;
    }
    //cout << totalUrl.c_str();
    //cout << ss.str();
    const regex pattern("(http[s]{0,1}|ftp)://+([a-z]{8,8})+\\.([a-z]{3,3})(/s/)([a-z0-9]{7,13})(/[0-9]{3,7})(\\-)([0-9]{1,4})");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
//    smatch resultUrl;
    int count = 0;
//    //vector<string> UrlFind;
//    while (regex_search(totalUrl, resultUrl, pattern)) {
//        if(count == 45) break;
//        string temp = resultUrl[0];
//        cout << temp << endl;
//        //UrlFind.push_back(temp);
//        totalUrl = resultUrl.suffix();
//        getIPandDownload(temp, 0);
//        count++;
//        //break;
//    }
    const std::sregex_token_iterator end;  //需要注意一下这里
    
    for (std::sregex_token_iterator i(result.begin(),result.end(), pattern); i != end ; i++){
        if(count == 45) break;
        string temp = *i;
        countofImageHaveDownload++; //这里是图片下载数目，用于规律化命名
        //messOutputText << temp << endl;
        if(!getIPandDownload(temp, 0)) {
            messOutputText << temp << endl;
            count++;
        }
        else count++;
    }
//    for (int i = 0; UrlFind.size(); i++) {
//        getIPandDownload(UrlFind[i], 0);
//    }
    //}
}

//static size_t write_downloadData(void *ptr, size_t size, size_t nmemb, void *stream){
//    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
//    return written;
//}

//int main(){
////    string url = "https://exhentai.org/g/919911/736869ff09/";
////    string outPath = "/Users/HuFH/Documents/Testbench/Testbench/out2.txt";
////    string outDownPath = "/Users/HuFH/Documents/Testbench/Testbench/out3.txt";
////    remove("/Users/HuFH/Documents/Testbench/Testbench/out2.txt");
//////    //findCookie();
////    GetUrl(url.c_str(), outPath);
////    readUrl();
////    //getIPandDownload("https://exhentai.org/s/cfc8f5259b/767763-1");
//    string homepagePath = "g.e-hentai.org/home.php";
//    char way[1000];
//    getcwd(way, sizeof(way));
//    
//    string command = way;
//    command = "cd " + command;
//    system(command.c_str());
//    
//    cout << "登录没？(Y/N/S)" << endl;
//    char symbol = '0';
//    cin >> symbol;
//    if (symbol == 'N') {
//        cout << endl << "用户名：" << endl;
//        cin >> usrName;
//        cout << endl << "密码：" << endl;
//        cin >> usrPass;
//        findCookie();
//    }
//    else if(symbol == 'S'){
//        remove("limit.txt");
//        if(!GetUrlHomepage(homepagePath, "limit.txt")){
//            cout << "GFW" << endl;
//        }
//        getLimit();
//    }
//    else {
//        //remove("/Users/HuFH/Documents/Testbench/Testbench/out2.txt");
//        while (1) {
//            remove("out2.txt");
//            string url = "";
//            cout << endl << "URL(enter 'exit' to exit):" << endl;
//            cin >> url;
//            if (url == "exit") {
//                break;
//            }
//            int page = 0;
//            cout << endl << "page:" << endl;
//            cin >> page;
//            ident = 0;
//            bool urlGetIden = GetUrl(url, "out2.txt");
//            analysisTitle();
//            readUrl(urlGetIden);
//            totalUrl.clear();
//            for (int i = 1; i < page; i++) {
//                ident = 0;
//                //remove("/Users/HuFH/Documents/Testbench/Testbench/out2.txt");
//                remove("out2.txt");
//                char num = char(i + int('0'));
//                string nextUrl = url + "?p=" + num;
//                cout << endl << endl << nextUrl << endl;
//                cout << endl << endl << "nextPage" << endl;
//                bool urlGetIden = GetUrl(nextUrl, "out2.txt");
//                readUrl(urlGetIden);
//                totalUrl.clear();
//            }
//        }
//    }
//}
