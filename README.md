# Name 

  ngx_request_status��һ��nginx״̬ͳ��ģ�飬��ͳ�����ǿ����õģ����ҿ���ͳ�Ʋ�ͬ��������������ͬ��URL������ͳ�Ƶİ����������������״̬��Ĵ���������������ۼ���Ϣ��ƽ������ʱ��ȵȡ�
 �����е�ngx_shmap.c��ngx_shmap.h��������Ŀ��https://github.com/jie123108/ngx_shmap�����Զ���ʹ�á�

# Table of Contents

* [Synopsis](#synopsis)
* [Nginx Compatibility] (#nginx-compatibility)
* [Module Compile] (#module-compile)
* [Module Variables](#module-variables)
* [Directives](#directives)
* [Statistical query](#statistical-query)
	* [�ı���ʽ](#�ı���ʽ)
	* [html��ʽ](#html��ʽ)
 	* [json��ʽ](#json��ʽ)
	* [��ѯ���ҽ���ѯ������](#��ѯ���ҽ���ѯ������)
	* [��ѯĳһ��ͳ����](#��ѯĳһ��ͳ����)
* [������˵��](#������˵��)
* [�򵥽ű�����](#�򵥽ű�����)

# Synopsis
```nginx
http {
	request_status statby_host "$host";	
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

		location /status {
			request_status off; #do not status.
			request_status_query on;
			allow 127.0.0.1;
			allow 192.168.0.0/16;
			deny all;
		}

		location /byuri {
			request_status statby_uri "uri:$uri";
			echo "byuri: $uri";
		}

		location /byarg {
			echo_sleep 0.005;
			request_status statby_arg "clitype:$arg_client_type";		
			echo "login $args";
		}
		
		location /byarg/404 {
			request_status statby_arg "clitype:$arg_client_type";		
			return 404;
		}

		location /byuriarg {
			request_status statby_uriarg "$uri?$arg_from";	
			echo "$uri?$args";
		}

		location /byhttpheaderin {
			request_status statby_headerin "header_in:$http_city";
			echo "city: $http_city";
		}
		
		location /byhttpheaderout/ {
			request_status statby_headerout "cache:$sent_http_cache";
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

# Nginx Compatibility
The latest module is compatible with the following versions of Nginx:
* 1.7.x (last tested: 1.7.4)
* 1.6.x (last tested: 1.6.1)
* 1.4.x (last tested: 1.4.7)
* 1.2.x (last tested: 1.2.9)
* 1.0.x (last tested: 1.0.15)


# Module Compile
```
# echo-nginx-moduleֻ�ǲ���ʱ��Ҫʹ��,��ģ�鲢����������
cd nginx-1.x.x
./configure --add-module=path/to/ngx_request_status \
--add-module=path/to/echo-nginx-module-0.49/
make
make install
```

# Module Variables
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
* [request_status](#request_status)
* [request_status_query](#request_status_query)

shmap_size
----------
**syntax:** *shmap_size &lt;size&gt;*

**default:** *32m*

**context:** *http*

����ͳ��ʹ�õĹ����ڴ��С������ʹ��k,m,g�ȵ�λ��ʾKB,MB,GB��


shmap_exptime
----------
**syntax:** *shmap_exptime &lt;expire time&gt;*

**default:** *2d*

**context:** *http*

����ͳ����Ϣ�ڹ����ڴ��еĹ���ʱ�䡣��λΪ�룬����ʹ��m,h,d�ȱ�ʾ���ӣ�Сʱ���졣

request_status
----------
**syntax:** *request_status &lt;status-name&gt; &lt;status-key&gt;*

**default:** *no*

**context:** *http,server,location,location if*

����ͳ����Ϣ��ʽ��ʹ�� `request_status off;` �ɹر�ĳ��http,server,location�µ�ͳ�ơ�
* status-name�Ǹ�ͳ������(���)���ɰ��������ⶨ�壬�ں���Ĳ�ѯָ���У���ָ��status-name��ѯָ����ͳ�����͡�
* status-key����ͳ�Ƶ�key��key�п�ʹ�ø��ֱ��������ַ�����������ͬ�������ֱ��¼��[֧�ֵı���](#֧�ֵı���)һ�����г�������֧�ֵı�����

#### ��host����ͳ��
```nginx
request_status statby_host "$host";	
```
#### ��uri����ͳ��
```nginx
request_status statby_uri "uri:$uri"; #������� uri:ǰ׺��
```
### ���������(GET)����ͳ��
```nginx
request_status statby_arg "clitype:$arg_client_type"; #������client_typeͳ��
```

### ��uri�Ͳ�������ͳ��
```nginx
request_status statby_uriarg "$uri?$arg_from";	
```

### ��HTTP����ͷ�ֶν���ͳ��
```nginx
request_status statby_uriarg "header_in:$http_city";
```
### ��HTTP��Ӧͷ�ֶν���ͳ��
```nginx
# *ע�⣬��ǰlocation��ͨ��add_header��ӵ���Ӧͷ��ȡ������
request_status statby_uriarg "cache:$sent_http_cache";
```

request_status_query
----------
**syntax:** *request_status_query &lt;on&gt;*

**default:** *off*

**context:** *location*

����ͳ�Ʋ�ѯģ�顣�����󣬾Ϳ���ͨ����location���ʵ�ͳ����Ϣ��
ͳ����Ϣ��ѯģ����������ѡ�Ĳ�����
* clean: Ϊtrueʱ��ʾ����ѯͳ����Ϣ���������β�ѯ��ͳ�������㡣
* fmt: ��ѡֵΪ��html,json,text���ֱ���html,json,text��ʽ��ʾ��Ĭ�ϸ�ʽΪtext��html����ֱ����������鿴��json��ʽ������ʹ��python�Ƚű����Խ��������text��ʽ�����������²�ѯ����ͨ��awk��shell������д���
* stat_name��Ҫ��ѯ��ͳ��������ͳ�����Ʊ�������request_statusָ��ĵ�һ������ָ����stat-name�е�һ���� ����ָ���ò���ʱ����ʾ��ѯ����ͳ�ơ�


��Сʾ����
```nginx
location /status {
	request_status_query on;
}
```
ͳ�Ʋ�ѯ���[ͳ�Ʋ�ѯ](#ͳ�Ʋ�ѯ)һ��

Statistical query
--------------
&nbsp;&nbsp;���ú�request_status_query�󣬾Ϳ���ͨ����Ӧ��uri����ͳ�ƽ���������Ͻ������У�����
http://192.168.1.201/status �Ϳ�����ʾ���ͳ����Ϣ��**192.168.1.201���ҵ�����**

��ѯ�����һ�������¼����ֶΣ�
* key, request_status�ж����key
* request, �������
* recv, �����ֽ���
* sent, �����ֽ���
* avg_time, ����ƽ��ʱ��(����)
* status, http��Ӧ��, ����499�Ǻ�˳�ʱ�ˡ�

&nbsp;&nbsp;**�������в�ѯ�������������[�򵥽ű�����](#�򵥽ű�����)һ���еĲ��Խű�������ġ�**

#### �ı���ʽ
http://192.168.1.201/status
```bash
# Optional parameters:
# clean=true, query statistics and set the all query data to zero in the share memory.
# fmt=[html|json|text], The default is text.
# stat_name=[ statby_host| statby_uri| statby_arg| statby_uriarg], The default is all.
key	request	recv	sent	avg_time	status
localhost	5	0	0	0	 400:5
127.0.0.1	80	14689	15855	0	 200:60, 404:20
cache:miss	20	3560	3800	0	 200:20
cache:hit	20	3540	3760	0	 200:20
header_in:beijing	20	3740	3580	0	 200:20
header_in:shengzheng	20	3800	3660	0	 200:20
header_in:shanghai	20	3760	3600	0	 200:20
/byuriarg?mobile_cli	20	3640	3840	0	 200:20
/byuriarg?pc_cli	20	3560	3760	0	 200:20
/byuriarg?partner	20	3580	3780	0	 200:20
clitype:android	40	7400	10280	2	 200:20, 404:20
clitype:ios	20	3580	3760	5	 200:20
clitype:pc	20	3560	3740	5	 200:20
uri:/byuri/12964	1	169	186	0	 200:1
uri:/byuri/25507	1	169	186	0	 200:1
uri:/byuri/10608	1	169	186	0	 200:1
...
```
#### html��ʽ
http://192.168.1.201/status?fmt=html
![��ѯ����](view_html.png)

#### json��ʽ
http://192.168.1.201/status?fmt=json
```json
{"Optional parameters":{
"clean":"clean=true, query statistics and set the all query data to zero in the share memory.",
"fmt":"fmt=[html|json|text], The default is text.",
"stat_name":[" statby_host| statby_uri| statby_arg| statby_uriarg"]
},
"request-status":{
"localhost":{"request":8,"recv":0,"sent":0,"avg_time":0,"status":{"400":8}},
"192.168.1.201":{"request":3,"recv":927,"sent":966,"avg_time":0,"status":{"404":3}},
"127.0.0.1":{"request":80,"recv":14689,"sent":15855,"avg_time":0,"status":{"200":60,"404":20}},
"cache:miss":{"request":20,"recv":3560,"sent":3800,"avg_time":0,"status":{"200":20}},
"cache:hit":{"request":20,"recv":3540,"sent":3760,"avg_time":0,"status":{"200":20}},
"header_in:beijing":{"request":20,"recv":3740,"sent":3580,"avg_time":0,"status":{"200":20}},
"header_in:shengzheng":{"request":20,"recv":3800,"sent":3660,"avg_time":0,"status":{"200":20}},
"header_in:shanghai":{"request":20,"recv":3760,"sent":3600,"avg_time":0,"status":{"200":20}},
"/byuriarg?mobile_cli":{"request":20,"recv":3640,"sent":3840,"avg_time":0,"status":{"200":20}},
"/byuriarg?pc_cli":{"request":20,"recv":3560,"sent":3760,"avg_time":0,"status":{"200":20}},
"/byuriarg?partner":{"request":20,"recv":3580,"sent":3780,"avg_time":0,"status":{"200":20}},
"clitype:android":{"request":40,"recv":7400,"sent":10280,"avg_time":2,"status":{"200":20,"404":20}},
"clitype:ios":{"request":20,"recv":3580,"sent":3760,"avg_time":5,"status":{"200":20}},
"clitype:pc":{"request":20,"recv":3560,"sent":3740,"avg_time":5,"status":{"200":20}},
"uri:/byuri/12964":{"request":1,"recv":169,"sent":186,"avg_time":0,"status":{"200":1}},
"uri:/byuri/25507":{"request":1,"recv":169,"sent":186,"avg_time":0,"status":{"200":1}},
"uri:/byuri/10608":{"request":1,"recv":169,"sent":186,"avg_time":0,"status":{"200":1}},
"uri:/byuri/9349":{"request":1,"recv":168,"sent":185,"avg_time":0,"status":{"200":1}},
"uri:/byuri/25684":{"request":1,"recv":169,"sent":186,"avg_time":0,"status":{"200":1}}
}
}
```
#### ��ѯ���ҽ���ѯ������
http://192.168.1.201/status?clean=true
ʹ��clean=true�����󣬱��β�ѯ�����Ȼ������ʾ��ֻ�����н����ᱻ���㡣

#### ��ѯĳһ��ͳ����
* http://192.168.1.201/status?stat_name=statby_headerin

```text
key	request	recv	sent	avg_time	status
header_in:beijing	20	3740	3580	0	 200:20
header_in:shengzheng	20	3800	3660	0	 200:20
header_in:shanghai	20	3760	3600	0	 200:20
```
* http://192.168.1.201/status?stat_name=statby_uri

```text
key	request	recv	sent	avg_time	status
uri:/byuri/9510	1	168	185	0	 200:1
uri:/byuri/31706	1	169	186	0	 200:1
uri:/byuri/22372	1	169	186	0	 200:1
uri:/byuri/28608	1	169	186	0	 200:1
uri:/byuri/28809	1	169	186	0	 200:1
uri:/byuri/2427	1	168	185	0	 200:1
uri:/byuri/700	1	167	184	0	 200:1
uri:/byuri/1096	1	168	185	0	 200:1
uri:/byuri/26854	1	169	186	0	 200:1
uri:/byuri/27665	1	169	186	0	 200:1
uri:/byuri/12983	1	169	186	0	 200:1
uri:/byuri/26040	1	169	186	0	 200:1
```

#������˵��
>request_statusָ����������:
`NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF`,Ҳ����˵request_statusָ����http,server,location,if�Ƚڵ��¶����Գ��֡��������ڱ�ģ����һ����NGX_HTTP_LOG_PHASE�׶εĲ����һ������ֻ����һ������������Ч�����ڲ�ͬ�Ĳ���г���ʱ��ֻ�����ڲ�Ļ������á���Ȼ�����ͬһ�������ж��request_statusָ����������Ч�������磺
```
http {
    #...
	request_status byhost "$host";	
	server {
		listen       80;
		location /login {
			echo "login";
		}
		location /login_new {
			request_status byarg "$arg_client_type";		
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
/login_new��������������Ѿ���һ����Ϊbyarg��ͳ�ƣ������ظ�ͳ�Ƶ�byhost���档��ʱ������ܲ�������Ҫ�Ľ�����������/login_newҲͳ�ƽ�byhost���棬������/login_new�����ټӸ�request_statusָ�����µ����ã�
```
http {
    #...
	request_status byhost "$host";	
	server {
		listen       80;
		location /login {
			echo "login";
		}
		location /login_new {
			request_status byarg "$arg_client_type";
			request_status byhost "$host";	
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



#### �򵥽ű�����

�����Զ�Ӧ������Synopsisһ���С�
����������curl�����ȷ�����ϵͳ�Ѿ���װcurl�����С�
Դ����Ŀ¼�µ�[test.sh](#test.sh)
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


��ϵ����:
=====
jie123108@163.com

