# Nginxͳ��ģ�� 

  ngx_request_stats��һ��nginxͳ��ģ�飬��ͳ�����ǿ����õģ����ҿ���ͳ�Ʋ�ͬ��������������ͬ��URL������ͳ�Ƶİ����������������״̬��Ĵ���������������ۼ���Ϣ��ƽ������ʱ��ȵȡ�

# Table of contents

* [ʾ������](#synopsis)
* [Nginx������](#compatibility)
* [ģ�����](#installation)
* [ģ�����](#variables)
* [ģ��ָ��](#directives)
    * [shmap_size](#shmap_size)
    * [shmap_exptime](#shmap_exptime)
    * [request_stats](#request_stats)
    * [request_stats_query](#request_stats_query)
* [ͳ�Ʋ�ѯ](#statistics-query)
	* [�ı���ʽ](#text-format)
	* [html��ʽ](#html-format)
 	* [json��ʽ](#json-format)
	* [��ѯ���ҽ���ѯ������](#query-and-clear)
	* [��ѯĳһ��ͳ����](#query-by-status_name)
* [������˵��](#scope)
* [�򵥽ű�����](#simple-test)
* [���ģ��](#see-also)

# Synopsis
```nginx
http {
	request_stats statby_host "$host";	
	shmap_size 32m;
	shmap_exptime 2d;

	server {
		listen 81;
		server_name localhost;
		location / {
			echo "byhost:$uri";
		}
		location /404 {
			return 404;
		}
	}
	
	server {
		listen       80;
		server_name  localhost;

		location /stats {
			request_stats off; #do not stats.
			request_stats_query on;
			allow 127.0.0.1;
			allow 192.168.0.0/16;
			deny all;
		}

		location /byuri {
			request_stats statby_uri "uri:$uri";
			echo "byuri: $uri";
		}

		location /byarg {
			echo_sleep 0.005;
			request_stats statby_arg "clitype:$arg_client_type";		
			echo "login $args";
		}
		
		location /byarg/404 {
			request_stats statby_arg "clitype:$arg_client_type";		
			return 404;
		}

		location /byuriarg {
			request_stats statby_uriarg "$uri?$arg_from";	
			echo "$uri?$args";
		}

		location /byhttpheaderin {
			request_stats statby_headerin "header_in:$http_city";
			echo "city: $http_city";
		}
		
		location /byhttpheaderout/ {
			request_stats statby_headerout "cache:$sent_http_cache";
			proxy_pass http://127.0.0.1:82;
		}
	}

  server {
	listen       82;
	server_name  localhost;
	location /byhttpheaderout/hit {
		add_header cache hit;
		echo "cache: hit";
	}
	location /byhttpheaderout/miss {
		add_header cache miss;
		echo "cache: miss";
	}
  }
}

```

# Compatibility
��ģ��������°汾nginx:
* 1.7.x (last tested: 1.7.4)
* 1.6.x (last tested: 1.6.1)
* 1.4.x (last tested: 1.4.7)
* 1.2.x (last tested: 1.2.9)
* 1.0.x (last tested: 1.0.15)


# Installation
```
# echo-nginx-moduleֻ�ǲ���ʱ��Ҫʹ��,��ģ�鲢����������
cd nginx-1.x.x
./configure --add-module=path/to/ngx_request_stats \
--add-module=path/to/echo-nginx-module-0.49/
make
make install
```

# Variables
* nginx_coreģ��֧�ֵı�����http://nginx.org/en/docs/http/ngx_http_core_module.html#variables
* ��ģ�����
    * uri_full �ض���֮ǰ��uri��
    * status Http��Ӧ��
    * date ��ǰ���ڣ���ʽΪ��1970-09-28
    * time ��ǰʱ�䣬��ʽΪ��12:00:00
    * year ��ǰ���
    * month ��ǰ�·�
    * day ��ǰ��
    * hour ��ǰСʱ
    * minute ��ǰ��
    * second ��ǰ��

# Directives
* [shmap_size](#shmap_size)
* [shmap_exptime](#shmap_exptime)
* [request_stats](#request_stats)
* [request_stats_query](#request_stats_query)

shmap_size
----------
**syntax:** *shmap_size &lt;size&gt;*

**default:** *32m*

**context:** *http*

����ͳ����ʹ�õĹ����ڴ��С��
*&lt;size&gt;*����ʹ�ô�С��λ����k,m,g��


shmap_exptime
----------
**syntax:** *shmap_exptime &lt;expire time&gt;*

**default:** *2d*

**context:** *http*

����ͳ����Ϣ�ڹ����ڴ��еĹ���ʱ�䡣
����ʹ��m,h,d�ȱ�ʾ���ӣ�Сʱ���졣

request_stats
----------
**syntax:** *request_stats &lt;stats-name&gt; &lt;stats-key&gt;*

**default:** *no*

**context:** *http,server,location,location if*

����ͳ����Ϣ��ʽ��ʹ�� `request_stats off;` �ɹر�ĳ��http,server,location�µ�ͳ�ơ�
* stats-name�Ǹ�ͳ������(���)���ɰ��������ⶨ�壬�ں���Ĳ�ѯָ���У���ָ��stats-name��ѯָ����ͳ�����͡�
* stats-key����ͳ�Ƶ�key��key�п�ʹ�ø��ֱ��������ַ�����������ͬ�������ֱ��¼��[֧�ֵı���](#֧�ֵı���)һ�����г�������֧�ֵı�����**ע�⣺��Ҫʹ�ù���������ı�������key,�����ᵼ��ÿ��������һ��ͳ����Ϣ�����ռ�ô��������ڴ�ռ�**

#### ��host����ͳ��
```nginx
request_stats statby_host "$host";	
```
#### ��uri����ͳ��
```nginx
request_stats statby_uri "uri:$uri"; #������� uri:ǰ׺��
```
#### ���������(GET)����ͳ��
```nginx
request_stats statby_arg "clitype:$arg_client_type"; #������client_typeͳ��
```

#### ��uri�Ͳ�������ͳ��
```nginx
request_stats statby_uriarg "$uri?$arg_from";	
```

#### ��HTTP����ͷ�ֶν���ͳ��
```nginx
request_stats statby_uriarg "header_in:$http_city";
```
#### ��HTTP��Ӧͷ�ֶν���ͳ��
```nginx
# *ע�⣬��ǰlocation��ͨ��add_header��ӵ���Ӧͷ��ȡ������
request_stats statby_uriarg "cache:$sent_http_cache";
```

request_stats_query
----------
**syntax:** *request_stats_query &lt;on&gt;*

**default:** *off*

**context:** *location*

����ͳ�Ʋ�ѯģ�顣�����󣬾Ϳ���ͨ����location���ʵ�ͳ����Ϣ��
ͳ����Ϣ��ѯģ����������ѡ�Ĳ�����
* clean: Ϊtrueʱ��ʾ����ѯͳ����Ϣ���������β�ѯ��ͳ�������㡣
* fmt: ��ѡֵΪ��html,json,text���ֱ���html,json,text��ʽ��ʾ��Ĭ�ϸ�ʽΪtext��html����ֱ����������鿴��json��ʽ������ʹ��python�Ƚű����Խ��������text��ʽ�����������²�ѯ����ͨ��awk��shell������д���
* stats_name��Ҫ��ѯ��ͳ��������ͳ�����Ʊ�������request_statsָ��ĵ�һ������ָ����stats-name�е�һ���� ����ָ���ò���ʱ����ʾ��ѯ����ͳ�ơ�


��Сʾ����
```nginx
location /stats {
	request_stats_query on;
}
```
ͳ�Ʋ�ѯ���[ͳ�Ʋ�ѯ](#ͳ�Ʋ�ѯ)һ��

Statistics-Query
--------------
&nbsp;&nbsp;����request_stats_query�󣬾Ϳ���ͨ����Ӧ��uri����ͳ�ƽ���������Ͻ������У�����
http://192.168.1.201/stats �Ϳ�����ʾ���ͳ����Ϣ��**192.168.1.201���ҵ�����**

��ѯ�����һ�������¼����ֶΣ�
* key, request_stats�ж����key
* stats_time, ͳ����Ϣ��ʼʱ��
* request, �������
* recv, �����ֽ���
* sent, �����ֽ���
* avg_time, ����ƽ��ʱ��(����)
* stats, http��Ӧ��, ����499�Ǻ�˳�ʱ�ˡ�

&nbsp;&nbsp;**�������в�ѯ�������������[�򵥽ű�����](#�򵥽ű�����)һ���еĲ��Խű�������ġ�**

#### Text Format
http://192.168.1.201/stats
```bash
# Optional parameters:
# clean=true, query stats and set the all query data to zero in the share memory.
# fmt=[html|json|text], The default is text.
# stats_name=[ statby_host| statby_uri| statby_arg| statby_uriarg| statby_headerin| statby_headerout], The default is all.
key	stats_time	request	recv	sent	avg_time	stat
localhost	2014-08-31 22:16:47	1	0	0	0	 400:1
127.0.0.1	2014-08-31 22:16:29	80	14687	15854	0	 200:60, 404:20
cache:miss	2014-08-31 22:16:29	20	3560	3800	0	 200:20
cache:hit	2014-08-31 22:16:29	20	3540	3760	0	 200:20
header_in:beijing	2014-08-31 22:16:29	20	3740	3580	0	 200:20
header_in:shengzheng	2014-08-31 22:16:29	20	3800	3660	0	 200:20
header_in:shanghai	2014-08-31 22:16:29	20	3760	3600	0	 200:20
/byuriarg?mobile_cli	2014-08-31 22:16:29	20	3640	3840	0	 200:20
/byuriarg?pc_cli	2014-08-31 22:16:29	20	3560	3760	0	 200:20
/byuriarg?partner	2014-08-31 22:16:29	20	3580	3780	0	 200:20
clitype:android	2014-08-31 22:16:29	40	7400	10280	2	 200:20, 404:20
clitype:ios	2014-08-31 22:16:29	20	3580	3760	5	 200:20
clitype:pc	2014-08-31 22:16:29	20	3560	3740	5	 200:20
uri:/byuri/14858	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/10475	2014-08-31 22:16:30	1	169	186	0	 200:1
...
``` 
#### Html Format
http://192.168.1.201/stats?fmt=html
![��ѯ����](view_html.png)

#### Json format 
http://192.168.1.201/stats?fmt=json
```json
{"Optional parameters":{
"clean":"clean=true, query stats and set the all query data to zero in the share memory.",
"fmt":"fmt=[html|json|text], The default is text.",
"stats_name":[" statby_host| statby_uri| statby_arg| statby_uriarg| statby_headerin| statby_headerout"]
},
"request-stat":{
"localhost":{"stats_time":"2014-08-31 22:16:47","request":2,"recv":0,"sent":0,"avg_time":0,"stat":{"400":2}},
"127.0.0.1":{"stats_time":"2014-08-31 22:16:29","request":80,"recv":14687,"sent":15854,"avg_time":0,"stat":{"200":60,"404":20}},
"cache:miss":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3560,"sent":3800,"avg_time":0,"stat":{"200":20}},
"cache:hit":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3540,"sent":3760,"avg_time":0,"stat":{"200":20}},
"header_in:beijing":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3740,"sent":3580,"avg_time":0,"stat":{"200":20}},
"header_in:shengzheng":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3800,"sent":3660,"avg_time":0,"stat":{"200":20}},
"header_in:shanghai":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3760,"sent":3600,"avg_time":0,"stat":{"200":20}},
"/byuriarg?mobile_cli":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3640,"sent":3840,"avg_time":0,"stat":{"200":20}},
"/byuriarg?pc_cli":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3560,"sent":3760,"avg_time":0,"stat":{"200":20}},
"/byuriarg?partner":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3580,"sent":3780,"avg_time":0,"stat":{"200":20}},
"clitype:android":{"stats_time":"2014-08-31 22:16:29","request":40,"recv":7400,"sent":10280,"avg_time":2,"stat":{"200":20,"404":20}},
"clitype:ios":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3580,"sent":3760,"avg_time":5,"stat":{"200":20}},
"clitype:pc":{"stats_time":"2014-08-31 22:16:29","request":20,"recv":3560,"sent":3740,"avg_time":5,"stat":{"200":20}},
"uri:/byuri/14858":{"stats_time":"2014-08-31 22:16:30","request":1,"recv":169,"sent":186,"avg_time":0,"stat":{"200":1}},
"uri:/byuri/10475":{"stats_time":"2014-08-31 22:16:30","request":1,"recv":169,"sent":186,"avg_time":0,"stat":{"200":1}}
}
}
```
#### query and clear
http://192.168.1.201/stats?clean=true
ʹ��clean=true�����󣬱��β�ѯ�����Ȼ������ʾ��ֻ�����н����ᱻ���㡣

#### Query by status_name
* http://192.168.1.201/stats?stats_name=statby_headerin

```text
key	stats_time	request	recv	sent	avg_time	stat
header_in:beijing	2014-08-31 22:16:29	20	3740	3580	0	 200:20
header_in:shengzheng	2014-08-31 22:16:29	20	3800	3660	0	 200:20
header_in:shanghai	2014-08-31 22:16:29	20	3760	3600	0	 200:20
```
* http://192.168.1.201/stats?stats_name=statby_uri

```text
key	stats_time	request	recv	sent	avg_time	stat
uri:/byuri/14858	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/10475	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/20090	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/7054	2014-08-31 22:16:30	1	168	185	0	 200:1
uri:/byuri/31520	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/22000	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/24415	2014-08-31 22:16:30	1	169	186	0	 200:1
uri:/byuri/20883	2014-08-31 22:16:30	1	169	186	0	 200:1
```

# Scope
>request_statsָ����������:
`NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF`,Ҳ����˵request_statsָ����http,server,location,if�Ƚڵ��¶����Գ��֡��������ڱ�ģ����һ����NGX_HTTP_LOG_PHASE�׶εĲ����һ������ֻ����һ������������Ч�����ڲ�ͬ�Ĳ���г���ʱ��ֻ�����ڲ�Ļ������á���Ȼ�����ͬһ�������ж��request_statsָ����������Ч�������磺
```
http {
    #...
	request_stats byhost "$host";	
	server {
		listen       80;
		location /login {
			echo "login";
		}
		location /login_new {
			request_stats byarg "$arg_client_type";		
			echo "login_new $args";
		}
	}
}
```
##### ʹ����������ã������������������
```shell
curl http://127.0.0.1:80/login
curl http://127.0.0.1:80/login_new?client_type=pc
curl http://127.0.0.1:80/login_new?client_type=android
```
##### ͳ�ƽ�����ǣ�
```
key         request recv    sent    avg_time
android     1       187     210     0
pc          1       182     205     0
127.0.0.1   1       163     185     0
```
/login_new��������������Ѿ���һ����Ϊbyarg��ͳ�ƣ������ظ�ͳ�Ƶ�byhost���档��ʱ������ܲ�������Ҫ�Ľ�����������/login_newҲͳ�ƽ�byhost���棬������/login_new�����ټӸ�request_statsָ�����µ����ã�
```
http {
    #...
	request_stats byhost "$host";	
	server {
		listen       80;
		location /login {
			echo "login";
		}
		location /login_new {
			request_stats byarg "$arg_client_type";
			request_stats byhost "$host";	
			echo "login_new $args";
		}
	}
}
```
##### ���²��Ժ󣬽�����£�
```
key         request recv    sent    avg_time
127.0.0.1   3       532     600     0
android     1       187     210     0
pc          1       182     205     0
```



#### Simple Test

�����Զ�Ӧ������Synopsisһ���С�
����������curl�����ȷ�����ϵͳ�Ѿ���װcurl�����С�
Դ����Ŀ¼�µ�[test.sh](test.sh)
```bash
for ((i=0;i<20;i++));do
curl http://127.0.0.1:81/$RANDOM
curl http://127.0.0.1:81/404/$RANDOM
curl http://127.0.0.1:80/byuri/$RANDOM
curl http://127.0.0.1:80/byarg?client_type=pc
curl http://127.0.0.1:80/byarg?client_type=ios
curl http://127.0.0.1:80/byarg?client_type=android
curl http://127.0.0.1:80/byarg/404?client_type=android
curl http://127.0.0.1:80/byuriarg?from=partner
curl http://127.0.0.1:80/byuriarg?from=pc_cli
curl http://127.0.0.1:80/byuriarg?from=mobile_cli
curl http://127.0.0.1:80/byhttpheaderin -H"city: shanghai"
curl http://127.0.0.1:80/byhttpheaderin -H"city: shengzheng"
curl http://127.0.0.1:80/byhttpheaderin -H"city: beijing"
curl http://127.0.0.1:80/byhttpheaderout/hit
curl http://127.0.0.1:80/byhttpheaderout/miss
done;

```

# See Also
&nbsp;&nbsp;&nbsp;&nbsp;��ģ�������ͳ����Ϣ���洢���ڴ��У���Ҫ�û��Լ���ȡ�����Ϣ���ٴ洢�����ܡ����ߵ�����һ����Ŀ[ngx_req_stat](https://github.com/jie123108/ngx_req_stat)Ҳ��һ������ͳ��ģ�飬�������ܸ���ǿ�󣬲���key�ǿ����Զ���ģ���ͳ�Ƶ�ֵҲ�ǿ����Զ���ġ�����ͳ����Ϣ�洢��mongodb�С���Ŀ��ַ��(https://github.com/jie123108/ngx_req_stat)


Authors
=======

* liuxiaojie (��С��)  <jie123108@163.com>

[Back to TOC](#table-of-contents)

Copyright & License
===================

This module is licenced under the BSD license.

Copyright (C) 2014, by liuxiaojie (��С��)  <jie123108@163.com>

All rights reserved.
