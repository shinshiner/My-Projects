# -*- coding: utf-8 -*- 

import urllib2
import re
import os
import glob

class getPicture:
    def __init__(self,baseUrl):
        self.baseUrl = baseUrl
        self.pos = None
        self.numOfPage = -1

    def getOnePage(self,pageNum):
        try:
            url = self.baseUrl + '?pn=' + str(pageNum)
            request = urllib2.Request(url)
            response = urllib2.urlopen(request)
            return response.read().decode('utf-8')
        except urllib2.URLError, e:
            if hasattr(e,"reason"):
                print "连接百度贴吧失败,错误原因",e.reason
                return None

    def getNumOfPage(self,page):
        # !!!
        reg = re.compile('<li class="l_reply_num.*?</span>.*?<span.*?>(.*?)</span>',re.S)
        result = re.search(reg,page)
        if result:
            return int(result.group(1).strip())
        else:
            return 0

    def getPicUrlList(self,num):
        html = self.getOnePage(num)
        # !!!
        l = re.findall(r'<img class="BDE_Image" pic_type="0" width="\d\d\d" height="\d\d\d" src="(.*?)"',html)
        return l

    def picRead(self,url):
        req = urllib2.Request(url)
        response = urllib2.urlopen(url)
        pic = response.read()
        return pic

    def picSave(self,url,num):
        number = str(num)
        print '正在抓取第',number,'张'
        filename = number + '.jpg'
        with open(filename,'wb') as fp:
            pic = self.picRead(url)
            fp.write(pic)

    def start(self):
        self.numOfPage = self.getNumOfPage(self.getOnePage(1))
        print self.numOfPage
        self.pos = 'd:/1/'
        os.chdir(pos)
        picUrlList = []
        for i in range(1,self.numOfPage + 1):
            url = self.baseUrl + '?pn=' + str(self.numOfPage - i)
            picUrlList += self.getPicUrlList(i)
        i = 0
        for url in picUrlList:
            i += 1
            self.picSave(url,i)
        print "Finish"

if __name__ == '__main__':
    add = raw_input("Please input address:")
    handler = getPicture(add)
    handler.start()
