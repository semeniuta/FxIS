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
    arg_parser.add_argument('--file', '-f', default='timeviz.pdf')
    arg_parser.add_argument('--stream', '-s', action='store_true')
    arg_parser.add_argument('--cameras', nargs='+', type=int, default=[0])
    args = arg_parser.parse_args()
    print(args)

    g = grabber.AVTGrabber(args.cameras)
    g.start(show_video=args.stream)
    time.sleep(2)

    responses = g.grab(meta=True)

    try:
        timeviz.save_timestamps_snapshot_viz(responses, file_name=args.file)
    except Exception as e:
        print(e)

    time.sleep(5)
    g.stop()
