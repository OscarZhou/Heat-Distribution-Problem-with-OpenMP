/******************************************************************************

Start up demo program for 159735 Assignment 3 Semester 1 2013

All this does is initialize the image and write it to a file.

To compile:

make heat_demo

To run (for example to make a 100X100 pixel image):

./heat_demo 100


******************************************************************************/
#include <iostream>
#include <math.h>
#include <ctime>

#include "arrayff.hxx"
#include "draw.hxx"





int main(int argc, char* argv[]) 
{
	long t_start = clock();   // the start time
	std::cout << "---------------t_start  = "<<t_start <<"--------------\n"<<std::endl;
	// X and Y dimensions. Force it to be a square.
	const int npix = atoi(argv[1]);
	const int npixx = npix;
	const int npixy = npix;
	const int ntotal = npixx * npixy;

	// Images as 2D arrays: h is the current image, g is the updated
	// image. To access individual pixel elements, use the () operator. 
	// Note: that y is the first index (to reflect row major
	// order). Eg: h(y, x) = fubar
	Array<float, 2> h(npixy, npixx), g(npixy, npixx);

	// Draw the printed circuit components
	fix_boundaries2<float>(h);
	

	//std::cout<<"Initialization    g(0,0) = "<<g(0,0) <<"address="<<&g<<std::endl;		
	//std::cout<<"Initialization    h(0,0) = "<<h(0,0) <<"address="<<&h<<std::endl;	
	g = h;   //1. assign array h to array g, make them enjoy same value in each cell.


	//std::cout<<"AFTER  Initialization    g(0,0) = "<<g(0,0) <<"address="<<&g<<std::endl;		
	//std::cout<<"AFTER  Initialization    h(0,0) = "<<h(0,0) <<"address="<<&h<<std::endl;
	//float npi_frame[npixx+2][npixy+2] = {0.0};
	float **npi_frame = new float*[npixx+2];
	for (int k=0; k< npixx+2;++k)
	{
		npi_frame[k] = new float[npixy+2];
	}
	
	int i, j;
/*
	for (i=0; i<npixx;i++)
	{
		for (j=0;j<npixy;j++)
		{
			npi_frame[i][j] = 255; 
			std::cout<<npi_frame[i][j]<<",";
		}
		std::cout<<std::endl;
	}
	
*/
	

	float left, above, right, below = 0;
	long count = 0;
	while (1)
	{
		count = 0;
		for (i=1; i<npixx+1;i++)
		{
			for (j=1;j<npixy+1;j++)
			{
				npi_frame[i][j] = g(i-1, j-1); //2. assign g to a new array npi_frame which is 2 more rows and columns than g and h.
				//std::cout<<npi_frame[i][j]<<",";
			}
			//std::cout<<std::endl;
		}
	
		float left, top, right, down = 0;
		for (i=1; i<npixx+1;i++)
		{
			for (j=1;j<npixy+1;j++)
			{
				left = npi_frame[i][j-1];
				above = npi_frame[i-1][j];
				right = npi_frame[i][j+1];
				below = npi_frame[i+1][j];
				npi_frame[i][j] = (float)((left + above + right + below)/4); //3. update each cell according to neigbours
				//std::cout<<"npi_frame["<<i<<"]["<<j<<"] = "<<npi_frame[i][j] <<std::endl;
				//std::cout<<npi_frame[i][j]<<",";
				
			}
			//std::cout<<std::endl;
		}

		for (i=1; i<npixx+1;i++)
		{
			for (j=1;j<npixy+1;j++)
			{


				if(abs((float)(g(i-1, j-1) - npi_frame[i][j])) < 0.00001) //4. verify if meet local synchronization
				{
					count++;
				}
			}
		}

		if (count == (npixx * npixy)) //5. jump out of loop after meeting local synchronization
		{
			break;
		}

		for (i=1; i<npixx+1;i++)
		{
			for (j=1;j<npixy+1;j++)
			{
				g(i-1, j-1) = npi_frame[i][j]; //5. reassign the updated data to g	

			}
		}

		// Draw the printed circuit components
		//fix_boundaries2<float>(g);
		//std::cout<<"------------------------counter="<<count<<std::endl;
		//h = g;
		// Draw the printed circuit components
		//fix_boundaries2<float>(g);
		
	}
	/*
	for (int i=0;i<npixy; i++)
	{
		for (int j=0;j<npixx; j++)
		{

			std::cout<<"h("<<i<<","<<j<<")="<<h(i, j)<<std::endl;
		}
	}
	*/
	
	
	

	
	// This is the initial value image where the boundaries and printed
	// circuit components have been fixed
	dump_array<float, 2>(h, "plate5.fit");


	for (int k=0; k< npixx+2;++k)
	{
		delete [] npi_frame[k];
	}
	delete [] npi_frame;

	long t_end = clock();
	std::cout << "---------------t_end  = "<<t_end <<"--------------\n"<<std::endl;
	std::cout << "The time of calculating is "<< (t_end - t_start)/double(CLOCKS_PER_SEC)<<std::endl;
	// Complete the sequential version to compute the heat transfer,
	// then make a parallel version of it
}
