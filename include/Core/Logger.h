/*
 * Logger.h
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
namespace fcv {

typedef enum {
	LOG_INFO, LOG_ERROR, LOG_WARNING, LOG_DEBUG
} Loglevel;

// DEBUG MACROS
#define LOG_ERROR(TXT) printf("ERROR: " TXT "\n")
#define LOG_INFO(TXT) printf("INFO: " TXT "\n")
#define LOG_WARNING(TXT) printf("WARNING: " TXT "\n")
#define LOG_FORMAT_ERROR(TXT_FMT, ...) printf("ERROR: " TXT_FMT "\n", __VA_ARGS__)
#define LOG_FORMAT_INFO(TXT_FMT, ...) printf("INFO: " TXT_FMT "\n", __VA_ARGS__)
#define LOG_FORMAT_WARNING(TXT_FMT, ...) printf("WARNING: " TXT_FMT "\n", __VA_ARGS__)

#ifdef DEBUG
	#define LOG_DEBUG(TXT) printf(__FILE__ "(" __LINE__ ") DEBUG: " TXT "\n")
	#define LOG_FORMAT_DEBUG(TXT_FMT, ...) printf(__FILE__ "(" __LINE__ ") DEBUG: " TXT_FMT "\n", __VA_ARGS__)
#else
	#define LOG_DEBUG(TXT) do { } while(0)
	#define LOG_FORMAT_DEBUG(TXT_FMT, ...) do { } while(0)
#endif



} /* namespace fcv */

#endif /* LOGGER_H_ */
