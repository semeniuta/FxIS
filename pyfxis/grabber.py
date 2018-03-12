from fxisext import AVTSimpleGrabService
import numpy as np

class AVTGrabber:

    def __init__(self, cam_indices, stream_size=10, n_frames=3):

        self._service = AVTSimpleGrabService()

        self._stream_size = stream_size
        self._cam_params = [{'camera_index': ci, 'n_frames': n_frames} for ci in cam_indices]

    def start(self, show_video=False):

        self._service.init(self._stream_size, self._cam_params, show_video)
        self._service.start()

    def stop(self):
        self._service.stop()

    def __enter__(self):

        self.start()
        return self

    def __exit__(self, type, value, traceback):
        self.stop()

    def grab(self, meta=False):

        responses = self._service.grab()

        if not meta:
            return tuple(np.array(resp.image, copy=False) for resp in responses)

        return responses
