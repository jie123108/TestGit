#nginx ͳ��ģ�顣
<a href="#"> ngx_req_stat</a>��һ��nginx״̬ͳ��ģ�飬��ͳ�����ǿ����õģ����ҿ���ͳ�Ʋ�ͬ��������������ͬ��URL������ͳ�Ƶİ����������������״̬��Ĵ�������ͬ��ʱ��εĴ���������������ۼ���Ϣ��ƽ������ʱ��ȵȡ�

ͳ����Ϣ���մ洢��mongodb�С�ʹ��mongodb����ͳ����Ϣ�洢������Ϊmongodb֧������Ĳ�����䣺
```bash
	db.stat_table.update({'date':'2014-04-04', 'url':'/test01'}, {'$inc':{'count':1,'req_time.all':0.005,'status.200': 1}}, true)
	#������䷴��ִ�У����ݿ��������Ӧ���ۼӼ�¼�������������(�Ե���ID�ֶ�)��
	{"count" : 1, "date" : "2014-04-04", "req_time" : { "all" : 0.005 }, "status" : { "200" : 1 }, "url" : "/test01" }
```
�����������а���������Ҫ��Ϣ��
`stat_table` �Ǳ�����
`{'date':'2014-04-04', 'url':'/test01'}` �Ǹ���������������������������в�ѯ��
`{'$inc':{'count':1,'req_time.all':0.005,'status.200': 1}}` �Ǹ��±��ʽ��������һ��json��ʽ�ķ���mongodb���ġ�

##��װ��Ŀ������##
* ��ַ
```
mongo-c-driver  https://github.com/mongodb/mongo-c-driver<br/> 
json-c http://www.linuxfromscratch.org/blfs/view/svn/general/json-c.html
```

* mongo-c-driver��װ��
```	
wget https://github.com/mongodb/mongo-c-driver/releases/download/0.92.2/mongo-c-driver-0.92.2.tar.gz
tar -xvf mongo-c-driver-0.92.2.tar.gz
cd mongo-c-driver-0.92.2
./configure --prefix=/usr
make && make install
```
* json-c ��װ:
```
wget https://s3.amazonaws.com/json-c_releases/releases/json-c-0.11.tar.gz
tar -xvf json-c-0.11.tar.gz 
./configure --prefix=/usr
make && make install
```

����|����
---|----
�ҵĴ�ѧ|���Ľ�


</table>

��ϵ����:
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
        return u'<option value="none">��</option>'

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
            #errmsg = u"��ѯ�������ڻ�URL������дһ��"
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
