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
using namespace std;

string homepagePath = "https://e-hentai.org/home.php";
FILE* FP;
string documentPathLimit = "";
size_t write_callbackLimit( void *ptr, size_t size, size_t nmemb, void *stream ){
    long int written = size * nmemb;
    //FILE *fp = (FILE *)stream;
    if ( access( (char*)stream, 0 ) == -1 ){
        FP = fopen((char*)stream , "wb" );
    }
    if (FP){
        written = fwrite( ptr, size, nmemb, FP );
    }
    return written;
}

string deleSymbolLimit(string input){
    string output = "";
    bool iden = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '<') {
            iden = false;
        }
        if (iden) {
            output = output + input[i];
        }
        if (input[i] == '>') {
            iden = true;
        }
    }
    return output;
}

int Retry = 0;
bool GetUrlHomepage(string url, string savepath){
    string cookiePath = documentPathLimit + "/cookie.txt";
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    
    curl = curl_easy_init();
    if ( curl ) {
        //http请求头
        headers = curl_slist_append(headers,"Host: g.e-hentai.org");
        //headers = curl_slist_append(headers,"Accept-Language: zh-cn");
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
        
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
        //curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/Users/HuFH/Documents/Testbench/Testbench/cookie.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath.c_str());
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,true);
        //指定回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callbackLimit);
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
            fclose(FP);
            return true;
        }
    }
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    if (Retry == 4) {
        Retry = 0;
        return false;
    }
    Retry++;
    GetUrlHomepage(url, savepath);
    return false;
}

string getLimit(){
    string path = documentPathLimit + "/limit.txt";
    fstream input(path.c_str(), ios::in);
    stringstream stream;
    
    string result = "";
    string temp = "";
    
    while (getline(input, temp)) {
        result = result + temp;
    }
    
    const regex pattern("You are currently at <strong>([0-9]{1,4})</strong");
    //"(http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?"
    //    smatch resultUrl;
    const std::sregex_token_iterator end;  //需要注意一下这里
    string output = "";
    for (std::sregex_token_iterator i(result.begin(),result.end(), pattern); i != end; i++){
        string temp = *i;
        //cout << temp << endl;
        temp = deleSymbolLimit(temp);
        output = "限额5000已经使用了" + temp;
    }
    return output;
}
