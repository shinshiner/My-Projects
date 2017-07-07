#encoding:UTF-8
import sys
import urllib
import re

reload(sys)  
sys.setdefaultencoding('utf8') 

# get target website
file = open("233.txt", "w")
code = raw_input("please input the code of your file:")
pagenum = int(raw_input("please input final page you want to get(0 for all):"))
url = "https://wapwenku.baidu.com/view/" + code + ".html?"

if(pagenum == 0):
	response = urllib.urlopen(url)
	html = response.read()
	html = html.decode("utf-8")

	# get page number
	reg = re.compile('.*maxpage=(.*)">.*')
	res = reg.findall(html)
	pagenum = int(res[0])

# get information of per page
for i in range(1,pagenum + 1):
	print "getting the " + str(i) + "th page"
	file.write("----------page" + str(i) + "----------\n")
	page_url = url + "pn=" + str(i) + "&pu="
	response = urllib.urlopen(page_url)
	html = response.read()
	html = html.decode("utf-8")

	index1 = re.search(r'<div class="content bgcolor1">', html)
	index2 = re.search(r'<div class="pagebox">', html)
	data = re.compile('<p class="txt">(.*?)</p>')
	texts = data.findall(html, index1.start(), index2.start())

	for text in texts:
		file.write(text)
		file.write("\n")

print "finish!"
file.close()
