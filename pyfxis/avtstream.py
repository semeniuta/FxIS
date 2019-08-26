import argparse

from grabber import AVTGrabber


if __name__ == '__main__':

    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('camera_indices', type=int, nargs='+')
    args = arg_parser.parse_args()

    # TODO Check for validity of camera_indices

    g = AVTGrabber(args.camera_indices)
    g.start(show_video=True)

    input("Press Enter to quit ...")

    g.stop()


