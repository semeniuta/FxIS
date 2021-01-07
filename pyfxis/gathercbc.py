"""
Gather good pairs of images with chessboard calibration pattern
using FxIS and visionfuncs/EPypes
"""

import sys
import os
import time
import numpy as np
import argparse
import cv2

CODEROOT = os.environ['CODEROOT']
sys.path.append(os.getcwd())
sys.path.append(os.path.join(CODEROOT, 'visionfuncs'))
sys.path.append(os.path.join(CODEROOT, 'EPypes'))

from grabber import AVTGrabber
from fxisext import get_timestamps_snaphot, get_timepoints
from visioncg import cbcalib

def get_timing_measurements(resp):

    t_snap = np.array(get_timestamps_snaphot(resp))
    t_read = np.array(get_timepoints(resp))

    return t_snap, t_read

def print_grab_missync(resp_1, resp_2):

    t_snap_1 = get_timepoints(resp_1)
    t_snap_2 = get_timepoints(resp_2)

    t_ms = abs(t_snap_1[0] - t_snap_2[0]) * 1e-6

    print('Missync: {:.3f} ms'.format(t_ms))


def find_corners(im1, im2, pattern_size_wh, flags):

    def fc(im):
        return cbcalib.find_cbc(im, pattern_size_wh, findcbc_flags=flags)

    found_1, corners_1 = fc(im1)
    found_2, corners_2 = fc(im2)

    success = found_1 and found_2

    return success, corners_1, corners_2


def save_data(index, im_1, im_2, resp_1, resp_2, save_dir):

    def save_one(im, resp, suffix):

        #t_snap, t_read = get_timing_measurements(resp)

        path_im = os.path.join(save_dir, 'img_{}_{}.jpg'.format(suffix, index))
        #path_tsnap = os.path.join(save_dir, 'tsnap_{}_{}.npy'.format(suffix, index))
        #path_tread = os.path.join(save_dir, 'tread_{}_{}.npy'.format(suffix, index))

        cv2.imwrite(path_im, im)
        #np.save(path_tsnap, t_snap)
        #np.save(path_tread, t_read)

    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    save_one(im_1, resp_1, '1')
    save_one(im_2, resp_2, '2')


def main_loop(psize, sleep_time, save_dir, do_stream=True, do_save=False):

    g = AVTGrabber([0, 1])
    g.start(show_video=do_stream)

    time.sleep(1)

    attempt = 1

    while True:

        try:

            print('Attempt', attempt)

            resp_1, resp_2 = g.grab(meta=True)

            print_grab_missync(resp_1, resp_2)

            im_1 = np.array(resp_1.image, copy=False)
            im_2 = np.array(resp_2.image, copy=False)

            im_1 = im_1.reshape(im_1.shape[0], im_1.shape[1])
            im_2 = im_2.reshape(im_2.shape[0], im_2.shape[1])

            t0 = time.time()

            flags = cv2.CALIB_CB_FAST_CHECK
            success, corners_1, corners_2 = find_corners(im_1, im_2, psize, flags)

            t1 = time.time()
            t_proc = t1 - t0

            if success:
                if do_save:
                    save_data(attempt, im_1, im_2, resp_1, resp_2, save_dir)

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

    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('--dir', help='directory to save images')
    arg_parser.add_argument('--ph', type=int, help='horizontal pattern size')
    arg_parser.add_argument('--pv', type=int, help='vertical pattern size')
    arg_parser.add_argument('--stream', action='store_true', help='launch video streaming')
    arg_parser.add_argument('--save', action='store_true', help='save images')
    args = arg_parser.parse_args()

    psize = (args.ph, args.pv)
    sleep_time = 1

    save_dir = 'data' if args.dir is None else args.dir

    main_loop(psize, sleep_time, save_dir, do_stream=args.stream, do_save=args.save)
