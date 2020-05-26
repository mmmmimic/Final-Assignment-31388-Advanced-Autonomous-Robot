#! /usr/bin/python
# -*- coding: UTF-8 -*-
'''
author: group 16
date: 05/26/2020
'''
import random
import numpy as np
from abc import ABCMeta, abstractmethod
import math
import os
import shutil
import yaml

###---CONSTANTS---###
BOUND = (1,1,3,2)
RECT_NUMBER = 2
RECT_HEIGHT = [0.15, 0.2]
RECT_WIDTH = [0.4, 0.3]
TRIAN_NUMBER = 2
TRIAN_HEIGHT = [0.1, 0.15]
TRIAN_WIDTH = [0.4, 0.3]
PROJ_NAME = "./388project"
MRC_NAME = "./mrc_src/createWalls"
###---------------###

## read configuration
def read_config(config_path):
    with open(config_path, 'r') as f:
        cfg = yaml.load(f.read(), Loader=yaml.FullLoader)
    return cfg or {}

# initialize logfile
if os.path.exists('./spawnLog.log'):
    os.remove('./spawnLog.log')

# create temp files for revision
shutil.copy(PROJ_NAME, PROJ_NAME+'_temp')
shutil.copy(MRC_NAME, MRC_NAME+'_temp')

# create objects
class MyObj(object):
    '''
    object
    '''
    def __init__(self, bound, width, height):
        super(MyObj,self).__init__()
        self.bound = bound
        self.width = width
        self.height = height
        self.origin = []
        self._points = []
        self.makePoints()
        self.writeText()

    @abstractmethod
    def makePoints(self):
        pass

    @property
    def points(self):
        return self._points

    @staticmethod
    def rotate(point, angle):
        return np.array([[math.cos(angle), -math.sin(angle)],[math.sin(angle), math.cos(angle)]]).dot(np.array([[point[0]],[point[1]]]))
    
    def writeText(self):
        '''
        generate the text to write
        '''
        text = '\n'
        for i in range(len(self._points)-1):
            text+= str(round(self._points[i][0], 2))+'\t'+str(round(self._points[i][1], 2))+'\t'+str(round(self._points[i+1][0], 2))+'\t'+str(round(self._points[i+1][1], 2))+'\n'
        with open(PROJ_NAME,'a+') as f:
            f.write(text)
        text = '\n'
        for i in range(len(self._points)-1):
            text+= "laser\t\"addline\tstartx="+str(round(self._points[i][0], 2))+'\tstarty='+str(round(self._points[i][1], 2))+'\tendx='+str(round(self._points[i+1][0], 2))+'\tendy='+str(round(self._points[i+1][1], 2))+'\tname=\''+str(random.random())+'\'\"\n'
        with open(MRC_NAME,'a+') as f:
            f.write(text)

    


class Rect(MyObj):
    '''
    Rectangle
    ''' 
    def __init__(self, bound, width, height):
        super(Rect,self).__init__(bound, width, height)

    def makePoints(self):
        # diagonal of the rectangle
        ax = np.linalg.norm([self.width, self.height], ord=2)/2
        roi = (self.bound[0]+ax, self.bound[1]+ax, self.bound[2]-ax, self.bound[3]-ax)
        # rotating angle
        angle = random.uniform(0, math.pi)
        # center
        o = np.array([[random.uniform(roi[0], roi[2])], [random.uniform(roi[1], roi[3])]])
        # left-lower
        p1 = self.rotate([-self.width/2, -self.height/2], angle)+o
        # right-lower
        p2 = self.rotate([self.width/2, -self.height/2], angle)+o
        # right-upper
        p3 = self.rotate([self.width/2, self.height/2], angle)+o
        # left-upper
        p4 = self.rotate([-self.width/2, self.height/2], angle)+o
        self._points = [p1,p2,p3,p4,p1]
        self.angle = angle
        self.origin = o
        

class Trian(MyObj):
    '''
    Triangle
    '''
    def __init__(self, bound, width, height):
        super(Trian, self).__init__(bound, width, height)

    def makePoints(self):
        roi = (self.bound[0]+self.width, self.bound[1]+self.width, self.bound[2]-self.width, self.bound[3]-self.width)
        # rotating angle
        angle = random.uniform(0, math.pi)
        # center
        o = np.array([[random.uniform(roi[0], roi[2])], [random.uniform(roi[1], roi[3])]])
        with open('./spawnLog.log', 'a+') as f:
            f.write('center: '+str(o.reshape((1,-1)))+'\twidth: '+str(self.width)+'\theight: '+str(self.height)+'\tangle: '+str(angle))
        # right
        p1 = self.rotate([self.width, 0], angle)+o
        # upper
        p2 = self.rotate([0, self.height], angle)+o
        self._points = [o,p1,p2,o]
        self.angle = angle
        self.origin = o

def spawn():
    # read confiugrations
    cfg = read_config('./objConfig.yaml')
    RANDOM = cfg['RANDOM']
    INDEX = cfg['INDEX']
    shape_bin = ['Rectangle','Triangle']
    if RANDOM:
        # shape of the object
        shape = random.randint(0,1)
        # size of the object
        idx = random.randint(0,1)
    else:
        shape = INDEX//3
        idx = 1-INDEX%2
    if shape:
        # rectangle
        obj = Rect(BOUND, RECT_WIDTH[idx], RECT_HEIGHT[idx])
    else:
        obj = Trian(BOUND, TRIAN_WIDTH[idx], TRIAN_HEIGHT[idx])
    # write log files
    with open('./spawnLog.log', 'a+') as f:
        f.write('shape: '+shape_bin[shape]+'\tcenter: '+str(obj.origin.reshape((1,-1)))+'\twidth: '+str(obj.width)+'\theight: '+str(obj.height)+'\tangle: '+str(obj.angle))

if __name__ == "__main__":
    spawn()




 




