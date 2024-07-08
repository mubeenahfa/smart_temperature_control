#include "TM4C123GH6PM.h"


extern void I2CINIT(void);
extern int wtstat(void);
extern void t_data(int slave_address, char slave_memory_address, int bytes_count, char* data);
extern void r_data(int slave_address, char slave_memory_address, int bytes_count, char* data);

char write_data[100];
char read_data[2];
int average[128];
int average1[128];
long finalmsb = 0;
long finallsb = 0;
int sensor_value_notreal = 0;
int sensor_value = 0;
int j = 0;
int lastvalue;
int test = 0;


/* Function to calibrate raw temperature*/
int realinator(double adc_t){
	int var1,var2,T;
	int dig_t1=27504;
	int dig_t2=26435;/* calibration constants from BMP280 datasheet */
	int dig_t3=-1000;
	
	/* calibration formulation from BMP280 datasheet */
	var1 = (((double)adc_t)/16384.0-((double)dig_t1)/1024.0)*((double)dig_t2);
	var2 = ((((double)adc_t)/131072.0-((double)dig_t1)/8192.0)*(((double)adc_t)/131072.0-((double)dig_t1)/8192.0))*((double)dig_t3);
	
	T=(var1+var2)/5120;
	
	return T;/* Return calibrated temperature */


}

void sensor_init(){
		I2CINIT();
	
		write_data[0] = 0xB6;
		t_data(0x76, 0xE0, 1, write_data);
	
		write_data[0] = 0x20;
		t_data(0x76, 0xF5, 1, write_data); //Send slave address, register address, number of data to send and data. Then, wait if any error occurs
		
		write_data[0] = 0x23;
		t_data(0x76, 0xF4, 1, write_data); //Send slave address, register address, number of data to send and data. Then, wait if any error occurs
}

int get_temp(){
		for (j = 0; j < 128; j++) //get 128 sample for both MSB temperature and LSB temperature
		{
				r_data(0x76, 0xFA, 2, read_data);  //Send slave address, register address(MSB temperature), number of data to read and storage array.

				average[j] = read_data[0];  //save MSB 8 bits of temperature value
				average1[j] = read_data[1];  //save LSB 8 bits of temperature value
		}
		for (j = 0; j < 128; j++) // take their average
		{
				finalmsb += average[j];
				finallsb += average[j];
		}
		finalmsb = finalmsb / 128;
		finallsb = finallsb / 128;

		sensor_value_notreal=((finalmsb*256)+finallsb)*16;//Unite MSB and LSB temperature value

		sensor_value = realinator(sensor_value_notreal);//send not calibrated temperature value and get calibrated temperature value
		
		return sensor_value;
}



/*
int main()
{


		//sensor_init();
		
		while(1)
		{
				
		}
}
*/