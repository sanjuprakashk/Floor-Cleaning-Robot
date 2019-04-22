#ifndef LOGGER_H
#define LOGGER_H


struct sensor_struct
{
    char task_name[6];
    uint32_t timeStamp;

    float sensor_data;

}sensor_struct;

struct logger_struct
{
	char task_name[6];
	uint32_t timeStamp;

	char log[100];
}logger_struct;


#endif


