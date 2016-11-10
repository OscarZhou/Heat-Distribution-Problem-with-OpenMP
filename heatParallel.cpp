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

#include "arrayff.hxx"
#include "draw.hxx"

# include <cstdlib>
# include <omp.h>




int main(int argc, char* argv[]) 
{
    long t_start = omp_get_wtime();  // the start time
	// X and Y dimensions. Force it to be a square.
	const int npix = atoi(argv[1]);
	const int thread_num = atoi(argv[2]);
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

    for (int i=0; i<npixy;i++)
	{
        for (int j=0;j<npixx;j++)
		{
			g(i,j)=h(i,j);	
		}
	}

	bool flag_exit = false;
    long count = 0;

    int time = 0;


    const int unit_pixels = npixy / thread_num;
    # pragma omp parallel num_threads(thread_num)
	{
		const int tid = omp_get_thread_num ();
		const int nthreads = omp_get_num_threads ();
		const int num_proc = omp_get_num_procs ();


        while (1)
        {
            time ++;
            for (int i= tid * unit_pixels; i < ((tid + 1) * unit_pixels);i++)
            {


                if (i == 0 || i == npixy -1) continue;


                for (int j=1;j<npixx-1;j++)
                {
                    g(i, j) = (g(i, j-1 ) + g(i+1, j) + g(i, j+1) + g(i-1, j))*0.25; //2. assign g to a new array npi_frame which is 2 more rows and columns than g and h.

                }
            }



            # pragma omp barrier
            // Draw the printed circuit components
            fix_boundaries2<float>(g);


            # pragma omp master
            {
                for (int i=0; i<npixy;i++)
                {
                    for (int j=0;j<npixx;j++)
                    {
                        if(fabs((float)(h(i, j) - g(i, j))) > 0) //4. verify if meet local synchronization
                        {
                            count++;
                        }
                    }
                }

                if (count == 0) //5. jump out of loop after meeting local synchronization
                {
                    flag_exit = true;
                    //break;
                }

                for (int i=0; i<npixy;i++)
                {
                    for (int j=0;j<npixx;j++)
                    {
                        h(i,j)=g(i,j);
                    }
                }

            }

            if (flag_exit == true) //5. jump out of loop after meeting local synchronization
            {
                break;
            }
            else
            {
                count = 0;
            }
            # pragma omp barrier

        }

	}
	

	
	// This is the initial value image where the boundaries and printed
	// circuit components have been fixed
    dump_array<float, 2>(h, "plate4.fit");


	long t_end = omp_get_wtime();
    std::cout<<"------------------------Parallel Version------------------------"<<std::endl;
    std::cout<<"================================================================"<<std::endl;
    std::cout << "---------------Start time = "<<t_start <<"------------------------------"<<std::endl;
    std::cout << "---------------End time = "<<t_end <<"--------------------------------"<<std::endl;
    std::cout << "---------------The time of calculating is "<< (t_end - t_start)/double(CLOCKS_PER_SEC)<<"-----------------"<<std::endl;
    std::cout<< "---------------The times for looping is "<< time<<"--------------------"<<std::endl;
    std::cout<<"================================================================"<<std::endl;
	// Complete the sequential version to compute the heat transfer,
	// then make a parallel version of it
}
