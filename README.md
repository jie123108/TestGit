#nginx 统计模块。
<a href="#" onclick="javascript:alert('test click!');"> ngx_req_stat</a>是一个nginx状态统计模块，其统计项是可配置的，并且可以统计不同的虚拟主机，不同的URL。可以统计的包括请求次数，各个状态码的次数，不同的时间段的次数。输出的流量累计信息，平均处理时间等等。

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

联系作者:
=====
jie123108@163.com

