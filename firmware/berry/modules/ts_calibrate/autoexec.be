# register the command 'DisplayCalibrate'
#
# load the module in memory only when the command is first used
#
var wd = Firmware.wd                 # capture value of the first run
Firmware.add_cmd("DisplayCalibrate",
  def ()
    import sys
    var path = sys.path()

    path.push(wd)
    import ts_calibrate
    path.pop()

    Firmware.set_timer(0, /-> ts_calibrate.start())
    Firmware.resp_cmnd_done()
  end
)
