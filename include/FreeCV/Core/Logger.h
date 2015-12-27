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


// DEBUG MACROS
// Release
#ifdef NDEBUG
	#define LOG_DEBUG(TXT) do { } while(0)
	#define LOG_FORMAT_DEBUG(TXT_FMT, ...) do { } while(0)

	#define LOG_ERROR(TXT) printf("ERROR: " TXT "\n")
	#define LOG_INFO(TXT) printf(TXT "\n")
	#define LOG_WARNING(TXT) printf("WARNING: " TXT "\n")
	#define LOG_FORMAT_ERROR(TXT_FMT, ...) printf("ERROR: " TXT_FMT "\n", __VA_ARGS__)
	#define LOG_FORMAT_INFO(TXT_FMT, ...) printf(TXT_FMT "\n", __VA_ARGS__)
	#define LOG_FORMAT_WARNING(TXT_FMT, ...) printf("WARNING: " TXT_FMT "\n", __VA_ARGS__)
// Debug
#else
	#define LOG_DEBUG(TXT) printf("%s (%d) DEBUG: " TXT "\n", __FILE__, __LINE__ )
	#define LOG_FORMAT_DEBUG(TXT_FMT, ...) printf("%s (%d) DEBUG: " TXT_FMT "\n", __FILE__, __LINE__ , __VA_ARGS__)

	#define LOG_ERROR(TXT) printf("%s (%d) ERROR: " TXT "\n", __FILE__, __LINE__)
	#define LOG_INFO(TXT) printf("%s (%d): " TXT "\n", __FILE__, __LINE__)
	#define LOG_WARNING(TXT) printf("%s (%d) WARNING: " TXT "\n", __FILE__, __LINE__)
	#define LOG_FORMAT_ERROR(TXT_FMT, ...) printf("%s (%d) ERROR: " TXT_FMT "\n", __FILE__, __LINE__ , __VA_ARGS__)
	#define LOG_FORMAT_INFO(TXT_FMT, ...) printf("%s (%d): " TXT_FMT "\n", __FILE__, __LINE__ , __VA_ARGS__)
	#define LOG_FORMAT_WARNING(TXT_FMT, ...) printf("%s (%d) WARNING: " TXT_FMT "\n", __FILE__, __LINE__ , __VA_ARGS__)
#endif


} /* namespace fcv */

#endif /* LOGGER_H_ */
