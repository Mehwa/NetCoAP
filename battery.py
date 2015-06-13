import time
full = 0;
now = 0;
while True:
	with open('/sys/class/power_supply/BAT0/charge_full', "rt") as f:
	    full = int(f.read())
	with open('/sys/class/power_supply/BAT0/charge_now', "rt") as f:
	    now = int(f.read())
	with open('battery',"wt") as f:
	    f.write(str(round(now/full*100)))
	time.sleep(1)
