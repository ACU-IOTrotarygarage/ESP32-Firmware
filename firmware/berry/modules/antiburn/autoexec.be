# Register the command 'Antiburn'
# Module loaded in memory only when the command is first used

var wd = Firmware.wd

lv.antiburn = def()
    import sys
    var path = sys.path()
    path.push(wd)
    import antiburn
    path.pop()
    antiburn.start()
end

Firmware.add_cmd("Antiburn",
    def ()
        lv.antiburn()
        Firmware.resp_cmnd_done()
    end
)
