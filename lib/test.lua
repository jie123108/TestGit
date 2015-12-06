local stats = require("resty.stats")

local mongo_cfg = {host="127.0.0.1", port=27017, dbname="ngx_stats"}
local flush_interval = 1
stats.init(mongo_cfg, flush_interval)

local var = {uri="test", status=200, request_time=0.003}
for i=0, 5 do 
	stats.log(var)
	ngx.sleep(1)
end