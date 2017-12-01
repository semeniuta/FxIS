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


def in_loop_save(index, responses, runner):

    def save_one(resp, suffix):

        im = runner['image_' + suffix]
        t_snap = np.array(get_timestamps_snaphot(resp))
        t_read = np.array(get_timepoints(resp))

        cv2.imwrite('img_{}_{}.jpg'.format(suffix, index), im)
        np.save('tsnap_{}_{}.npy'.format(suffix, index), t_snap)
        np.save('tread_{}_{}.npy'.format(suffix, index), t_read)

    save_one(responses[0], '1')
    save_one(responses[1], '2')


def main_loop(runner):

    g = AVTGrabber([0, 1])
    g.start(show_video=True)

    time.sleep(1)

    attempt = 1

    while True:

        try:

            print('Attempt', attempt)

            resp_1, resp_2 = g.grab(meta=True)

            t0 = time.time()
            runner.run(
                image_1=np.array(resp_1.image, copy=False),
                image_2=np.array(resp_2.image, copy=False)
            )
            t1 = time.time()

            success = runner['success_1'] and runner['success_2']
            if success:
                #in_loop_save(attempt, (resp_1, resp_2), runner)
                print('Success:', t1 - t0)
            else:
                print('Fail:', t1 - t0)

            attempt += 1

        except KeyboardInterrupt as e:

            g.stop()
            break


cg_corners_stereo = compgraph.graph_union_with_suffixing(
    cbcalib.CGFindCorners(),
    cbcalib.CGFindCorners(),
    exclude=['pattern_size_wh']
)

if __name__ == '__main__':

    psize = (15, 9)

    runner = compgraph.CompGraphRunner(
        cg_corners_stereo,
        frozen_tokens={'pattern_size_wh': psize}
    )

    main_loop(runner)

    # pipe = pipeline.SinkPipeline(
    #     'StereoChessboardDetector',
    #     cg_corners_stereo,
    #     q_images,
    #     dispatch_from_q,
    #     frozen_tokens={'pattern_size_wh': psize}
    # )
