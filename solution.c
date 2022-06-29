/* 
 * Use your preferred naming convention
 * Add comments to help others to understand your code and to document assumptions
 * You are welcome to add additional files if required
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

//Total number of expected vehicles
#define MAX_DATA_SIZE 2000000

//Struct defining vehicle "object" containing the 5 fields specified to be read from file.
typedef struct{
	int32_t positionID;
    char* vehicleRegistration;
    float latitude;
    float longitude;
    uint64_t utc_timestamp;
    //Declared data types are as specified. Assumes float data type is 32 bit
}VehicleData;

//Test data struct
typedef struct{
    float latitude;
    float longitude;
}TestData;

int readData(VehicleData* vehicles);
int compareLatitude(const void * a, const void * b);
double distance(VehicleData vehicle, TestData testCoord);
VehicleData nearestVehicle_5(TestData testCoord, VehicleData* vehicles, int numVehicles, float threshold);


//Function to read vehicle data from the specified file. 'vehicles' is an array of VehicleData structs. 
//Through each vehicle[i], the ID, registration, latitude, longitude and timestamp data can be accesssed.
int readData(VehicleData* vehicles){
    FILE *fPtr;
    int i = 0;

    //Assumes a binary file, which is true in this case
	fPtr=fopen("positions.dat","rb");
	if(fPtr==NULL)
	{
		printf("Error. File could not be opened.\n Please try again.\n");
	}
	else
	{
        //Assumes complete set of valid vehicle datafor all 2 million vehicles
        while(i < MAX_DATA_SIZE){
            fread(&vehicles[i].positionID, sizeof(int32_t),1,fPtr);

            //Read registration no. 1 character at a time till a null character is found.
            vehicles[i].vehicleRegistration = (char*)malloc(sizeof(char*));
            int j = 0;
            char c;
            do{
                fread(&c, sizeof(char),1,fPtr);
                vehicles[i].vehicleRegistration[j] = c;
                j++;
            }while(c != '\0');

            //Read latitude, longitude and timestamp data
            fread(&vehicles[i].latitude, sizeof(float),1,fPtr);
            fread(&vehicles[i].longitude, sizeof(float),1,fPtr);
            fread(&vehicles[i].utc_timestamp, sizeof(uint64_t),1,fPtr);

            /*
            //Check that data was properly read.
            printf("PositionID %d: %d\n", i,vehicles[i].positionID);
            printf("Registration %d: %s\n", i,vehicles[i].vehicleRegistration);
            printf("Latitude %d: %f\n", i,vehicles[i].latitude);
            printf("Longitude %d: %f\n", i, vehicles[i].longitude);
            printf("UTC %d: %llu\n", i,vehicles[i].utc_timestamp);
            */
           
            i++;
        }
        fclose(fPtr);
    }
    //return the number of vehicle entries read from file.
    return i;
}


//Helper function needed for inbuilt qsort function
int compareLatitude(const void * a, const void * b){
  VehicleData *vehicleA = (VehicleData *)a;
  VehicleData *vehicleB = (VehicleData *)b;

  if(vehicleA->latitude > vehicleB->latitude) return 1;
  if(vehicleA->latitude < vehicleB->latitude) return -1;
  else return 0;
}

//Function to calculate the distance between a vehicle and a test location. Square root not calculated as the inequality for shortest distance holds regardless
double distance(VehicleData vehicle, TestData testCoord){
    double latDist, longDist, distance;
    latDist = (vehicle.latitude-testCoord.latitude)*(vehicle.latitude-testCoord.latitude);
    longDist = (vehicle.longitude-testCoord.longitude)*(vehicle.longitude-testCoord.longitude);
    distance = (latDist + longDist);
    return distance;
}

//Function to find the closest vehicle. Assumes vehicles have been sorted by latitude
VehicleData nearestVehicle_5(TestData testCoord, VehicleData* vehicles, int numVehicles, float threshold){
    int32_t nearestID = -1;
    VehicleData nearestVehicle;
    nearestVehicle.positionID = nearestID;

    double minDistance = MAX_DATA_SIZE;
    double xDistance;

    //iterates through all vehicles but only calculates distances within specified threshold. Assumes that closest vehicle is within a close range of latitude too
    for(int i = 0; i < numVehicles; i++){
        if(((vehicles[i].latitude - testCoord.latitude) < threshold) && ((vehicles[i].latitude - testCoord.latitude) > -threshold)){
            VehicleData car = vehicles[i];
            xDistance = distance(vehicles[i], testCoord);
            //find closest distance for vehicles within certain threshold. minDistance holds smallest distance. nearest ID & minDistance only updated when a closer vehicle is found
            if(xDistance < minDistance){
                minDistance = xDistance;
                nearestID = vehicles[i].positionID;
                nearestVehicle = vehicles[i];
            }
        }
    }
    return nearestVehicle;
}

int main(){
    struct timespec begin, end;
    //begin timer with nanosecond accuracy
    clock_gettime(CLOCK_MONOTONIC, &begin); 
    
    //char* filename = "positions.dat";
    int nearestID;
    VehicleData nearestVehicle;

    //manually load test location data
    TestData testCoords[10] = {{34.544909, -102.100843}, {32.345544,-99.123124},
    {33.234235,-100.214124}, {35.195739, -95.348899},
    {31.895839, -97.789573}, {32.895839, -101.789573},
    {34.115839, -100.225732}, {32.335839, -99.992232},
    {33.535339, -94.792232}, {32.234235, -100.222222}};

    //allocate memory for vehicle data and read from file
    VehicleData* vehicles = malloc(MAX_DATA_SIZE * sizeof(VehicleData));
    int numVehicles = readData(vehicles);     
    //sort vehicles according to latitude 
    qsort(vehicles, numVehicles, sizeof(VehicleData), compareLatitude);    

    //iterate through test locations
    for(int i = 0; i < 10; i++){
        nearestID = -1;
        float threshold = 0.01; 

        /*Threshold value determines how strict the closest vehicle search is.
        While loop performs closest vehicle search with starting threshold.
        If no vehicle is found, increase by 5 and perform closest search again. This process continues until vehicles are found within the specified latitude range.
        Given test data {32,-100}, this will only consider vehicles in range (31.99,32.01). If none are found range increased to (31.95, 32.05), then (31.75, 32.25), (30.75, 33.25) and so on.
        This assumes that  closest vehicle is within a close range of latitude too, which should stand true in most cases.
        */
        while(nearestID == -1)
        {
            nearestVehicle = nearestVehicle_5(testCoords[i], vehicles, numVehicles, threshold);
            nearestID = nearestVehicle.positionID;
            threshold = threshold * 5;
        }    
        //printf("Car found with threshold %f\n",threshold/5.0);
        //Print out closest vehicle result
        printf("Test %d: {%f, %f}, Vehicle ID: %d\n", i+1, testCoords[i].latitude, testCoords[i].longitude, nearestID);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    //Total time taken with millisecond accuracy
    printf ("Total optimal time = %f milliseconds\n", ((end.tv_sec-begin.tv_sec)*1000.0 + (end.tv_nsec - begin.tv_nsec) / 1000000.0));

    return 0;
}
