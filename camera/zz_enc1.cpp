#include "zz_enc1.h"

void zz_enc1::process()
{

	int		i, j, k, l;
	int		temp_block[64];                     
	int		block[64];


	ask_i.write(false); i=0; wait(); ready_o.write(false);
	while(1) {

i=0;
		while(i<=63)
		{	ask_i.write(true);
			while(ready_i.read()==false){wait();}
			temp_block[i]=input.read();
			i=i+1;
			ask_i.write(false);
			wait();
		}


		i = 0 , j = -1 , k = 0;

		for ( l = 0 ; l < 4 ; l++ ) {
			for ( j++ ; i >= 0 ; j++ , i-- ) {
				block[k] = temp_block[i*8+j];
				k++;
			}

			for ( i++ ; j >= 0 ; j-- , i++ ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
		}

		for ( l = 0 ; l < 3 ; l++ ) {
			for ( i-- , j += 2 ; j < 8 ; j++ , i-- ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
			for ( j-- , i += 2 ; i < 8 ; j-- , i++ ) {
				block[k] = temp_block[i*8+j];
				k++;
			}
		}

		i-- , j += 2;
		block[k] = temp_block[i*8+j];

		i=0;


		while (i<=63)
		{
		 while (ask_o.read()==false)
		 {;wait(); }

		   output.write(block[i]);
		   ready_o.write(true); wait();


		   ready_o.write(false);  wait();
		   i++;
	}

		/*for ( i = 0 ; i < 64 ; ++i ) {
			output.write (block[i]);

		}*/
	}
}







