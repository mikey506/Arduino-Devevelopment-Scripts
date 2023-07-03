-- Load necessary libraries
require("credentials")
require("lfs")
require("httpserver")

-- Configure WiFi
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID, PASSWORD)

-- Configure SSH
ssh = net.createServer(net.TCP)
ssh:listen(22, function(conn)
    conn:on("receive", function(conn, payload)
        -- Handle SSH data received
    end)
    conn:on("sent", function(conn)
        -- Handle SSH data sent
    end)
end)

-- Initialize debugging information
local cpuTotal, cpuUsed, memTotal, memUsed, storageTotal, storageUsed

-- Function to update debugging information
local function updateDebugInfo()
    -- Update CPU information
    cpuTotal = node.getcpufreq()
    cpuUsed = node.getcpuusage()

    -- Update memory information
    collectgarbage()
    memUsed = collectgarbage("count")

    -- Update storage information
    storageTotal = file.fsinfo()["total"]
    storageUsed = file.fsinfo()["used"]

    -- Update WiFi module information
    local wifiMode = wifi.getmode()
    local wifiStatus = wifi.sta.status()
    local wifiIP = wifi.sta.getip()

    -- Print debugging information
    print("CPU: " .. cpuTotal .. " MHz (Used: " .. cpuUsed .. "%)")
    print("Memory: " .. memTotal .. " KB (Used: " .. memUsed .. " KB)")
    print("Storage: " .. storageTotal .. " KB (Used: " .. storageUsed .. " KB)")
    print("WiFi Mode: " .. wifiMode)
    print("WiFi Status: " .. wifiStatus)
    print("WiFi IP: " .. wifiIP)
end

-- Create an HTTP server
local server = httpserver.create()

-- Define request handlers
server:use('/', function(req, res)
    -- Handle root path request
    res:send("Hello, world!")
end)

server:use('/debug', function(req, res)
    -- Handle debug information request
    updateDebugInfo()
    res:send("Debug information updated.")
end)

-- Start the HTTP server
server:listen(80)

-- Main loop
tmr.create():alarm(5000, tmr.ALARM_AUTO, function()
    -- Handle HTTPd data (received/sent)
    local httpReceived = server.stats.requests
    local httpSent = server.stats.responses

    -- Handle SSHd data (received/sent)
    -- Update the values for sshReceived and sshSent

    -- Print data statistics
    print("HTTPd data - Received: " .. httpReceived .. ", Sent: " .. httpSent)
    -- Print SSHd data statistics

    -- Update debug information periodically
    updateDebugInfo()
end)
