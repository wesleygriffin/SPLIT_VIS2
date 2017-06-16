#include "svContour.h"

//#define _VTK

struct POINT{
	double x;
	double y;
	double z;
};
vector<int> isoIndex;
vector<POINT> isoPoint;

bool sortfunction(int a, int b)
{
	if(fabs(isoPoint[a].z - isoPoint[b].z)>0.0001)
		return (isoPoint[a].z<isoPoint[b].z);
	else if(fabs(isoPoint[a].y - isoPoint[b].y)>0.0001)
		return (isoPoint[a].y<isoPoint[b].y);
	else if(fabs(isoPoint[a].x - isoPoint[b].x)>0.0001)
		return (isoPoint[a].x<isoPoint[b].x);
}
using namespace std;

svContour::svContour()
{
		zLayer = 0 ;
		pointnum = 0;
}

//Contour generation using VTK
void svContour::Get2DContour(char *infile,  double density) //density is only the ratio
{
//#ifdef _VTK
        vtkUnstructuredGridReader *pl3d = vtkUnstructuredGridReader::New();
        pl3d->SetFileName(infile);
        pl3d->SetScalarsName("density");
        pl3d->SetVectorsName("velocity");
        pl3d->Update();
        pl3d->DebugOn();

        vtkDelaunay2D *del2d = vtkDelaunay2D::New();
        del2d->SetInputConnection(pl3d->GetOutputPort());
        del2d->SetTolerance(0.001);
        del2d->SetAlpha(0);

	vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
	double *range = new double[2]; //cerr<<range[1]<<" ";
		//vtkDataSet *data = vtkDataSet::New();
		//data->SetInputConnection(del2d->GetOutputPort());
		//range = pl3d->GetOutput()->GetScalarRange();
	grid=	pl3d->GetOutput();//->GetScalarRange();
	range = grid->GetScalarRange();
//	if(type == 1) density = density * range[1];

//	cerr<<infile<<" "<<density<<endl;

        vtkContourFilter *iso = vtkContourFilter::New();
        iso->SetInputConnection(del2d->GetOutputPort());
        iso->SetValue(0,density);

        vtkDataSetSurfaceFilter *filter = vtkDataSetSurfaceFilter::New();
        filter->SetInput(iso->GetOutput());
        //JC
  //      cerr<<"a"<<endl;
        //filter->SetInputData(iso->GetOutput());
        filter->Update();
    //    cerr<<"a"<<endl;

        vtkPolyData *pd = filter->GetOutput();
      //  cerr<<"a"<<endl;

  	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();

        //cerr<<pd->GetCellType(0)<<" "<<pd->GetNumberOfCells()<<" "<<pd->GetNumberOfPoints()<<endl;
        for(int i=0;i<pd->GetNumberOfPoints();i++)
        {
            double *tmpp = new  double [3];
			tmpp = pd->GetPoint(i);
			polygon->GetPoints()->InsertNextPoint(tmpp[0], tmpp[1], tmpp[2]);
	//delete [] tmpp;
	}
        //cerr<<"a"<<endl;
	
  		double testIn[3] = {0., -0, -10.0};
  		double testOut[3] = {2.0, 0.5, 0.0};
 
  		double nn[3];
  		polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
		static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), nn);
 
 		double bounds[6];
  		polygon->GetPoints()->GetBounds(bounds);
 
  		//cout << "testIn in polygon? " <<zLayer<<" "<< polygon->PointInPolygon(testIn,
		//polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(
		//polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, nn) <<endl;

        //double *p = new  double[3];
	double *p;//[3];
       // double *pos = new double[3];
        //double *den = new double[1];
	double pos[3], den[3];

		vector<double> p3;
		vector<double> v3;

        for(int i=0;i<pd->GetNumberOfPoints();i++)
        {

//cerr<<i<<endl;
		p = pd->GetPoint(i);

		p3.push_back(p[0]);
		p3.push_back(p[1]);
		p3.push_back(p[2]);				
		isoPointPos.push_back(p3);

                pd->GetPointData()->GetArray(0)->GetTuple(i,pos);
		v3.push_back(pos[0]);
		v3.push_back(pos[1]);
		v3.push_back(pos[2]);
		double sum_v3 = v3[0] * v3[0] + v3[1] * v3[1] + v3[2] * v3[2];
		sum_v3 = sqrt(sum_v3);
		if(sum_v3 > 0)
		{

			v3[0] = v3[0]/sum_v3;
			v3[1] = v3[1]/sum_v3;
			v3[2] = v3[2]/sum_v3;
		}
                isoPointDir.push_back(v3);

                pd->GetPointData()->GetArray(1)->GetTuple(i,den);
                isoPointDen.push_back(den[0]);

				//double exp = getNumOfIntegerDigits(den[0]);
				//double tmppow = 1;
				//for(int iii=0;iii<(int)exp;iii++)
					//tmppow = tmppow * 10.;		
				//double	coe = tmpden/tmppow;//pow(10.,exp);
		
				//isoPointExp.add(exp);
				//isoPointCoe.add(coe);

                pointnum++;

		p3.clear();
		v3.clear();
        }

		zLayer ++;



//#endif
/*	polygon->Delete();cerr<<"1"<<endl;
	pd->Delete();cerr<<"1"<<endl;
	filter->Delete();cerr<<"1"<<endl;
	iso->Delete();cerr<<"1"<<endl;
	grid->Delete();cerr<<"1"<<endl;
	del2d->Delete();	cerr<<"1"<<endl;
	pl3d->Delete();cerr<<"1"<<endl;*/


		//delete  range; 
		//delete [] pos;
		//delete [] den;
		//delete  p; 
		//delete p;p=NULL;
		//delete range; range = NULL;
}


int svContour::getNumOfIntegerDigits(double num)
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

void svContour::SavePointtoFile(char *file)
{
	for(int i=0;i<pointnum;i++)
	{
		POINT tmp;
		tmp.x = isoPointPos[i][0];
		tmp.y = isoPointPos[i][1];
		tmp.z = isoPointPos[i][2];
		isoPoint.push_back(tmp);
		isoIndex.push_back(i);
	}
	sort(isoIndex.begin(), isoIndex.end(), sortfunction);

        ofstream ofile;
        ofile.open(file);

        ofile<<pointnum<<endl;

        for(int i=0;i<pointnum;i++)
        {
		int ii = isoIndex[i];

	      double exp, coe;
	      exp = (double) getNumOfIntegerDigits(isoPointDen[ii]);
	      coe = isoPointDen[ii]/pow(10.,exp);
               		 ofile<<isoPointPos[ii][0]<<" "<<isoPointPos[ii][1]<<" "<<isoPointPos[ii][2]<<" "<<
                      		 isoPointDir[ii][0]<<" "<<isoPointDir[ii][1]<<" "<<isoPointDir[ii][2]<<" "<<
                       		 isoPointDen[ii]<<endl;//" "<<exp<<" "<<coe<<endl;

        }
        
        ofile.close();

	isoPoint.clear();
	isoIndex.clear();
}

void svContour::SavePointtoVTK(char *file)
{
        ofstream ofile;
        ofile.open(file);

        ofile<<"# vtk DataFile Version 2.0"<<endl;
	ofile<<file<<endl;
	ofile<<"ASCII"<<endl;
	ofile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

	ofile<<"POINTS "<<pointnum<<" float"<<endl;
        for(int i=0;i<pointnum;i++)
        {
               	 ofile<<isoPointPos[i][0]<<" "<<isoPointPos[i][1]<<" "<<isoPointPos[i][2]<<endl;
	}
	ofile<<endl;


	ofile<<"POINT_DATA "<<pointnum<<endl;
	ofile<<"VECTORS velocity float"<<endl; 
        for(int i=0;i<pointnum;i++)
        {	
                 ofile<<isoPointDir[i][0]<<" "<<isoPointDir[i][1]<<" "<<isoPointDir[i][2]<<endl;
	}
	ofile<<endl;

	ofile<<"SCALARS density float 1"<<endl;
	ofile<<"LOOKUP_TABLE default"<<endl;    
        for(int i=0;i<pointnum;i++)
        {
                  ofile<<isoPointDen[i]<<endl;

        }

	ofile<<endl;
        
        ofile.close();	
}

/*void svContour::SavetosvlFile(char *file)
{
        ofstream ofile;
        ofile.open(file);

        ofile<<pointnum<<endl;

        for(int i=0;i<pointnum;i++)
        {
        	ofile<<2<<endl;
                ofile<<isoPointPos[i][0]<<" "<<isoPointPos[i][1]<<" "<<isoPointPos[i][2]<<" "<<1<<" "<<1<<" "<<1<<endl;
                ofile<<isoPointPos[i][0]+isoPointDir[i][0] * isoPointDen[i]<<" "
                		<<isoPointPos[i][1]+isoPointDir[i][1] * isoPointDen[i]<<" "
						<<isoPointPos[i][2]+isoPointDir[i][2] * isoPointDen[i]<<" "
						<<1<<" "<<1<<" "<<1<<endl;
        }
        
        ofile.close();
}*/


void svContour::cleanup()
{

	for(int i=0;i<pointnum;i++)
	{
		isoPointPos[i].clear();
		isoPointDir[i].clear();
	}

	isoPointPos.clear();
	isoPointDir.clear();
	isoPointDen.clear();

	zLayer = 0;
	pointnum = 0;
}


