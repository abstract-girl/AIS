-- local frandom = io.open("/dev/urandom", "rb")
-- local d = frandom:read(4)
-- math.randomseed(d:byte(1) + (d:byte(2) * 256) + (d:byte(3) * 65536) + (d:byte(4) * 4294967296))

local json = require 'cjson'

local ids = {}

local file = io.open('ids.json', 'r')
if file then
    ids = json.decode(file:read('*all'))
    file:close()
end

math.randomseed(os.time())

request = function()
    headers = {}
    headers['Content-Type'] = 'application/json'
    body = ''
    local id = ids[math.random(#ids)]
    return wrk.format('GET', '/users/search?no_cache&id='.. id, headers, body)
end
