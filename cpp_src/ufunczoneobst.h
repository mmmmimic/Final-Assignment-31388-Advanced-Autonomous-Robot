/***************************************************************************
 *   Copyright (C) 2005 by Christian Andersen and DTU                             *
 *   jca@oersted.dtu.dk                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef UFUNC_MRCOBST_H
#define UFUNC_MRCOBST_H
/////CONSTANTS
#define PI 3.1415926
/////

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <math.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ulms4/ufunclaserbase.h>


using namespace std;

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/**
 * Laserscanner function to demonstrate
 * simple laser scanner data handling and analysis
 * @author Christian Andersen
*/
class UFunczoneobst : public UFuncLaserBase
{
public:
  /**
  Constructor */
  UFunczoneobst()
  { // set the command (or commands) handled by this plugin
    setCommand("zoneobst", "zoneobstif", "obstacle detect for MRC (Compiled " __DATE__ " " __TIME__ ")");
    createBaseVar();
  }
  /**
  Handle incomming command
  (intended for command separation)
  Must return true if the function is handled -
  otherwise the client will get a failed - reply */
  virtual bool handleCommand(UServerInMsg * msg, void * extra);
  
  protected:
    void createBaseVar();
    UVariable *var_zone;
    vector<double> point_x, point_y;
    vector<double> vex_x,vex_y;
    void scanit(ULaserData * data, vector<double> &x_laser, vector<double> &y_laser);
    void transform(vector<double> &x_laser, vector<double> &y_laser, double pose[3]);
    void getPoints();
    void removeDuplicates(vector<double> &cand_x, vector<double> &cand_y);
    double calcDist(double x1, double y1, double x2, double y2);
    void sort(vector<double> &vec);
    double calcAngle(int idx1, int idx2);
};

#endif


