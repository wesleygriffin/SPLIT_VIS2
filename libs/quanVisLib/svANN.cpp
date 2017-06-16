void svANN::ANN_One(svVector3 *pos1, svVector3 *pos2, int num1, int num2, int *IDX)
{
		//cerr<<"start"<<endl;
		int	nPts;					// actual number of data points
		ANNpointArray	dataPts;				// data points
		ANNpoint	queryPt;				// query point
		ANNidxArray	nnIdx;					// near neighbor indices
		ANNdistArray    dists;					// near neighbor distances
		ANNkd_tree*	kdTree;					// search structure
		int dim = 3;
		//int maxPts = dataSize*dim;
		int k = 1;

		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists
		queryPt = annAllocPt(dim);					// allocate query point

		double *wholepos = new double[num1*3];								
		double  *querypos = new double[num2*3];

		int kk=0;	
		svVector4 *idx_q = new svVector4[num2];

		for(int i=0;i<num1;i++)
		{
			wholepos[kk] = pos1[i][0];
			wholepos[kk+num1] = pos1[i][1];
			wholepos[kk+num1*2] = pos1[i][2];
			kk++;
		}

		kk = 0;
		for(int i=0;i<num2;i++)
		{
			querypos[kk] = pos2[i][0];
			querypos[kk+num2] = pos2[i][1];
			querypos[kk+num2*2] = pos2[i][2];
			kk++;
		}

		nPts = num1;
		dataPts = annAllocPts(num1*dim, dim);
		int ii=0;
		int jj = 0;

		for(int i=0;i<num1;i++)//in_count;i++)
		{
			dataPts[jj][0] = wholepos[i];
			dataPts[jj][1] = wholepos[i+num1];
			dataPts[jj][2] = wholepos[i+num1*2];
			jj++;		
		}

		kdTree = new ANNkd_tree(							// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space

		for(int i=0;i<num2;i++)
		{
			queryPt[0] = querypos[i];
			queryPt[1] = querypos[i+num2];
			queryPt[2] = querypos[i+num2*2];

			kdTree->annkSearch(							// search
					queryPt,						// query point
					1,							// number of near neighbors
					nnIdx,							// nearest neighbors (returned)
					dists,							// distance (returned)
					0.0);		
					// error bound

			/*int count[5] = {0,0,0,0,0};
			for(int j=0;j<5;j++)
			{
				for(int t=0;t<5;t++)
				{
					if(fabs(IDX[nnIdx[j]][0]-IDX[nnIdx[t]][0])<0.0001 && j!=t)
					{
						count[j]++;
					}
				}
			}

			int temp = 0;
			int index;
			for(int j=0;j<5;j++)
			{
				if(count[j] > temp)
				{
					temp = count[j];
					index = j;
				}
			}*/

			idx_q[i] = IDX[nnIdx[0]];
		}

	   delete [] nnIdx;							// clean things up
	   delete [] dists;
	   delete kdTree;

	dataPts = annAllocPts(0,0);
	queryPt = annAllocPt(0);
	annClose();			// done with ANN

	return idx_q;
}
