from fxisext import AVTSimpleGrabService
import numpy as np

class AVTGrabber:

    def __init__(self, cam_indices, stream_size=10, n_frames=3):

        self._service = AVTSimpleGrabService()

        self._stream_size = stream_size
        self._cam_indices = cam_indices
        self._n_frames = n_frames

    def start(self, show_video=False):

        cam_params = [{'camera_index': ci, 'n_frames': self._n_frames} for ci in self._cam_indices]

        self._service.init(self._stream_size, cam_params, show_video)
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

    def __repr__(self):

        class_name = self.__class__.__name__
        return '{0}{1}'.format(class_name, self._cam_indices)
