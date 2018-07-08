import os
import sys
import time
import random

sys.path.append(os.getcwd())
from grabber import AVTGrabber

if __name__ == '__main__':

    g = AVTGrabber([2])

    g.start()

    for i in range(4):
        time.sleep(1 + random.random())
        resp = g.grab(meta=True)[0]

        print()
        print(resp.target_index, resp.current_index)
        print(resp.timestamps_snapshot)

    g.stop()
