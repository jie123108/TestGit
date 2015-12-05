Name
====

nginx-multi-disk - Let nginx support multi directory (multi disk) and cache directory, directory selection algorithm using the consistency of Hash.

[中文版说明](README-cn.md)

Synopsis
========

```lua
# nginx.conf

http {
    lua_package_path '/path/to/nginx-multi-disk/?.lua;;';

    init_worker_by_lua '
        local multidir = require("resty.multidir")
        multidir.init({["/data/disk01"]=2, ["/data/disk02"]=1}, "/data/cache")
    ';

    server {
        ...
        # Get file storage directory, write files to the disk use
        location = /getsavedir {
            set_by_lua $root_dir '
                if ngx.var.arg_url == nil or ngx.var.arg_url == "" then
                    return ""
                end
                local multidir = require "resty.multidir"
                local url = ngx.var.arg_url or ""
                local root_dir, x_cache = multidir.get_path_by_uri(url)
                ngx.header["X-Cache"] = x_cache
                ngx.log(ngx.INFO, "set root_dir:", root_dir)
                return root_dir                
            ';
            root $root_dir;

            content_by_lua '                
                local url = ngx.var.arg_url or ""
                ngx.say(ngx.var.document_root .. url)
            ';
        }

        # Download File
        location / {
            set_by_lua $root_dir '
                local multidir = require "resty.multidir"
                local root_dir, x_cache = multidir.get_path_by_uri(ngx.var.uri)
                ngx.header["X-Cache"] = x_cache
                ngx.log(ngx.INFO, "set root_dir:", root_dir)
                return root_dir                
            ';
            root $root_dir;
        }
    }
}

```

Description
===========
The standard nginx default is not supported by multi directory (multi disk), the need to do raid or other ways to support. The library is to let nginx support multi directory and cache directory support. Multi directory selection using the consistency of Hash. support weight settings. This module requires ngx_lua.


Methods
=======

Load module

* To use the library, the first to set the ngx_lua's environment variable:

```lua
lua_package_path '/path/to/nginx-multi-disk/?.lua;;';
```

* Need to load the module with require to a local variable:

```lua
local multidir = require("resty.multidir")
```


init
---
* `syntax: multidir.init(multi_dir_table, cachedir)`

Initialize multi directory module

* `multi_dir_table` 
Specify the storage directory and directory weights. You can only give a directory, default weight is 1.
    * initialization with weights:  `multidir.init({["dir01"]=weight, ["dir02"]=weight})`
    * initialization without weights:  `multidir.init({"dir01""dir02"})`
* `cachedir`
Specify the cache directory, which can be empty. 
After specifying the directory, when the HTTP request is coming, if the cache directory contains the request file, return the cache directory, and then read file date from the cache directory.


Prerequisites
=============

* [LuaJIT](http://luajit.org) 2.0+
* [ngx_lua](https://github.com/chaoslawful/lua-nginx-module) 0.8.10+

Authors
=======

* liuxiaojie (刘小杰)  <jie123108@163.com>

[Back to TOC](#table-of-contents)

Copyright & License
===================

This module is licenced under the BSD license.

Copyright (C) 2014, by liuxiaojie (刘小杰)  <jie123108@163.com>

All rights reserved.
