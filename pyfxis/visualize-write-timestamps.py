import os
import sys
import time
import argparse

sys.path.append(os.getcwd())

import grabber
import timeviz
from fxisext import get_timepoints

if __name__ == '__main__':

    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('--file', '-f')
    arg_parser.add_argument('--stream', '-s', action='store_true')
    args = arg_parser.parse_args()

    g = grabber.AVTGrabber([0, 1])
    g.start(show_video=args.stream)
    time.sleep(2)

    responses = g.grab(meta=True)

    try:
        timeviz.save_timestamps_snapshot_viz(responses, file_name=args.file)
    except Exception as e:
        print(e)

    time.sleep(5)
    g.stop()
