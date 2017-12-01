# Gather good pairs of images with chessboard calibration pattern
# using FxIS and EPypes

import sys
import os
import time
import numpy as np
import cv2

PHD_CODE = os.environ['PHD_CODE']
sys.path.append(os.getcwd())
sys.path.append(os.path.join(PHD_CODE, 'EPypes'))
sys.path.append(os.path.join(PHD_CODE, 'RPALib'))

from grabber import AVTGrabber
from fxisext import get_timestamps_snaphot, get_timepoints

from rpa import cbcalib
from epypes import pipeline
from epypes import compgraph
from epypes.queue import Queue

def get_timing_measurements(resp):

    t_snap = np.array(get_timestamps_snaphot(resp))
    t_read = np.array(get_timepoints(resp))

    return t_snap, t_read

def print_grab_missync(resp_1, resp_2):

    t_snap_1 = get_timepoints(resp_1)
    t_snap_2 = get_timepoints(resp_2)

    t_ms = abs(t_snap_1[0] - t_snap_2[0]) * 1e-6

    print('Missync: {:.3f} ms'.format(t_ms))


def save_data(index, responses, runner):

    def save_one(resp, suffix):

        im = runner['image_' + suffix]
        t_snap, t_read = get_timing_measurements(resp)

        cv2.imwrite('img_{}_{}.jpg'.format(suffix, index), im)
        np.save('tsnap_{}_{}.npy'.format(suffix, index), t_snap)
        np.save('tread_{}_{}.npy'.format(suffix, index), t_read)

    save_one(responses[0], '1')
    save_one(responses[1], '2')


def main_loop(runner, sleep_time, do_stream=True, do_save=False):

    g = AVTGrabber([0, 1])
    g.start(show_video=do_stream)

    time.sleep(1)

    attempt = 1

    while True:

        try:

            print('Attempt', attempt)

            resp_1, resp_2 = g.grab(meta=True)

            print_grab_missync(resp_1, resp_2)

            t0 = time.time()
            runner.run(
                image_1=np.array(resp_1.image, copy=False),
                image_2=np.array(resp_2.image, copy=False)
            )
            t1 = time.time()
            t_proc = t1 - t0

            success = runner['success_1'] and runner['success_2']

            if success:
                if do_save:
                    save_data(attempt, (resp_1, resp_2), runner)

                print('Success, {:.3f} ms'.format(t_proc * 1e3))
            else:
                print('Fail, {:.3f} ms'.format(t_proc * 1e3))

            attempt += 1
            if t_proc < sleep_time:
                time.sleep(sleep_time)

        except KeyboardInterrupt as e:
            g.stop()
            break


if __name__ == '__main__':

    psize = (15, 9)
    sleep_time = 1
    data_dir = 'data'

    cg_corners_stereo = compgraph.graph_union_with_suffixing(
        cbcalib.CGFindCorners(),
        cbcalib.CGFindCorners(),
        exclude=['pattern_size_wh']
    )

    runner = compgraph.CompGraphRunner(
        cg_corners_stereo,
        frozen_tokens={'pattern_size_wh': psize}
    )

    main_loop(runner, sleep_time, do_stream=False, do_save=False)
