#include "genVTKfromOrg.h"
#include <iostream>

bool Sort_Spin_Z(const Spin &a, const Spin &b)
{
 if (fabs(a.pz - b.pz)>0.0001)
 {
   return (a.pz<b.pz);
 }
 else if (fabs(a.py - b.py)>0.0001)
 {
   return (a.py<b.py);
 }
 else if (fabs(a.px - b.px)>0.0001)
 {
   return (a.px<b.px);
 }

 return 0;
}

bool linePlane(double * a, double* b, double* p, double *n)
{
        bool flag = false; 
        //svVector3 AP = p - a;
        //svVector3 BP = p - b;
        double AP[3];
	double BP[3];	
	AP[0] = p[0] - a[0];
        AP[1] = p[1] - a[1];
        AP[2] = p[2] - a[2];

        BP[0] = p[0] - b[0];
        BP[1] = p[1] - b[1];
        BP[2] = p[2] - b[2];
   
        double dd1 = AP[0]*n[0]+AP[1]*n[1]+AP[2]*n[2];//GetDot(AP, n);
        double dd2 = BP[0]*n[0]+BP[1]*n[1]+BP[2]*n[2];//GetDot(BP, n);
        if(dd1 * dd2 <= 0 ) flag = true;

        return flag;
}
bool planeBox(double* lb, double *rb, double * p, double *n)
{
        bool flag = false;

        double a[3],b[3];
        a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;
        a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;
        a[0] = lb[0];a[1] = lb[1]; a[2] = rb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = rb[0];a[1] = lb[1]; a[2] = rb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = rb[0];a[1] = rb[1]; a[2] = rb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = lb[0];a[1] = rb[1]; a[2] = rb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        return flag;
}

genVTKfromOrg::genVTKfromOrg()
{
    dataSize = 0;
}

int genVTKfromOrg::CheckFormat(char *file)
{
  char filename[2056];
  sprintf(filename, "%s", file);

  ifstream infile(filename);

  if(infile.fail())
  {
	cerr<<filename<<" not found "<<endl;
	cerr<<"Load QDOT data failed. prog exit \n"<<endl;

	exit(-1);
  }
  else
  {
    cerr<<"Load data "<<filename<<endl;
  }

  double pos[3];
  double vel[3];
  double  den;

  string str;
  int count = 0;
  bool flag = false;

  getline(infile, str);

  for(int i=0;i<str.length(); i++)
  {

	if((str[i] < '0' 
	|| str[i] > '9')
	&& str[i] != '-'
	&& str[i] !='.'
	&& flag == true)
	{
		count++;
		flag = false;
	}
	else if((str[i] >= '0' 
	&& str[i] <= '9')
	|| str[i] == '-'
	|| str[i] =='.')
	{
	//cerr<<str[i]<<endl;
		flag = true;
	}
  }
  count = count + 1;

  infile.close(); 
  
  return count; 
}

int genVTKfromOrg::getNumOfIntegerDigits(double num)
{
	  int multiplier = 0;

	  double numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}

void genVTKfromOrg::ReadRawData(char *file, int DEN_SCALAR)
{
  long double vx, vy, vz;
  long double px, py, pz;
  long double dummy;

  double pos[3];
  double vel[3];
  double  den;
  double a;
  double r;

  char filename[2056];
  sprintf(filename, "%s", file);//cerr<<filename<<endl;

  int data_type; 
  int count = CheckFormat(filename);
  //cerr<<count<<endl;
  if(count == 8)
  {
	data_type = 1;//new
  }
  else if(count == 9)
  {
	data_type = 2;//old
  }
  else
  {
	data_type = 0;//not defined
	cerr << filename << " ";
	cerr << "Load QDOT data failed. Wrong format. Prog exit\n" <<endl;
	exit(-1);
  }

  ifstream infile(filename);
  
  if(infile.fail())
  {
    cerr << filename<< "not found.\n";
	cerr << "Load QDOT data failed. prog exit\n" <<endl;
	exit(-1);
  }
  else
  {
	cerr << "Loading data: " << filename << endl;
  }
  min_den_zero = 9e+9;
  min_den = 9e+9;
  max_den = -1;  
  //cerr<<data_type<<endl;

  if(data_type == 2)
  {
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> r >> a >> vel[0] >> vel[1] >> vel[2] >> den)
	  {
	
	    //vel*=VEL_SCALAR;
		double tmpden;
		tmpden = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		tmpden = sqrt(tmpden);
		den = tmpden * den;
		if(tmpden > 0)
		{

		  vel[0] = vel[0]/tmpden;
		  vel[1] = vel[1]/tmpden;
		  vel[2] = vel[2]/tmpden;
		}
		
		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = r;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
		
		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}
                if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

	        if(max_den < d.den)
                {
                 max_den = d.den;
                }	
		//dataSize ++;
	  }
  }
  else if(data_type == 1)
 {
	//cerr<<data_type<<endl;
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> r >> a>> vel[0] >> vel[1] >> vel[2])
	  {
	    //vel*=VEL_SCALAR;
	
		den = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		den = sqrt(den);
		if(den>0)
		{
		  vel[0] = vel[0]/den;
		  vel[1] = vel[1]/den;
		  vel[2] = vel[2]/den;
		}

		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = r;		
		//cerr<<den<<endl;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
               if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}		
		if(max_den < d.den)
                {
                   max_den = d.den;
                }
		//dataSize ++;
	  }
  }
  
  infile.close();

//  cerr<<den<<endl;

		  
int  int_min_den = (double) getNumOfIntegerDigits(min_den);
		    //cerr<<min_den<<endl;
  if((-int_min_den) < DEN_SCALAR)
  {
	  int_min_den = DEN_SCALAR;
  }
  else
  {
	 int_min_den = -int_min_den;
  }
  
  lbbox[0] = 9e+9;lbbox[1] = 9e+9; lbbox[2] = 9e+9;
  rbbox[0] = -9e+9;rbbox[1] = -9e+9; rbbox[2] = -9e+9;

  min_region = 9e+1;

  dataSize = data.size();

  for(int i=0;i<dataSize;i++)
  {
    //data[i].den = data[i].den * pow(10., min_den);
    
    if(data[i].px < lbbox[0]) lbbox[0] = data[i].px;
    if(data[i].py < lbbox[1]) lbbox[1] = data[i].py;
    if(data[i].pz < lbbox[2]) lbbox[2] = data[i].pz;
    if(data[i].px > rbbox[0]) rbbox[0] = data[i].px;
    if(data[i].py > rbbox[1]) rbbox[1] = data[i].py;
    if(data[i].pz > rbbox[2]) rbbox[2] = data[i].pz;

    int flag = false;
    for(int j=0;j<region_count.size();j++)
    {
	if(data[i].region == unique_region[j])
	{
		flag = true;
		break;
	}
    }
    if(!flag)
    {
	unique_region.push_back(data[i].region);
	region_count.push_back(0);
    }
    
    if(data[i].region < min_region)
    {
	min_region = data[i].region;
    }
  }
//cerr<<min_region<<endl;
  for(int i=0;i<dataSize;i++)
  {
	region_count[data[i].region-min_region]++;
  }

  //outfile.close();
  //for(int i=0;i<zValue.size();i++)cerr<<denMax[i]<<" ";

  scaling =int_min_den;

  //cerr<<scaling<<endl;
  
  sort(data.begin(), data.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
  sort(data.begin(), data.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
  sort(data.begin(), data.end(), Sort_Spin_Z);
  

  
  //cerr<<scaling<<endl;  
  
  SplitData();
  

}

void genVTKfromOrg::SplitData(double pos_x, double pos_y, double pos_z,
                        double dir_x, double dir_y, double dir_z,
			double distance)
{
	double min_p[3];
	double max_p[3];

	min_p[0] = pos_x; min_p[1] = pos_y; min_p[2] = pos_z;
	max_p[0] = pos_x; max_p[1] = pos_y; max_p[3] = pos_z;

	if(dir_z < 0)
	{
		dir_x = - dir_x;
		dir_y = - dir_y;
		dir_z = - dir_z;		
	}
	else if(dir_y < 0)
	{
                dir_x = - dir_x;
                dir_y = - dir_y;
                dir_z = - dir_z;
	}
	else if(dir_x < 0)
	{
                dir_x = - dir_x;
                dir_y = - dir_y;
                dir_z = - dir_z;
	}
                double dir[3];
                dir[0] = dir_x;
                dir[1] = dir_y;
                dir[2] = dir_z;

//cerr<<min_p[0]<<" "<<min_p[1]<<" "<<min_p[2]<<" "<<distance<<" "<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<endl;

	while(1)
        {
                min_p[0] = min_p[0] - dir_x * distance;
                min_p[1] = min_p[1] - dir_y * distance;
                min_p[2] = min_p[2] - dir_z * distance;


                bool flag = planeBox(lbbox, rbbox, min_p, dir);

                if(flag == false)
                {
                        min_p[0] = min_p[0] + dir_x * distance;
                        min_p[1] = min_p[1] + dir_y * distance;
                        min_p[2] = min_p[2] + dir_z * distance;
                        break;
                }

//		cerr<<min_p[2]<<endl;
        }
        while(1)
        {
                max_p[0] = max_p[0] + dir_x * distance;
                max_p[1] = max_p[1] + dir_y * distance;
                max_p[2] = max_p[2] + dir_z * distance;

                bool flag = planeBox(lbbox, rbbox, max_p ,dir);

                if(flag == false)
                {
                        max_p[0] = max_p[0] - dir_x * distance;
                        max_p[1] = max_p[1] - dir_y * distance;
                        max_p[2] = max_p[2] - dir_z * distance;
                        break;
                }
        }
	int numPlane = sqrt((max_p[0] - min_p[0]) * (max_p[0] - min_p[0])
                + (max_p[1] - min_p[1]) * (max_p[1] - min_p[1])
                + (max_p[2] - min_p[2]) * (max_p[2] - min_p[2]))/distance + 1;
//cerr<<numPlane<<endl;

	plane_min.resize(3);
	plane_dir.resize(3);
	plane_min[0] = min_p[0];
        plane_min[1] = min_p[1];
        plane_min[2] = min_p[2];
	plane_dir[0] = dir[0];
        plane_dir[1] = dir[1];
        plane_dir[2] = dir[2];
	plane_distance = distance;

	
	splitData.resize(numPlane);
	layerDensity.resize(numPlane);
cerr<<"Split data by "<<numPlane<<" layers along direction ("<<dir[0]<<","<<dir[1]<<","<<dir[2]<<")"<<endl;
	for(int i=0;i<numPlane;i++)
	{
		layerDensity[i] = 0;	
	}

	for(int i=0;i<dataSize;i++)
	{
		double tmp[3];
		tmp[0] = data[i].px - min_p[0];
		tmp[1] = data[i].py - min_p[1];
		tmp[2] = data[i].pz - min_p[2];	

		double dot = tmp[0] * dir_x
				+ tmp[1] * dir_y
				+ tmp[2] * dir_z;

		if((fmod(dot, (double)distance)<1e-5)|| (distance-fmod(dot, (double)distance)<1e-5))
		{
			int index = (int)(dot/distance);
			splitData[index].push_back(data[i]);
			if(data[i].den > layerDensity[index])
			{
				layerDensity[index] = data[i].den;
			}
		}	
	}			
}

void genVTKfromOrg::SplitData()
{
  int count = 0;
  
  Zlayer_Num.push_back(0);
  
  for(int i=1;i<dataSize;i++)
  {
    if(fabs(data[i].pz - data[i-1].pz) > 0.0001)
    {
      Zlayer_Num.push_back(i);
    }
  }
  
 // cerr<<Zlayer_Num.size();
  
  Zlayer_Num.push_back(dataSize);
}
void genVTKfromOrg::SaveDensityBylayer(char *file)
{
	ofstream outfile (file);

	outfile<<splitData.size()<<endl;
	for(int i=0;i<splitData.size();i++)
	{
		outfile<<layerDensity[i]<<endl;
	}

	outfile.close();	
}
void genVTKfromOrg::SortedData(char *file)
{
  ofstream outfile;
 
 //=======================================================
  outfile.open(file);
  
  outfile<<dataSize<<endl;
  //outfile<<"Density_Scaling "<<scaling<<endl;
  //outfile<<"Zlayer_Num "<<Zlayer_Num.size()<<endl;
  //outfile<<"Z_Range "<<data[0].pz<<" "<<data[dataSize-1].pz<<endl;
  //outfile<<"Z_Distance "<<data[Zlayer_Num[1]].pz - data[0].pz<<endl;  

 for(int i=0;i<dataSize;i++)
 {
   double exp,coe;
   
   exp = (double)getNumOfIntegerDigits(data[i].den);
   coe = data[i].den/pow(10., exp);
   
   outfile<<data[i].px<<" "<<data[i].py<<" "<<data[i].pz<<" "
	  <<data[i].vx<<" "<<data[i].vy<<" "<<data[i].vz<<" "
	  <<data[i].den<<endl;//" "<<exp<<" "<<coe<<" "<<data[i].atom<<" "<<data[i].region<<endl;
 }
 outfile.close();
/*
//=======================================================
  char *str = new char[400];	//cerr<<file<<endl;
  for(int i=0;i<Zlayer_Num.size()-1;i++)
  {
  	double mean[6];
  	for(int ii=0;ii<6;ii++)
	{
		mean[ii] = 0;  
	}
	    sprintf(str, "%s_layer%d_sorted", file2, i);
	    cerr<<str<<endl;

//=======================================================
	    outfile.open(str);

	  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<" "
		  <<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<" "
		  <<data[j].den<<" "<<exp<<" "<<coe<<endl;

		mean[0] = mean[0] + data[j].px;
		mean[1] = mean[1] + data[j].py;
		mean[2] = mean[2] + data[j].vx;
		mean[3] = mean[3] + data[j].vy;
		mean[4] = mean[4] + data[j].vz;
		mean[5] = mean[5] + data[j].den;
	  }
	  outfile.close();

//=======================================================
	 for(int ii=0;ii<6;ii++)
	 {
		mean[ii] = mean[ii]/(Zlayer_Num[i+1] - Zlayer_Num[i]);
	 }

	  double std[6];
	  for(int ii=0;ii<6;ii++)
	  {
		std[ii] = 0; 
	  }
	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
		std[0] = std[0] + (data[j].px - mean[0]) * (data[j].px - mean[0]);
		std[1] = std[1] + (data[j].py - mean[1]) * (data[j].py - mean[1]);
		std[2] = std[2] + (data[j].vx - mean[2]) * (data[j].vx - mean[2]);
		std[3] = std[3] + (data[j].vy - mean[3]) * (data[j].vy - mean[3]);
		std[4] = std[4] + (data[j].vz - mean[4]) * (data[j].vz - mean[4]);
		std[5] = std[5] + (data[j].den - mean[5]) * (data[j].den - mean[5]);
	  }
	  for(int ii=0;ii<6;ii++)
	  {
		std[ii] = sqrt(std[ii]/(Zlayer_Num[i+1] - Zlayer_Num[i]-1));
	  }

//==================================================================================
	    sprintf(str, "%s_layer%d_sorted.txt", file2, i);
	    cerr<<str<<endl;

	    outfile.open(str);

	//  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<(data[j].px-mean[0])/std[0]<<" "<<(data[j].py-mean[1])/std[1]<<" "
		  <<(data[j].vx-mean[2])/std[2]<<" "<<(data[j].vy-mean[3])/std[3]<<" "<<(data[j].vz-mean[4])/std[4]<<" "
		  <<(data[j].den-mean[5])/std[5]<<endl;
	  }
	    
	    outfile.close();
//==================================================================================
	    sprintf(str, "%s_layer%d_sorted_ld.txt", file2, i);
	    cerr<<str<<endl;

	    outfile.open(str);

	//  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<(data[j].px-mean[0])/std[0]<<" "<<(data[j].py-mean[1])/std[1]<<" "
		  <<(data[j].den-mean[5])/std[5]<<endl;
	  }
	    
	    outfile.close();
  }
  delete [] str;*/
//=======================================================
}

void genVTKfromOrg::SaveSplitData(char *file)
{
	ofstream outfile;

	char *str = new char[400];
	
	for(int i=0;i<splitData.size();i++)
	{
		sprintf(str, "%s/%d.txt", file, i);
    		outfile.open(str);
		
		outfile<<splitData[i].size()<<endl;
		for(int j=0;j<splitData[i].size();j++)
		{
   			double exp,coe;
   			exp = (double)getNumOfIntegerDigits(splitData[i][j].den);
   			coe = splitData[i][j].den/pow(10., exp);

			outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz
				<<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
				<<" "<<splitData[i][j].den<<endl;
				//" "<<exp<<" "<<coe
				//<<" "<<splitData[i][j].atom<<" "<<splitData[i][j].region<<endl;
		}

		outfile.close();
	}

	delete [] str;
	
	
	
}

void genVTKfromOrg::SaveRegiontoFile(char *file)
{
  for(int i=0;i<region_count.size();i++)
  {
	  char *str = new char[200];
	sprintf(str, "%s/region%d.txt", file, i+min_region);
	ofstream outfile(str);

	    outfile<<i<<endl; 

	    outfile<<region_count[i]<<endl;  
		
		int count = 0;
	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	      	{
			outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<" ";

	     		outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<" ";

	      		outfile<<data[j].den<<endl;
	    }
	  }
	  delete [] str;

	outfile.close();
  }	
}

void genVTKfromOrg::SavetoVTK(char *file)
{
  ofstream outfile;
  
  char *str = new char[400];	//cerr<<file<<endl;

//cerr<<file<<" "<<Zlayer_Num.size()-1<<endl;
//=======================================================
/*  for(int i=1;i<Zlayer_Num.size();i++)
  {
    sprintf(str, "%s/%d.vtk", file, i-1);
    
    outfile.open(str);
  
    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<Zlayer_Num[i] - Zlayer_Num[i-1]<<" float"<<endl;   
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
    }
    outfile<<endl;
    
    outfile<<"POINT_DATA "<<Zlayer_Num[i] - Zlayer_Num[i-1]<<endl;
    outfile<<"VECTORS velocity float"<<endl;  
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;    
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].den<<endl;
    }
    outfile<<endl; 
    
    outfile.close();
  }
*/
    for(int i=0;i<splitData.size();i++)
    {
    sprintf(str, "%s/%d.vtk", file, i);
    
    outfile.open(str);
  
    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<splitData[i].size()<<" float"<<endl;   
    for(int j=0;j<splitData[i].size();j++)
    {
      outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz<<endl;
    }
    outfile<<endl;
    
    outfile<<"POINT_DATA "<<splitData[i].size()<<endl;
    outfile<<"VECTORS velocity float"<<endl;  
    for(int j=0;j<splitData[i].size();j++)
    {
      outfile<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;    
    for(int j=0;j<splitData[i].size();j++)
    {
      outfile<<splitData[i][j].den<<endl;
    }
    outfile<<endl; 
    
    outfile.close();

  }
//=======================================================
  sprintf(str, "%s/all.vtk", file);
  outfile.open(str);

    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<dataSize<<" float"<<endl;  

  for(int j=0;j<dataSize;j++)
  {
      outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
   }
    outfile<<endl;
    
    outfile<<"POINT_DATA "<<dataSize<<endl;
    outfile<<"VECTORS velocity float"<<endl;  

  for(int j=0;j<dataSize;j++)
  {
      outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;   
 
  for(int j=0;j<dataSize;j++)
  {
      outfile<<data[j].region<<endl;
    }
    outfile<<endl;


  outfile.close();

//=======================================================
  for(int i=0;i<region_count.size();i++)
  {
	sprintf(str, "%s/region%d.vtk", file, i+min_region);
	outfile.open(str);

	    outfile<<"# vtk DataFile Version 2.0"<<endl;
	    outfile<<str<<endl;
	    outfile<<"ASCII"<<endl;
	    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

	    outfile<<"POINTS "<<region_count[i]<<" float"<<endl;  
		int count = 0;
	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	      	{
			outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
			count++;
		}
	   }
	//	cerr<<data[0].region<<" "<<i+min_region<<" "<<region_count[i]<<" "<<count<<endl;
	    outfile<<endl;
	    
	    outfile<<"POINT_DATA "<<region_count[i]<<endl;
	    outfile<<"VECTORS velocity float"<<endl;  

	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	     		outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
	    }
	    outfile<<endl; 
	    
	    outfile<<"SCALARS density float 1"<<endl;
	    outfile<<"LOOKUP_TABLE default"<<endl;   
	 
	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	      		outfile<<data[j].region<<endl;
	    }
	    outfile<<endl;

	outfile.close();
  }

  delete [] str;
}

void genVTKfromOrg::SaveDataFormat(char *file)
{
	ofstream outfile(file);

	vector<double> temp_x;
	for(int i=0;i<dataSize;i++)
	{
	  temp_x . push_back(data[i].px);
	}
	sort(temp_x.begin(), temp_x.end());
	temp_x.erase(unique(temp_x.begin(), temp_x.end()), temp_x.end());
	sort(temp_x.begin(), temp_x.end());
	double min_xdistance = temp_x[1] - temp_x[0];
	
	vector<double> temp_y;
	for(int i=0;i<dataSize;i++)
	{
	  temp_y . push_back(data[i].py);
	}
	sort(temp_y.begin(), temp_y.end());
	temp_y.erase(unique(temp_y.begin(), temp_y.end()), temp_y.end() );
	sort(temp_y.begin(), temp_y.end());
	double min_ydistance = temp_y[1] - temp_y[0]; 	
	
	//Overall 
	outfile<<splitData.size()<<endl; //# of Z layers
	outfile<<plane_min[0]<<" "<<plane_min[1]<<" "<<plane_min[2]<<endl;
	outfile<<plane_dir[0]<<" "<<plane_dir[1]<<" "<<plane_dir[2]<<endl;
	outfile<<plane_distance<<endl;

	outfile<<min_xdistance<<endl; //distance of two x values
	outfile<<min_ydistance<<endl; //distance of two y values
	outfile<<data[Zlayer_Num[1]].pz - data[0].pz<<endl; //distance of two z layers
	outfile<<scaling<<endl; //scalar for magnitude
        outfile<<min_den_zero<<" "<<min_den<<" "<<max_den<<endl;
        outfile<<getNumOfIntegerDigits(max_den)-getNumOfIntegerDigits(min_den)+1<<endl;
	outfile<<lbbox[0]<<" "<<lbbox[1]<<" "<<lbbox[2]<<endl;
	outfile<<rbbox[0]<<" "<<rbbox[1]<<" "<<rbbox[2]<<endl;

	//Details
	//z_start_value z_start_index z_end_value z_end_index z_distance
	outfile<<data[0].pz<<" "<<0
	  <<" "<<data[dataSize-1].pz<<" "<<dataSize-1
	  <<" "<<data[Zlayer_Num[1]].pz - data[0].pz<<endl; 
	  
	outfile<<Zlayer_Num.size()-1<<endl;
	for(int i=1;i<Zlayer_Num.size();i++)
	{
	  double y_distance;

	  vector<int> x_index;
	  x_index.push_back(Zlayer_Num[i-1]);
	  
	  for(int j=Zlayer_Num[i-1]+1; j<Zlayer_Num[i];j++)
	  {
	    if(data[j].py > data[j-1].py)
	    {
	      x_index.push_back(j);
	    }
	  }
	  x_index.push_back(Zlayer_Num[i]);
	  y_distance = data[x_index[1]].py - data[x_index[0]].py;
	  
	  //y_start_value y_start_index y_end_value y_end_index y_distance
	  outfile<<data[Zlayer_Num[i-1]].py<<" "<<Zlayer_Num[i-1]
	    <<" "<<data[Zlayer_Num[i]-1].py<<" "<<Zlayer_Num[i]-1
	    <<" "<<y_distance<<endl;
	    
	  outfile<<x_index.size()-1<<endl;	    
	  for(int j=1;j<x_index.size();j++)
	  {
	    double x_distance = data[x_index[j-1]+1].px - data[x_index[j-1]].px;
	    //x_start_value x_start_index x_end_value x_end_index x_distance
	    outfile<<data[x_index[j-1]].px<<" "<<x_index[j-1]
	      <<" "<<data[x_index[j]-1].px<<" "<<x_index[j]-1
	      <<" "<<x_distance<<endl;;
	  }
	  
	  x_index.clear();
	}

	outfile.close();
	
	temp_x.clear();
	temp_y.clear();
}

void genVTKfromOrg::cleanup()
{
  data.clear();
  Zlayer_Num.clear();
  region_count.clear();
  unique_region.clear();
  dataSize = 0;
  plane_min.clear();
  plane_dir.clear();

  for(int i=0;i<splitData.size();i++)
  {
		splitData[i].clear();
   }
  splitData.clear();
}
