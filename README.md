#nginx ͳ��ģ�顣
<a href="#" onclick="javascript:alert('test click!');"> ngx_req_stat</a>��һ��nginx״̬ͳ��ģ�飬��ͳ�����ǿ����õģ����ҿ���ͳ�Ʋ�ͬ��������������ͬ��URL������ͳ�Ƶİ����������������״̬��Ĵ�������ͬ��ʱ��εĴ���������������ۼ���Ϣ��ƽ������ʱ��ȵȡ�

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

��ϵ����:
=====
jie123108@163.com

