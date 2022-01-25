import requests
import re
import time
import pandas as pd
#from bs4 import BeautifulSoup
#soup = BeautifulSoup(r.text, 'lxml')

#BOOK_LINK_PATTERN = 'style="color: #1F82D2;font-size: 18px;font-weight: bold;" href="(.*)"'
#url_list = []
#for i in range (1,76):
#    r = requests.get('http://kyqgs.mnr.gov.cn/search_bar.jspx?year1=98&year2=95&year3=182&pageNo='
#    + str (i)
#    +'&firstListTotalCount=27&secondListTotalCount=348&thirdListTotalCount=0&fourthListTotalCount=0&searchParams=%E6%B5%B7%E8%9E%BA%E6%B0%B4%E6%B3%A5&pagecount=75&times=&kuangquans=kq-0&areas=dq-0&_goPs='
#    + str (i))
#    url_list += list(set(re.findall(BOOK_LINK_PATTERN, r.text)))
#    time.sleep(3)
#
#fp = open(r'f:\test.txt', 'w')
#for i in range(len(url_list)):
#    fp.writelines(url_list[i])
#    fp.writelines('\n')
#fp.close()

url_patten = '''style="color: #1F82D2;font-size: 18px;font-weight: bold;" href="(.*)">
				(.*)
				<a class="jqxx">(.*)年度</a>'''
url_list = []
for i in range (1,76):
    r = requests.get('http://kyqgs.mnr.gov.cn/search_bar.jspx?year1=98&year2=95&year3=182&pageNo='
    + str (i)
    +'&firstListTotalCount=27&secondListTotalCount=348&thirdListTotalCount=0&fourthListTotalCount=0&searchParams=%E6%B5%B7%E8%9E%BA%E6%B0%B4%E6%B3%A5&pagecount=75&times=&kuangquans=kq-0&areas=dq-0&_goPs='
    + str (i))
    url_list += list(set(re.findall(url_patten, r.text)))
    time.sleep(3)

fp = open(r'f:\test1.txt', 'w')
for i in range(len(url_list)):
    for j in range(len(url_list[i])):
        fp.writelines(url_list[i][j])
        fp.writelines(',')
    fp.writelines('\n')
fp.close()

fp = open(r'f:\test3.txt', 'r')
url_list = fp.readlines()
for i in range(len(url_list)):
    url_list[i] = url_list[i].replace('\n','')

clm = ['name','category','area','expire','province','ratified quantity','actual quantity']
df = pd.DataFrame(columns = clm)
patten1 = '''<tr>
						<td colspan="1" class="title_back_right_bt" style="width: 15%">矿山名称：</td>
						<td colspan="3" class="title_back_left" style="width: 35%"  >(.*)</td>
					</tr>'''
patten2 = '''<td class="title_back_right_bt" style="width: 15%">开采矿种：</td>
						<td class="title_back_left" style="width: 35%"  >(.*)</td>'''
patten3 = '''<td class="title_back_right_bt" style="width: 15%">矿区面积（平方公里）：</td>
						<td.* class="title_back_left" style="width: 35%"   >(.*)<span style="float: right;">'''
patten4 = '''<td class="title_back_right_bt" style="width: 15%">有效期限：</td>
						<td colspan="3" class="title_back_left" style="width: 35%"  ><span>.*</span>&nbsp;&nbsp;至&nbsp;&nbsp;<span>(.*)</span>'''
patten5 = '''<td class="title_back_right_bt" style="width: 15%">发证机关：</td>
						<td class="title_back_left" style="width: 35%"  >(.*)</td>'''
patten6 = '''<td class="title_back_right_bt" >设计.*矿山.*规模.*：</td>
						<td class="title_back_left" colspan="2" > (.*)</td>'''
patten7 = '''<td class="title_back_right_bt" >实际年采矿石量.*：</td>
						<td class="title_back_left" colspan="2" >(.*)</td>'''
patten = [patten1, patten2, patten3, patten4, patten5, patten6, patten7]

#r = requests.get('http://kyqgs.mnr.gov.cn' + url_list[215])
#Ser = pd.Series('',index = clm)
#for j in range(len(patten)):
#    try:
#        Ser[j] = re.findall(patten[j], r.text)[0]
#    except:
#        Ser[j] = 'NAN'
#df = df.append(Ser, ignore_index=True)
#df

for i in url_list:
    r = requests.get('http://kyqgs.mnr.gov.cn' + i)
    Ser = pd.Series('',index = clm)
    for j in range(len(patten)):
        try:
            Ser[j] = re.findall(patten[j], r.text)[0]
        except:
            Ser[j] = 'NAN'
    df = df.append(Ser, ignore_index=True)
    time.sleep(5)
df.to_excel(r'f:\test1.xlsx', sheet_name= 'test')
