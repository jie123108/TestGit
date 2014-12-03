# Nginx mmap geo module
   ngx_geo_mod ��һ������λ�ò�ѯģ�顣����ݿͻ���IP��ѯ����Ӧ��ʡ�ݣ����У�ISP��Ϣ��д������ͷ��
   
   ��ģ�����һ�����ı���ʽ�ĵ���λ����Ϣ���ݱ���ɶ��������ݵı�������ģ������ʱֱ��ʹ�ö��������ݣ����Ҳ���mmap��ʽ�����ļ�������Ҫ�κν������̡�

# Ŀ¼
* [����ʾ��](#����ʾ��)
* [Nginx������](#Nginx������)
* [ģ�����](#ģ�����)
* [����GEO������](#����GEO������)
* [ģ��ָ��](#ģ��ָ��)
    * [geodata_file](#geodata_file)
    * [mm_geo](#mm_geo)
    * [ip_from_url](#ip_from_url)
    * [ip_from_head](#ip_from_head)
    * [proxies](#proxies)
    * [proxies_recursive](#proxies_recursive)
* [��ȡIP�ķ�ʽ](#��ȡIP�ķ�ʽ)
* [������ʹ��](#������ʹ��)
* [��������ʹ��](#��������ʹ��)
    * [�����ļ���ʽ](#�����ļ���ʽ)
    * [���������ļ�](#���������ļ�)
    * [ʹ��geot���Զ�����GEO�ļ�](#ʹ��geot���Զ�����GEO�ļ�)

# ����ʾ��
```nginx
http {
    include       mime.types;
    default_type  application/octet-stream;

    #gzip  on;
    # set the data file path
    geodata_file /root/work/GitHub/ngx_geo_mod/top20.geo;
    # Get the IP address from the URL, only for testing.
    ip_from_url on;
    # Get the IP address from the request header, set to on when front-end have proxy.
    ip_from_head on;
    # Set the trusted proxy address. when the ip_from_head is true to used
    proxies 127.0.0.1/24;
    proxies_recursive on;

    # used variables in access log.
    log_format  main  '$remote_addr@$http_x_province $http_x_city '
                      '$http_x_isp [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';
    access_log  logs/access.log  main;

    # Use the geo 
	mm_geo on;
    server {
        listen       80;
        server_name  localhost;
        location /area {
            # used variables in module
        	echo "      ip: $http_x_ip";
        	echo "province: $http_x_province";
        	echo "    city: $http_x_city";
        	echo "     isp: $http_x_isp";
        }

        location /not_used {
            # do not use the geo
            mm_geo off;
            echo "$http_x_province";
        }
    }
}

```
# Nginx������
��ģ��������°汾nginx:
* 1.7.x (last tested: 1.7.4)
* 1.6.x (last tested: 1.6.1)
* 1.4.x (last tested: 1.4.7)
* 1.2.x (last tested: 1.2.9)
* 1.0.x (last tested: 1.0.15)

# ģ�����
```shell
# echo-nginx-moduleֻ�ǲ���ʱ��Ҫʹ��,��ģ�鲢����������
cd nginx-1.x.x
./configure --add-module=path/to/ngx_geo_mod \
            --add-module=path/to/echo-nginx-module
make
make install
```

#����GEO������
```shell
cd path/to/ngx_geo_mod
make
>gcc -g geodata_compiler.c array.c -o geoc
>gcc -D_TOOLS_ -g geodata.c -o geot
>gcc -g -Werror geodata.c -fPIC -shared -o libgeo.so
```
* geoc��GEO������
* geot��GEO�����ļ����Գ���
* libgeo.so �Ƕ�̬�⣬�����ڸ��ֳ����е��ø�ģ�顣

# ģ��ָ��
* [geodata_file](#geodata_file)
* [mm_geo](#mm_geo)
* [ip_from_url](#ip_from_url)
* [ip_from_head](#ip_from_head)
* [proxies](#proxies)
* [proxies_recursive](#proxies_recursive)

geodata_file
----------
**syntax:** *geodata_file &lt;path to binary geodata file&gt;*

**default:** *--*

**context:** *http*

ָ�������Ƶ�GEO�����ļ�·�����������ļ�ʹ��geoc�������ɡ����ɷ����ο�[��������ʹ��](#��������ʹ��)

mm_geo
----------
**syntax:** *mm_geo &lt;on | off&gt;*

**default:** *off*

**context:** *http*,*server*,*location*,*location if*

�򿪻��߹ر�geoģ�顣��GEOģ���HTTP����ͷ�л����4���Զ���ͷ��
* x-province *ʡ����Ϣ*
* x-city *������Ϣ*
* x-isp *ISP��Ϣ*
* x-ip *IP*

����ͷ������nginx����ģ����ʹ�á�ʹ�÷�����[������ʹ��](#������ʹ��)��

ip_from_url
----------
**syntax:** *ip_from_url &lt;on | off&gt;*

**default:** *off*

**context:** *http*

�����Ƿ���Դ�HTTP��������л�ȡIP��Ϣ����ָ����Ҫ���ڲ��ԡ�
�򿪺󣬿���ͨ��get����*ip*ָ��IP��Ϣ���磺 `http://server/url?ip=192.168.1.40 `


ip_from_head
----------
**syntax:** *ip_from_head &lt;on | off&gt;*

**default:** *off*

**context:** *http*

�����ǿ��Դ�HTTP����ͷ��ȡIP��ַ��Ϣ�������ó�onʱ��ģ����ȡX-Real-IP��X-Forwarded-For�е�IP��ַ����nginxǰ���Ǵ���ʱ��ʹ�ø�ѡ�

proxies
----------
**syntax:** *proxies &lt;address | CIDR&gt;*

**default:** *--*

**context:** *http*

���������εĴ����ַ������Ҫ������ͷX-Forwarded-For�л�ȡIP��ַ������nginx�汾����1.3.13ʱ��Ҫʹ�ø�ָ�

proxies_recursive
----------
**syntax:** *proxies_recursive &lt;on | off&gt;*

**default:** *off*

**context:** *http*

If recursive search is disabled, the original client address that matches one of the trusted addresses is replaced by the last address sent in the request header field defined by the real_ip_header directive.

If recursive search is enabled, the original client address that matches one of the trusted addresses is replaced by the last non-trusted address sent in the request header field.

# ��ȡIP�ķ�ʽ
GEOģ���ȡIP��ַ�ĵ�˳�����£�
* ���ip_from_url�����ˣ�������GET����ip�л�ȡ��
* ���ip_from_head�����ˣ�
    * �ȴ�����ͷX-Real-IP�л�ȡ�����û�У�������ͷX-Forwarded-For�л�ȡ��
* �����������ѡ�δ������ʹ��socket�е�ʵ��IP��ַ��

# ������ʹ��
mm_geoָ�����ó�on��������ҵ���Ӧ��GEO��Ϣ�������4���Զ�����Ϣ������ͷ�С�����ͷ��ָ��[mm_geo](#mm_geo)����Щ����ͷ���Ը���������ͷһ����nginx��ģ�������á����ʷ�ʽ�ǣ�$http_����ͷ ��
* ��access log��ʹ�ã�

```nginx
log_format  main  '$http_x_province $http_x_city $http_x_isp xxxx';
```

* ��echoģ����ʹ��

```nginx
location /area {
    # used variables in module
	echo "      ip: $http_x_ip";
	echo "province: $http_x_province";
	echo "    city: $http_x_city";
	echo "     isp: $http_x_isp";
}
```

# ��������ʹ��
##### �����ļ���ʽ
ʹ�ñ���������GEOǰ����׼������Ӧ���ı���ʽ�����ļ����ļ���ʽ���£�
```
################# comment ##################
1.2.3.0  1.2.3.255   BeiJing   BeiJing Unicom
1.2.4.0  1.2.4.255   BeiJing   BeiJing Telecom
1.2.5.0  1.3.5.255   BeiJing   BeiJing Mobile
2.1.1.0  2.1.2.255   HuBei Wuhan Unicom
```
* ��#��ͷ����Ϊע�ͣ���������ֱ�Ӻ��Ե���
* ����һ��5�У��ֱ�Ϊ��IP�ο�ʼֵ����ʼ�ν���ֵ��ʡ�ݣ����У�ISP����֮����һ�������ո�(��Tab)�ָ
* ���ݱ����ǰ�IP�ź���ġ��������ʱ�����ע�⣺���IP���а�����ϵ��ͬ�����ж�Ϊδ����ģ��ᵼ�±������

##### ���������ļ�
```shell
./geoc geodata.txt
--------- Compile geodata.txt OK
--------- Output: geodata.geo
```
�����֮�󣬾Ϳ�����geodata.geoȥmm_geoģ����ʹ���ˡ�

##### ʹ��geot���Զ�����GEO�ļ�
```shell
./geot geodata.geo
#����֮��������ʾ����IP������һ��IP�󣬻���ʾ��ѯ�����
Input a ip:1.2.4.1
> [1.2.4.1] -----------  [1.2.4.0-1.2.4.255 BeiJing BeiJing Telecom]
```

Authors
=======

* liuxiaojie (��С��)  <jie123108@163.com>

[Back to TOC](#Ŀ¼)

Copyright & License
===================

This module is licenced under the BSD license.

Copyright (C) 2014, by liuxiaojie (��С��)  <jie123108@163.com>

All rights reserved.
