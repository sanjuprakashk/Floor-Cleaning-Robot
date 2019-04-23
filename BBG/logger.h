#ifndef LOGGER_H
#define LOGGER_H


typedef struct sensor_struct
{
    char task_name[5];
    uint32_t timeStamp;

    float sensor_data;

}sensor_struct;

typedef struct logger_struct
{
	char task_name[5];
	uint32_t timeStamp;

	char log[100];
}logger_struct;

char task_name[15];
#endif


