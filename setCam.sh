
#!/bin/sh

/usr/bin/v4l2-ctl -c brightness=-64
/usr/bin/v4l2-ctl -c contrast=95
/usr/bin/v4l2-ctl -c saturation=0
/usr/bin/v4l2-ctl -c hue=2000
/usr/bin/v4l2-ctl -c white_balance_temperature_auto=0
/usr/bin/v4l2-ctl -c white_balance_temperature=4000
/usr/bin/v4l2-ctl -c gamma=100
/usr/bin/v4l2-ctl -c power_line_frequency=1
/usr/bin/v4l2-ctl -c sharpness=5
/usr/bin/v4l2-ctl -c backlight_compensation=1
/usr/bin/v4l2-ctl -c exposure_auto=1
/usr/bin/v4l2-ctl -c exposure_absolute=5000
/usr/bin/v4l2-ctl --set-fmt-video=width=640,height=360,pixelformat=YUYV4
