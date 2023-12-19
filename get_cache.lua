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
    return wrk.format('GET', '/users/search?id='.. id, headers, body)
end