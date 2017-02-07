#include <fstream>

using namespace std;

int main()
{
	ifstream infile("Spiral_Color_4.txt");
	ofstream outfile("spiralcolor");
	
	int count = 0;
	double color[4];
	outfile<<"{"<<endl;
	while(infile>>color[0]>>color[1]>>color[2]>>color[3])
	{
		if(count>=200 && count<800 && count%10==0)
		{
			outfile<<"{"<<color[1]<<","<<color[2]<<","<<color[3]<<"},"<<endl;
		} 
		count++;
	}
	outfile<<"}"<<endl;
	
	outfile.close();
	infile.close();
	return 0;
}
