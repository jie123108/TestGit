#nginx 统计模块。
<a href="#"> ngx_req_stat</a>是一个nginx状态统计模块，其统计项是可配置的，并且可以统计不同的虚拟主机，不同的URL。可以统计的包括请求次数，各个状态码的次数，不同的时间段的次数。输出的流量累计信息，平均处理时间等等。

统计信息最终存储在mongodb中。使用mongodb进行统计信息存储，是因为mongodb支持下面的操作语句：
```bash
	db.stat_table.update({'date':'2014-04-04', 'url':'/test01'}, {'$inc':{'count':1,'req_time.all':0.005,'status.200': 1}}, true)
	#这种语句反复执行，数据库会生成相应的累加记录，大概是这样的(略掉了ID字段)：
	{"count" : 1, "date" : "2014-04-04", "req_time" : { "all" : 0.005 }, "status" : { "200" : 1 }, "url" : "/test01" }
```
在上面的语句中包含几个主要信息：
`stat_table` 是表名，
`{'date':'2014-04-04', 'url':'/test01'}` 是更新主键，后面可以以主键来进行查询。
`{'$inc':{'count':1,'req_time.all':0.005,'status.200': 1}}` 是更新表达式，必须是一个json格式的符合mongodb语句的。

##安装项目依赖项##
* 网址
```
mongo-c-driver  https://github.com/mongodb/mongo-c-driver<br/> 
json-c http://www.linuxfromscratch.org/blfs/view/svn/general/json-c.html
```

* mongo-c-driver安装：
```	
wget https://github.com/mongodb/mongo-c-driver/releases/download/0.92.2/mongo-c-driver-0.92.2.tar.gz
tar -xvf mongo-c-driver-0.92.2.tar.gz
cd mongo-c-driver-0.92.2
./configure --prefix=/usr
make && make install
```
* json-c 安装:
```
wget https://s3.amazonaws.com/json-c_releases/releases/json-c-0.11.tar.gz
tar -xvf json-c-0.11.tar.gz 
./configure --prefix=/usr
make && make install
```

标题|作者
---|----
我的大学|林心洁


</table>

联系作者:
=====
jie123108@163.com

```python

# coding=utf8
'''
Created on 2013-2-28

@author: nic
'''
import time

from flask import Blueprint, request, render_template as render

from ..configs import settings
from ..views.ngx_stat_dao import tables,tablesContent,mongo_query_stat

views = Blueprint('ngxstatquery', __name__)

@views.route('/', methods=['get'])
def home():
    return render('dwz_base.html')

def get_options(tables, selTable):
    arr = []
    if tables:
        for table in tables:
            if selTable == table:
                arr.append('''<option value='%s' selected='selected'>%s</option>''' % (table, table))
            else:
                arr.append('''<option value='%s'>%s</option>''' % (table, table))
        return ''.join(arr)
    else:
        return u'<option value="none">无</option>'

@views.route('/ngxstatmgr/query', methods=['get', 'post'])
#@check_permission('PRIV_NGXSTAT_QUERY')
def main():
    args = request.args if request.method == 'GET' else request.form
    
    serverid = args.get('serverid',settings.servers.keys()[0])
    table = args.get('table', '')
    today = time.strftime('%Y-%m-%d',time.localtime(int(time.time())))
    logdate = args.get('logdate', today)
    url = args.get('url', '')
    if logdate == 'no':
        logdate = ''
    
    defTables = None
    if serverid:
        defTables = tables[serverid]
    #print "defTables:", defTables
    record_cnt = 0
    
    errmsg = None
    stats_cur = None
    while 1:
        if logdate is None and url is None:
            #errmsg = u"查询错误，日期或URL必须填写一项"
            break
        
        host = settings.servers[serverid]
        #print "serverid:", serverid
        #print "host:", host
        #print "table:", table
        if host and table:
            stats_cur = mongo_query_stat(host, table, logdate, url)
            record_cnt = stats_cur.count()
        #print "records:", record_cnt
        break
    
    server_names = sorted(settings.servers.keys())
    tcpstat = table and table.startswith('tcp_')
    print "tcpstat: ", tcpstat
    return render('query.html', servers=settings.servers,server_names=server_names,
                  defTables=get_options(defTables,table),tablesContent=tablesContent,
                  logdate=logdate,url=url,record_cnt=record_cnt,records=stats_cur,
                  errmsg=errmsg,serverid=serverid,table=table,tcpstat=tcpstat)
    #return render('list_types.html', msg_types=msg_type_list)

```
