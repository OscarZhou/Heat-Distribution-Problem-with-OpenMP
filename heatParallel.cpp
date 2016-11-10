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

/*


void updateingCells(int tid, int thread, int npixx, int npixy, Array<float, 2> g)
{
	
	//Array<float, 2> h(npixy, npixx);
	int time = 0;

	int i, j;
	int istart = iend= 0;

	float left, above, right, below = 0;
	long count = 0;
	while (1)
	{
		time ++;
		count = 0;

		istart = tid * npixy / thread;
		iend = (tid + 1) * npixy / thread;
		if (tid == thread - 1)
		{
			iend = npixy - 1;
		}

		for (i=istart; i<iend;i++)
		{
			if (i == 0 || i == npixy -1) continue;

			for (j=0;j<npixx;j++)
			{
				if (j == 0 || j == npixx -1) continue;
			
				# pragma omp critical
				{
					g(i, j) = (float)((g(i, j-1 ) + g(i+1, j) + g(i, j+1) + g(i-1, j))/4); //2. assign g to a new array npi_frame which is 2 more rows and columns than g and h.
				}
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
	
}

*/

int main(int argc, char* argv[]) 
{
	long t_start = omp_get_wtime();  // the start time
	
	std::cout << "---------------t_start  = "<<t_start <<"--------------\n"<<std::endl;
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
	
	int i, j;
	for (i=0; i<npixy;i++)
	{
		for (j=0;j<npixx;j++)
		{
			g(i,j)=h(i,j);	
		}
	}

	bool flag_exit = false;
    long count = 0;

    int time = 0;
    # pragma omp parallel num_threads(thread_num)
	{
		const int tid = omp_get_thread_num ();
		const int nthreads = omp_get_num_threads ();
		const int num_proc = omp_get_num_procs ();
        //std::cout << " Hello from thread " << tid<< " / " << nthreads<< " : " << num_proc << std::endl;
		
		
		//pooh(tid, g); // create thread parallel region
	
		//updateingCells()		

        //int time = 0;

		int istart = 0 ; 
		int iend= 0;


        //#pragma omp for
        //for(i = 0; i< nthreads; i++)
        //{

            while (1)
            {
                time ++;
                std::cout<<"------------------------times ="<<time<<std::endl;
                count = 0;

                /***** Set different start position according tid and the numbers of thread *******/
                istart = tid * npixy / nthreads;
                iend = (tid + 1) * npixy / nthreads;
                if (tid == nthreads - 1)
                {
                    iend = npixy - 1;
                }
                //std::cout<<"tid="<<tid<<": istart="<<istart<<", iend =" <<iend<<std::endl;
                /****************/

                for (i=istart; i<iend;i++)
                {
                    if (i == 0 || i == npixy -1) continue;

                    for (j=0;j<npixx;j++)
                    {
                        if (j == 0 || j == npixx -1) continue;

                        //# pragma omp critical
                        //{
                            g(i, j) = (float)((g(i, j-1 ) + g(i+1, j) + g(i, j+1) + g(i-1, j))/4); //2. assign g to a new array npi_frame which is 2 more rows and columns than g and h.
                        //}
                    }
                }



                # pragma omp barrier
                // Draw the printed circuit components
                //std::cout<<"****************************"<<std::endl;
                fix_boundaries2<float>(g);
                # pragma omp taskwait



                //# pragma omp master
                # pragma omp master
                {
                    for (i=0; i<npixy;i++)
                    {
                        for (j=0;j<npixx;j++)
                        {
                            if(fabs((float)(h(i, j) - g(i, j))) > 0) //4. verify if meet local synchronization
                            {
                                //std::cout<<"*************count*************"<<count<<std::endl;
                                count++;
                            }
                        }
                    }
                //}
                //#pragma omp critical
                //{
                    //std::cout<<"--------------count--------------"<<count<<std::endl;
                    if (count == 0) //5. jump out of loop after meeting local synchronization
                    {
                        flag_exit = true;
                        //break;
                    }


                    for (i=0; i<npixx;i++)
                    {
                        for (j=0;j<npixy;j++)
                        {
                            h(i,j)=g(i,j);
                        }
                    }

                }


                if (flag_exit == true) //5. jump out of loop after meeting local synchronization
                {

                    break;
                }

            }

        //}

		

	}
	

	
	// This is the initial value image where the boundaries and printed
	// circuit components have been fixed
    dump_array<float, 2>(h, "plate3.fit");


	long t_end = omp_get_wtime();
	std::cout << "---------------t_end  = "<<t_end <<"--------------\n"<<std::endl;
	std::cout << "The time of calculating is "<< (t_end - t_start)/double(CLOCKS_PER_SEC)<<std::endl;
	// Complete the sequential version to compute the heat transfer,
	// then make a parallel version of it
}
