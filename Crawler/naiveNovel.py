# encoding:UTF-8
# get the content of a naive novel in "http://www.23us.cc/html/113/113702/"
import sys
import urllib
import re

reload(sys)  
sys.setdefaultencoding('utf8') 

# get target chapter
file = open("233.txt", "w")
bookId = raw_input("please input book id:")
chapterId = raw_input("please input the chapter id:")

url = 'http://www.23us.cc/html/113/113702/'
response = urllib.urlopen(url)
html = response.read()
html = html.decode("utf-8")

# use book id
bookData = re.compile('<dt>(.*?)</dt>')
bookTexts = bookData.findall(html)

bookIndex1 = re.search(bookTexts[int(bookId)], html)
if bookId >= 7:
	bookIndex2 = re.search('<div class="notice">', html)
else:
	bookIndex2 = re.search('<dt>', html)

# use chapter id
chpData = re.compile('href="(.*?).html')
chpTexts = chpData.findall(html, bookIndex1.start(), bookIndex2.start())
chapterId  = chpTexts[int(chapterId) - 1]

# url of target chapter
url = "http://www.23us.cc/html/113/113702/" + chapterId + ".html"

# get content of target chapter
print "start"
response = urllib.urlopen(url)
html = response.read()
html = html.decode("utf-8")

data = re.compile('<div id="content">(.*?)</div>', re.S | re.M)
texts = data.findall(html)

for text in texts:
	text = text.replace("&nbsp;"," ")
	text = text.replace("<br/>", '\n')
	file.write(text)
	file.write("\n")

print "finish!"
file.close()
