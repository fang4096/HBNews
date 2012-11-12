#include "logger.h"
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdarg>

using namespace std;

static const char *prefix_str[] = {"Finest: ", "Finer: ", "Fine: ", 
	"Config: ", "Info: ", "Waring: ", "Severe: "};
static FILE *log_fp = NULL;
static level log_level = FINEST;
static const int file_name_len = 256; 

void init_log_file(const char *file_name_prefix, level l)
{
	char file_name[file_name_len];
	time_t now;
	struct tm *time_info;

	time(&now);
	time_info = localtime(&now);
	sprintf(file_name, "%s", file_name_prefix);
	strftime(file_name + strlen(file_name_prefix), 10, "%Y%m%d", time_info);
	sprintf(file_name + strlen(file_name), ".log");

	if (log_fp == NULL)
		log_fp = fopen(file_name, "a");

	log_level = l;
}


void log(level l, const char *fmt, ...)
{
	va_list arg_ptr;
	char time_str[20];
	struct tm *time_info;
	time_t now;

	time(&now);
	time_info = localtime(&now);
	strftime (time_str, 20, "%Y-%m-%d %H:%M:%S", time_info);

	if (l < log_level)
		return;
	
	fprintf(log_fp, "%s %s", time_str, prefix_str[l]);
	va_start(arg_ptr, fmt);
	vfprintf(log_fp, fmt, arg_ptr);
	fflush(log_fp);
	va_end(arg_ptr);
}

void log_f(FILE *fp, level l, const char *fmt, ...)
{
	va_list arg_ptr;
	char time_str[20];
	struct tm *time_info;
	time_t now;

	time(&now);
	time_info = localtime(&now);
	strftime (time_str, 20, "%Y-%m-%d %H:%M:%S", time_info);

	if (l < log_level)
		return;
	
	fprintf(fp, "%s %s", time_str, prefix_str[l]);
	va_start(arg_ptr, fmt);
	vfprintf(fp, fmt, arg_ptr);
	fflush(fp);
	va_end(arg_ptr);
}

void set_log_level(level l)
{
	log_level = l;
}

void close_log_file()
{
	fclose(log_fp);
	log_fp = NULL;
}
