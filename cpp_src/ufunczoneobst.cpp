/***************************************************************************
 *   Copyright (C) 2005 by Christian Andersen and DTU                      *
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
#include "ufunczoneobst.h"

#ifdef LIBRARY_OPEN_NEEDED

/**
 * This function is needed by the server to create a version of this plugin */
UFunctionBase * createFunc()
{ // create an object of this type
  /** replace 'UFunczoneobst' with your class name */
  return new UFunczoneobst();
}
#endif

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
// #define SMRWIDTH 0.4
bool UFunczoneobst::handleCommand(UServerInMsg * msg, void * extra)
{  // handle a plugin command
  //const int MRL = 500;
  //char reply[MRL];
  vector<double> x_laser, y_laser;
  //---------useful commands
  //bool ask4help=false;
  bool ifDetect=false;
  bool ifRecog = false;
  //------------------------
  const int MVL = 30;
  char value[MVL];
  ULaserData * data;
  double pose[3]={0};

  //double r,delta;
  //double minRange; // min range in meter
  // double minAngle = 0.0; // degrees
//   double d,robotwidth;
  //double zone[9];
  // check for parameters - one parameter is tested for - 'help'
  //ask4help = msg->tag.getAttValue("help", value, MVL);
  ifDetect = msg->tag.getAttValue("detect", value, MVL);
  ifRecog = msg->tag.getAttValue("recog", value, MVL);
  /*if (ask4help)
  { // create the reply in XML-like (html - like) format
    sendHelpStart(msg, "zoneobst");
    sendText("--- available zoneobst options\n");
    sendText("help            This message\n");
    sendText("fake=F          Fake some data 1=random, 2-4 a fake corridor\n");
    sendText("device=N        Laser device to use (see: SCANGET help)\n");
    sendText("see also: SCANGET and SCANSET\n");
    sendHelpDone();
  }
  else
  { // do some action and send a reply
    data = getScan(msg, (ULaserData*)extra);
    //
    if (data->isValid())
    { // make analysis for closest measurement
      minRange = 1000; // long range in meters
      fov=data->getRangeCnt();
      delta=fov/9.0;
      for (j=0;j<9;j++)
	 zone[j]=minRange;
      for(j=0;j<9;j++){
      for (i = 0+(int)(j*delta); i < (int)((j+1)*delta); i++)
      { // range are stored as an integer in current units
	r = data->getRangeMeter(i);
        if (r >= 0.020)
        { // less than 20 units is a flag value for URG scanner
          if (r<zone[j])
	     zone[j]=r;
        }
      }
      }
  */
      /* SMRCL reply format */
/*
      snprintf(reply, MRL, "<laser l0=\"%g\" l1=\"%g\" l2=\"%g\" l3=\"%g\" l4=\"%g\" "
                                  "l5=\"%g\" l6=\"%g\" l7=\"%g\" l8=\"%g\" />\n", 
	                   zone[0],zone[1],zone[2],zone[3],zone[4],
                           zone[5],zone[6],zone[7],zone[8]);
      // send this string as the reply to the client
      sendMsg(msg, reply);

      // save also as global variable
      for(i = 0; i < 9; i++)
        var_zone->setValued(zone[i], i);
    }
    else
      sendWarning(msg, "No scandata available");
  }
*/
  if (ifDetect)
  {
  // store data from laser in a log file
  data = getScan(msg, (ULaserData*)extra);
  // get data from the laser
  scanit(data, x_laser, y_laser);
  // get current pose
  msg->tag.getAttValue("x", value, MVL);
  pose[0] = strtod(value, NULL);
  msg->tag.getAttValue("y", value, MVL);
  pose[1] = strtod(value, NULL);
  msg->tag.getAttValue("th", value, MVL);
  pose[2] = strtod(value, NULL);
  // transform the coordinates from laser frame to world frame
  transform(x_laser, y_laser, pose);
  ifDetect = false;
  }  
  else if(ifRecog)
  {
    getPoints();
    // output the info on the object
    // origion coordinates, width, height, orientation
    if (vex_x.size()==3)
    {
      // if there are 3 vex points, then triangle
      cout<<"Triangle"<<endl;
      vector<double> dist;
      double center_x = 0;
      double center_y = 0;
      double width = 0;
      double height = 0;
      double angle = 0;
      int oriIdx = 0;
      dist.push_back(calcDist(vex_x[0], vex_y[0], vex_x[1], vex_y[1]));
      dist.push_back(calcDist(vex_x[1], vex_y[1], vex_x[2], vex_y[2]));
      dist.push_back(calcDist(vex_x[2], vex_y[2], vex_x[0], vex_y[0]));
      vector<double>::iterator p = min_element(dist.begin(),dist.end());
      double dist_min = *p;
      p = max_element(dist.begin(),dist.end());
      double dist_max = *p;
      // get width ang height of the object
      for (int i = 0;(double)i<dist.size();i++)
      {
        if(dist[i]==dist_min)
        {
          height = dist[i];
        }
        if((dist[i]!=dist_min) & (dist[i]!=dist_max))
        {
          width = dist[i];
        }
      }
      // determine origin coordinates
      if (((dist[0]==width) & (dist[2]==height)) | ((dist[0]==height) & (dist[2]==width)))
      {
        center_x = vex_x[0];
        center_y = vex_y[0];
        oriIdx = 0;
      }
      if (((dist[0]==width) & (dist[1]==height)) | ((dist[0]==height) & (dist[1]==width)))
      {
        center_x = vex_x[1];
        center_y = vex_y[1];
        oriIdx = 1;
      }
      if (((dist[1]==width) & (dist[2]==height)) | ((dist[1]==height) & (dist[2]==width)))
      {
        center_x = vex_x[2];
        center_y = vex_y[2];
        oriIdx = 2;
      }      
      // calculate rotation angle
      if (dist[0]==width)
      {
        // then we use the line connecting point 0 and 1
        if(oriIdx==0)
        {
          angle = calcAngle(0,1);
        }
        else
        {
          angle = calcAngle(1,0);
        }
      }
      if (dist[1]==width)
      {
        // then we use the line connecting point 1 and 2
        if(oriIdx==1)
        {
          angle = calcAngle(1,2);
        }
        else
        {
          angle = calcAngle(2,1);
        }
      }
      if (dist[2]==width)
      {
        // then we use the line connecting point 0 and 2
        if(oriIdx==2)
        {
          angle = calcAngle(2,0);
        }
        else
        {
          angle = calcAngle(0,2);
        }
      }
      cout<<"angle "<<angle<<endl;
      cout<<"origin"<<center_x<<'\t'<<center_y<<endl;
      cout<<width<<'\t'<<height<<'\n';
      // write logs
      ofstream file("detectResult.log", ios::trunc);
      file << "shape: Triangle" << '\t'<<"center: ["<<center_x<<'\t'<<","<<center_y<<']'<<'\t'<<"width: "<<width<<'\t'<<"height: "<<height<<'\t'<<"angle: "<<angle<<'\t'<<'\n';
      file.close();
    }
    else if (vex_y.size()==4)
    {
      cout<<"Rectangle"<<endl;
      vector<double> dist;
      double angle1, angle2, angle;
      // determine origin coordinares
      double center_x = (vex_x[0]+vex_x[1]+vex_x[2]+vex_x[3])/4;
      double center_y = (vex_y[0]+vex_y[1]+vex_y[2]+vex_y[3])/4;
      dist.push_back(calcDist(vex_x[0], vex_y[0], vex_x[2], vex_y[2]));
      dist.push_back(calcDist(vex_x[0], vex_y[0], vex_x[3], vex_y[3]));
      dist.push_back(calcDist(vex_x[1], vex_y[1], vex_x[2], vex_y[2]));
      dist.push_back(calcDist(vex_x[1], vex_y[1], vex_x[3], vex_y[3]));
      sort(dist);
      // get width ang height of the object
      double width = dist[2];
      double height = dist[0];
      cout<<"origin"<<center_x<<'\t'<<center_y<<endl;
      cout<<width<<'\t'<<height<<'\n';
      // get angle
      if(calcDist(vex_x[0], vex_y[0], vex_x[2], vex_y[2]) == width)
      {
        angle1 = calcAngle(0,2);
        if(angle1<0)
        {angle1 += 180;}
        angle2 = calcAngle(1,3);
        if(angle2<0)
        {angle2 += 180;}
        angle = (angle1+angle2)/2;
      }
      if(calcDist(vex_x[0], vex_y[0], vex_x[3], vex_y[3]) == width)
      {
        angle1 = calcAngle(0,3);
        if(angle1<0)
        {angle1 += 180;}
        angle2 = calcAngle(1,2);
        if(angle2<0)
        {angle2 += 180;}
        angle = (angle1+angle2)/2;
      }
      cout<<"angle: "<<angle<<endl;
      // write logs
      ofstream file("detectResult.log", ios::trunc);
      file << "shape: Rectangle" << '\t'<<"center: ["<<center_x<<'\t'<<","<<center_y<<']'<<'\t'<<"width: "<<width<<'\t'<<"height: "<<height<<'\t'<<"angle: "<<angle<<'\t'<<'\n';
      file.close();
    }
    ifRecog = false;
  }

  // return true if the function is handled with a positive result
  // used if scanpush or push has a count of positive results
  return true;
}

void UFunczoneobst::createBaseVar()
{ // add also a global variable (global on laser scanner server) with latest data
  var_zone = addVarA("zone", "0 0 0 0 0 0 0 0 0", "d", "Value of each laser zone. Updated by zoneobst.");
}
void UFunczoneobst::scanit(ULaserData * data, vector<double> &x_laser, vector<double> &y_laser)
/**
 * call laser scan
**/
{
// field of view of the laser
int fov = data->getRangeCnt();
double maxDist = 1000;
double dist = 0;
//cout<<"fov is\t"<<fov<<endl;

// loop over the angles
// get point Cartesian coordinates
for(int i = 0; i<fov; i++)
{
  dist = data->getRangeMeter(i);
  dist = dist>maxDist?maxDist:dist;
  x_laser.push_back(dist*cos(data->getAngleRad(i)));
  y_laser.push_back(dist*sin(data->getAngleRad(i)));
}
}

void UFunczoneobst::transform(vector<double> &x_laser, vector<double> &y_laser, double pose[3])
/**
 * transfrom the point coordinates from laser frame to world frame
 * Input: 
 * pose: the robot pose
 * x_laser, y_laser: the coordinates in laser frame
 **/ 
{
  double x,y,theta, x_w, y_w;
  x = pose[0];
  y = pose[1];
  theta = pose[2];
  x += 0.26*cos(theta);
  y += 0.26*sin(theta);
  for(int i=0;(double)i<x_laser.size();i++)
  {
    // the laser pose is (0.26, 0, 0) in robot frame
    // transform from laser frame to world frame
    x_w = x_laser[i]*cos(theta)-y_laser[i]*sin(theta)+x;
    y_w = x_laser[i]*sin(theta)+y_laser[i]*cos(theta)+y;
    // we only need the points in the target region
    if ((x_w>=1) & (x_w<=3) & (y_w>=1) & (y_w<=2))
    {
      point_x.push_back(x_w);
      point_y.push_back(y_w);
    }
  }
}

void UFunczoneobst::getPoints()
/**
 * get the left-most, right-most, upper-most and lower-most point in a set of points
 **/ 
{
  vector<double>::iterator p = min_element(point_x.begin(), point_x.end());
  double min_x = *p;
  p = max_element(point_x.begin(), point_x.end());
  double max_x = *p;
  p = min_element(point_y.begin(), point_y.end());
  double min_y = *p;
  p = max_element(point_y.begin(), point_y.end());
  double max_y = *p;
  int left = -1;
  int right = -1;
  int lower = -1;
  int upper = -1;
  vector<double> cand_x, cand_y;
  // find the 4 points
  for(int i = 0;(double)i<point_x.size();i++)
  {
    //left
    if(point_x[i]==min_x)
    {
      if((left==-1))
      {
        left = i;
      }
      else
      {
        left = -2;
      }
      cout<<"left "<<i<<'\t'<<point_x[i]<<'\t'<<point_y[i]<<endl;
    }
    //right
    if(point_x[i]==max_x)
    {
      if((right==-1))
      {
        right = i;
      }
      else
      {
        right = -2;
      }
      cout<<"right "<<i<<'\t'<<point_x[i]<<'\t'<<point_y[i]<<endl;
    }
    //lower
    if(point_y[i]==min_y)
    {
      if((lower==-1))
      {
        lower = i;
      }
      else
      {
        lower = -2;
      }
      cout<<"lower "<<i<<'\t'<<point_x[i]<<'\t'<<point_y[i]<<endl;
    }
    //upper
    if(point_y[i]==max_y)
    {
      if((upper==-1))
      {
        upper = i;
      }
      else
      {
        upper = -2;
      }
      cout<<"upper "<<i<<'\t'<<point_x[i]<<'\t'<<point_y[i]<<endl;
    }
  }
  if((left==-2)&(right==-2)&(lower==-2)&(upper==-2))
  {
    vex_x.push_back(min_x);
    vex_x.push_back(min_x);
    vex_x.push_back(max_x);
    vex_x.push_back(max_x);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);    
  }else if((left==-2)&(lower==-2))
  {
    vex_x.push_back(min_x);
    vex_x.push_back(min_x);
    vex_x.push_back(max_x);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);
    vex_y.push_back(min_y);
  }else if((right==-2)&(lower==-2))
  {
    vex_x.push_back(max_x);
    vex_x.push_back(max_x);
    vex_x.push_back(min_x);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);
    vex_y.push_back(min_y);
  }else if((right==-2)&(upper==-2))
  {
    vex_x.push_back(min_x);
    vex_x.push_back(max_x);
    vex_x.push_back(max_x);
    vex_y.push_back(max_y);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);
  }else if((left==-2)&(upper==-2))
  {
    vex_x.push_back(min_x);
    vex_x.push_back(min_x);
    vex_x.push_back(max_x);
    vex_y.push_back(min_y);
    vex_y.push_back(max_y);
    vex_y.push_back(max_y);
  }else
  {
    cand_x.push_back(point_x[left]);
    cand_y.push_back(point_y[left]);
    cand_x.push_back(point_x[right]);
    cand_y.push_back(point_y[right]);
    cand_x.push_back(point_x[lower]);
    cand_y.push_back(point_y[lower]);
    cand_x.push_back(point_x[upper]);
    cand_y.push_back(point_y[upper]);  
    removeDuplicates(cand_x, cand_y);
  }
}

void UFunczoneobst::removeDuplicates(vector<double> &cand_x, vector<double> &cand_y)
/**
 * if two of the points are close to each other, use their centroid to replace them
 * graph: left(0)<->lower(2)<->right(1)<->upper(3)<->left(0)
 **/ 
{
double dist1 = 0;
double dist2 = 0;
// remove duplicates
// check left
dist1 = calcDist(cand_x[0], cand_y[0], cand_x[2], cand_y[2]);
dist2 = calcDist(cand_x[0], cand_y[0], cand_x[3], cand_y[3]);
cout<<dist1<<'\t'<<dist2<<endl;
if (dist1<=0.06)
  {
    // left and lower should be combined
    vex_x.push_back((cand_x[0]+cand_x[2])/2);
    vex_y.push_back((cand_y[0]+cand_y[2])/2);
    vex_x.push_back(cand_x[1]);
    vex_y.push_back(cand_y[1]);  
    vex_x.push_back(cand_x[3]);
    vex_y.push_back(cand_y[3]);  
  }else if (dist2<=0.06)
  {
    // left and upper should be combined
    vex_x.push_back((cand_x[0]+cand_x[3])/2);
    vex_y.push_back((cand_y[0]+cand_y[3])/2);
    vex_x.push_back(cand_x[1]);
    vex_y.push_back(cand_y[1]);  
    vex_x.push_back(cand_x[2]);
    vex_y.push_back(cand_y[2]);    
  }else
  {
  dist1 = calcDist(cand_x[1], cand_y[1], cand_x[2], cand_y[2]);
  dist2 = calcDist(cand_x[1], cand_y[1], cand_x[3], cand_y[3]);
  cout<<dist1<<'\t'<<dist2<<endl;
  if (dist1<=0.06)
  {
    // right and lower should be combined
    vex_x.push_back((cand_x[1]+cand_x[2])/2);
    vex_y.push_back((cand_y[1]+cand_y[2])/2);
    vex_x.push_back(cand_x[0]);
    vex_y.push_back(cand_y[0]);  
    vex_x.push_back(cand_x[3]);
    vex_y.push_back(cand_y[3]);   
  }else if (dist2<=0.06)
  {
    // right and upper should be combined
    vex_x.push_back((cand_x[1]+cand_x[3])/2);
    vex_y.push_back((cand_y[1]+cand_y[3])/2);
    vex_x.push_back(cand_x[0]);
    vex_y.push_back(cand_y[0]);  
    vex_x.push_back(cand_x[2]);
    vex_y.push_back(cand_y[2]);     
  }else
  {
    vex_x.push_back(cand_x[0]);
    vex_y.push_back(cand_y[0]);  
    vex_x.push_back(cand_x[1]);
    vex_y.push_back(cand_y[1]);  
    vex_x.push_back(cand_x[2]);
    vex_y.push_back(cand_y[2]);  
    vex_x.push_back(cand_x[3]);
    vex_y.push_back(cand_y[3]);      
  }
  }
}

double UFunczoneobst::calcDist(double x1, double y1, double x2, double y2)
/**
 * calculate the distance between 2 points
 **/ 
{
 double dist = 0;
 double dx = x2-x1;
 double dy = y2-y1;
 dist = sqrt(pow(dx,2)+pow(dy,2));
 return dist;
}

void UFunczoneobst::sort(vector<double> &vec)
/**
 *  bubble sort, ascend order
 **/ 
{
  if (vec.size()>1)
  {
    for(int i=0;(double)i<vec.size()-1;i++)
    {
      for(int j=0;(double)j<vec.size()-1-i;j++)
      {
        if(vec[j]>vec[j+1])
        {
        double temp = vec[j];
        vec[j] = vec[j+1];
        vec[j+1] = temp;
        }
      }
    }
  }
}

double UFunczoneobst::calcAngle(int idx1, int idx2)
/**
 * return rotating angle, range=(-180,180]
 **/ 
{
double dx = vex_x[idx2] - vex_x[idx1];
double dy = vex_y[idx2] - vex_y[idx1];
double angle = atan2(dy,dx)*180/PI;
return angle;
}