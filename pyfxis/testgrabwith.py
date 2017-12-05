import grabber
import time

with grabber.AVTGrabber([0, 1]) as g:

    for i in range(5):
        time.sleep(2)
        res = g.grab()
