import cv2
import numpy as np
import fxisext2 as fx

im = np.array(
    fx.open_image('../../UntilFocus/data/untilfocus_1/14.png', True)
)

cv2.imwrite('image.jpg', im)