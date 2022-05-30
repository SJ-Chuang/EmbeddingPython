import cv2
import numpy as np

def dot(a: int, b: int):
    """
    Returns:
        the multiplication of two integers.
    """
    return a * b

def gaussianblur(height: int, width: int, imageList):
    """
    Apply Gaussian Blur on an image.
    Args:
        height (int): the height of the input image
        width (int): the width of the input image
        imageList (List): 1-D image values.
    
    Returns:
        a flatten image list
    """
    img = np.array(imageList, np.uint8).reshape(height, width, 3)
    blur = cv2.GaussianBlur(img, (21, 21), 0)
    return blur.ravel().tolist()