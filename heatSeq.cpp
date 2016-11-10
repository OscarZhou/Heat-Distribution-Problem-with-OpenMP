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
	
	int time = 0;

	int i, j;
	for (i=0; i<npixx;i++)
	{
		for (j=0;j<npixy;j++)
		{
			g(i,j)=h(i,j);	
		}
	}


	float left, above, right, below = 0;
	long count = 0;
	while (1)
	{
		time ++;
		count = 0;
		for (i=0; i<npixy;i++)
		{
			if (i == 0 || i == npixy -1) continue;

			for (j=0;j<npixx;j++)
			{
				if (j == 0 || j == npixx -1) continue;

				left = g(i, j-1 );
				above = g(i+1, j); 
				right = g(i, j+1);
				below = g(i-1, j);
				g(i, j) = (float)((left + above + right + below)/4); //2. assign g to a new array npi_frame which is 2 more rows and columns than g and h.
			}
		}
	

		// Draw the printed circuit components
		fix_boundaries2<float>(g);

		

		for (i=0; i<npixy;i++)
		{
			for (j=0;j<npixx;j++)
			{
				if(fabs((float)(h(i, j) - g(i, j))) > 0) //4. verify if meet local synchronization
				{
					count++;
				}
			}
		}		


		if (count == 0) //5. jump out of loop after meeting local synchronization
		{
			break;
		}

		for (i=0; i<npixx;i++)
		{
			for (j=0;j<npixy;j++)
			{
				h(i,j)=g(i,j);	
			}
			
		}

		
	}

	
	// This is the initial value image where the boundaries and printed
	// circuit components have been fixed
	dump_array<float, 2>(h, "plate4.fit");


	long t_end = clock();

    std::cout<<"------------------------Sequential Version----------------------"<<std::endl;
    std::cout<<"================================================================"<<std::endl;
    std::cout << "---------------Start time = "<<t_start <<"--------------------------------"<<std::endl;
    std::cout << "---------------End time = "<<t_end <<"--------------------------------"<<std::endl;
    std::cout << "---------------The time of calculating is "<< (t_end - t_start)/double(CLOCKS_PER_SEC)<<"--------------"<<std::endl;
    std::cout<< "---------------The times for looping is "<< time<<"--------------------"<<std::endl;
    std::cout<<"================================================================"<<std::endl;

	// Complete the sequential version to compute the heat transfer,
	// then make a parallel version of it
}
